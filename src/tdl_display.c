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

#include "tdl/tdl_canvas.h"
#include "tdl/tdl_dbuffer.h"
#include "tdl/tdl_default_renderer.h"
#include "tdl/tdl_renderer.h"
#include "tdl/tdl_renderer_signals.h"
#include "tdl/tdl_rowdiff.h"
#include <stdio.h>

static bool
_tdl_render_doublebuffered_row (tdl_renderer_t renderer,
				 tdl_row_t row,
				 tdl_rowdiff_t rowdiff)
{
  size_t i;
  tdl_char_t curr, prev;

  if (renderer.render_before_line != NULL) {
    renderer.render_before_line((tdl_point_t) {
	.x = (int) rowdiff.first_modified,
	.y = (int) rowdiff.row_number },
      renderer.optional_data);
  }

  for (i = rowdiff.first_modified; i <= rowdiff.last_modified; ++i)
    {
      curr = row[i];
      
      tdl_renderer_signals_t signals = tdl_get_renderer_signals (&curr, &prev);
      
      renderer.render_char((tdl_point_t) {
	  .x = (int) i,
	  .y = (int) rowdiff.row_number
	},
	signals,
	curr,
	renderer.optional_data);
      
      prev = curr;
    }

  return true;
}

static bool
_tdl_display_render_doublebuffered (tdl_canvas_t *canv, tdl_renderer_t renderer)
{
  size_t i, diffs_count;
  tdl_rowdiff_t rowdiff;
  tdl_row_t row;

  if (renderer.render_char == NULL)
    return false;

  diffs_count = tdl_dbufdiff_size (canv->buffer.as.double_buffer.diff);
  
  if(diffs_count == 0)
    return true;
 
  for (i = 0; i < diffs_count; ++i)
    {
      rowdiff = canv->buffer.as.double_buffer.diff[i];
      row = canv->buffer.as.double_buffer.main[rowdiff.row_number];

      if (tdl_row_is_clear(row) && renderer.render_empty_line != NULL)
        {
	  renderer.render_empty_line((tdl_point_t) {
	      .x = 0,
	      .y = (int) rowdiff.row_number },
	    renderer.optional_data);

          continue;
        }
      
      if (rowdiff.first_modified > rowdiff.last_modified)
        continue;

      _tdl_render_doublebuffered_row (renderer, row, rowdiff);
    }

  tdl_dbuffer_throw_shadow (&canv->buffer.as.double_buffer);
  tdl_dbufdiff_clear (canv->buffer.as.double_buffer.diff);

  return true;
}

static bool
_tdl_render_singlebuffered_row (tdl_renderer_t renderer,
				 tdl_row_t row,
				 size_t row_n)
{
  static const tdl_renderer_signals_t signals =
    {
      .set_attribute = true,
      .set_color_bg = true,
      .set_color_fg = true
    };
  
  size_t i = 0;
  tdl_char_t tchar;
  size_t row_size = tdl_row_size (row);

  if (renderer.render_before_line != NULL) {
    renderer.render_before_line((tdl_point_t) { 0, (int) row_n },
				renderer.optional_data);
  }

  for (i = 0; i < row_size; ++i)
    {
      tchar = row[i];
      
      renderer.render_char((tdl_point_t) {
	  .x = (int) i, .y = (int) row_n
	},
	signals,
	tchar,
	renderer.optional_data);
    }

  return true;
}

static bool
_tdl_display_render_singlebuffered (tdl_canvas_t *canv, tdl_renderer_t renderer)
{
  size_t y;
  tdl_row_t row;

  if (renderer.render_char == NULL)
    return false;
  
  for (y = 0; y < canv->size.height; ++y)
    {
      row = canv->buffer.as.single_buffer[y];

      _tdl_render_singlebuffered_row (renderer, row, y);
    }

  return true;
}

bool
tdl_display_with_renderer (tdl_canvas_t *canv, tdl_renderer_t renderer)
{
  switch (canv->buffer.type)
    {
    case TDL_DOUBLE_BUFFER:
      return _tdl_display_render_doublebuffered (canv, renderer);
    case TDL_SINGLE_BUFFER:
      return _tdl_display_render_singlebuffered (canv, renderer);
    }

  return false;
}

bool
tdl_display (tdl_canvas_t *canv)
{
  tdl_display_with_renderer (canv, tdl_default_renderer);
  fflush (stdout);
  
  return true;
}
