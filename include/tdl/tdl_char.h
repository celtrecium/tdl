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

#ifndef TDL_LETTER_H
#define TDL_LETTER_H

#include "tdl/tdl_bufferpoint.h"
#include "tdl/tdl_style.h"
#include "tdl/tdl_symbols_export.h"
#include <u8string.h>

typedef tdl_buffer_point_t tdl_char_t;

TDL_API tdl_char_t tdl_char (u8char_t ch, tdl_style_t style);

#endif  /* TDL_LETTER_H */
