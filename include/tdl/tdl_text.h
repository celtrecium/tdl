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

#ifndef TDL_TEXT_H
#define TDL_TEXT_H

#include <sbvector.h>
#include <u8string.h>
#include "tdl_style.h"
#include "tdl_symbols_export.h"

typedef struct tdl_text
{
  u8string_t string;
  tdl_style_t style;
} tdl_text_t;

TDL_API tdl_text_t tdl_text (u8string_t string, tdl_style_t style);
TDL_API bool tdl_text_free (tdl_text_t text);

#define tdl_default_text(string)                                              \
  tdl_text (u8string (string),                                                \
            tdl_style (tdl_point_color (256, 256), TDL_NO_ATTRIBUTES));

#endif  /* TDL_TEXT_H */
