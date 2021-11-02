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

#ifndef TDL_IMAGE_H
#define TDL_IMAGE_H

#include "tdl/tdl_geometry.h"
#include "tdl/tdl_canvas.h"
#include <sbvector.h>
#include <stdbool.h>

typedef struct tdl_image
{
  sbvector_t image;
  tdl_size_t size;
} tdl_image_t;

bool tdl_image_save (const tdl_image_t img, const char *filename);
tdl_image_t tdl_image_load (const char *filename);
tdl_image_t tdl_image_crop_from_canvas (tdl_canvas_t *canv,
                                        tdl_rectangle_t rect);
bool tdl_image_free (tdl_image_t *img);
bool tdl_image_print_to_canvas (tdl_canvas_t *canv, tdl_image_t img,
                                tdl_point_t pos);

#endif /* TDL_IMAGE_H */
