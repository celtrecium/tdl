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
#include <u8string.h>
#include <sbvector.h>

typedef unsigned int tdl_color_t;

typedef enum tdl_attributes
{
  TDL_NO_ATTRIBUTES = 0,
  TDL_BOLD =          1,
  TDL_ITALIC =        1 << 1,
  TDL_UNDERLINE =     1 << 2,
  TDL_CROSSED_OUT =   1 << 3,
  TDL_DIM =           1 << 4
} tdl_attributes_t;

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
  tdl_point_t a, b;
} tdl_line_t;

typedef struct tdl_rectangle
{
  tdl_point_t point;
  tdl_size_t size;
} tdl_rectangle_t;

typedef struct tdl_point_color
{
  tdl_color_t bg, fg;
} tdl_point_color_t;

typedef struct tdl_style
{
  tdl_attributes_t attributes;
  tdl_point_color_t color;
} tdl_style_t;

typedef struct tdl_buffer_point
{
  u8char_t character;
  tdl_style_t style;
} tdl_buffer_point_t;

typedef struct tdl_ldiff
{
  size_t line_number;
  size_t first_modified;
  size_t last_modified;
} tdl_ldiff_t;

typedef struct tdl_text
{
  u8string_t *string;
  tdl_style_t style;
} tdl_text_t;

typedef struct tdl_buffer
{
  sbvector_t fbuff; /* First buffer */
  sbvector_t sbuff; /* Second buffer */
  tdl_size_t size;
} tdl_buffer_t;

typedef struct tdl_buffer_line
{
  sbvector_t line;
  bool _is_empty;
} tdl_buffer_line_t;

typedef struct tdl_canvas
{
  tdl_point_t cursor;
  tdl_size_t size;
  tdl_buffer_t buffer;
  sbvector_t diff;
  tdl_error_t err;
} tdl_canvas_t;

#endif /* TDL_H */
