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

#ifndef TDL_RENDERER_SIGNALS_H
#define TDL_RENDERER_SIGNALS_H

#include <stdbool.h>
#include "tdl/tdl_symbols_export.h"
#include "tdl/tdl_char.h"

typedef struct tdl_renderer_signals
{
  bool set_color_bg;
  bool set_color_fg;
  bool set_attribute;
} tdl_renderer_signals_t;

TDL_API tdl_renderer_signals_t tdl_renderer_signals (void);    
TDL_API tdl_renderer_signals_t tdl_get_renderer_signals (tdl_char_t *curr,
							 tdl_char_t *prev);

#endif /* TDL_RENDERER_SIGNALS_H */
