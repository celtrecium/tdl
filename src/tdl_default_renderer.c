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

#include "tdl/tdl_default_renderer.h"
#include "tdl/tdl_ansi_sequences.h"
#include "tdl/tdl_renderer.h"
#include <stdio.h>

static const tdl_attributes_t _attrib_val[] = {
  TDL_BOLD,
  TDL_ITALIC,
  TDL_UNDERLINE,
  TDL_CROSSED_OUT,
  TDL_DIM
};

static const char *_attrib_str[] = {
  ";1", /* Bold */
  ";3", /* Italic */
  ";4", /* Underline */
  ";9", /* Crossed out */
  ";2"  /* Dim */
};

static const size_t _ATTRIB_NUM
    = sizeof (_attrib_val) / sizeof (_attrib_val[0]);

static void
_tdl_print_attributes (tdl_buffer_point_t *curr)
{
  size_t i = 0;
  tdl_attributes_t attrib = curr->style.attributes;

  fputs (ESC, stdout);
  
  for (i = 0; i < _ATTRIB_NUM; ++i)
    {
      if ((attrib & _attrib_val[i]) == _attrib_val[i])
        fputs (_attrib_str[i], stdout);
    }

  fputs (ATTRIBUTE, stdout);
}

static inline void
_tdl_print_bg_color (tdl_color_t color)
{
  if (color == TDL_DEFAULT_COLOR)
    fputs (ESC "49" ATTRIBUTE, stdout);
  else
    printf (ESC BG_COLOR "%u" ATTRIBUTE, color);
}

static inline void
_tdl_print_fg_color (tdl_color_t color)
{
  if (color == TDL_DEFAULT_COLOR)
    fputs (ESC "39" ATTRIBUTE, stdout);
  else
    printf (ESC FG_COLOR "%u" ATTRIBUTE, color);
}

static void
_tdl_render_empty_line (tdl_point_t point, void *optional_data)
{
  printf (ESC "%d;1" CURSOR_POS ESC ATTRIBUTE ESC ERASE_LINE,
	  point.y + 1);

  (void)optional_data;
}

static void
_tdl_prepare_render_new_line (tdl_point_t pos,
			     void *optional_data)
{
  (void)optional_data;

  printf (ESC "%d;%d" CURSOR_POS, pos.y + 1, pos.x + 1);
}

static void
_tdl_render_char (tdl_point_t point,
		  tdl_renderer_signals_t ren_signals,
		  tdl_char_t *ch,
		  void *optional_data)
{
  (void)optional_data;
  (void)point;

  if (ren_signals.set_attribute)
    _tdl_print_attributes (ch);

  if (ren_signals.set_color_bg)
    _tdl_print_bg_color (ch->style.color.bg);

  if (ren_signals.set_color_fg)
    _tdl_print_fg_color (ch->style.color.fg);

  fputs (ch->character, stdout);
}

tdl_renderer_t tdl_default_renderer =
  {
    .render_empty_line = _tdl_render_empty_line,
    .render_before_line = _tdl_prepare_render_new_line,
    .render_char = _tdl_render_char,
    .optional_data = NULL
  };
