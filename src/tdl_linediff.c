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
#include <stdlib.h>

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
