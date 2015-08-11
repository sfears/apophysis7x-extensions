/*
    Apophysis Plugin: crackle

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

// "Crackle" variation is a type of blur - it is not affected by incoming data, simply
// generates a texture.

// You must call the argument "vp".
int PluginVarPrepare(Variation* vp)
{
	// Pre-calculate cache of grid centres, to save time later . . .
	int x, y;
	for (x = -CACHE_NUM; x <= CACHE_NUM; x++) {
		for (y = -CACHE_NUM; y <= CACHE_NUM; y++) {
			position(x, y, VAR(crackle_z), VAR(crackle_cellsize) / 2.0, VAR(crackle_distort), VAR(C)[x + CACHE_NUM][y + CACHE_NUM]);
		}
	}

	return TRUE; // Always return TRUE.
}

// You must call the argument "vp".
int PluginVarCalc(Variation* vp)
{
	double XCo, YCo, DXo, DYo, L, R, s, trgL;
	double U[2];
	int XCv, YCv;

	// An infinite number of invisible cells? No thanks!
	if (0.0 == VAR(crackle_cellsize)) {
		return TRUE;
	}

	// Scaling factor
	s = VAR(crackle_cellsize) / 2.0;

	// For a blur effect, base everything starting on a circle radius 1.0
	// (as opposed to reading the values from FTx and FTy)
	double blurr = (random01() + random01()) / 2.0 + (random01() - 0.5) / 4.0;

	double theta = 2 * M_PI * random01();
	U[_x_] = blurr * sin(theta);
	U[_y_] = blurr * cos(theta);

	// Use integer values as Voronoi grid co-ordinates
	XCv = (int)floor(U[_x_] / s);
	YCv = (int)floor(U[_y_] / s);

	// Get a set of 9 square centre points, based around the one above
	int di, dj;
	int i = 0;
	for (di = -1; di < 2; di++) {
		for (dj = -1; dj < 2; dj++) {
			cached_position(VAR(C), XCv + di, YCv + dj, VAR(crackle_z), s, VAR(crackle_distort), VAR(P)[i]);
			i++;
		}
	}

	int q = closest(VAR(P), 9, U);

	int offset[9][2] = { { -1, -1 },{ -1, 0 },{ -1, 1 },
	{ 0, -1 },{ 0, 0 },{ 0, 1 },
	{ 1, -1 },{ 1, 0 },{ 1, 1 } };

	// Remake list starting from chosen square, ensure it is completely surrounded (total 9 points)

	// First adjust centres according to which one was found to be closest
	XCv += offset[q][_x_];
	YCv += offset[q][_y_];

	// Get a new set of 9 square centre points, based around the definite closest point
	i = 0;
	for (di = -1; di < 2; di++) {
		for (dj = -1; dj < 2; dj++) {
			cached_position(VAR(C), XCv + di, YCv + dj, VAR(crackle_z), s, VAR(crackle_distort), VAR(P)[i]);
			i++;
		}
	}

	L = voronoi(VAR(P), 9, 4, U); // index 4 is centre cell

								  // Delta vector from centre
	DXo = U[_x_] - VAR(P)[4][_x_];
	DYo = U[_y_] - VAR(P)[4][_y_];

	/////////////////////////////////////////////////////////////////
	// Apply "interesting bit" to cell's DXo and DYo co-ordinates

	// trgL is the new value of L
	trgL = pow(L + 1e-100, VAR(crackle_power)) * VAR(crackle_scale);  // ( 0.9 )

	R = trgL / (L + 1e-100);

	DXo *= R;
	DYo *= R;

	// Add cell centre co-ordinates back in
	DXo += VAR(P)[4][_x_];
	DYo += VAR(P)[4][_y_];

	// Finally add values in
	FPx += VVAR * DXo;
	FPy += VVAR * DYo;

	return TRUE;
}
