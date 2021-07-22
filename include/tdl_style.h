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

#ifndef TDL_STYLE_H
#define TDL_STYLE_H

#include "tdl_objects.h"

tdl_point_color_t tdl_point_color (tdl_color_t bg, tdl_color_t fg);
tdl_attributes_t tdl_attributes (tdl_attributes_t attrs, ...);
tdl_style_t tdl_style (tdl_point_color_t color, tdl_attributes_t attrs);

#endif  /* TDL_STYLE_H */
