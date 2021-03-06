//Astro.h//////////////////////////////////////////////
//
//  Header file for astrophysical objects.
//
//
///////////////////////////////////////////////////////

#ifndef __Mizar5__Astro__
#define __Mizar5__Astro__

#include <vector>
#include <iostream>
#include "Utilities.h"
#include "StarGen/structs.h"

//OrbitZone===========================================
//
//  Enumeration of orbital zones for planets
//
//--------------------------------------------
enum OrbitZone {
    ozNear,
    ozTemperate,
    ozFar
};
//--------------------------------------------

//OrbitZone===========================================
//
//  Enumeration of orbital zones for planets
//
//--------------------------------------------
enum PlanetType {
    ptUnknown,
    ptRock,
    ptVenusian,
    ptTerrestrial,
    ptGasGiant,
    ptMartian,
    ptWater,
    ptIce,
    ptSubGasGiant,
    ptSubSubGasGiant,
    ptAsteroid,
    pt1Face
};
//------------------------------------------------
stdstring convertPlanetTypeToString(PlanetType p);
//------------------------------------------------


//forward decs~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Planetary;//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class System;//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Sector;//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//NavObj===========================================
//
//  Virtual base class for objects at which
//  nav points can be set.
//
//--------------------------------------------
class NavObj {
    //stuff goes here
};
//----------------------------------------------





//AstroObj===========================================
//
//  Virtual base class for astrophysical objects.
//
//--------------------------------------------
class AstroObj : public NavObj {
    
protected:
    
    System* Sys;
   
    stdstring CategoryName;
    
public:
    
    double dMass;               //(M_Sol), M_Earth for planets
    double dEquitorialRadius;   //(km)
    double dPeriodRotational;   //(days)
    
    bool checkSystem();
    
    System* getContainingSystem();
    void setContainingSystem(System& sys);
    
    void setCategoryName(const stdstring& name);
    stdstring getCategoryName();
};
//---------------------------------------------



//Star===========================================
//
//  A star.
//
//--------------------------------------------
class Star : public AstroObj {
private:
    
public:
    
    double dLuminosity;
    double dAge;
    double dLifetime;
    double dEcosphereRadius;
    
    //SQL stuff
    void enterIntoDatabase(SQLite::Database* data);
    
};
//--------------------------------------------


//forward decs~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class City;//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//Planetary===========================================
//
//  Base class for planetary bodies
//
//--------------------------------------------
class Planetary : public AstroObj {
private:
    
    int iPrimaryIndex;
    
    void setPrimaryPlanetary(const AstroObj& prime);
    void setPrimaryStar(const AstroObj& prime);
    
    Planetary* getPrimaryPlanetary();
    Star* getPrimaryStar();
    
    double getXPosition();
    double getYPosition();
    double getZPosition();
    
    //SQL Stuff_______________________________________________
    static std::vector<stdstring> getAttributeNameStrings();
    static stdstring convertPlanetTypeToString(PlanetType p);
    
    std::vector<stdstring> getAttributeStrings();
    
    void feedAttributesToStatement(SQLite::Statement* s);
    
public:
    
    //CITIES_________________________________________
    std::vector<City> Cities;
    
    //DATA____________________________________________
    PlanetType Type;            
    OrbitZone Zone;             //is an enum
    
    bool bMoon;                 //true if a moon
    bool bJovian;               //duh
    bool bResonant;             //resonant rotation
    bool bGreenhouse;           //runaway greenhouse effect
    
    int iNumber;                //number around primary
    int iGases;                 //number of atmo gases
    int iMinorMoons;            //number of minor moons
    
    double dSemiMajorAxis;      //(ls)
    double dEccentricity;
    double dAxialTilt;          //(radians)
    double dMassWithoutDust;    //mass ignoring dust
    double dMassWithoutGas;     //mass ignoring gas
    double dCoreRadius;         //radius of rocky core (km)
    double dDensity;            //density (g/cm^3)
    double dPeriodOrbital;      //(years)
    double dEscapeVelocity;     //(cm/s)
    double dSurfaceAccell;      //(cm/s^2)
    double dSurfaceGravity;     //(earth gravities)
    double dRMSVelocity;        //(cm/s)
    double dMolecWeight;        //smallest molecular weight retained
    double dSurfacePressure;    //(mb)
    double dBoilWater;          //boiling point of water (K)
    double dAlbedo;             //albedo
    double dTExosphere;         //exosphere temp (K)
    double dTSurface;           //surface temp (K)
    double dGreenhouseRise;     //temperature rise due to greenhouse (K)
    double dTHigh;              //high day/night temp
    double dTLow;               //low day/night temp
    double dTMax;               //high winter/summer temp
    double dTMin;               //low winter/summer temp
    double dHydrosphere;        //hydro coverage
    double dCloudCover;         //cloud coverage
    double dIceCover;           //ice coverage
    
