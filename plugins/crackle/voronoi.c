/*
    Apophysis Plugin (Supporting Library)

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

#include "voronoi.h"

// This implements some basic 2D vector routines that assist with Voronoi
// cell calculations.

// Distance between U and P compared to U and Q. If U is at P, then the value is 0, if it is
// equally far to Q and P then the value is 1.0
double vratio( double P[2], double Q[2], double U[2] ) {
	double PmQx, PmQy;

	PmQx = P[_x_] - Q[_x_];
	PmQy = P[_y_] - Q[_y_];

	if ( 0.0 == PmQx && 0.0 == PmQy ) {
		return 1.0;
	}

	return 2.0 * ( ( U[_x_] - Q[_x_] ) * PmQx + ( U[_y_] - Q[_y_] ) * PmQy ) / ( PmQx * PmQx + PmQy * PmQy );
}

// Closest point to U from array P.
//  P is an array of points
//  n is number of points to check
//  U is location to find closest
int closest( double P[VORONOI_MAXPOINTS][2], int n, double U[2] ) {
	double d2;
	double d2min = 1.0e100;
	int i, j;

	for( i = 0; i < n; i++ ) {
		d2 = (P[i][_x_] - U[_x_]) * (P[i][_x_] - U[_x_]) + (P[i][_y_] - U[_y_]) * (P[i][_y_] - U[_y_]);
		if ( d2 < d2min ) {
			d2min = d2;
			j = i;
		}
	}

	return j;
}

// Voronoi "value" is 0.0 (centre) to 1.0 (edge) if inside cell . . . higher values
// mean that point is not in the cell defined by chosen centre.
//  P is an array of points defining cell centres
//  n is number of points in array
//  q is chosen centre to measure distance from
//  U is point to test
double voronoi( double P[VORONOI_MAXPOINTS][2], int n, int q, double U[2] ) {
	double ratio;
	double ratiomax = -1.0e100;
	int i;

	for( i = 0; i < n; i++ ) {
		if ( i != q ) {
			ratio = vratio( P[i], P[q], U );
			if ( ratio > ratiomax ) {
				ratiomax = ratio;
			}
		}
	}

	return ratiomax;
}
