/*
    Apophysis Plugin: x_waves2PL

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

int PluginVarPrepare(Variation* vp)
{
    return 1;
}

int PluginVarCalc(Variation* vp)
{
	FPx += VVAR * (FTx + VAR(x_waves2PL_scalex) * sin(FTy * VAR(x_waves2PL_freqx)));
	FPy += VVAR * (FTy + VAR(x_waves2PL_scaley) * sin(FTx * VAR(x_waves2PL_freqy)));
    return 1;
}