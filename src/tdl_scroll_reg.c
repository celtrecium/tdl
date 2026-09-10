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

#include "tdl/tdl_scroll_reg.h"

tdl_scroll_reg_t
tdl_scroll_reg (size_t canv_height)
{
  return (tdl_scroll_reg_t)
    {
      .is_whole_canvas = true,
      .top = 0,
      .bottom = canv_height-1
    };
}

tdl_scroll_reg_t
tdl_scroll_reg_with (size_t top, size_t bottom)
{
  return (tdl_scroll_reg_t)
    {
      .is_whole_canvas = false,
      .top = top,
      .bottom = bottom
    };
}
