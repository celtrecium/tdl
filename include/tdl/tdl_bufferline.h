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


#ifndef TDL_BUFFERLINE_H
#define TDL_BUFFERLINE_H

#include "tdl_objects.h"

tdl_buffer_line_t tdl_buffer_line (size_t width);
tdl_buffer_point_t *tdl_buffer_line_get (tdl_buffer_line_t *line,
                                         size_t index);
bool tdl_buffer_line_free (tdl_buffer_line_t *line);
bool tdl_buffer_line_copy (tdl_buffer_line_t *dest, tdl_buffer_line_t *src);

#endif /* TDL_BUFFERLINE_H */
