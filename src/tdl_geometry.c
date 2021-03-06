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

#include "tdl/tdl_geometry.h"

tdl_line_t
tdl_line (tdl_point_t point_a, tdl_point_t point_b)
{
  return (tdl_line_t){ point_a, point_b };
}

tdl_rectangle_t
tdl_rectangle (tdl_point_t point, tdl_size_t size)
{
  return (tdl_rectangle_t){ point, size };
}

tdl_point_t
tdl_point (int x, int y)
{
  return (tdl_point_t){ x, y };
}

tdl_size_t
tdl_size (size_t width, size_t height)
{
  return (tdl_size_t){ width, height };
}
