#include	<math.h>
#include	<stdlib.h>
#include	"StarGen/const.h"
#include	"StarGen/utils.h"

//better random numbers
Random R(111);

void setStarGenRSeed(int seed) {
    R.setSeed(seed);
}

int getStarGenRandomInt() {
    R.setDiscreteRange(0,std::numeric_limits<int>::max());
    return R.FlatDiscrete();
}

/*----------------------------------------------------------------------*/
/*	This function returns a random real number between the specified	*/
/* inner and outer bounds.												*/
/*----------------------------------------------------------------------*/

long double random_number(long double inner, long double outer)
{
	//long double range;
	//range = outer - inner;
	//return((((long double)rand()) / (long double)(RAND_MAX)) * range + inner);
    return R.FlatStretched(inner,outer);
}


/*----------------------------------------------------------------------*/
/*	 This function returns a value within a certain variation of the	*/
/*	 exact value given it in 'value'.									*/
/*----------------------------------------------------------------------*/

long double about(long double value, long double variation)
{
	return(value + (value * random_number(-variation,variation)));
}

long double random_eccentricity()
{
	long double	e;
	
	e = 1.0 - pow(random_number(0.0, 1.0), ECCENTRICITY_COEFF);
	
	if (e > .99)	// Note that this coresponds to a random
		e = .99;	// number less than 10E-26
					// It happens with GNU C for -S254 -W27
	return(e);
}
