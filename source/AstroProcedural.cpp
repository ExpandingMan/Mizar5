//AstroProcedural.cpp//////////////////////////////////////////////
//
//  Source file for AstroProcedural.h
//
//
///////////////////////////////////////////////////////

#include "AstroProcedural.h"

//===================================================================
//          <AstroGen>
//--------------------------------------------------

AstroGen::AstroGen(int seed)
: R(seed)
{
    iMaxStarsPerSystem      = 4;
    dMeanNAddStars          = 1.;
    dChanceMultiple         = 0.7;
    dChanceEarthlike        = 0.3;
    iSeed                   = seed;
    initRandom();
}

void AstroGen::initRandom() {
    R.setPoissonMean(dMeanNAddStars);
    R.setDiscreteRange(0,1E6-1);  //for naming
}

void AstroGen::setSeed(int seed) {
    R.setSeed(seed);
}

void AstroGen::setMeanNAddedStars(double mean) {
    dMeanNAddStars = mean;
    R.setPoissonMean(dMeanNAddStars);
}

void AstroGen::setChanceForMultipleStars(double chance) {
    dChanceMultiple = chance;
}

void AstroGen::setChanceMustHaveEarthlike(double chance) {
    dChanceEarthlike = chance;
}

System_ptr AstroGen::genBaseSystemNew(stdstring name,
                                      int seed,
                                      bool OnlyHabitable,
                                      bool OnlyDualHabitable,
                                      bool OnlyEarthlike)
{
    System_ptr S(StarGenSystemNew(name,0.,seed,7,
                                  OnlyHabitable,
                                  OnlyDualHabitable,
                                  OnlyEarthlike));
    return S;
}

System_ptr AstroGen::genSystemNew(stdstring name,
                                  bool OnlyHabitable,
                                  bool OnlyDualHabitable,
                                  bool OnlyEarthlike)
{
    int seed = R.FlatDiscreteAll();
    System_ptr S(genBaseSystemNew(name,seed,
                                  OnlyHabitable,
                                  OnlyDualHabitable,
                                  OnlyEarthlike));
    
    if (R.RandomBit(dChanceMultiple)) {
        int NAddStars = R.Poisson();
        for (int i=0; i<NAddStars; i++) {
            if (S->getNStars() >= iMaxStarsPerSystem) break;
            seed = R.FlatDiscreteAll();
            S->mergeWithOther(*genBaseSystemNew(name,seed));
        }
    }
    
    return S;
}
                        
Sector_ptr AstroGen::genSectorNew(int NSystems, double size) {
    Sector_ptr Sec(new Sector(size));
    R.setRange(0.,size);
    
    for (int i=0; i<NSystems; i++) {
        stdstring name = std::to_string(R.FlatDiscrete());
        bool hasEarthlike = R.RandomBit(dChanceEarthlike);
        cartvec3 x(R.Flat(),R.Flat(),R.Flat());
        System_ptr sys(genSystemNew(name,0,0,hasEarthlike));
        Sec->addSystemDestr(sys);
        Sec->getLastSystemAdded()->setLocation(x);
    }
    
    return std::move(Sec);
}

Sector_ptr AstroGen::genSectorOfDensityNew(double density, double size) {
    int NSystems = (int)(density*pow(size,3));
    return std::move(genSectorNew(NSystems,size));
}

//--------------------------------------------------
//          </AstroGen>
//===================================================================



