    //the following are not generated by StarGen
    double dOrbitInitPhase;     //initial phase of orbit
    double dLongOfAscNode;      //longitude of ascending node
    double dOrbitInclination;   //set to 0 for now
    
    //FUNCTIONS______________________________________
    void setPrimary(const AstroObj& prime);
    AstroObj* getPrimary();

    int getPrimaryIndex();
    void setPrimaryIndex(int i);
    
    //ORBITAL STUFF__________________________________
    double getOrbitPhase();         //units not decided on yet!!!
    double getDistanceFromPrimary();
    cartvec3 getLocation();
    
    //SQL Stuff
    static stdstring getPlanetDBInitString();
    void enterIntoDatabase(SQLite::Database* data);
};
//---------------------------------------------



//System===========================================
//
//  A star system.
//  For now this is just one star.
//
//--------------------------------------------
class System {
    
private:
    Sector* Sec;
    
    stdstring Name;
    
    cartvec3 loc;
    
    std::vector<Star> Stars;
    std::vector<Planetary> Planetaries;
    
    void getFromOther(const System& other);
    
public:
    
    System(const stdstring& name="");
    
    ~System();
    
    System(const System& other);            //copy constructor
    System(System&& other);                 //move constructor
    
    System& operator=(const System& other); //copy ass operator
    System& operator=(System&& other);      //move ass operator
    
    void setName(const stdstring& name);
    stdstring getName();
    
    void addStar(Star& sun);
    void addStarDestr(Star* sun);
    void addPlanetary(Planetary& planet);
    void addPlanetaryDestr(Planetary* planet);
    
    void setPlanetaryPointers();
    void setStarPointers();
    void setAstroPointers();
    
    void setLocation(cartvec3& v);
    
    int getNStars() const;
    int getNPlanetaries() const;
    
    void mergeWithOther(System& other);   //combine with other system 
    
    Star* getStarWithIndex(int i);
    Planetary* getPlanetaryWithIndex(int i);
    
    Planetary*  getPlanet(int n);           //for now this just does first added
    Planetary*  getLastPlanetaryAdded();
    Star*       getLastStarAdded();
    Star*       getPrimaryStar();           //for now just first star in list
    
    cartvec3 getLocation();
    
    double getTime() {return -999.;};           //NOT IMPLEMENTED YET!
    
    
    //SQL stuff
    void enterIntoDatabase(SQLite::Database* data);
    void addStarsToDatabase(SQLite::Database* data);
    void addPlanetsToDatabase(SQLite::Database* data);
};
//---------------------------------------------
//typedef for system pointer
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
typedef std::unique_ptr<System> System_ptr;


//Sector===========================================
//
//  A rectangular vector of space
//  For now this class is in charge of setting up
//  databases.
//
//--------------------------------------------
class Sector {
    
private:
    cartvec3 vertex1;
    cartvec3 vertex2;
    
    std::vector<System> Systems;
    
    void setSize(double size);              //doesn't rescale systems
                                            //so can only call this when empty
public:
    Sector(double size);
    
    void addSystemDestr(System_ptr& sys);
    void addSystemDestr(System* sys);
    void addSystem(System& sys);
    
    int getNSystems();
    
    System* getSystemWithIndex(int i);
    
    System* getLastSystemAdded();
    
    double getTime() {return -999.;};       //not implemented yet!!
    
    
    //SQL stuff
    void addSystemsToDatabase(SQLite::Database* data);      //creates a table with member systems
    void addStarsToDatabase(SQLite::Database* data);        //creates a table with all stars
    void addPlanetsToDatabase(SQLite::Database* data);      //creates a table with all planets
};
//--------------------------------------------
//typedef for sector pointer
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
typedef std::unique_ptr<Sector> Sector_ptr;


#endif















