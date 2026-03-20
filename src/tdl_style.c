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

#include "tdl/tdl_style.h"
#include <stdarg.h>
#include <string.h>

tdl_char_color_t
tdl_char_color (tdl_color_t bg, tdl_color_t fg)
{
  tdl_char_color_t pcolor;

  pcolor.bg = bg;
  pcolor.fg = fg;

  pcolor.bg_rgb = tdl_rgb(0x00, 0x00, 0x00);
  pcolor.fg_rgb = tdl_rgb(0x00, 0x00, 0x00);
  
  return pcolor;
}

tdl_attributes_t
__tdl_attributes_intern (tdl_attributes_t attrs, ...)
{
  tdl_attributes_t retattrs = attrs;
  tdl_attributes_t attr = 0;
  va_list lst;

  va_start (lst, attrs);
  
  while (attr != LAST_STYLES_ARG)
    {
      retattrs |= attr;
      attr = va_arg (lst, tdl_attributes_t);
    }
    
  va_end (lst);
  
  return retattrs;
}

tdl_style_t
tdl_style (tdl_char_color_t color, tdl_attributes_t attrs)
{
  tdl_style_t style;

  memset (&style, 0, sizeof (tdl_style_t));

  style.attributes = attrs;
  style.color = color;

  return style;
}

bool
tdl_style_compare (tdl_style_t *first, tdl_style_t *second)
{
  if (!first && !second)
    return true;
  else if (!first || !second)
    return false;

  bool a1 = first->color.bg == TDL_RGB_COLOR && second->color.bg == TDL_RGB_COLOR;
  bool a2 = a1 && (tdl_rgb_compare(first->color.bg_rgb, second->color.bg_rgb));
  bool a3 = first->color.fg == TDL_RGB_COLOR && second->color.fg == TDL_RGB_COLOR;
  bool a4 = a3 && (tdl_rgb_compare(first->color.fg_rgb, second->color.fg_rgb));
  
  return first->attributes == second->attributes
    && ((first->color.bg == second->color.bg) || a2)
    && ((first->color.fg == second->color.fg) || a4);
}

tdl_rgb_t
tdl_rgb(uint8_t r, uint8_t g, uint8_t b)
{
  return (tdl_rgb_t) { r, g, b };
}

tdl_char_color_t
tdl_char_color_rgb (tdl_rgb_t bg, tdl_rgb_t fg)
{
  return (tdl_char_color_t) {
    .bg = TDL_RGB_COLOR,
    .fg = TDL_RGB_COLOR,
    .bg_rgb = bg,
    .fg_rgb = fg
  };
}

tdl_char_color_t
tdl_char_color_rgb_bg (tdl_rgb_t bg, tdl_color_t fg)
{
  return (tdl_char_color_t) {
    .bg = TDL_RGB_COLOR,
    .fg = fg,
    .bg_rgb = bg,
    .fg_rgb = { 0x00, 0x00, 0x00 }
  };
}

tdl_char_color_t
tdl_char_color_rgb_fg (tdl_color_t bg, tdl_rgb_t fg)
{
  return (tdl_char_color_t) {
    .bg = bg,
    .fg = TDL_RGB_COLOR,
    .bg_rgb = { 0x00, 0x00, 0x00 },
    .fg_rgb = fg
  };
}

bool
tdl_rgb_compare (tdl_rgb_t first, tdl_rgb_t second)
{
  return first.r == second.r
    && first.g == second.g
    && first.b == second.b;
}
