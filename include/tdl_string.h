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

#ifndef TDL_STRING_H
#define TDL_STRING_H

#include "tdl_objects.h"

size_t tdl_strlen (char *string);
tdl_string_t tdl_string (char *string);
int tdl_string_free (tdl_string_t str);
tdl_string_t tdl_string_set (tdl_string_t *string, char *newstr);

#endif  /* TDL_STRING_H */
