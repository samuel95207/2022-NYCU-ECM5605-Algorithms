#include <cmath>
#include <cfloat>
#include <limits>
#include "answer.h"

using namespace std;

double bound(double num){
	return max(-DBL_MAX, min(DBL_MAX, num));
}

/*
double f(long in_number){
        return in_number;
}
// */


/*
double f(long in_number){
        return -in_number;
}
// */


/*
double f(long in_number){
        return bound(abs(in_number)*100.3);
}
// */


// /*
double f(long in_number){
        return bound( 100*sin(in_number*10) );
}
// */

/*
double f(long in_number){
	return bound( ((in_number^2) + sqrtl(in_number) * in_number)/abs(in_number) );
}
// */
