//Commerce.cpp//////////////////////////////////////////////
//
//  Source file for Commerce.h
//
//
///////////////////////////////////////////////////////

#include "Commerce.h"

//=================================================================
//               <Cargo>                        //
//--------------------------------------------------//

Cargo::Cargo(CommodityTypes type, int amount) {
    Type = type;
    Amount = amount;
}

CommodityCategories Cargo::getCategory() {
    int n = (int)Type;
    if (n<=8) return ccMineral;
    if ((n>=9)&&(n<=22)) return ccMetal;
    return ccNone;
}

//--------------------------------------------------//
//              </Cargo>                        //
//==================================================================








//=================================================================
//               <CargoBay>                        //
//--------------------------------------------------//

CargoBay::CargoBay(int cap) {
    setCapacity(cap);
}

int CargoBay::checkCapacity(int n) {
    if (Capacity-Amount >= n) {
        return n;
    } else if (Capacity-Amount<n) {
        return Capacity - Amount;
    } else {
        return -1;
    }
}

void CargoBay::mergeCommonTypes() {
    for (int i=0; i<Store.size(); i++) {
        CommodityTypes type = Store[i].Type;
        for (int j=i+1; j<Store.size(); j++) {
            if (Store.at(j).Type==type) {
                Store[i].Amount += Store[j].Amount;
                Store[j].Amount = 0;
            }
        }
    }
    cleanStore();
}

void CargoBay::cleanStore() {
    for (int i=0; i<Store.size(); i++) {
        if (Store.at(i).Amount==0) {
            Store.erase(Store.begin()+i);
            i--;
        }
    }
}

void CargoBay::setAmount() {
    int amount = 0;
    for (int i=0; i<Store.size(); i++) {
        amount += Store[i].Amount;
    }
    Amount = amount;
}

void CargoBay::setCapacity(int cap) {
    Capacity = cap;
}

int CargoBay::getCapacity() {
    return Capacity;
}

void CargoBay::addCargo(CommodityTypes type, int amount) {
    int n = checkCapacity(amount);
    if (n>0) {
        Amount += n;
        Store.push_back(Cargo(type,n));
        mergeCommonTypes();
    }
}

void CargoBay::addCargo(Cargo C) {
    int n = checkCapacity(C.Amount);
    if (n>0) {
        C.Amount -= n;
        Amount += n;
        Store.push_back(Cargo(C.Type,n));
        mergeCommonTypes();
    }
}

void CargoBay::removeCargo(CommodityTypes type, int amount) {
    int removedFrom = -1;
    for (int i=0; i<Store.size(); i++) {
        if (Store.at(i).Type==type) {
            if (Store[i].Amount-amount>=0) {
                Store[i].Amount -= amount;
                Amount -= amount;
                removedFrom = i;
            }
        }
    }
    if (removedFrom>=0 && Store.at(removedFrom).Amount==0)
        Store.erase(Store.begin()+removedFrom);
}

void CargoBay::removeCargo(Cargo C) {
    removeCargo(C.Type,C.Amount);
}

bool CargoBay::checkFor(CommodityTypes type, int amount) {
    for (int i=0; i<Store.size(); i++) {
        if (Store.at(i).Type==type &&
            Store.at(i).Amount>=amount) return true;
    }
    return false;
}

bool CargoBay::checkFor(Cargo C) {
    return checkFor(C.Type,C.Amount);
}

bool CargoBay::roomFor(int amount) {
    if (Capacity-Amount>=amount) return true;
    return false;
}

bool CargoBay::roomFor(Cargo C) {
    return roomFor(C.Amount);
}

void CargoBay::transferCargoTo(CommodityTypes type, int amount, CargoBay& other) {
    if (checkFor(type,amount)) {
        int init = other.Amount;
        other.addCargo(type,amount);
        removeCargo(type,other.Amount-init);
    }
}

void CargoBay::transferCargoTo(Cargo C, CargoBay& other) {
    transferCargoTo(C.Type,C.Amount,other);
}

void CargoBay::transferCargoFrom(CommodityTypes type, int amount, CargoBay& other) {
    if (other.checkFor(type,amount)) {
        int init = Amount;
        addCargo(type,amount);
        other.removeCargo(type,Amount-init);
    }
}

void CargoBay::transferCargoFrom(Cargo C, CargoBay& other) {
    transferCargoFrom(C.Type,C.Amount,other);
}

void CargoBay::transferAllCargoTo(CargoBay& other) {
    for (int i=0; i<Store.size(); i++) {
        int n = Store[i].Amount;
        int init = other.Amount;
        other.addCargo(Store[i]);
        int diff = other.Amount - init;
        removeCargo(Store[i].Type,diff);
        if (diff==n) i--;
    }
}

void CargoBay::transferAllCargoFrom(CargoBay &other) {
    for (int i=0; i<other.Store.size(); i++) {
        int n = other.Store[i].Amount;
        int init = Amount;
        addCargo(other.Store[i]);
        int diff = Amount - init;
        other.removeCargo(other.Store[i].Type,diff);
        if (diff==n) i--;
    }
}

//--------------------------------------------------//
//              </CargoBay>                        //
//==================================================================
















