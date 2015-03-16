//Utilitiess.cpp//////////////////////////////////////////////
//
//  Source file for Utilities.h
//
//
///////////////////////////////////////////////////////


#include "Utilities.h"
#include <iostream>
#include <cmath>


//===================================================================
//          <Random>
//--------------------------------------------------

Random::Random(int seed, double min, double max)
:   gen(seed), flat_gen01(gen,boost::uniform_real<double>(0.,1.)),
    flat_gen(gen,boost::uniform_real<double>(min,max)),
    flat_gen_discrete(gen,boost::uniform_int<>(0,10)),
    flat_gen_discrete_all(gen,boost::uniform_int<>(0,std::numeric_limits<int>::max())),
    pois_gen(gen,boost::poisson_distribution<int>(1.))
{
    flat_min = min;
    flat_max = max;
    discrete_flat_min = 0;
    discrete_flat_max = 10;
    poisson_mean = 1.;
}

void Random::reInitFlat01() {
    flat_gen01 =
    std::move(boost::variate_generator<PRNGen,boost::uniform_real<double>>
              (gen,boost::uniform_real<double>(0.,1.)));
}

void Random::reInitFlat() {
    flat_gen =
    std::move(boost::variate_generator<PRNGen,boost::uniform_real<double>>
              (gen,boost::uniform_real<double>(flat_min,flat_max)));
}

void Random::reInitFlatDiscrete() {
    flat_gen_discrete =
    std::move(boost::variate_generator<PRNGen,boost::uniform_int<>>
              (gen,boost::uniform_int<>(discrete_flat_min,discrete_flat_max)));
}

void Random::reInitFlatDiscreteAll() {
    flat_gen_discrete_all =
    std::move(boost::variate_generator<PRNGen,boost::uniform_int<>>
              (gen,boost::uniform_int<>(0,std::numeric_limits<int>::max())));
}

void Random::reInitPoisson() {
    pois_gen =
    std::move(boost::variate_generator<PRNGen,boost::poisson_distribution<int>>
              (gen,boost::poisson_distribution<int>(poisson_mean)));
}

void Random::reInitAll() {
    reInitFlat01();
    reInitFlat();
    reInitFlatDiscrete();
    reInitFlatDiscreteAll();
    reInitPoisson();
}

void Random::setSeed(int seed) {
    gen.seed(seed);
    reInitAll();
}

void Random::skipIterations(int skip) {
    gen.discard(skip);
    reInitAll();
}

void Random::setRange(double min, double max) {
    flat_min = min;
    flat_max = max;
    reInitFlat();
}

void Random::setDiscreteRange(int min, int max) {
    discrete_flat_min = min;
    discrete_flat_max = max;
    reInitFlatDiscrete();
}

void Random::setPoissonMean(double mean) {
    poisson_mean = mean;
    reInitPoisson();
}

double Random::Flat01() {
    return flat_gen01();
}

double Random::Flat() {
    return flat_gen();
}

double Random::FlatStretched(double min, double max) {
    return min + (max-min)*Flat01();
}

int Random::FlatDiscrete() {
    return flat_gen_discrete();
}

int Random::FlatDiscreteAll() {
    return flat_gen_discrete_all();
}

int Random::Poisson() {
    return pois_gen();
}

bool Random::RandomBit(double up) {
    double x = Flat01();
    if (x < up) return true;
    return false;
}

//--------------------------------------------------
//          </Random>
//===================================================================











//===================================================================
//          <cartvec3>
//--------------------------------------------------

cartvec3::cartvec3() {
    for (int i=0; i<3; i++) v.push_back(0.);
}

cartvec3::cartvec3(double x, double y, double z) {
    v.push_back(x);
    v.push_back(y);
    v.push_back(z);
}

double& cartvec3::at(int i) {
    return v.at(i);
}

double& cartvec3::operator[](int i) {
    return v[i];
}

double cartvec3::view(int i) const {
    return v[i];
}

cartvec3 cartvec3::operator+(const cartvec3& other) {
    cartvec3 vec;
    for (int i=0; i<v.size(); i++) {
        vec[i] = v[i] + other.view(i);
    }
    return std::move(vec);
}

cartvec3 cartvec3::operator-(const cartvec3& other) {
    cartvec3 vec;
    for (int i=0; i<v.size(); i++) {
        vec[i] = v[i] + other.view(i);
    }
    return std::move(vec);
}

cartvec3 cartvec3::operator*(const double c) {
    cartvec3 vec;
    for (int i=0; i<v.size(); i++) {
        vec[i] = c*v[i];
    }
    return std::move(vec);
}

double cartvec3::norm() {
    double sum = 0.;
    for (int i=0; i<v.size(); i++) {
        sum += pow(v[i],2);
    }
    return sum;
}

double cartvec3::dot(const cartvec3& other) {
    double sum = 0.;
    for (int i=0; i<v.size(); i++) {
        sum += v[i]*other.view(i);
    }
    return sum;
}

//--------------------------------------------------
//          </cartvec3>
//===================================================================



















