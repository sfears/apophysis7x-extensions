// These set cache size for cell centres, they take a lot of processing, so it's handy to
// keep values between calls
#define CACHE_NUM 10
#define CACHE_WIDTH 21

// voronoi and noiseb are additional, not required in all Apophysis plugins
#include "voronoi.h"
#include "noiseb.h"

// Must define this structure before we include apoplugin.h
typedef struct
{
	double crackle_cellsize;
	double crackle_power;
	double crackle_distort;
	double crackle_scale;
	double crackle_z;
	// P is a working list of points
	double P[VORONOI_MAXPOINTS][2];
	// C is a cache of pre-calculated centres
	double C[CACHE_WIDTH][CACHE_WIDTH][2];
} Variables;

#ifdef _APOMSVC
	#include "apoplugin_msvc.h"
#else
	#include "apoplugin.h"
#endif

// Waffle's cells are based on a simple square grid which is distorted using a noise function

// position() calculates cell centre for cell (x, y), given plane slice z, scale factor s, distortion d
//  and stores in supplied array
void position(int x, int y, double z, double s, double d, double V[2]) {
	double E[3], F[3];

	// Values here are arbitrary, chosen simply to be far enough apart so they do not correlate
	E[_x_] = x * 2.5;
	E[_y_] = y * 2.5;
	E[_z_] = z * 2.5;
	// Cross-over between x and y is intentional
	F[_x_] = y * 2.5 + 30.2;
	F[_y_] = x * 2.5 - 12.1;
	F[_z_] = z * 2.5 + 19.8;

	V[_x_] = (x + d * simplexNoise3D(E)) * s;
	V[_y_] = (y + d * simplexNoise3D(F)) * s;
}

// cached_position gives centre co-ordinates either from cache, or calculated from scratch if needed
void cached_position(double Cache[CACHE_WIDTH][CACHE_WIDTH][2], int x, int y, double z, double s, double d, double V[2]) {
	if (abs(x) <= CACHE_NUM && abs(y) <= CACHE_NUM) {
		V[_x_] = Cache[x + CACHE_NUM][y + CACHE_NUM][_x_];
		V[_y_] = Cache[x + CACHE_NUM][y + CACHE_NUM][_y_];
	}
	else {
		position(x, y, z, s, d, V);
	}
}

// Set the name of this plugin
APO_PLUGIN("crackle");

// Define the Variables
APO_VARIABLES(
	VAR_REAL(crackle_cellsize, 1.0),
	VAR_REAL(crackle_power, 0.2),
	VAR_REAL(crackle_distort, 0.0),
	VAR_REAL(crackle_scale, 1.0),
	VAR_REAL(crackle_z, 0.0)
	);