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

#include <string.h>
#include <u8string.h>
#include "tdl/tdl_char.h"
#include "tdl/tdl_style.h"

tdl_char_t
tdl_char (u8char_t uchar, tdl_style_t style)
{
  tdl_char_t tchar;

  memset (&tchar, 0, sizeof (tdl_char_t));
  
  tchar.style = style;

  if (uchar)
    u8char_copy (tchar.ch, uchar);

  return tchar;
}

void
tdl_char_copy (tdl_char_t *dest, tdl_char_t *src)
{
  u8char_copy (dest->ch, src->ch);
  dest->style = src->style;
}

bool
tdl_char_compare (tdl_char_t *first, tdl_char_t *second)
{
  return u8char_compare (first->ch, second->ch)
    && tdl_style_compare (&(first->style), &(second->style));
}
