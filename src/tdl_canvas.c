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

#include <tdl/u8utils/u8utils.h>
#include <tdl/tdl_buffer.h>
#include <tdl/tdl_canvas.h>
#include <tdl/tdl_dbuffer.h>
#include <tdl/tdl_geometry.h>
#include <tdl/tdl_row.h>
#include <tdl/tdl_scroll_reg.h>
#include <tdl/tdl_style.h>
#include <tdl/tdl_terminal.h>
#include <tdl/tdl_text.h>

#define TAB_SIZE 8

static const tdl_style_t CTRL_CHAR_STYLE = {
  .attributes = TDL_NO_ATTRIBUTES,
  .color = {
    .bg = TDL_RED,
    .fg = TDL_BLACK
  }
};

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

  if (sr.bottom >= canv->size.height)
    sr.bottom = canv->size.height - 1;
  
  if (sr.is_whole_canvas)
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
  canv->cursor = tdl_point_wrap_in_size (pos, canv->size);
}

static int
_force_value_in_bounds (int v, int top, int bottom)
{
  return v < top
    ? top
    : v > bottom
    ? bottom
    : v;
}

void
tdl_move_cursor (tdl_canvas_t *canv, tdl_point_t vector)
{
  tdl_point_t newpos = {
    canv->cursor.x + vector.x,
    canv->cursor.y + vector.y
  };

  newpos.x = _force_value_in_bounds (newpos.x, 0, (int) canv->size.width - 1);
  newpos.y = _force_value_in_bounds (newpos.y, 0, (int) canv->size.height - 1);

  if (canv->cursor.y < (int) canv->scrolling_region.bottom
      && newpos.y > (int) canv->scrolling_region.bottom)
    {
      newpos.y = (int) canv->scrolling_region.bottom;
    }
  else if (canv->cursor.y > (int) canv->scrolling_region.top
      && newpos.y < (int) canv->scrolling_region.top)
    {
      newpos.y = (int) canv->scrolling_region.top;
    }

  canv->cursor = newpos;
}

static void
_tdl_set_char_at_pos (tdl_canvas_t *canv, tdl_point_t pos, tdl_char_t ch)
{
  switch (canv->buffer.type)
    {
    case TDL_DOUBLE_BUFFER:
      tdl_dbuffer_set_char (&canv->buffer.as.double_buffer, pos, ch);
      break;

    case TDL_SINGLE_BUFFER:
      canv->buffer.as.single_buffer[pos.y][pos.x] = ch;
      break;
    }
}

static void _tdl_print_control_char (tdl_canvas_t *canv, char ch)
{
  char cc_sym = ch + '@';

  tdl_char_t tdlchar = { .ch = { 0, 0 }, .style = CTRL_CHAR_STYLE };
  
  tdlchar.ch[0] = '^';
  tdl_putchar(canv, tdlchar);
  tdlchar.ch[0] = cc_sym;
  tdl_putchar(canv, tdlchar);
}

void
tdl_putchar (tdl_canvas_t *canv, tdl_char_t ch)
{
  if ((ch.ch[0] >= 0) && (ch.ch[0] < 32) && (ch.ch[0] != '\t'))
    {
      _tdl_print_control_char (canv, ch.ch[0]);
    }
  else if (ch.ch[0] == '\t')
    {
      canv->cursor.x = (canv->cursor.x / TAB_SIZE + 1) * TAB_SIZE;
    }
  else if (canv->cursor.x < (int) canv->size.width
      && canv->cursor.y < (int) canv->size.height
      && canv->cursor.x >= 0
      && canv->cursor.y >= 0)
    {
      _tdl_set_char_at_pos (canv, canv->cursor, ch);

      ++canv->cursor.x;
    }
  else
    {
      ++canv->cursor.x;
    }
}

