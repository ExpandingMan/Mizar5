//Utils.cpp//////////////////////////////////////////////
//
//  Source file for Utils.h
//
//
///////////////////////////////////////////////////////


#include "Astro.h"

//=================================================================
//               <AstroObj>                        //
//--------------------------------------------------//

bool AstroObj::checkSystem() {
    if (!Sys) {
        std::cerr << "ERROR! (AtroObj) System not found. ";
        std::cerr << "(" + CategoryName + ").\n";
        return false;
    } else {
        return true;
    }
}

System* AstroObj::getContainingSystem() {
    return Sys;
}

void AstroObj::setContainingSystem(System& sys) {
    Sys = &sys;
}

void AstroObj::setCategoryName(const stdstring& name) {
    CategoryName = name;
}

stdstring AstroObj::getCategoryName() {
    return CategoryName;
}

//--------------------------------------------------//
//              </AstroObj>                        //
//==================================================================





//=================================================================
//               <Planetary>                        //
//--------------------------------------------------//

void Planetary::setPrimaryPlanetary(const AstroObj &prime) {
    const int N = Sys->getNPlanetaries();
    iPrimaryIndex = -1;
    for (int i=0; i<N; i++) {
        if (&prime==Sys->getPlanetaryWithIndex(i))
            iPrimaryIndex = i;
    }
}

void Planetary::setPrimaryStar(const AstroObj &prime) {
    const int N = Sys->getNStars();
    iPrimaryIndex = -1;
    for (int i=0; i<N; i++) {
        if (&prime==Sys->getStarWithIndex(i))
            iPrimaryIndex = i;
    }
}

AstroObj* Planetary::getPrimaryPlanetary() {
    return Sys->getPlanetaryWithIndex(iPrimaryIndex);
}

AstroObj* Planetary::getPrimaryStar() {
    return Sys->getStarWithIndex(iPrimaryIndex);
}

double Planetary::getXPosition() {
    double phi = getOrbitPhase();
    double o = cos(dLongOfAscNode)*cos(phi);
    o -= sin(dLongOfAscNode)*sin(phi)*cos(dOrbitInclination);
    o *= getDistanceFromPrimary();
    return o;
}

double Planetary::getYPosition() {
    double phi = getOrbitPhase();
    double o = sin(dLongOfAscNode)*cos(phi);
    o += cos(dLongOfAscNode)*sin(phi)*cos(dOrbitInclination);
    o *= getDistanceFromPrimary();
    return o;
}

double Planetary:: getZPosition() {
    double o = sin(getOrbitPhase())*sin(dOrbitInclination);
    o *= getDistanceFromPrimary();
    return o;
}

void Planetary::setPrimary(const AstroObj& prime) {
    if (!checkSystem()) return;
    if (bMoon) {
        setPrimaryPlanetary(prime);
    } else {
        setPrimaryStar(prime);
    }
}

AstroObj* Planetary::getPrimary() {
    if (!Sys) return nullptr;
    if (bMoon) {
        return getPrimaryPlanetary();
    } else {
        return getPrimaryStar();
    }
}

void Planetary::setPrimaryIndex(int i) {
    iPrimaryIndex = i;
}

int Planetary::getPrimaryIndex() {
    return iPrimaryIndex;
}

//FIX UNITS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
double Planetary::getOrbitPhase() {
    double omega = 2.*PI / dPeriodOrbital;
    return omega * Sys->getTime() + dOrbitInitPhase;
}

double Planetary::getDistanceFromPrimary() {
    double o = dSemiMajorAxis;
    o *= 1. - pow(dEccentricity,2);
    o /= 1. + dEccentricity*cos(getOrbitPhase());
    return o;
}

cartvec3 Planetary::getLocation() {
    return cartvec3(getXPosition(),
                    getYPosition(),
                    getZPosition());
}

//--------------------------------------------------//
//              </Planetary>                        //
//==================================================================






//=================================================================
//               <System>                        //
//--------------------------------------------------//

System::System(const stdstring& name) {
    setName(name);
}

System::~System() {
    Name.clear();
    Stars.clear();
    Planetaries.clear();
}

