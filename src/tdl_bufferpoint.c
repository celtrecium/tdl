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


#include "tdl/tdl_bufferpoint.h"
#include <string.h>

tdl_buffer_point_t
tdl_buffer_point (u8char_t uchar, tdl_style_t style)
{
  tdl_buffer_point_t bpt;

  memset (&bpt, 0, sizeof (tdl_buffer_point_t));
  
  bpt.style = style;

  if (uchar != NULL)
    u8char_copy (bpt.character, uchar);

  return bpt;
}

int
tdl_buffpt_copy (tdl_buffer_point_t *dest, tdl_buffer_point_t *src)
{
  if (dest == NULL || src == NULL)
    return EXIT_FAILURE;

  u8char_copy (dest->character, src->character);
  dest->style = src->style;

  return EXIT_SUCCESS;
}
