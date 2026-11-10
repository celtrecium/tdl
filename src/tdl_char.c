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

#include <tdl/tdl_char.h>

tdl_char_t
tdl_char (u8codept_t uchar, tdl_style_t style)
{
  tdl_char_t tchar;

  memset (&tchar, 0, sizeof (tdl_char_t));
  
  tchar.style = style;

  if (uchar)
    u8codept_copy (tchar.ch, uchar);

  return tchar;
}

void
tdl_char_copy (tdl_char_t *dest, tdl_char_t *src)
{
  u8codept_copy (dest->ch, src->ch);
  dest->style = src->style;
}

bool
tdl_char_compare (tdl_char_t *first, tdl_char_t *second)
{
  return u8codept_compare (first->ch, second->ch)
    && tdl_style_compare (&(first->style), &(second->style));
}
