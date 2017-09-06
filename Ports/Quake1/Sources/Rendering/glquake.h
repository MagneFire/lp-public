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

#ifndef glquake_h
#define glquake_h

#include "Common/common.h"
#include "Common/cvar.h"
#include "Rendering/gl_model.h"

#include "OpenGLES/OpenGLWrapper.h"

void GL_BeginRendering(int *x, int *y, int *width, int *height);
void GL_EndRendering();

extern int texture_extension_number;

extern float gldepthmin, gldepthmax;

int GL_LoadTexture(char *identifier, int width, int height, byte *data, qboolean mipmap, qboolean alpha);
int GL_FindTexture(char *identifier);

extern int glx, gly, glwidth, glheight;

// r_local.h -- private refresh defs

#define ALIAS_BASE_SIZE_RATIO (1.0f / 11.0f)
// normalizing factor so player model works out to about
//  1 pixel per triangle
#define MAX_LBM_HEIGHT 480

#define TILE_SIZE 128 // size of textures generated by R_GenTiledSurf

#define SKYSHIFT 7
#define SKYSIZE (1 << SKYSHIFT)
#define SKYMASK (SKYSIZE - 1)

#define BACKFACE_EPSILON 0.01f

void R_TimeRefresh_f();
void R_ReadPointFile_f();
texture_t* R_TextureAnimation(texture_t *base);

typedef enum
{
	pt_static, pt_grav, pt_slowgrav, pt_fire, pt_explode, pt_explode2, pt_blob, pt_blob2
} ptype_t;

typedef struct particle_s
{
	// driver-usable fields
	vec3_t org;
	float color;
	// drivers never touch the following fields
	struct particle_s *next;
	vec3_t vel;
	float ramp;
	float die;
	ptype_t type;
} particle_t;

//====================================================

extern entity_t r_worldentity;
extern vec3_t modelorg;
extern entity_t *currententity;
extern int r_visframecount; // ??? what difs?
extern int r_framecount;
extern mplane_t frustum[4];
extern int c_brush_polys, c_alias_polys;

//
// view origin
//
extern vec3_t vup;
extern vec3_t vpn;
extern vec3_t vright;
extern vec3_t r_origin;

//
// screen size info
//
extern refdef_t r_refdef;
extern mleaf_t *r_viewleaf, *r_oldviewleaf;
extern texture_t *r_notexture_mip;
extern int d_lightstylevalue[256]; // 8.8 fraction of base light value

extern qboolean envmap;
extern int currenttexture;
extern int cnttextures[2];
extern int particletexture;
extern int playertextures;

extern int skytexturenum; // index in cl.loadmodel, not gl texture object

extern cvar_t vid_fullscreen;

extern cvar_t r_norefresh;
extern cvar_t r_drawentities;
extern cvar_t r_drawworld;
extern cvar_t r_drawviewmodel;
extern cvar_t r_waterwarp;
extern cvar_t r_fullbright;
extern cvar_t r_lightmap;
extern cvar_t r_shadows;
extern cvar_t r_mirroralpha;
extern cvar_t r_wateralpha;
extern cvar_t r_dynamic;
extern cvar_t r_novis;

extern cvar_t gl_clear;
extern cvar_t gl_cull;
extern cvar_t gl_texsort;
extern cvar_t gl_smoothmodels;
extern cvar_t gl_affinemodels;
extern cvar_t gl_polyblend;
extern cvar_t gl_keeptjunctions;
extern cvar_t gl_reporttjunctions;
extern cvar_t gl_flashblend;
extern cvar_t gl_nocolors;
extern cvar_t gl_doubleeyes;
extern cvar_t gl_ztrick;

extern cvar_t gl_particle_min_size;
extern cvar_t gl_particle_max_size;
extern cvar_t gl_particle_size;
extern cvar_t gl_particle_att_a;
extern cvar_t gl_particle_att_b;
extern cvar_t gl_particle_att_c;

extern cvar_t gl_max_size;
extern cvar_t gl_playermip;

extern int mirrortexturenum; // quake texturenum, not gltexturenum
extern qboolean mirror;
extern mplane_t *mirror_plane;

extern float r_world_matrix[16];

extern const GLubyte *gl_vendor;
extern const GLubyte *gl_renderer;
extern const GLubyte *gl_version;
extern const GLubyte *gl_extensions;

void R_TranslatePlayerSkin(int playernum);
void GL_Bind(int texnum);

extern qboolean gl_mtexable;

void GL_DisableMultitexture();
void GL_EnableMultitexture();

#endif