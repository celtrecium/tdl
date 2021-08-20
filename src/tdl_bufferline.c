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

#include "tdl/tdl_bufferline.h"
#include "tdl/tdl_bufferpoint.h"
#include "tdl/tdl_style.h"
#include "tdl/tdl_buffer.h"
#include "tdl/tdl_objects.h"
#include <sbvector.h>
#include <stdbool.h>

tdl_buffer_line_t
tdl_buffer_line (size_t width)
{
  tdl_buffer_line_t bl = { sbvector (sizeof (tdl_buffer_point_t)), true };
  tdl_buffer_point_t bpt;
  
  if (bl.line.vector)
    {
      sbv_resize (&bl.line, width);

      bpt = tdl_buffer_point (" ",
                              tdl_style (tdl_point_color (256, 256),
                                         tdl_attributes (TDL_NO_ATTRIBUTES)));

      sbv_fill (&bl.line, &bpt, bl.line.length);
    }

  return bl;
}

tdl_buffer_point_t *
tdl_buffer_line_get (tdl_buffer_line_t *line, size_t index)
{  
  if (!line)
    return NULL;

  return sbv_get (&line->line, tdl_buffer_point_t, index);
}

bool
tdl_buffer_line_free (tdl_buffer_line_t *line)
{
  if (!line || !sbv_free (&line->line))
    return false;

  return true;
}

bool
tdl_buffer_line_copy (tdl_buffer_line_t *dest, tdl_buffer_line_t *src)
{
  if (!dest || !src || !sbv_copy (&dest->line, &src->line))
    return false;
  
  return true;
}
