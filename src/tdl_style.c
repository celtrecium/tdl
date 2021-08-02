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
#include "tdl/tdl_objects.h"
#include <stdarg.h>
#include <string.h>

tdl_point_color_t
tdl_point_color (tdl_color_t bg, tdl_color_t fg)
{
  tdl_point_color_t pcolor;

  pcolor.bg = bg;
  pcolor.fg = fg;

  return pcolor;
}

tdl_attributes_t
__tdl_attributes_intern (tdl_attributes_t attrs, ...)
{
  tdl_attributes_t retattrs = attrs;
  tdl_attributes_t attr = 0;
  va_list lst;

  va_start (lst, attrs);
  
  while (attr != _LAST_STYLES_ARG)
    {
      retattrs |= attr;
      attr = va_arg (lst, tdl_attributes_t);
    }
    
  va_end (lst);
  
  return retattrs;
}

tdl_style_t
tdl_style (tdl_point_color_t color, tdl_attributes_t attrs)
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
  if (first == NULL && second == NULL)
    return true;
  else if (first == NULL || second == NULL)
    return false;

  if (memcmp (first, second, sizeof (tdl_style_t)) == 0)
    return true;
  else
    return false;
}
