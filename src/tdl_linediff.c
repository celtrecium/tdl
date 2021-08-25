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
#include <sbvector.h>
#include <stdlib.h>

static inline tdl_buffer_point_t *
_tdl_internal_buffer_get_point (sbvector_t *buff, tdl_point_t point)
{
  return tdl_buffer_line_get (
      sbv_get (buff, tdl_buffer_line_t, (size_t)point.y), (size_t)point.x);
}

#define tdl_def_clarify_func(modified, beg_value, expr, operation)            \
  static inline size_t _tdl_clarify_##modified##_modified_point (             \
      tdl_ldiff_t *ldiff, tdl_buffer_t *buff)                                 \
  {                                                                           \
    tdl_point_t point                                                         \
      = tdl_point ((int)beg_value, (int)ldiff->line_number);                  \
                                                                              \
    for (; expr; operation)                                                   \
      {                                                                       \
        if (_tdl_internal_buffer_get_point (&buff->fbuff, point)              \
            != _tdl_internal_buffer_get_point (&buff->sbuff, point))          \
          return (size_t)point.x;                                             \
      }                                                                       \
    return (size_t)point.x;                                                   \
  }

tdl_def_clarify_func (first, ldiff->first_modified,
                      point.x < (int)ldiff->last_modified, ++point.x)
tdl_def_clarify_func (last, ldiff->last_modified,
                      point.x != (int)ldiff->first_modified, --point.x)

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
  if (ldiff == NULL)
    return false;

  if (modified > ldiff->last_modified)
    ldiff->last_modified = modified;
  else if (modified < ldiff->first_modified)
    ldiff->first_modified = modified;

  return true;
}

bool
tdl_ldiff_clarify_line_edges (tdl_ldiff_t *ldiff, tdl_buffer_t *buff)
{
  if (!ldiff || !buff)
    return false;
  
  ldiff->first_modified = _tdl_clarify_first_modified_point (ldiff, buff);
  ldiff->last_modified  = _tdl_clarify_last_modified_point (ldiff, buff);

  return true;
}
