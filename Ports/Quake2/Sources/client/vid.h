/*
 * Copyright (C) 1997-2001 Id Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 * USA.
 *
 * =======================================================================
 *
 * ABI to the video oute driver
 *
 * =======================================================================
 */

#ifndef CL_VID_H
#define CL_VID_H

#include "common/shared/shared.h"

typedef struct vrect_s
{
	int x, y, width, height;
} vrect_t;

typedef struct
{
	int width, height; /* coordinates from main game */
} viddef_t;

extern viddef_t viddef; /* global video state */

/* Video module initialisation, etc */
void VID_Init();
void VID_Shutdown();
void VID_CheckChanges();

void VID_Printf(int print_level, char *fmt, ...);
void VID_Error(int err_level, char *fmt, ...);

void VID_NewWindow(int width, int height);

#endif