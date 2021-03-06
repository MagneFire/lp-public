/*
   Copyright (C) 1996-1997 Id Software, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

 */
// video driver defs
#ifndef r_video_h
#define r_video_h

#include "Common/common.h"

#define VID_CBITS 6
#define VID_GRADES (1 << VID_CBITS)

// a pixel can be one, two, or four bytes
typedef byte pixel_t;

typedef struct vrect_s
{
	int x, y, width, height;
	struct vrect_s *pnext;
} vrect_t;

typedef struct
{
	pixel_t *colormap; // 256 * VID_GRADES size
	int fullbright; // index of first fullbright color
	int width, height;
	float aspect; // width / height -- < 0 is taller than wide
	int numpages;
	int recalc_refdef; // if true, recalc vid-based stuff
} viddef_t;

extern viddef_t vid; // global video state
extern unsigned d_8to24table[256];

// Called at startup to set up translation tables, takes 256 8 bit RGB values
// the palette data will go away after the call, so it must be copied off if
// the video driver will need it again
void VID_Init(unsigned char *palette);

void VID_Shutdown();

// called for bonus and pain flashes, and for underwater color changes
void VID_SetPalette(unsigned char *palette);

// called only on Win32, when pause happens, so the mouse can be released
void VID_HandlePause(qboolean pause);

#endif
