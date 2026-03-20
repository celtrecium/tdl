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

#include <u8string.h>
#include "tdl/tdl_buffer.h"
#include "tdl/tdl_canvas.h"
#include "tdl/tdl_style.h"
#include "tdl/tdl_terminal.h"

tdl_canvas_t *
tdl_canvas_with (tdl_size_t size, tdl_buffer_type_t buffer_type)
{
  tdl_canvas_t *canv = calloc (1, sizeof (tdl_canvas_t));
  
  canv->cursor = tdl_point (0, 0);
  canv->size = size;
  canv->buffer.type = buffer_type;

  switch (buffer_type)
    {
    case TDL_DOUBLE_BUFFER:
      canv->buffer.as.double_buffer = tdl_dbuffer (size);
      break;
    case TDL_SINGLE_BUFFER:
      canv->buffer.as.single_buffer = tdl_buffer (size);
      break;
    }
  
  return canv;
}

tdl_canvas_t *
tdl_canvas (void)
{
  return tdl_canvas_with (tdl_terminal_get_size(), TDL_DOUBLE_BUFFER);
}

bool
tdl_destroy_canvas (tdl_canvas_t *canv)
{  
  if (canv == NULL)
    return false;

  switch (canv->buffer.type)
    {
    case TDL_DOUBLE_BUFFER:
      tdl_dbuffer_free (&canv->buffer.as.double_buffer);
      break;
    case TDL_SINGLE_BUFFER:
      tdl_buffer_free (canv->buffer.as.single_buffer);
      break;
    }
  
  free (canv);

  return true;
}

/* 2. Canvas setters */
bool
tdl_set_cursor_pos (tdl_canvas_t *canv, tdl_point_t pos)
{
  if (!canv)
    return false;

  canv->cursor = tdl_point_in_bounds (pos, canv->size);

  return true;
}

inline static void
_tdl_putchar(tdl_canvas_t *canv, tdl_char_t ch)
{
  switch (canv->buffer.type)
    {
    case TDL_DOUBLE_BUFFER:
      tdl_dbuffer_set_char (&canv->buffer.as.double_buffer,
			    canv->cursor,
			    ch);
      break;
    case TDL_SINGLE_BUFFER:
      canv->buffer.as.single_buffer[canv->cursor.y][canv->cursor.x] = ch;
      break;
    }
}

bool tdl_putchar (tdl_canvas_t *canv, tdl_char_t ch)
{
  tdl_point_t cur;
  cur = canv->cursor;

  switch (ch.ch[0]) {
  case '\n':
      ++cur.y;
      tdl_set_cursor_pos (canv, cur);

      return true;
  case '\r':
      cur.x = 0;
      tdl_set_cursor_pos (canv, cur);

      return true;
  case '\t':
      cur.x += 8;             /* Tab character size */
      tdl_set_cursor_pos (canv, cur);

      return true;
  case '\b':
    --cur.x;
    tdl_set_cursor_pos (canv, cur);
    _tdl_putchar (canv, tdl_char (" ", ch.style));
    
    return true;
  case '\a':
    return true;
  }

  _tdl_putchar (canv, ch);
  ++cur.x;
  tdl_set_cursor_pos (canv, cur);

  return true;
}

bool
tdl_print (tdl_canvas_t *canv, tdl_text_t text)
{
  size_t i;
  char *offset_ptr = NULL;
  u8char_t ch_buff;
  
  if (!canv)
    return false;

  offset_ptr = text.string;
  
  for (i = 0; i < u8str_strlen(text.string); ++i)
    {
      offset_ptr = u8string_next_char(offset_ptr, &ch_buff);
      tdl_putchar(canv, tdl_char (ch_buff, text.style));
    }
  
  return true;
}

bool
tdl_clear (tdl_canvas_t *canv)
{
  if (canv == NULL)
    return false;

  switch (canv->buffer.type)
    {
    case TDL_DOUBLE_BUFFER:
      tdl_dbuffer_clear (&canv->buffer.as.double_buffer);
      break;
    case TDL_SINGLE_BUFFER:
      tdl_buffer_clear (canv->buffer.as.single_buffer);
      break;
    }

  return true;
}
