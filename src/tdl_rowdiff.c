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

#include <tdl/tdl_rowdiff.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIX(a, b) ((a) < (b) ? (a) : (b))

tdl_rowdiff_t
tdl_rowdiff (void)
{
  return (tdl_rowdiff_t) {
    .was_modified = false,
    .first_modified = 0,
    .last_modified = 0
  };
}

tdl_rowdiff_t
tdl_rowdiff_with (size_t first_modified, size_t last_modified)
{
  return (tdl_rowdiff_t) {
    .was_modified = true,
    .first_modified = first_modified,
    .last_modified = last_modified
  };
}

void
tdl_rowdiff_set (tdl_rowdiff_t *bsdiff, size_t modified)
{
  bsdiff->was_modified = true;
  
  if (modified > bsdiff->last_modified)
    bsdiff->last_modified = modified;
  else if (modified < bsdiff->first_modified)
    bsdiff->first_modified = modified;
}

void
tdl_rowdiff_combine (tdl_rowdiff_t *dest, tdl_rowdiff_t rdiff)
{
  dest->was_modified = true;
  dest->first_modified = MIX (dest->first_modified, rdiff.first_modified);
  dest->last_modified = MAX (dest->last_modified, rdiff.last_modified);
}
