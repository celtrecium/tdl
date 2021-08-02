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


#ifndef TDL_CANVAS_H
#define TDL_CANVAS_H

#include "tdl_objects.h"

/* This header contains methods for canvas structure */

/* 1. Create/destroy canvas functions */
tdl_canvas_t *tdl_create_canvas (void);
int tdl_destroy_canvas (tdl_canvas_t *canv);

/* 2. Canvas setters */
int tdl_set_cursor_pos (tdl_canvas_t *canv, tdl_point_t pos);
int tdl_print (tdl_canvas_t *canv, tdl_text_t text);

#endif /* TDL_CANVAS_H */