void
tdl_print (tdl_canvas_t *canv, tdl_text_t text)
{
  size_t i;
  char *offset_ptr = NULL;
  u8codept_t ch_buff;

  offset_ptr = text.string;

  for (i = 0; i < u8str_codepoints_count (text.string); ++i)
    {
      offset_ptr = u8str_next_codepoint (offset_ptr, &ch_buff);
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

  switch (canv->buffer.type)
    {
    case TDL_SINGLE_BUFFER:
      tdl_buffer_resize (&canv->buffer.as.single_buffer, newsize);
      break;
    case TDL_DOUBLE_BUFFER:
      tdl_dbuffer_resize (&canv->buffer.as.double_buffer, newsize);
      break;
    }

  if ((canv->scrolling_region.bottom > newsize.height-1)
      || (canv->scrolling_region.bottom <= newsize.height-1
          && canv->scrolling_region.is_whole_canvas))
    canv->scrolling_region.bottom = newsize.height-1;

  canv->size = newsize;
  
  tdl_set_cursor_pos (canv, canv->cursor);

  return true;
}

void
tdl_insert_rows (tdl_canvas_t *canv, size_t n_rows)
{
  if ((size_t) canv->cursor.y >= canv->size.height)
    return;

  switch (canv->buffer.type)
    {
    case TDL_SINGLE_BUFFER:
      tdl_buffer_insert_rows_down (canv->buffer.as.single_buffer,
                                   canv->cursor,
                                   canv->scrolling_region,
                                   n_rows);
      break;
    case TDL_DOUBLE_BUFFER:
      tdl_dbuffer_insert_rows_down (&canv->buffer.as.double_buffer,
                                    canv->cursor,
                                    canv->scrolling_region,
                                    n_rows);
      break;
    }
}

void
tdl_delete_rows (tdl_canvas_t *canv, size_t n_rows)
{
  tdl_scroll_reg_t tmp_scrollreg =
    {
      .top = (size_t) canv->cursor.y,
      .bottom = canv->scrolling_region.bottom,
      .is_whole_canvas = false
    };
  tdl_point_t tmp_cursor =
    {
      .x = 0,
      .y = (int) canv->scrolling_region.bottom
    };

  if ((size_t) canv->cursor.y >= canv->size.height - 1)
    return;

  switch (canv->buffer.type)
    {
    case TDL_SINGLE_BUFFER:
      tdl_buffer_insert_rows_up (canv->buffer.as.single_buffer,
                                 tmp_cursor,
                                 tmp_scrollreg,
                                 n_rows);
      break;
    case TDL_DOUBLE_BUFFER:
      tdl_dbuffer_insert_rows_up (&canv->buffer.as.double_buffer,
                                  tmp_cursor,
                                  tmp_scrollreg,
                                  n_rows);
      break;
    }
}

void
tdl_insert_char (tdl_canvas_t *canv, tdl_char_t ch)
{
  switch (canv->buffer.type)
    {
    case TDL_SINGLE_BUFFER:
      tdl_buffer_insert_blank_char (canv->buffer.as.single_buffer,
                                    canv->cursor);
      break;
    case TDL_DOUBLE_BUFFER:
      tdl_dbuffer_insert_blank_char (&canv->buffer.as.double_buffer,
                                     canv->cursor);
      break;
    }

  tdl_putchar (canv, ch);
  --canv->cursor.x;
}

void tdl_delete_char (tdl_canvas_t *canv) {
  switch (canv->buffer.type)
    {
    case TDL_SINGLE_BUFFER:
      tdl_buffer_delete_char (canv->buffer.as.single_buffer, canv->cursor);
      break;
    case TDL_DOUBLE_BUFFER:
      tdl_dbuffer_delete_char (&canv->buffer.as.double_buffer, canv->cursor);
      break;
    }
}

void
tdl_scroll_up (tdl_canvas_t *canv, size_t n_rows)
{
  tdl_point_t pos = tdl_point (0, (int) canv->scrolling_region.top);

  switch (canv->buffer.type)
    {
    case TDL_SINGLE_BUFFER:
      tdl_buffer_insert_rows_down (canv->buffer.as.single_buffer,
                                   pos,
                                   canv->scrolling_region,
                                   n_rows);
      break;
    case TDL_DOUBLE_BUFFER:
      tdl_dbuffer_insert_rows_down (&canv->buffer.as.double_buffer,
                                    pos,
                                    canv->scrolling_region,
                                    n_rows);
      break;
  }
}

void
tdl_scroll_down (tdl_canvas_t *canv, size_t n_rows)
{
  tdl_point_t pos = tdl_point (0, (int) canv->scrolling_region.bottom);

  switch (canv->buffer.type)
    {
    case TDL_SINGLE_BUFFER:
      tdl_buffer_insert_rows_up (canv->buffer.as.single_buffer,
                                 pos,
                                 canv->scrolling_region,
                                 n_rows);
      break;
    case TDL_DOUBLE_BUFFER:
      tdl_dbuffer_insert_rows_up (&canv->buffer.as.double_buffer,
                                  pos,
                                  canv->scrolling_region,
                                  n_rows);
      break;
  }
}
