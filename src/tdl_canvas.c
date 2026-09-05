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
#include "tdl/tdl_dbuffer.h"
#include "tdl/tdl_scroll_reg.h"
#include "tdl/tdl_style.h"
#include "tdl/tdl_terminal.h"

#define TAB_SIZE 8

tdl_canvas_t *
tdl_canvas_with (tdl_size_t size, tdl_buffer_type_t buffer_type)
{
  tdl_canvas_t *canv = calloc (1, sizeof (tdl_canvas_t));
  
  canv->cursor = tdl_point (0, 0);
  canv->size = size;
  canv->buffer.type = buffer_type;
  canv->scrolling_region = tdl_scroll_reg (size.height);

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

void
tdl_set_scrolling_region (tdl_canvas_t *canv, tdl_scroll_reg_t sr)
{
  if (sr.top >= canv->size.height)
    sr.top = canv->size.height - 1;
  else if (sr.is_whole_canvas)
    sr = tdl_scroll_reg (canv->size.height - 1);

  canv->scrolling_region = sr;
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
void
tdl_set_cursor_pos (tdl_canvas_t *canv, tdl_point_t pos)
{
  canv->cursor = tdl_point_in_bounds (pos, canv->size);
}

static void
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

void
tdl_putchar (tdl_canvas_t *canv, tdl_char_t ch)
{
  tdl_point_t cur = canv->cursor;

  switch (ch.ch[0]) {
  case '\n':
    if ((cur.y == (int)(canv->scrolling_region.bottom))
	|| (cur.y >= (int)(canv->size.height-1)))
      tdl_scroll_down (canv, 1);
    else
      ++canv->cursor.y;

    return;
  case '\r':
    canv->cursor.x = 0;

    return;
  case '\t':
    cur.x = (cur.x / TAB_SIZE + 1) * TAB_SIZE; 
    tdl_set_cursor_pos (canv, cur);

    return;
  case '\b':
    --cur.x;
    tdl_set_cursor_pos (canv, cur);
    _tdl_putchar (canv, tdl_char (" ", ch.style));
    
    return;
  case '\a':
    return;
  }

  _tdl_putchar (canv, ch);
  ++cur.x;
  tdl_set_cursor_pos (canv, cur);
}

void
tdl_print (tdl_canvas_t *canv, tdl_text_t text)
{
  size_t i;
  char *offset_ptr = NULL;
  u8char_t ch_buff;
  
  offset_ptr = text.string;
  
  for (i = 0; i < u8str_strlen(text.string); ++i)
    {
      offset_ptr = u8string_next_char(offset_ptr, &ch_buff);
      tdl_putchar(canv, tdl_char (ch_buff, text.style));
    }
}

void
tdl_clear (tdl_canvas_t *canv)
{
  switch (canv->buffer.type)
    {
    case TDL_DOUBLE_BUFFER:
      tdl_dbuffer_clear (&canv->buffer.as.double_buffer);
      break;
    case TDL_SINGLE_BUFFER:
      tdl_buffer_clear (canv->buffer.as.single_buffer);
      break;
    }
}

bool
tdl_canvas_resize (tdl_canvas_t *canv, tdl_size_t newsize)
{
  if (!canv)
    return false;

  if (canv->size.width == newsize.width &&
      canv->size.height == newsize.height)
    return true;

  switch (canv->buffer.type) {
  case TDL_SINGLE_BUFFER:
      tdl_buffer_resize (&canv->buffer.as.single_buffer, newsize);
    break;
  case TDL_DOUBLE_BUFFER:
      tdl_dbuffer_resize (&canv->buffer.as.double_buffer, newsize);
    break;
  }

  canv->size = newsize;
  
  return true;
}

tdl_buffer_t
_get_main_buffer (tdl_canvas_t *canv)
{
  switch (canv->buffer.type)
    {
    case TDL_SINGLE_BUFFER:
      return canv->buffer.as.single_buffer;
    case TDL_DOUBLE_BUFFER:
      return canv->buffer.as.double_buffer.main;
    }

  return NULL;
}

void
_insert_rows_up (tdl_buffer_t buff, tdl_point_t cur,
		 tdl_scroll_reg_t reg, size_t n_rows)
{
  size_t i, j;
  tdl_row_t emptyrow;

  if (n_rows == 0)
    return;
  
  if ((size_t) cur.y - n_rows < reg.top)
    n_rows = (size_t) cur.y - reg.top;

  for (i = 0; i < n_rows; ++i)
    tdl_row_clear (buff[i + reg.top]);

  for (i = 0; i < n_rows; ++i)
    {
      emptyrow = buff[reg.top];

      for (j = reg.top + 1; j <= (size_t) cur.y; ++j)
	buff[j - 1] = buff[j];

      buff[cur.y] = emptyrow;
    }
}

void
_insert_rows_down (tdl_buffer_t buff, tdl_point_t cur,
		   tdl_scroll_reg_t reg, size_t n_rows)
{
  size_t i;
  int j;
  tdl_row_t emptyrow;

  if (n_rows == 0)
    return;

  if (reg.top + n_rows > reg.bottom)
    n_rows = reg.bottom - reg.top;

  for (i = 0; i < n_rows; ++i)
    tdl_row_clear (buff[reg.bottom - i]);

  for (i = 0; i < n_rows; ++i)
    {
      emptyrow = buff[reg.bottom];

      for (j = (int) reg.bottom - 1; j >= cur.y; --j)
	buff[j + 1] = buff[j];

      buff[cur.y] = emptyrow;
    }
}

void
tdl_insert_rows (tdl_canvas_t *canv, size_t n_rows)
{
  tdl_buffer_t buff = _get_main_buffer (canv);

  _insert_rows_down (buff, canv->cursor, canv->scrolling_region, n_rows);
}

void
tdl_delete_rows (tdl_canvas_t *canv, size_t n_rows)
{
  tdl_buffer_t buff = _get_main_buffer (canv);

  _insert_rows_down (buff, canv->cursor, canv->scrolling_region, n_rows);
}

void
tdl_scroll_up (tdl_canvas_t *canv, size_t n_rows)
{
  tdl_buffer_t buff = _get_main_buffer (canv);

  _insert_rows_down (buff, tdl_point (0, (int) canv->scrolling_region.top),
		     canv->scrolling_region, n_rows);
}

void
tdl_scroll_down (tdl_canvas_t *canv, size_t n_rows)
{
  tdl_buffer_t buff = _get_main_buffer (canv);

  _insert_rows_up (buff, tdl_point (0, (int) canv->scrolling_region.bottom),
		   canv->scrolling_region, n_rows);
}