System::System(const System& other) {
    getFromOther(other);
    setAstroPointers();
}

System::System(System&& other) {
    getFromOther(other);
    setAstroPointers();
    delete &other;
}

System& System::operator=(const System& other) {
    getFromOther(other);
    setAstroPointers();
    return *this;
}

System& System::operator=(System&& other) {
    getFromOther(other);
    setAstroPointers();
    delete& other;
    return *this;
}

void System::getFromOther(const System &other) {
    Name        = other.Name;
    loc         = other.loc;
    Stars       = other.Stars;
    Planetaries = other.Planetaries;
}

void System::setName(const stdstring& name) {
    Name = name;
}

void System::addStar(Star &sun) {
    Stars.push_back(sun);
}

void System::addStarDestr(Star* sun) {
    Stars.push_back(std::move(*sun));
    delete sun;
}

void System::addPlanetary(Planetary& planet) {
    Planetaries.push_back(planet);
}

void System::addPlanetaryDestr(Planetary* planet) {
    Planetaries.push_back(std::move(*planet));
    delete planet;
}

void System::setPlanetaryPointers() {
    for (Planetary& p : Planetaries) {
        p.setContainingSystem(*this);
    }
}

void System::setStarPointers() {
    for (Star& s : Stars) {
        s.setContainingSystem(*this);
    }
}

void System::setAstroPointers() {
    setPlanetaryPointers();
    setStarPointers();
}

void System::setLocation(cartvec3 &v) {
    loc = v;
}

int System::getNStars() const {
    return (int)Stars.size();
}

int System::getNPlanetaries() const {
    return (int)Planetaries.size();
}

void System::mergeWithOther(System& other) {
    int NStarsOrig   = getNStars();
    int NPlanetsOrig = getNPlanetaries();
    int NPlanets     = other.getNPlanetaries();
    int NStars       = other.getNStars();
    
    for (int i=0; i<NStars; i++)
        Stars.push_back(*other.getStarWithIndex(i));
    
    for (int i=0; i<NPlanets; i++) {
        Planetaries.push_back(*other.getPlanetaryWithIndex(i));
        Planetary& p = Planetaries.back();
        int iOld = p.getPrimaryIndex();
        if (!p.bMoon) {
            p.setPrimaryIndex(NStarsOrig+iOld);
        } else {
            p.setPrimaryIndex(NPlanetsOrig+iOld);
        }
    }
    
    setAstroPointers();
}

Star* System::getStarWithIndex(int i) {
    return &Stars.at(i);
}

Planetary* System::getPlanetaryWithIndex(int i) {
    return &Planetaries.at(i);
}

Planetary* System::getPlanet(int n) {
    Planetary* p = nullptr;
    for (int i=0; i<Planetaries.size(); i++) {
        Planetary& P = Planetaries[i];
        if ((P.iNumber==n && !P.bMoon)) {
            p = &P;
            return p;
        }
    }
    return p;
}

Planetary* System::getLastPlanetaryAdded() {
    return &Planetaries.back();
}

Star* System::getLastStarAdded() {
    return &Stars.back();
}

Star* System::getPrimaryStar() {
    return &Stars.front();
}

cartvec3 System::getLocation() {
    return loc;
}

//--------------------------------------------------//
//              </System>                        //
//==================================================================








//=================================================================
//               <Sector>                        //
//--------------------------------------------------//

Sector::Sector(double size)
: vertex1(cartvec3()),
  vertex2(cartvec3(size,size,size))
{
}

void Sector::setSize(double size) {
    vertex2 = cartvec3(size,size,size);
}

void Sector::addSystemDestr(System* sys) {
    Systems.push_back(std::move(*sys));
    delete sys;
}

void Sector::addSystemDestr(System_ptr& sys) {
    Systems.push_back(*sys);
}

void Sector::addSystem(System& sys) {
    Systems.push_back(std::move(sys));
}

int Sector::getNSystems() {
    return (int)Systems.size();
}

System* Sector::getSystemWithIndex(int i) {
    return &Systems.at(i);
}

System* Sector::getLastSystemAdded() {
    return &Systems.back();
}

//--------------------------------------------------//
//              </Sector>                        //
//==================================================================






























