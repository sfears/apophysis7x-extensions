typedef struct
{
	double x_waves2PL_scalex;
	double x_waves2PL_scaley;
	double x_waves2PL_freqx;
	double x_waves2PL_freqy;

} Variables;

#ifdef _APOMSVC
	#include "apoplugin_msvc.h"
#else
	#include "apoplugin.h"
#endif

APO_PLUGIN("x_waves2PL");
APO_VARIABLES(
	VAR_REAL(x_waves2PL_freqx, 2.0),
	VAR_REAL(x_waves2PL_freqy, 2.0),
	VAR_REAL(x_waves2PL_scalex, 1.0),
	VAR_REAL(x_waves2PL_scaley, 1.0)	
);
