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

#ifndef TDL_ATTRIBUTES_H
#define TDL_ATTRIBUTES_H

#include "tdl_objects.h"

#define TDL_NO_ATTRIBUTES 0
#define TDL_BOLD          1
#define TDL_ITALIC        2
#define TDL_UNDERLINE     4
#define TDL_CROSSED_OUT   8
#define TDL_DIM           16

tdl_point_color_t tdl_point_color (tdl_color_t bg, tdl_color_t fg);

#endif  /* TDL_ATTRIBUTES_H */
