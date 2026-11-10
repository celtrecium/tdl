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

#ifndef TDL_SCROLL_REG_H
#define TDL_SCROLL_REG_H

#include <stdlib.h>
#include <stdbool.h>
#include <tdl/tdl_symbols_export.h>

typedef struct tdl_scroll_reg
{
  bool is_whole_canvas;
  size_t top;
  size_t bottom;
} tdl_scroll_reg_t;

TDL_API tdl_scroll_reg_t tdl_scroll_reg (size_t canv_height);
TDL_API tdl_scroll_reg_t tdl_scroll_reg_with (size_t top, size_t bottom);

#endif /* TDL_SCROLL_REG_H */
