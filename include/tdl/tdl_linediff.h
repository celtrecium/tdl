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

#ifndef TDL_LINEDIFF_H
#define TDL_LINEDIFF_H

#include <stdlib.h>
#include <stdbool.h>
#include "tdl_buffer.h"
#include "tdl_symbols_export.h"

typedef struct tdl_ldiff
{
  size_t line_number;
  size_t first_modified;
  size_t last_modified;
} tdl_ldiff_t;

tdl_ldiff_t tdl_ldiff (size_t line, size_t first_m, size_t last_m);
bool tdl_ldiff_set (tdl_ldiff_t *ldiff, size_t modified);
bool tdl_ldiff_clarify_line_edges (tdl_ldiff_t *ldiff, tdl_buffer_t *buff);

#endif  /* TDL_LINEDIFF_H */
