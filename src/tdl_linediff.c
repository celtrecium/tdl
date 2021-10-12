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

#include "tdl/tdl_linediff.h"
#include "tdl/tdl_objects.h"
#include "tdl/tdl_buffer.h"
#include "tdl/tdl_geometry.h"
#include "tdl/tdl_bufferline.h"
#include "tdl/tdl_bufferpoint.h"
#include <sbvector.h>
#include <stdlib.h>

tdl_ldiff_t
tdl_ldiff (size_t line, size_t first_m, size_t last_m)
{
  tdl_ldiff_t ldiff;

  ldiff.first_modified = first_m;
  ldiff.last_modified = last_m;
  ldiff.line_number = line;

  return ldiff;
}

bool
tdl_ldiff_set (tdl_ldiff_t *ldiff, size_t modified)
{
  if (!ldiff)
    return false;

  if (modified > ldiff->last_modified)
    ldiff->last_modified = modified;
  else if (modified < ldiff->first_modified)
    ldiff->first_modified = modified;

  return true;
}

static inline size_t
_tdl_clarify_first_modified (tdl_ldiff_t *ldiff, tdl_buffer_line_t **buff_lines)
{
  size_t i = 0;

  for (i = ldiff->first_modified; i <= ldiff->last_modified; ++i)
    {
      if (!tdl_buffpt_compare(tdl_buffer_line_get (buff_lines[0], i),
                              tdl_buffer_line_get (buff_lines[1], i)))
        break;
    }

  return i;
}

static inline size_t
_tdl_clarify_last_modified (tdl_ldiff_t *ldiff, tdl_buffer_line_t **buff_lines)
{
  size_t i = 0;
  size_t last = 0;

  for (i = ldiff->first_modified; i <= ldiff->last_modified; ++i)
    {
      if (!tdl_buffpt_compare(tdl_buffer_line_get (buff_lines[0], i),
                              tdl_buffer_line_get (buff_lines[1], i)))
        last = i;
    }

  return last;
}

bool
tdl_ldiff_clarify_line_edges (tdl_ldiff_t *ldiff, tdl_buffer_t *buff)
{
  tdl_buffer_line_t *buff_lines[2]
      = { sbv_get (&buff->fbuff, tdl_buffer_line_t, ldiff->line_number),
          sbv_get (&buff->sbuff, tdl_buffer_line_t, ldiff->line_number) };

  if (!ldiff || !buff)
    return false;

  ldiff->first_modified = _tdl_clarify_first_modified (ldiff, buff_lines);
  ldiff->last_modified = _tdl_clarify_last_modified (ldiff, buff_lines);

  if (ldiff->first_modified > ldiff->last_modified)
    buff_lines[0]->_is_empty = buff_lines[1]->_is_empty = true;
  
  return true;
}
