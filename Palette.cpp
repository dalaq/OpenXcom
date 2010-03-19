/*
 * Copyright 2010 Daniel Albano
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Palette.h"

Palette::Palette() : _colors(NULL), _ncolors(0)
{
}

Palette::~Palette()
{
	free(_colors);
}

void Palette::loadDat(string filename, int ncolors, int offset)
{
	_ncolors = ncolors;
	_colors = (SDL_Color *)malloc(sizeof(SDL_Color) * _ncolors);

	// Load file and put colors in pallete
	ifstream palFile (filename.c_str(), ios::in | ios::binary);
	if (!palFile)
	{
		throw "Failed to load palette";
	}

	// Move pointer to proper pallete
	palFile.seekg(offset, ios::beg);
	
	char value[3];

	for (int j = 0; j < _ncolors && palFile.read(value, 3); j++)
	{
		// Correct X-Com colors to RGB colors
		_colors[j].r = Uint8(value[0])*4;
		_colors[j].g = Uint8(value[1])*4;
		_colors[j].b = Uint8(value[2])*4;
	}

	/*
	if (!palFile.eof())
	{
		throw "Invalid data from file";
	}
	*/

	palFile.close();
}

SDL_Color *Palette::getColors(int offset)
{
	return _colors+offset;
}