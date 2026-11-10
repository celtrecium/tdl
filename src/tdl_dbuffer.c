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

#include <tdl/tdl_dbuffer.h>

tdl_dbuffer_t
tdl_dbuffer (tdl_size_t size)
{
  return (tdl_dbuffer_t)
    {
      .main = tdl_buffer (size),
      .shadow = tdl_buffer (size),
      .size = size,
      .diff = tdl_dbufdiff (size.height)
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

  tdl_buffer_resize (&dbuff->main, newsize);
  tdl_buffer_resize (&dbuff->shadow, newsize);
  tdl_dbufdiff_resize (&dbuff->diff, newsize.height);

  dbuff->size = newsize;
  
  return true;
}

bool
tdl_dbuffer_is_char_mod (tdl_dbuffer_t *dbuff, tdl_point_t point)
{
  return !tdl_char_compare (&(dbuff->main[point.y][point.x]),
                            &(dbuff->shadow[point.y][point.x]));
}

void
tdl_dbuffer_set_char (tdl_dbuffer_t *dbuff, tdl_point_t point,
                      tdl_char_t tchar)
{
  tdl_row_set_clear (dbuff->main[point.y], false);

  dbuff->main[point.y][point.x] = tchar;

  if (tdl_dbuffer_is_char_mod (dbuff, point))
    tdl_rowdiff_set(&(dbuff->diff[point.y]), (size_t) point.x);
}

void
tdl_dbuffer_throw_shadow (tdl_dbuffer_t *dbuff)
{
  tdl_buffer_copy (&(dbuff->shadow), &(dbuff->main));
}

void
tdl_dbuffer_mark_row_as_changed (tdl_dbuffer_t *dbuff, size_t row_n)
{
  tdl_row_set_clear (dbuff->main[row_n], false);

  dbuff->diff[row_n] =
    tdl_rowdiff_with (0, tdl_row_size (dbuff->main[row_n]) - 1);
}

void tdl_dbuffer_insert_rows_up (tdl_dbuffer_t *dbuff, tdl_point_t cur,
                                 tdl_scroll_reg_t reg, size_t n_rows) {
  int i;

  if ((size_t) cur.y - n_rows < reg.top)
    n_rows = (size_t) cur.y - reg.top;

  tdl_buffer_insert_rows_up (dbuff->main, cur, reg, n_rows);

  for (i = cur.y; i >= (int) reg.top; --i)
    tdl_dbuffer_mark_row_as_changed (dbuff, (size_t) i);
}

void tdl_dbuffer_insert_rows_down (tdl_dbuffer_t *dbuff, tdl_point_t cur,
                                   tdl_scroll_reg_t reg, size_t n_rows) {
  size_t i;

  if (reg.top + n_rows - 1 > reg.bottom)
    n_rows = reg.bottom - reg.top;

  tdl_buffer_insert_rows_down (dbuff->main, cur, reg, n_rows);
  
  for (i = (size_t) cur.y; i <= reg.bottom; ++i)
    tdl_dbuffer_mark_row_as_changed (dbuff, i);
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

  tdl_dbuffer_mark_row_as_changed (dbuff, row_n);
}

void
tdl_dbuffer_clear (tdl_dbuffer_t *dbuff)
{
  size_t i;

  for (i = 0; i < dbuff->size.height; ++i)
    tdl_dbuffer_clear_row (dbuff, i);
}

static void
_clarify_rdiff_first (tdl_dbuffer_t *dbuff, size_t row_n)
{
  size_t i;
  tdl_point_t curr_pt;
  tdl_rowdiff_t *rowdiff = &(dbuff->diff[row_n]);
  
  for (i = rowdiff->first_modified; i <= rowdiff->last_modified; ++i)
    {
      curr_pt = (tdl_point_t) { .x = (int) i, .y = (int) row_n };

      if (tdl_dbuffer_is_char_mod (dbuff, curr_pt))
        break;
    }

  rowdiff->first_modified = i;
}

static void
_clarify_rdiff_last (tdl_dbuffer_t *dbuff, size_t row_n)
{
  size_t i;
  tdl_point_t curr_pt;
  tdl_rowdiff_t *rowdiff = &(dbuff->diff[row_n]);

  for (i = rowdiff->last_modified; i >= rowdiff->first_modified; --i)
    {
      curr_pt = (tdl_point_t) { .x = (int) i, .y = (int) row_n };

      if (tdl_dbuffer_is_char_mod (dbuff, curr_pt))
        break;
    }

  rowdiff->last_modified = i;
}

static void
_clarify_rowdiff (tdl_dbuffer_t *dbuff, size_t row_n)
{
  tdl_rowdiff_t *rowdiff = &(dbuff->diff[row_n]);

  if (rowdiff->first_modified >= dbuff->size.width)
    rowdiff->first_modified = dbuff->size.width - 1;

  if (rowdiff->last_modified >= dbuff->size.width)
    rowdiff->last_modified = dbuff->size.width - 1;

  _clarify_rdiff_first (dbuff, row_n);
  _clarify_rdiff_last(dbuff, row_n);
}

void
tdl_dbuffer_clarify_diff (tdl_dbuffer_t *dbuff)
{
  size_t i;
  size_t count = tdl_dbufdiff_size (dbuff->diff);
  
  for (i = 0; i < count; ++i)
    if (dbuff->diff[i].was_modified)
      _clarify_rowdiff (dbuff, i);
}

void
tdl_dbuffer_insert_blank_char (tdl_dbuffer_t *dbuff, tdl_point_t point)
{
  if (tdl_buffer_insert_blank_char (dbuff->main, point))
      dbuff->diff[point.y] =
        tdl_rowdiff_with ((size_t) point.x, dbuff->size.width-1);
}

void
tdl_dbuffer_delete_char (tdl_dbuffer_t *dbuff, tdl_point_t point)
{
  if (tdl_buffer_delete_char (dbuff->main, point))
      dbuff->diff[point.y] =
        tdl_rowdiff_with ((size_t) point.x, dbuff->size.width-1);
}

