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
//               <Star>                        //
//--------------------------------------------------//

void Star::enterIntoDatabase(SQLite::Database *data) {
    stdstring SysName   = Sys->getName();
    stdstring Mass      = std::to_string(dMass);
    stdstring Age       = std::to_string(dAge);
    stdstring Lifetime  = std::to_string(dLifetime);
    stdstring EcoRad    = std::to_string(dEcosphereRadius);
    
    stdstring c = " , ";
    
    stdstring command =
    "INSERT INTO Stars (SysName,Mass,Age,Lifetime,EcosphereRadius)"
    "   values ("+
    SysName+c+Mass+c+Age+c+Lifetime+c+EcoRad+");";
    
    data->exec(command);
}

//--------------------------------------------------//
//              </Star>                        //
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

Planetary* Planetary::getPrimaryPlanetary() {
    return Sys->getPlanetaryWithIndex(iPrimaryIndex);
}

Star* Planetary::getPrimaryStar() {
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

double Planetary::getZPosition() {
    double o = sin(getOrbitPhase())*sin(dOrbitInclination);
    o *= getDistanceFromPrimary();
    return o;
}

stdstring Planetary::getPlanetDBInitString() {
    stdstring o = "( "
    "   SysName text,"
    "   Number int,"
    "   isMoon int,"
    "   PrimaryNumber int,"
    "   Mass real,"
    "   Type text,"
    "   EquitorialRadius real,"
    "   PeriodRotational real,"
    "   PeriodOrbital real,"
    "   SemiMajorAxis real,"
    "   Eccentricity real,"
    "   Density real,"
    "   Albedo real,"
    "   TSurface real,"
    "   PSurface real,"
    "   Hydrosphere real,"
    "   CloudCover real,"
    "   IceCover real"
    ")";
    return o;
}

stdstring Planetary::convertPlanetTypeToString(PlanetType p) {
    switch (p) {
        case (ptUnknown):
            return "Unkown";
        case (ptRock):
            return "Rock";
        case (ptVenusian):
            return "Venusian";
        case (ptTerrestrial):
            return "Terrestrial";
        case (ptGasGiant):
            return "GasGiant";
        case (ptMartian):
            return "Martian";
        case (ptWater):
            return "Water";
        case (ptIce):
            return "Ice";
        case (ptSubGasGiant):
            return "SubGasGiant";
        case (ptSubSubGasGiant):
            return "SubSubGasGiant";
        case (ptAsteroid):
            return "Asteroid";
        case (pt1Face):
            return "1Face";
            
        default:
            return "Unknown";
            break;
    }
}

std::vector<stdstring> Planetary::getAttributeNameStrings() {
    std::vector<stdstring> v;
    v.push_back("SysName");//1
    v.push_back("Number");//2
    v.push_back("isMoon");//3
    v.push_back("PrimaryNumber");//4           //if 0 then not a moon
    v.push_back("Mass");//5
    v.push_back("Type");//6
    v.push_back("EquitorialRadius");//7
    v.push_back("PeriodRotational");//8
    v.push_back("PeriodOrbital");//9
    v.push_back("SemiMajorAxis");//10
    v.push_back("Eccentricity");//11
    v.push_back("Density");//12
    v.push_back("Albedo");//13
    v.push_back("TSurface");//14
    v.push_back("PSurface");//15
    v.push_back("Hydrosphere");//16
    v.push_back("CloudCover");//17
    v.push_back("IceCover");//18
    return std::move(v);
}

std::vector<stdstring> Planetary::getAttributeStrings() {
    std::vector<stdstring> v;
    v.push_back(Sys->getName());
    v.push_back(std::to_string(iNumber));
    if (bMoon) {
        v.push_back("1");
        int PNumber = getPrimaryPlanetary()->iNumber;
        v.push_back(std::to_string(PNumber));
    } else {
        v.push_back("0");
        v.push_back(std::to_string(getPrimaryIndex()));
    }
    v.push_back(std::to_string(dMass));
    stdstring type = convertPlanetTypeToString(Type);
    type = "'"+type+"'";
    v.push_back(std::move(type));
    v.push_back(std::to_string(dEquitorialRadius));
    v.push_back(std::to_string(dPeriodRotational));
    v.push_back(std::to_string(dPeriodOrbital));
    v.push_back(std::to_string(dSemiMajorAxis));
    v.push_back(std::to_string(dEccentricity));
    v.push_back(std::to_string(dDensity));
    v.push_back(std::to_string(dAlbedo));
    v.push_back(std::to_string(dTSurface));
    v.push_back(std::to_string(dSurfacePressure));
    v.push_back(std::to_string(dHydrosphere));
    v.push_back(std::to_string(dCloudCover));
    v.push_back(std::to_string(dIceCover));
    
    return std::move(v);
}

void Planetary::feedAttributesToStatement(SQLite::Statement *s) {
    s->bind(1,Sys->getName());
    s->bind(2,iNumber);
    if (bMoon) {
        s->bind(3,1);
        s->bind(4,getPrimaryPlanetary()->iNumber);
    } else {
        s->bind(3,0);
        s->bind(4,getPrimaryIndex());
    }
    s->bind(5,dMass);
    s->bind(6,convertPlanetTypeToString(Type));
    s->bind(7,dEquitorialRadius);
    s->bind(8,dPeriodRotational);
    s->bind(9,dPeriodOrbital);
    s->bind(10,dSemiMajorAxis);
    s->bind(11,dEccentricity);
    s->bind(12,dDensity);
    s->bind(13,dAlbedo);
    s->bind(14,dTSurface);
    s->bind(15,dSurfacePressure);
    s->bind(16,dHydrosphere);
    s->bind(17,dCloudCover);
    s->bind(18,dIceCover);
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

void Planetary::enterIntoDatabase(SQLite::Database *data) {

    stdstring c = " , ";
    
    stdstring command = "INSERT INTO Planets (";
    
    std::vector<stdstring> NameList = getAttributeNameStrings();
    for (int i=0; i<NameList.size()-1; i++) {
        command += NameList.at(i)+c;
    }
    command += NameList.back()+") ";
    
    command += "VALUES (";
    
    for (int i=0; i<NameList.size()-1; i++) {
        command += "?"+c;
    }
    command += "?);";
    
    SQLite::Statement statement(*data,command);
    
    feedAttributesToStatement(&statement);
    statement.exec();
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

stdstring System::getName() {
    return Name;
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

void System::enterIntoDatabase(SQLite::Database *data) {
    stdstring NStars = std::to_string(getNStars());
    stdstring NPlanets = std::to_string(getNPlanetaries());
    stdstring PositionX = std::to_string(loc.at(0));
    stdstring PositionY = std::to_string(loc.at(1));
    stdstring PositionZ = std::to_string(loc.at(2));
    
    stdstring c = " , ";
    
    stdstring command =
    "INSERT INTO Systems (Name, NStars, NPlanets,"
    "PositionX, PositionY, PositionZ) VALUES ("
    +Name+c+NStars+c+NPlanets+c+PositionX+c
    +PositionY+c+PositionZ+");";
    
    data->exec(command);
}

void System::addStarsToDatabase(SQLite::Database *data) {
    for (int i=0; i<Stars.size(); i++) {
        Stars.at(i).enterIntoDatabase(data);
    }
}

void System::addPlanetsToDatabase(SQLite::Database *data) {
    for (int i=0; i<Planetaries.size(); i++) {
        Planetaries.at(i).enterIntoDatabase(data);
    }
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

void Sector::addSystemsToDatabase(SQLite::Database *data) {
    SQLite::Transaction trans(*data);
    
    stdstring command =
    "CREATE TABLE IF NOT EXISTS Systems ("
    "    Name text,"
    "    NStars int,"
    "    NPlanets int,"
    "    PositionX real,"
    "    PositionY real,"
    "    PositionZ real"
    ");";
    
    data->exec(command);
    
    for (int i=0; i<Systems.size(); i++) {
        Systems.at(i).enterIntoDatabase(data);
    }
    
    trans.commit();
}

void Sector::addStarsToDatabase(SQLite::Database *data) {
    SQLite::Transaction trans(*data);
    
    stdstring command =
    "CREATE TABLE IF NOT EXISTS Stars ("
    "   SysName text,"
    "   Mass real,"
    "   Age real,"
    "   Lifetime real,"
    "   EcosphereRadius real"
    ");";
    
    data->exec(command);
    
    for (int i=0; i<Systems.size(); i++) {
        Systems.at(i).addStarsToDatabase(data);
    }
    
    trans.commit();
}

void Sector::addPlanetsToDatabase(SQLite::Database *data) {
    SQLite::Transaction trans(*data);
    
    stdstring command =
    "CREATE TABLE IF NOT EXISTS Planets "
    +Planetary::getPlanetDBInitString()+";";
    
    data->exec(command);
    
    for (int i=0; i<Systems.size(); i++) {
        Systems.at(i).addPlanetsToDatabase(data);
    }
    
    trans.commit();
}

//--------------------------------------------------//
//              </Sector>                        //
//==================================================================






























