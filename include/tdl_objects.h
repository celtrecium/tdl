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


#ifndef TDL_OBJECTS_H
#define TDL_OBJECTS_H

#include <stdio.h>
#include <stdlib.h>

typedef unsigned int tdl_textattr_t;
typedef unsigned int tdl_color_t;
typedef char tdl_utf8char_t[4];

typedef enum tdl_error
{
  TDL_OK,
  TDL_NULL_PTR_ERR
} tdl_error_t;

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
  tdl_point_t points[2];
} tdl_line_t;

typedef struct tdl_rectangle
{
  tdl_point_t point;
  tdl_size_t size;
} tdl_rectangle_t;

typedef struct tdl_point_color
{
  tdl_color_t bg;
  tdl_color_t fg;
} tdl_point_color_t;

typedef struct tdl_buffer_point
{
  tdl_utf8char_t character;
  tdl_textattr_t attributes;
  tdl_point_color_t color;
} tdl_buffer_point_t;

typedef struct tdl_line_diff
{
  size_t line_number;
  size_t first_modified;
  size_t last_modified;
} tdl_line_diff_t;

typedef struct tdl_canvas
{
  tdl_point_color_t color;
  tdl_point_t cursor;
  tdl_textattr_t text_attrs;
  tdl_size_t size;
  tdl_error_t err;
  tdl_buffer_point_t **buffer;
  tdl_line_diff_t *diff;
} tdl_canvas_t;

#endif /* TDL_H */
