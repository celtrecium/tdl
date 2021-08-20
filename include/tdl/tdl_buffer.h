
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


#ifndef TDL_BUFFER_H
#define TDL_BUFFER_H

#include "tdl_objects.h"

#define _TDL_DEFAULT_BLOCKSZ 20

tdl_buffer_t tdl_buffer (tdl_size_t size);
bool tdl_buffer_free (tdl_buffer_t *buff);
bool tdl_buffer_resize (tdl_buffer_t *buff, tdl_size_t newsize);
tdl_buffer_point_t *tdl_buffer_get_point (tdl_buffer_t *buff,
                                          tdl_point_t point);
bool tdl_buffer_fbuff_to_sbuff (tdl_buffer_t *buff);


#endif  /* TDL_BUFFER_H */
