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

#include "tdl/tdl_text.h"
#include <string.h>

tdl_text_t
tdl_text (u8string_t *string, tdl_style_t style)
{
  tdl_text_t text;

  text.string = string;
  text.style = style;

  return text;
}

int
tdl_text_free (tdl_text_t text)
{
  u8string_free (text.string);
  memset (&text.style, 0, sizeof (tdl_style_t));
  
  return EXIT_SUCCESS;
}
