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

#ifndef TDL_ROWDIFF_H
#define TDL_ROWDIFF_H

#include <stdlib.h>
#include <stdbool.h>
#include <tdl/tdl_symbols_export.h>

typedef struct tdl_rowdiff
{
  bool was_modified;
  size_t first_modified;
  size_t last_modified;
} tdl_rowdiff_t;

tdl_rowdiff_t tdl_rowdiff (void);
tdl_rowdiff_t tdl_rowdiff_with (size_t first_modified, size_t last_modified);
void tdl_rowdiff_set (tdl_rowdiff_t *rdiff, size_t modified);
void tdl_rowdiff_combine (tdl_rowdiff_t *dest, tdl_rowdiff_t rdiff);

#endif  /* TDL_ROWDIFF_H */
