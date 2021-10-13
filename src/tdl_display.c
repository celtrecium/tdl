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

#include "tdl/tdl_display.h"
#include "tdl/tdl_linediff.h"
#include "tdl/tdl_objects.h"
#include "tdl/tdl_style.h"
#include "tdl/tdl_buffer.h"
#include "tdl/tdl_bufferline.h"
#include <sbvector.h>
#include <stdio.h>
#include <stdlib.h>
#include "tdl/tdl_ansi_sequences.h"


typedef struct tdl_display_signal
{
  bool display_color_bg;
  bool display_color_fg;
  bool display_attribute;
} _tdl_display_signal_t;

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

static inline _tdl_display_signal_t
_tdl_display_signal (bool bg, bool fg, bool attrib)
{
  return (_tdl_display_signal_t){ bg, fg, attrib };
}

static _tdl_display_signal_t
_tdl_display_state (tdl_buffer_point_t *curr, tdl_buffer_point_t *prev)
{
  _tdl_display_signal_t ret = _tdl_display_signal (false, false, false);

  if (!prev)
    return _tdl_display_signal (true, true, true);

  ret.display_attribute = curr->style.attributes != prev->style.attributes;
  ret.display_color_bg = curr->style.color.bg != prev->style.color.bg;
  ret.display_color_fg = curr->style.color.fg != prev->style.color.fg;

  return ret;
}

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

static bool
_tdl_print_line (sbvector_t *line, tdl_ldiff_t *ldiff)
{
  size_t i = 0;
  _tdl_display_signal_t dispsig;
  tdl_buffer_point_t *curr = NULL, *prev = NULL;

  for (i = ldiff->first_modified; i <= ldiff->last_modified; ++i)
    {
      curr = sbv_get (line, tdl_buffer_point_t, i);
      
      if (!curr)
        return false;

      dispsig = _tdl_display_state (curr, prev);

      if (dispsig.display_attribute)
        _tdl_print_attributes (curr);

      if (dispsig.display_color_bg)
        printf (ESC BG_COLOR "%u" ATTRIBUTE, curr->style.color.bg);

      if (dispsig.display_color_fg)
        printf (ESC FG_COLOR "%u" ATTRIBUTE, curr->style.color.fg);

      fputs (curr->character, stdout);

      prev = curr;
    }

  return true;
}

bool
tdl_display (tdl_canvas_t *canv)
{
  size_t i;
  tdl_ldiff_t *ldiffptr;
  tdl_buffer_line_t *bl;
  
  for (i = 0; i < canv->diff.length; ++i)
    {
      ldiffptr = sbv_get (&canv->diff, tdl_ldiff_t, i);
      bl       = sbv_get (&canv->buffer.fbuff, tdl_buffer_line_t,
                    ldiffptr->line_number);

      if (bl->_is_empty)
        {
          printf (ESC "%zu;0" CURSOR_POS ESC ATTRIBUTE ESC ERASE_LINE,
                  ldiffptr->line_number + 1);

          continue;
        }
      
      tdl_ldiff_clarify_line_edges (ldiffptr, &canv->buffer);

      if (ldiffptr->first_modified > ldiffptr->last_modified)
         continue;
      
      printf (ESC "%zu;%zu" CURSOR_POS, ldiffptr->line_number + 1,
              ldiffptr->first_modified + 1);

      _tdl_print_line (&bl->line, ldiffptr);

      putchar ('\n');
    }

  puts (ESC ATTRIBUTE);

  tdl_buffer_fbuff_to_sbuff (&canv->buffer);
  sbv_clear (&canv->diff);
  
  return true;
}
