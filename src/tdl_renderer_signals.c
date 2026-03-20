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

#include "tdl/tdl_renderer_signals.h"
#include "tdl/tdl_bufferpoint.h"

tdl_renderer_signals_t
tdl_renderer_signals (void)
{
  return (tdl_renderer_signals_t)
    {
      .set_attribute = false,
      .set_color_bg = false,
      .set_color_fg = false
    };
}

tdl_renderer_signals_t
tdl_get_renderer_signals (tdl_buffer_point_t *curr, tdl_buffer_point_t *prev)
{
  if (!prev)
    return (tdl_renderer_signals_t)
      {
	.set_attribute = true,
	.set_color_bg = true,
	.set_color_fg = true
      };

  tdl_renderer_signals_t signals;
  
  signals.set_attribute = curr->style.attributes != prev->style.attributes;
  signals.set_color_bg = curr->style.color.bg != prev->style.color.bg;
  signals.set_color_fg = curr->style.color.fg != prev->style.color.fg;

  return signals;
}
