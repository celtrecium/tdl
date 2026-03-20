/*
 * this file is part of text drawing library.
 *
 * text drawing library is free software: you can redistribute it and/or modify
 * it under the terms of the gnu general public license as published by
 * the free software foundation, either version 2 of the license, or
 * (at your option) any later version.
 *
 * text drawing library is distributed in the hope that it will be useful,
 * but without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose.  see the
 * gnu general public license for more details.
 *
 * you should have received a copy of the gnu general public license
 * along with text drawing library. if not, see
 * <https://www.gnu.org/licenses/>.
 */

#include "tdl/tdl_renderer.h"

tdl_renderer_t
tdl_renderer (void (*render_before_line)(tdl_point_t new_line_pos,
					 void *optional_data),
	      void (*render_char)(tdl_point_t pos,
				  tdl_renderer_signals_t ren_signals,
				  tdl_char_t ch,
				  void *optional_data),
	      void (*render_empty_line)(tdl_point_t pos, void *optional_data),
	      void *optional_data)
{
  return (tdl_renderer_t) {
    .render_before_line = render_before_line,
    .render_char = render_char,
    .render_empty_line = render_empty_line,
    .optional_data = optional_data
  };
}
