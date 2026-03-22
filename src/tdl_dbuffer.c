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

#include "tdl/tdl_dbuffer.h"
#include "tdl/tdl_dbufdiff.h"
#include "tdl/tdl_geometry.h"
#include "tdl/tdl_row.h"
#include "tdl/tdl_rowdiff.h"
#include "tdl/tdl_style.h"

tdl_dbuffer_t
tdl_dbuffer (tdl_size_t size)
{
  return (tdl_dbuffer_t)
    {
      .main = tdl_buffer (size),
      .shadow = tdl_buffer (size),
      .size = size,
      .diff = tdl_dbufdiff ()
    };
}

void
tdl_dbuffer_free (tdl_dbuffer_t *dbuff)
{
  tdl_buffer_free (dbuff->main);
  tdl_buffer_free (dbuff->shadow);
  tdl_dbufdiff_free(dbuff->diff);

  dbuff->size = (tdl_size_t) { 0, 0 };
}

bool
tdl_dbuffer_resize (tdl_dbuffer_t *dbuff, tdl_size_t newsize)
{
  if (dbuff == NULL)
    return false;

  if (!tdl_buffer_resize (&dbuff->main, newsize))
    return false;
  
  if (!tdl_buffer_resize (&dbuff->shadow, newsize))
    return false;

  dbuff->size = newsize;
  
  return true;
}

bool
tdl_dbuffer_is_char_mod (tdl_dbuffer_t *dbuff, tdl_point_t point)
{
  return tdl_char_compare(&dbuff->main[point.y][point.x],
			  &dbuff->shadow[point.y][point.x]);
}

void
tdl_dbuffer_set_char (tdl_dbuffer_t *dbuff, tdl_point_t point,
                      tdl_char_t tchar)
{
  tdl_row_set_clear (dbuff->main[point.y], false);

  dbuff->main[point.y][point.x] = tchar;

  if (tdl_dbuffer_is_char_mod (dbuff, point))
    tdl_dbufdiff_set (&dbuff->diff, (tdl_rowdiff_t) {
	.row_number = (size_t) point.y,
	.first_modified = (size_t) point.x,
	.last_modified = (size_t) point.x
      });
}

void
tdl_dbuffer_throw_shadow (tdl_dbuffer_t *dbuff)
{
  tdl_buffer_copy (&dbuff->shadow, &dbuff->main);
}

void
tdl_dbuffer_clear_row (tdl_dbuffer_t *dbuff, size_t row_n)
{
  tdl_char_t empty_char = tdl_char (" ", tdl_default_style);
  size_t i;
  size_t w = tdl_row_size (dbuff->main[row_n]);

  tdl_row_set_clear (dbuff->main[row_n], true);
  
  for (i = 0; i < w; ++i)
    dbuff->main[row_n][i] = empty_char;

  tdl_dbufdiff_set (&dbuff->diff, (tdl_rowdiff_t) {
      .row_number = row_n,
      .first_modified = 0,
      .last_modified = w
    });
}

void
tdl_dbuffer_clear (tdl_dbuffer_t *dbuff)
{
  size_t i;

  for (i = 0; i < dbuff->size.height; ++i)
    tdl_dbuffer_clear_row (dbuff, i);
}

static tdl_rowdiff_t
_clarify_rdiff_first (tdl_dbuffer_t *dbuff, tdl_rowdiff_t rowdiff)
{
  size_t i;
  tdl_point_t curr_pt;

  for (i = rowdiff.first_modified; i <= rowdiff.last_modified; ++i)
    {
      curr_pt = (tdl_point_t) { .x = (int) i, .y = (int) rowdiff.row_number };

      if (tdl_dbuffer_is_char_mod (dbuff, curr_pt))
	break;
    }

  rowdiff.first_modified = i;

  return rowdiff;
}

static tdl_rowdiff_t
_clarify_rdiff_last (tdl_dbuffer_t *dbuff, tdl_rowdiff_t rowdiff)
{
  size_t i;
  tdl_point_t curr_pt;

  for (i = rowdiff.last_modified; i >= rowdiff.first_modified; --i)
    {
      curr_pt = (tdl_point_t) { .x = (int) i, .y = (int) rowdiff.row_number };

      if (tdl_dbuffer_is_char_mod (dbuff, curr_pt))
	break;
    }

  rowdiff.last_modified = i;

  return rowdiff;
}

static tdl_rowdiff_t
_clarify_rowdiff (tdl_dbuffer_t *dbuff, tdl_rowdiff_t rowdiff)
{
  return _clarify_rdiff_last(dbuff, _clarify_rdiff_first (dbuff, rowdiff));
}

void
tdl_dbuffer_clarify_diff (tdl_dbuffer_t *dbuff)
{
  size_t i;
  size_t diffs_count = tdl_dbufdiff_size (dbuff->diff);
  
  for (i = 0; i < diffs_count; ++i) {
    dbuff->diff[i] = _clarify_rowdiff (dbuff, dbuff->diff[i]);
  }
}
