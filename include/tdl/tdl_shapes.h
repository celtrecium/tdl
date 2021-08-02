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


#ifndef TDL_SHAPES_H
#define TDL_SHAPES_H

#include "tdl_objects.h"

/* 1.1. Functions for drawing shapes */
int tdl_draw_line (tdl_canvas_t *canv, tdl_text_t text, tdl_line_t line);
int tdl_draw_rectangle (tdl_canvas_t *canv, tdl_text_t text,
                        tdl_rectangle_t rect);
int tdl_draw_filled_rectangle (tdl_canvas_t *canv, tdl_text_t text,
                               tdl_rectangle_t rect);

/* 1.2. Functions for drawing an array of shapes */
int tdl_draw_lines (tdl_canvas_t *canv, tdl_text_t text, tdl_line_t *lines,
                    size_t n);
int tdl_draw_rectangles (tdl_canvas_t *canv, tdl_text_t text,
                         tdl_rectangle_t *rect, size_t n);
int tdl_draw_filled_rectangles (tdl_canvas_t *canv, tdl_text_t text,
                                tdl_rectangle_t *rect, size_t n);

#endif /* TDL_SHAPES_H */
