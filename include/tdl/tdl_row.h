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


#ifndef TDL_ROW_H
#define TDL_ROW_H

#include "tdl/tdl_symbols_export.h"
#include "tdl/tdl_char.h"

typedef tdl_char_t* tdl_row_t;

tdl_row_t tdl_row (size_t width);

size_t tdl_row_size (tdl_row_t bufstr);
bool tdl_row_resize (tdl_row_t *bufstr, size_t newsize);
void tdl_row_set_clear (tdl_row_t bufstr, bool is_clear);
bool tdl_row_is_clear (tdl_row_t bufstr);
void tdl_row_clear (tdl_row_t row);
void tdl_row_free (tdl_row_t bufstr);
bool tdl_row_copy (tdl_row_t *dest, tdl_row_t *src);

#endif /* TDL_ROW_H */
