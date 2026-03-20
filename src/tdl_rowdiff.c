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

#include "tdl/tdl_rowdiff.h"
#include <stdlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIX(a, b) ((a) < (b) ? (a) : (b))

tdl_rowdiff_t
tdl_rowdiff (size_t row_n, size_t first_m, size_t last_m)
{
  tdl_rowdiff_t bsdiff;

  bsdiff.first_modified = first_m;
  bsdiff.last_modified = last_m;
  bsdiff.row_number = row_n;

  return bsdiff;
}

bool
tdl_rowdiff_set (tdl_rowdiff_t *bsdiff, size_t modified)
{
  if (bsdiff == NULL)
    return false;

  if (modified > bsdiff->last_modified)
    bsdiff->last_modified = modified;
  else if (modified < bsdiff->first_modified)
    bsdiff->first_modified = modified;

  return true;
}

bool
tdl_rowdiff_combine (tdl_rowdiff_t *dest, tdl_rowdiff_t rdiff)
{
  if (dest == NULL || dest->row_number != rdiff.row_number)
    return false;

  dest->first_modified = MIX (dest->first_modified, rdiff.first_modified);
  dest->last_modified = MAX (dest->last_modified, rdiff.last_modified);
  
  return true;
}
