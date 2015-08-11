typedef struct
{
	double x_blur_circlePL_hole;
	double VVAR4_PI;

} Variables;

#define _USE_MATH_DEFINES
const double M_PI3_4 = 2.35619449;
const double M_PI_4 = 0.785398163;

#ifdef _APOMSVC
	#include "apoplugin_msvc.h"
#else
	#include "apoplugin.h"
#endif

APO_PLUGIN("x_blur_circlePL");
APO_VARIABLES(
	VAR_REAL(x_blur_circlePL_hole, 0.0)
);