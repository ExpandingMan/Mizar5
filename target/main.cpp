//
//  main.cpp
//  Mizar5
//
//  Created by Michael Savastio on 3/9/15.
//  Copyright (c) 2015 The Royal Scam. All rights reserved.
//

#include <iostream>
#include <ctime>
#include "StarGen/stargen.h"
#include "Utilities.h"
#include "AstroProcedural.h"

#include "SQLiteCpp/SQLiteCpp.h"

using namespace std;

int main() {
    
    double size = 20.;
    double density = 1./284.;
    
    const stdstring data_dir = "/Users/ExpandingMan/Mizar5/data/";
    
    AstroGen Gen(999);
    
    Sector_ptr Sec(Gen.genSectorOfDensityNew(density,size));

    SQLite::Database db(data_dir+"test.db3",SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE);
    
    Sec->addSystemsToDatabase(&db);
    Sec->addStarsToDatabase(&db);
    
    return 0;
}


/*
int main() {
 
    double size = 20.;
    double density = 1./284.;
    
    AstroGen Gen(998);
    
    Sector_ptr Sec(Gen.genSectorOfDensityNew(density,size));
    
    cout << Sec->getNSystems() << endl;
    
    for (int i=0; i<Sec->getNSystems(); i++) {
        cout << Sec->getSystemWithIndex(i)->getNStars() << endl;
    }
    
    return 0;
}
*/