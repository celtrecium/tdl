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

#include "tdl_symbols_export.h"
#include "tdl/tdl_geometry.h"
#include "tdl/tdl_dbuffer.h"
#include "tdl/tdl_text.h"

typedef enum tdl_buffer_type {
  TDL_SINGLE_BUFFER,
  TDL_DOUBLE_BUFFER
} tdl_buffer_type_t;

typedef struct tdl_canvas
{
  tdl_point_t cursor;
  tdl_size_t size;

  struct {
    tdl_buffer_type_t type;

    union {
      tdl_dbuffer_t double_buffer;
      tdl_buffer_t single_buffer;
    } as;
  } buffer;
} tdl_canvas_t;

/* This header contains methods for canvas structure */

/* 1. Create/destroy canvas functions */
TDL_API tdl_canvas_t *tdl_canvas (void);
TDL_API tdl_canvas_t *tdl_canvas_with (tdl_size_t size,
				       tdl_buffer_type_t buffer_type);
TDL_API bool tdl_destroy_canvas (tdl_canvas_t *canv);

/* 2. Canvas setters */
TDL_API void tdl_set_cursor_pos (tdl_canvas_t *canv, tdl_point_t pos);
TDL_API bool tdl_canvas_resize (tdl_canvas_t *canv, tdl_size_t newsize);
TDL_API void tdl_print (tdl_canvas_t *canv, tdl_text_t text);
TDL_API void tdl_putchar (tdl_canvas_t *canv, tdl_char_t ch);
TDL_API void tdl_clear (tdl_canvas_t *canv);

#endif /* TDL_CANVAS_H */
