/* EINA - EFL data type library
 * Copyright (C) 2002-2009 Rafael Antognolli
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library;
 * if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EINA_TILER_INLINE_H_
#define EINA_TILER_INLINE_H_

#include "eina_safety_checks.h"

/**
 * @cond LOCAL
 * This struct should not be accessed directly, it is used by
 * eina_tile_grid_slicer functions to maintain context and fill "info"
 * member with correct values for given iteration.
 */
struct _Eina_Tile_Grid_Slicer
{
   unsigned long col1, col2, row1, row2; // initial and final col,row
   int tile_w, tile_h; // tile width, height
   int x_rel, y_rel; // starting x,y coordinates of the first col,row
   int w1_rel, h1_rel; // width,height of the first col,row
   int w2_rel, h2_rel; // width,height of the last col,row
   struct Eina_Tile_Grid_Info info; // info about the current tile
   Efl_Bool first;
};

/**
 * @endcond
 */

static inline Efl_Bool
eina_tile_grid_slicer_next(Eina_Tile_Grid_Slicer *slc, const Eina_Tile_Grid_Info **rect)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(slc, 0);

   if (slc->first)
     {
	slc->first = 0;
	*rect = &slc->info;
	return EFL_TRUE;
     }

   slc->info.col++;

   if (slc->info.col > slc->col2)
     {
	slc->info.row++;
	if (slc->info.row > slc->row2)
	  return EFL_FALSE;
	else if (slc->info.row < slc->row2)
	  slc->info.rect.h = slc->tile_h;
	else
	  slc->info.rect.h = slc->h2_rel;
	slc->info.rect.y = 0;
	slc->info.col = slc->col1;
	slc->info.rect.x = slc->x_rel;
	slc->info.rect.w = slc->w1_rel;
     }
   else
     {
	slc->info.rect.x = 0;
	if (slc->info.col < slc->col2)
	  slc->info.rect.w = slc->tile_w;
	else
	  slc->info.rect.w = slc->w2_rel;
     }

   if (slc->info.rect.w == slc->tile_w && slc->info.rect.h == slc->tile_h)
     slc->info.full = EFL_TRUE;
   else
     slc->info.full = EFL_FALSE;

   *rect = &slc->info;

   return EFL_TRUE;
}

static inline Efl_Bool
eina_tile_grid_slicer_setup(Eina_Tile_Grid_Slicer *slc, int x, int y, int w, int h, int tile_w, int tile_h)
{
   int tx1, tx2, ty1, ty2;

   EINA_SAFETY_ON_NULL_RETURN_VAL(slc, 0);

   tx1 = x;
   ty1 = y;
   tx2 = x + w - 1;
   ty2 = y + h - 1;

   if (x < 0 || y < 0 || w <= 0 || h <= 0 || tile_w <= 0 || tile_h <= 0)
     {
	slc->first = 0;
	slc->col1 = slc->row1 = 0;
	slc->col2 = slc->row2 = 0;
	slc->info.col = slc->col1;
	slc->info.row = slc->row1;
	return EFL_TRUE;
     }

   slc->col1 = tx1 / tile_w;
   slc->row1 = ty1 / tile_h;
   slc->col2 = (tx2 - 0) / tile_w;
   slc->row2 = (ty2 - 0) / tile_h;
   slc->x_rel = tx1 % tile_w;
   slc->y_rel = ty1 % tile_h;
   slc->w1_rel = tile_w - slc->x_rel;
   slc->h1_rel = tile_h - slc->y_rel;
   slc->w2_rel = tx2 % tile_w + 1;
   slc->h2_rel = ty2 % tile_h + 1;

   slc->tile_w = tile_w;
   slc->tile_h = tile_h;

   slc->first = 1;
   slc->info.col = slc->col1;
   slc->info.row = slc->row1;
   slc->info.rect.x = slc->x_rel;
   slc->info.rect.y = slc->y_rel;

   if (slc->info.col == slc->col2)
     slc->w1_rel = slc->w2_rel - slc->x_rel;

   if (slc->info.row == slc->row2)
     slc->h1_rel = slc->h2_rel - slc->y_rel;

   slc->info.rect.w = slc->w1_rel;
   slc->info.rect.h = slc->h1_rel;

   if (slc->info.rect.w == slc->tile_w && slc->info.rect.h == slc->tile_h)
     slc->info.full = EFL_TRUE;
   else
     slc->info.full = EFL_FALSE;

   return EFL_TRUE;
}

#endif
