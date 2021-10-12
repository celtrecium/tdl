/*
 * This file is part of Text Drawing Library.
 *
 * Text Drawing Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Text Drawing Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Text Drawing Library. If not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef TDL_GEOMETRY_H
#define TDL_GEOMETRY_H

#include <stdlib.h>
#include "tdl_symbols_export.h"

typedef struct tdl_point
{
  int x;
  int y;
} tdl_point_t;

typedef struct tdl_size
{
  size_t width;
  size_t height;
} tdl_size_t;

typedef struct tdl_line
{
  tdl_point_t a, b;
} tdl_line_t;

typedef struct tdl_rectangle
{
  tdl_point_t point;
  tdl_size_t size;
} tdl_rectangle_t;

TDL_API tdl_line_t tdl_line (tdl_point_t point_a, tdl_point_t point_b);
TDL_API tdl_rectangle_t tdl_rectangle (tdl_point_t point, tdl_size_t size);
TDL_API tdl_point_t tdl_point (int x, int y);
TDL_API tdl_size_t tdl_size (size_t width, size_t height);

#endif  /* TDL_GEOMETRY_H */
