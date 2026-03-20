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
#include "tdl/tdl_linediff.h"
#include "tdl/tdl_bufferline.h"
#include "tdl/tdl_default_renderer.h"
#include "tdl/tdl_renderer.h"
#include "tdl/tdl_renderer_signals.h"
#include <stdio.h>

static bool
_tdl_render_doublebuffered_line (tdl_renderer_t renderer,
				 sbvector_t *line,
				 tdl_ldiff_t *ldiff)
{
  size_t i = 0;
  tdl_char_t *curr = NULL, *prev = NULL;

  if (renderer.render_before_line != NULL) {
    renderer.render_before_line((tdl_point_t) {
	.x = (int) ldiff->first_modified,
	.y = (int) ldiff->line_number },
      renderer.optional_data);
  }

  for (i = ldiff->first_modified; i <= ldiff->last_modified; ++i)
    {
      curr = sbv_get (line, tdl_char_t, i);
      
      if (!curr)
        return false;

      tdl_renderer_signals_t signals = tdl_get_renderer_signals (curr, prev);
      
      renderer.render_char((tdl_point_t) {
	  .x = (int) i,
	  .y = (int) ldiff->line_number
	},
	signals,
	curr,
	renderer.optional_data);
      
      prev = curr;
    }

  return true;
}

static bool
_tdl_display_render_doublebuffered(tdl_canvas_t *canv, tdl_renderer_t renderer)
{
  size_t i;
  tdl_ldiff_t *ldiffptr;
  tdl_buffer_line_t *bl;

  if (renderer.render_char == NULL)
    return false;
  
  if(canv->diff.length == 0)
    return true;
 
  for (i = 0; i < canv->diff.length; ++i)
    {
      ldiffptr = sbv_get (&canv->diff, tdl_ldiff_t, i);
      bl       = sbv_get (&canv->buffer.fbuff, tdl_buffer_line_t,
                    ldiffptr->line_number);

      if (bl->_is_empty && renderer.render_empty_line != NULL)
        {
	  renderer.render_empty_line((tdl_point_t) {
	      .x = 0,
	      .y = (int) ldiffptr->line_number },
	    renderer.optional_data);

          continue;
        }
      
      tdl_ldiff_clarify_line_edges (ldiffptr, &canv->buffer);

      if (ldiffptr->first_modified > ldiffptr->last_modified)
        continue;

      _tdl_render_doublebuffered_line (renderer, &bl->line, ldiffptr);
    }

  tdl_buffer_fbuff_to_sbuff (&canv->buffer);
  sbv_clear (&canv->diff);

  return true;
}

static bool
_tdl_render_singlebuffered_line (tdl_renderer_t renderer,
				 sbvector_t *line,
				 size_t line_number)
{
  static const tdl_renderer_signals_t signals =
    {
      .set_attribute = true,
      .set_color_bg = true,
      .set_color_fg = true
    };
  
  size_t i = 0;
  tdl_char_t *curr = NULL;

  if (renderer.render_before_line != NULL) {
    renderer.render_before_line((tdl_point_t) {
	.x = 0, .y = (int) line_number
      },
      renderer.optional_data);
  }

  for (i = 0; i < line->length; ++i)
    {
      curr = sbv_get (line, tdl_char_t, i);
      
      if (!curr)
        return false;

      renderer.render_char((tdl_point_t) {
	  .x = (int) i, .y = (int) line_number
	},
	signals,
	curr,
	renderer.optional_data);
    }

  return true;
}

static bool
_tdl_display_render_singlebuffered(tdl_canvas_t *canv, tdl_renderer_t renderer)
{
  size_t y;
  tdl_buffer_line_t *bl;

  if (renderer.render_char == NULL)
    return false;
  
  for (y = 0; y < canv->buffer.fbuff.length; ++y)
    {
      bl = sbv_get (&canv->buffer.fbuff, tdl_buffer_line_t, y);

      _tdl_render_singlebuffered_line (renderer, &bl->line, y);
    }

  tdl_buffer_fbuff_to_sbuff (&canv->buffer);
  sbv_clear (&canv->diff);

  return true;
}

bool
tdl_display_with_renderer (tdl_canvas_t *canv, tdl_renderer_t renderer)
{
  return canv->buffer.is_doublebuffered
    ? _tdl_display_render_doublebuffered(canv, renderer)
    : _tdl_display_render_singlebuffered(canv, renderer);
}

bool
tdl_display (tdl_canvas_t *canv)
{
  tdl_display_with_renderer (canv, tdl_default_renderer);
  fflush (stdout);
  
  return true;
}
