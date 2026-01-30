/*       A value of 0.08 for the error tolerance parameter is adequate for single precision 
(7 significant digits). Since the error scales as en, we see that 0.0025 will yield double 
precision (16 significant digits) and require at Most two or three more iterations. Since the 
coefficients of the sixth-order truncation error are different for the other elliptic functions,
 these values for the errortolerance should be changed to 0.04 andO.0012 in the algorithm for 
R--, andO.05 and 0.0015 for Rj and RD. As well as being an algorithm in its own right for 
certain combinations of elementary functions, the algorithm for Bc is used repeatedly in the 
computation of Ri.       The C implementations test the input arguments against two 
machine-dependent con- stants, TINY and BIG, to ensure that there will be no underflow or 
overflow during the computation. We have chosen conservative values, corresponding to a machine 
minimum of 3 x  10-39 and a machine maximum of 1.7 x          1038 . You can always extend the
 range of admissible argument values by using the homogeneity relations (6.11.22), below.*/ 

#include <math.h> 
#include "nrutil.h" 
#define ERRTOL 0.05
#define TINY 1.0e-25 
#define BIG 4.5e2l 
#define C1  (3.0/14.0) 
#define C2  (1.0/6.0) 
#define C3  (9.0/22.0) 
#define C4  (3.0/26.0) 
#define C5  (0.25*C3) 
#define C6  (1.5*C4) 

double rd(double x, double y, double z)  

/*Computes Carlson's elliptic integral of the second kind, RD (X, Y, z). x and y must be non- 
negative, and at most one can be zero. z must be positive. TINY must be at least twice the  
negative 2/3 power of the machine overflow limit. BIG must be at most 0.1 x MTOL times  the 
negative 2/3 power of the machine underflow limit.*/  

{ 
	double alamb,ave,delx,dely,delz,ea,eb,ec,ed,ee,fac,sqrtx,sqrty,     
	sqrtz,sum,xt,yt,zt; 

	if (FMIN(x,y) < 0.0 || FMIN(x+y,z) < TINY || FMAX(FMAX(x,y),z) > BIG)
	  nrerror("invalid arguments in rd"); 
	xt=x; 
	yt=y; 
	zt=z; 
	sum=0.0; 
	fac=1.0; 
	do {
	  sqrtx=sqrt(xt);     
	  sqrty=sqrt(yt);     
	  sqrtz=sqrt(zt);     
	  alamb=sqrtx*(sqrty+sqrtz)+sqrty*sqrtz;     
	  sum += fac/(sqrtz*(zt+alamb));     
	  fac=0.25*fac;     
	  xt=0.25*(xt+alamb);     
	  yt=0.25*(yt+alamb);     
	  zt=0.25*(zt+alamb);     
	  ave=0.2*(xt+yt+3.0*zt);     
	  delx=(ave-xt)/ave;     
	  dely=(ave-yt)/ave;     
	  delz=(ave-zt)/ave;   
	}  while (FMAX(FMAX(fabs(delx),fabs(dely)),fabs(delz)) > ERRTOL); 
	ea=delx*dely; 
	eb=delz*delz; 
	ec=ea-eb; 
	ed=ea-6.0*eb; 
	ee=ed+ec+ec; 
	return 3.0*sum+fac*(1.0+ed*(-C1+C5*ed-C6*delz*ee)     
	      +delz*(C2*ee+delz*(-C3*ec+delz*C4*ea)))/(ave*sqrt(ave)); 
}

