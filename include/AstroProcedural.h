//AstroProcedural.h//////////////////////////////////////////////
//
//  Header file for procedural generation of astrophysical
//  objects.
//
///////////////////////////////////////////////////////

#ifndef __Mizar5__AstroProcedural__
#define __Mizar5__AstroProcedural__

#include "Astro.h"
#include "Utilities.h"
#include "StarGen/structs.h"
#include "StarGen/stargen.h"



//AstroGen=========================================
//
//  Class for generating sectors and everything
//  in them.  Contains a wrapper for StarGen.
//
//------------------------------------------
class AstroGen {
    
private:
    Random R;
    
    double dMeanNAddStars;      //number of stars if system has more than 1
                                //don't make too small because of Poisson shape
    
    double dChanceMultiple;     //chance for multiple stars
    double dChanceEarthlike;    //chance system MUST have earthlike
    
    int iSeed;                  //for now used for everything
    int iMaxStarsPerSystem;
    
    void initRandom();
    
public:
    
    AstroGen(int seed=999);
    
    void setSeed(int seed);
    void setMeanNAddedStars(double mean);
    void setChanceForMultipleStars(double chance);
    void setChanceMustHaveEarthlike(double chance);
    
    System_ptr genBaseSystemNew(stdstring name, int seed,
                                bool OnlyHabitable=0,
                                bool OnlyDualHabitable=0,
                                bool OnlyEarthlike=0);      //wrapper for StarGen
    
    System_ptr genSystemNew(stdstring name,                 //chance for multiple stars
                            bool OnlyHabitable=0,           //only first subsystem
                            bool OnlyDualHabitable=0,       //only first subsystem
                            bool OnlyEarthlike=0);          //only first subsystem
    
    Sector_ptr genSectorNew(int NSystems, double size);     //generates cubical sector
    Sector_ptr genSectorOfDensityNew(double density, double size);  //for specified density
};
//-------------------------------------------


























#endif