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

#ifndef TDL_BUFFER_H
#define TDL_BUFFER_H

#include "tdl/tdl_row.h"
#include "tdl/tdl_geometry.h"

typedef tdl_row_t *tdl_buffer_t;

tdl_buffer_t tdl_buffer (tdl_size_t size);
bool tdl_buffer_resize (tdl_buffer_t *buffer, tdl_size_t newsize);
bool tdl_buffer_copy (tdl_buffer_t *dest, tdl_buffer_t *src);
void tdl_buffer_clear_row (tdl_buffer_t buff, size_t row_n);
void tdl_buffer_clear (tdl_buffer_t buff);
tdl_size_t tdl_buffer_size (tdl_buffer_t *buffer);
void tdl_buffer_free (tdl_buffer_t buffer);

#endif /* TDL_BUFFER_T */
