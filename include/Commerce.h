//Commerce.h//////////////////////////////////////////////
//
//  Header file for commercial objects.
//
//
///////////////////////////////////////////////////////

#pragma once

#include "Utilities.h"
#include "Astro.h"

//===========================================
//
//  Enumeration of commodity types
//
//--------------------------------------------
enum CommodityTypes {
    //MINERALS.........................
    ctBauxite,//0
    ctBertrandite,//1
    ctColtan,//2
    ctGallite,//3
    ctIndite,//4
    ctLepidolite,//5
    ctPainite,//6
    ctRutile,//7
    ctUraninite,//8
    
    //METALS...........................
    ctAluminum,//9
    ctBeryllium,//10
    ctCobalt,//11
    ctCopper,//12
    ctGallium,//13
    ctGold,//14
    ctIndium,//15
    ctLithium,//16
    ctPalladium,//17
    ctPlatinum,//18
    ctSilver,//19
    ctTantalum,//20
    ctTitanium,//21
    ctUranium,//22
    
    ctAny//23       //special type for inventory management
};
//--------------------------------------------

//===========================================
//
//  Enumeration of commodity categories
//
//--------------------------------------------
enum CommodityCategories {
    ccMineral,
    ccMetal,
    ccNone
};
//--------------------------------------------


//Cargo===========================================
//
//  cargo
//
//--------------------------------------------
struct Cargo {
    Cargo(CommodityTypes type, int amount);
    
    CommodityTypes Type;
    int Amount;
    
    CommodityCategories getCategory();
};
typedef std::vector<Cargo> CargoVec;
//--------------------------------------------




//CargoBay===========================================
//
//  Anything that holds commodities
//
//--------------------------------------------
class CargoBay {
    
private:
    CargoVec Store;

    int Amount;
    int Capacity;
    
    int checkCapacity(int n);
    void mergeCommonTypes();
    void cleanStore();
    void setAmount();
    
public:
    
    CargoBay(int cap);
    
    void setCapacity(int cap);
    int getCapacity();
    
    void addCargo(CommodityTypes type, int amount);
    void addCargo(Cargo C);                            
    
    void removeCargo(CommodityTypes type, int amount);
    void removeCargo(Cargo C);
    
    bool checkFor(CommodityTypes type, int amount);     //checks if in inventory
    bool checkFor(Cargo C);
    
    bool roomFor(int amount);                            //checks if room available to add
    bool roomFor(Cargo C);
    
    void transferCargoTo(CommodityTypes type,
                         int amount,
                         CargoBay& other);
    
    void transferCargoTo(Cargo C, CargoBay& other);
    
    void transferCargoFrom(CommodityTypes type,
                           int amount,
                           CargoBay& other);
    
    void transferCargoFrom(Cargo C, CargoBay& other);
    
    void transferAllCargoTo(CargoBay& other);
    void transferAllCargoFrom(CargoBay& other);
    
};
//--------------------------------------------


//Facility===========================================
//
//  Virtual base class for facilities.
//
//--------------------------------------------
class Facility {
private:
    stdstring TypeName;
    
public:

    CargoBay Input;
    CargoBay Output;
    
    stdstring getTypeName();
};
//--------------------------------------------



//IndustrialCenter===========================================
//
//  Base class for industrial centers
//
//--------------------------------------------
class IndustrialCenter {
    
private:
    std::vector<Facility> Facilities;
    
    //FACILITY_MANAGEMENT__________________________
    void addFacility(Facility f);
    void removeFacility(int i);
    
    Facility* getFacility(int i);
};


//Ship===========================================
//
//  Base class for ships.
//
//--------------------------------------------
class Ship {
    
private:
    NavObj* Location;
    NavObj* Destination;
    
public:
    
    void setDestination(NavObj* dest);
    
};
typedef std::shared_ptr<Ship> Ship_ptr;
//--------------------------------------------


//SmallFreighter===========================================
//
//  a small freighter
//
//--------------------------------------------
class SmallFreighter {
    
private:
    
public:
    SmallFreighter();
    
    CargoBay Bay1;
};
//--------------------------------------------


//LargeFreighter===========================================
//
//  a large freighter
//
//--------------------------------------------
class LargeFreighter {
    
private:
    
public:
    LargeFreighter();
    
    CargoBay Bay1;
};
//---------------------------------------------



//City===========================================
//
//  A city or settlement on a planet.
//
//--------------------------------------------
class City : public IndustrialCenter {
    
private:
    int Population;
    
public:
    
    City(int pop);          //for now this will be static
};




//Station===========================================
//
//  A space station.
//
//--------------------------------------------
class Station : public NavObj, public IndustrialCenter {

private:
    AstroObj* Primary;
    
    std::vector<Ship_ptr> Hangar;      //may eventually be replaced by a class
    
public:
    Station(AstroObj* primary);
    
    CargoBay PublicBay;             //for now this has maximum capacity.
    
    
    //HANGAR_MANAGEMENT____________________________
    bool checkIfDocked(Ship_ptr s);
    
    void dock(Ship_ptr s);
    void undock(int i);
    void undock(Ship_ptr s);
    
};
//--------------------------------------------






































