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


#ifndef TDL_DBUFFER_H
#define TDL_DBUFFER_H

#include "tdl/tdl_symbols_export.h"
#include "tdl/tdl_buffer.h"
#include "tdl/tdl_dbufdiff.h"

typedef struct tdl_dbuffer
{
  tdl_buffer_t main;
  tdl_buffer_t shadow;
  tdl_dbufdiff_t diff;
  tdl_size_t size;
} tdl_dbuffer_t;

tdl_dbuffer_t tdl_dbuffer (tdl_size_t size);
void tdl_dbuffer_free (tdl_dbuffer_t *dbuff);
bool tdl_dbuffer_resize (tdl_dbuffer_t *dbuff, tdl_size_t newsize);

void tdl_dbuffer_set_char (tdl_dbuffer_t *dbuff, tdl_point_t point,
			   tdl_char_t tchar);
void tdl_dbuffer_clear_row (tdl_dbuffer_t *dbuff, size_t row_n);
void tdl_dbuffer_clear (tdl_dbuffer_t *dbuff);
bool tdl_dbuffer_is_char_mod (tdl_dbuffer_t *dbuff, tdl_point_t point);
void tdl_dbuffer_throw_shadow (tdl_dbuffer_t *dbuff);
void tdl_dbuffer_clarify_diff (tdl_dbuffer_t *dbuff);

#endif  /* TDL_DBUFFER_H */
