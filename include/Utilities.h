//Utilities.h//////////////////////////////////////////////
//
//  Header file for various wrappers and utilities.
//
//
///////////////////////////////////////////////////////

#ifndef __Mizar5__Utils__
#define __Mizar5__Utils__


#include <vector>
#include <string>
#include <cmath>
#include "boost/random.hpp"

#define PI acos(-1.)


//-----------------------------------------------
//typedef for default double container class
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
typedef std::vector<double> stdvec_double;
//-----------------------------------------------
//typedef for default string class
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
typedef std::string stdstring;
//-----------------------------------------------
//typedef for random generator, for now must be boost
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
typedef boost::mt19937 PRNGen;


//Random=========================================
//
//  Random number generator wrapper.
//
//------------------------------------------
class Random {
  
private:
    
    PRNGen gen;

    boost::variate_generator<PRNGen,boost::uniform_real<double>> flat_gen01;
    boost::variate_generator<PRNGen,boost::uniform_real<double>> flat_gen;
    boost::variate_generator<PRNGen,boost::uniform_int<>> flat_gen_discrete;
    boost::variate_generator<PRNGen,boost::uniform_int<>> flat_gen_discrete_all;
    boost::variate_generator<PRNGen,boost::poisson_distribution<int>> pois_gen;
    
    double flat_min;
    double flat_max;
    
    double poisson_mean;
    
    int discrete_flat_min;
    int discrete_flat_max;
    
    void reInitFlat01();
    void reInitFlat();
    void reInitFlatDiscrete();
    void reInitFlatDiscreteAll();
    void reInitPoisson();
    void reInitAll();
    
public:
    Random(int seed, double min=-1., double max=1.);
    
    void setSeed(int seed);
    void skipIterations(int skip);
    void setRange(double min, double max);
    void setDiscreteRange(int min, int max);
    void setPoissonMean(double mean);
    
    double Flat01();
    double Flat();
    double FlatStretched(double min, double max);    //uses 01
    int FlatDiscrete();
    int FlatDiscreteAll();                          //out of all ints
    int Poisson();
    
    bool RandomBit(double up=0.5);     //returns random bit
                                        //...arg is probability to be true
};
//--------------------------------------------



//euclidvec3=========================================
//
//  Class for all cartesian 3 vectors
//
//------------------------------------------
class cartvec3 {
    
private:
    
    stdvec_double v;
    
    
public:
    
    cartvec3();
    cartvec3(double x, double y, double z);
    
    double& at(int i);
    
    double& operator[](int i);
    
    double view(int i) const;
    
    cartvec3 operator+(const cartvec3& other);
    cartvec3 operator-(const cartvec3& other);
    
    cartvec3 operator*(const double c);
    
    double norm();
    
    double dot(const cartvec3& other);
    
};
//--------------------------------------------















#endif