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

#ifndef TDL_STYLE_H
#define TDL_STYLE_H

#include <stdlib.h>
#include <stdbool.h>
#include "tdl_symbols_export.h"

#define LAST_STYLES_ARG 17

#define TDL_BLACK 0
#define TDL_RED 1
#define TDL_GREEN 2
#define TDL_YELLOW 3
#define TDL_BLUE 4
#define TDL_MAGENTA 5
#define TDL_CYAN 6
#define TDL_WHITE 7
#define TDL_GREY 8
#define TDL_BRIGHT_RED 9
#define TDL_BRIGHT_GREEN 10
#define TDL_BRIGHT_YELLOW 11
#define TDL_BRIGHT_BLUE 12
#define TDL_BRIGHT_MAGENTA 13
#define TDL_BRIGHT_CYAN 14
#define TDL_BRIGHT_WHITE 15

typedef enum tdl_attributes
{
  TDL_NO_ATTRIBUTES = 0,
  TDL_BOLD =          1,
  TDL_ITALIC =        1 << 1,
  TDL_UNDERLINE =     1 << 2,
  TDL_CROSSED_OUT =   1 << 3,
  TDL_DIM =           1 << 4
} tdl_attributes_t;

typedef unsigned int tdl_color_t;

typedef struct tdl_point_color
{
  tdl_color_t bg, fg;
} tdl_point_color_t;

typedef struct tdl_style
{
  tdl_attributes_t attributes;
  tdl_point_color_t color;
} tdl_style_t;

TDL_API tdl_point_color_t tdl_point_color (tdl_color_t bg, tdl_color_t fg);
TDL_API tdl_attributes_t __tdl_attributes_intern (tdl_attributes_t attrs, ...);
TDL_API tdl_style_t tdl_style (tdl_point_color_t color, tdl_attributes_t attrs);
TDL_API bool tdl_style_compare (tdl_style_t *first, tdl_style_t *second);

#define tdl_attributes(...) \
  __tdl_attributes_intern (__VA_ARGS__, LAST_STYLES_ARG)

#endif  /* TDL_STYLE_H */
