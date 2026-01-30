#include <math.h> 
#include "nrutil.h" 
#define ERRTOL 0.08 
#define TINY 1.5e-38 
#define BIG 3.0e37 
#define THIRD (1.0/3.0) 
#define Cl (1.0/24.0) 
#define C2 0.1 
#define C3 (3.0/44.0) 
#define C4 (1.0/14.0) 
double rf(double x, double y, double z) 
/*Computes Carlson's elliptic integral of the first kind, Rp(x, y, z). x, y, and z must be 
nonneg- ative, and at most one can be zero. TINY must be at least 5 times the machine underflow 
limit, BIG at most one fifth the machine overflow limit. */
{
	double alamb,ave,delx,dely,delz,e2,e3,sqrtx,sqrty,sqrtz,xt,yt,zt; 

	if (FMIN(FMIN(x,y),z) < 0.0 || FMIN(FMIN(x+y,x+z),y+z) < TINY ||     
	    FMAX(FMAX(x,y),z) > BIG)
	    nrerror("invalid arguments in rf");
 	xt=x; 
	yt=y; 
	zt=z; 
	do {     
		sqrtx=sqrt(xt);     
		sqrty=sqrt(yt);     
		sqrtz=sqrt(zt);     
		alamb=sqrtx*(sqrty+sqrtz)+sqrty*sqrtz;     
		xt=0.25*(xt+alamb);     
		yt=0.25*(yt+alamb);     
		zt=0.25*(zt+alamb);     
		ave=THIRD*(xt+yt+zt);     
		delx=(ave-xt)/ave;     
		dely=(ave-yt)/ave;     
		delz=(ave-zt)/ave;   
    	}  while (FMAX(FMAX(fabs(delx),fabs(dely)),fabs(delz)) > ERRTOL);  
	e2=delx*dely-delz*delz; 
	e3=delx*dely*delz; 
	return ((1.0+(Cl*e2-C2-C3*e3)*e2+C4*e3)/sqrt(ave));
}
