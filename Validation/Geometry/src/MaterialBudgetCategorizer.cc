#include "Validation/Geometry/interface/MaterialBudgetCategorizer.h"

#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4UnitsTable.hh"
#include "G4EmCalculator.hh"
#include "G4UnitsTable.hh"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <fstream>
#include <vector>

MaterialBudgetCategorizer::MaterialBudgetCategorizer()
{
  buildMaps();
}

void MaterialBudgetCategorizer::buildMaps()
{
  //----- Build map volume name - volume index
  G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
  G4LogicalVolumeStore::iterator ite;
  int ii = 0;
  for (ite = lvs->begin(); ite != lvs->end(); ite++) {
    theVolumeMap[(*ite)->GetName()] = ii++;
  }
  
  //----- Build map material name - volume index
  const G4MaterialTable* matTable = G4Material::GetMaterialTable();
  G4int matSize = matTable->size();
  edm::LogInfo("MaterialBudget") << "MaterialBudgetCategorizer: matSize:" << matSize;
  
  //----- Build map material name - X0 contributes
  edm::LogInfo("MaterialBudget") << "MaterialBudgetCategorizer: Fill X0 Map";
  std::string theMaterialX0FileName = edm::FileInPath("Validation/Geometry/data/trackerMaterials.x0").fullPath();
  buildCategoryMap(theMaterialX0FileName, theX0Map);
  //For the HGCal
  std::string thehgcalMaterialX0FileName = edm::FileInPath("Validation/Geometry/data/hgcalMaterials.x0").fullPath();
  buildHGCalCategoryMap(thehgcalMaterialX0FileName, theHGCalX0Map);
  //----- Build map material name - L0 contributes
  edm::LogInfo("MaterialBudget") << "MaterialBudgetCategorizer: Fill L0 Map";
  std::string theMaterialL0FileName = edm::FileInPath("Validation/Geometry/data/trackerMaterials.l0").fullPath();
  buildCategoryMap(theMaterialL0FileName, theL0Map);
  //For the HGCal
  std::string thehgcalMaterialL0FileName = edm::FileInPath("Validation/Geometry/data/hgcalMaterials.l0").fullPath();
  buildHGCalCategoryMap(thehgcalMaterialL0FileName, theHGCalL0Map);
  // summary of all the materials loaded
  edm::LogInfo("MaterialBudget") << "MaterialBudgetCategorizer: Material Summary Starts";
  G4EmCalculator calc;
}

void MaterialBudgetCategorizer::buildCategoryMap(std::string theMaterialFileName, std::map<std::string,std::vector<float> >& theMap) {
  
  std::ifstream theMaterialFile(theMaterialFileName);
  if (!theMaterialFile) 
    cms::Exception("LogicError") << " File not found " << theMaterialFileName;
  
  // fill everything as "other"
  float sup,sen,cab,col,ele,oth,air;
  sup=sen=cab=col=ele=0.;

  std::string materialName;

  while(theMaterialFile) {
    theMaterialFile >> materialName;
    theMaterialFile >> sup >> sen >> cab >> col >> ele;

    oth = 0.000;
    air = 0.000;
    theMap[materialName].clear();        // clear before re-filling
    theMap[materialName].push_back(sup); // sup
    theMap[materialName].push_back(sen); // sen
    theMap[materialName].push_back(cab); // cab
    theMap[materialName].push_back(col); // col
    theMap[materialName].push_back(ele); // ele
    theMap[materialName].push_back(oth); // oth
    theMap[materialName].push_back(air); // air
    edm::LogInfo("MaterialBudget") 
      << "MaterialBudgetCategorizer: Material " << materialName << " filled " 
      << " SUP " << sup 
      << " SEN " << sen 
      << " CAB " << cab 
      << " COL " << col 
      << " ELE " << ele 
      << " OTH " << oth 
      << " AIR " << air;
  }
}

void MaterialBudgetCategorizer::buildHGCalCategoryMap(std::string theMaterialFileName, 
						      std::map<std::string,std::vector<float> >& theMap)
{
  
  std::ifstream theMaterialFile(theMaterialFileName);
  if (!theMaterialFile) 
    cms::Exception("LogicError") <<" File not found " << theMaterialFileName;
  
  // fill everything as "other"
  float Air,Cables,Copper,H_Scintillator,Lead,M_NEMA_FR4_plate,Silicon,StainlessSteel,WCu, oth; 
  Air=Cables=Copper=H_Scintillator=Lead=M_NEMA_FR4_plate=Silicon=StainlessSteel=WCu=0.;

  std::string materialName;
  while(theMaterialFile) {
    theMaterialFile >> materialName;
    theMaterialFile >> Air >> Cables >> Copper >> H_Scintillator >> Lead >> M_NEMA_FR4_plate >> Silicon >> StainlessSteel >> WCu;
    // Skip comments
    if (materialName[0] == '#')
      continue;
    // Substitute '*' with spaces
    std::replace(materialName.begin(), materialName.end(), '*', ' ');
    oth = 0.000;
    theMap[materialName].clear();        // clear before re-filling
    theMap[materialName].push_back(Air             ); // Air
    theMap[materialName].push_back(Cables          ); // Cables          
    theMap[materialName].push_back(Copper          ); // Copper          
    theMap[materialName].push_back(H_Scintillator  ); // H_Scintillator  
    theMap[materialName].push_back(Lead            ); // Lead            
    theMap[materialName].push_back(M_NEMA_FR4_plate); // M_NEMA_FR4_plate
    theMap[materialName].push_back(Silicon         ); // Silicon         
    theMap[materialName].push_back(StainlessSteel  ); // StainlessSteel
    theMap[materialName].push_back(WCu             ); // WCu
    theMap[materialName].push_back(oth             ); // oth
    edm::LogInfo("MaterialBudget") 
      << "MaterialBudgetCategorizer: material " << materialName << " filled " 
      << std::endl
      << "\tAir              " << Air << std::endl
      << "\tCables           " << Cables << std::endl
      << "\tCopper           " << Copper << std::endl
      << "\tH_Scintillator   " << H_Scintillator << std::endl
      << "\tLead             " << Lead << std::endl
      << "\tM_NEMA_FR4_plate " << M_NEMA_FR4_plate << std::endl
      << "\tSilicon          " << Silicon << std::endl
      << "\tStainlessSteel   " << StainlessSteel << std::endl
      << "\tWCu              " << WCu << std::endl
      << "\tOTH              " << oth;
  }

}
