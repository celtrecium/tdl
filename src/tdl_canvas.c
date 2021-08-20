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

#include "tdl/tdl_canvas.h"
#include "tdl/tdl_geometry.h"
#include "tdl/tdl_bufferpoint.h"
#include "tdl/tdl_linediff.h"
#include "tdl/tdl_objects.h"
#include "tdl/tdl_buffer.h"
#include <sbvector.h>
#include <stdbool.h>

#if defined(__unix__)
#  include <termios.h>
#  include <sys/ioctl.h>
#elif defined(_WIN32) || defined(__CYGWIN__)
#  include <windows.h>
#endif

/* Crossplatform function for getting the terminal size */
static tdl_size_t
_tdl_get_term_size (void)
{
  tdl_size_t termsize;

#if defined(__unix__)
  struct winsize size;

  ioctl (0, TIOCGWINSZ, (char *)&size);

  termsize.height = size.ws_row;
  termsize.width = size.ws_col;
#elif defined(_WIN32) || defined(__CYGWIN__)
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  GetConsoleScreenBufferInfo (GetStdHandle (STD_OUTPUT_HANDLE), &csbi);
  termsize.width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  termsize.height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#endif

  return termsize;
}

static inline tdl_point_t
_tdl_get_point_in_buffer (tdl_canvas_t *canv, tdl_point_t point)
{
  tdl_point_t ptret;
  
  ptret.x = point.x % (long int)canv->size.width;
  ptret.y = point.y % (long int)canv->size.height;

  return ptret;
}

static void
_tdl_set_diff (sbvector_t *sbv, tdl_point_t modpt)
{
  size_t i = 0;
  tdl_ldiff_t *ldiff;

  for (i = 0; i < sbv->length; ++i)
    {
      if ((ldiff = sbv_get (sbv, tdl_ldiff_t, i))->line_number
          == (size_t)modpt.y)
        {
          tdl_ldiff_set (ldiff, (size_t)modpt.x);
          
          return;
        }
    }

  sbv_push (sbv, tdl_ldiff_t,
            tdl_ldiff ((size_t)modpt.y, (size_t)modpt.x, (size_t)modpt.x));
}

tdl_canvas_t *
tdl_canvas (void)
{
  tdl_canvas_t *canv = calloc (1, sizeof (tdl_canvas_t));
  
  canv->err = TDL_OK;
  
  canv->cursor = tdl_point (0, 0);
  canv->size = _tdl_get_term_size ();
  canv->diff = sbvector (sizeof (tdl_ldiff_t));

  canv->buffer = tdl_buffer (canv->size);

  return canv;
}

bool
tdl_destroy_canvas (tdl_canvas_t *canv)
{  
  if (canv == NULL)
    return false;

  canv->err = TDL_OK;

  sbv_free (&canv->diff);
  tdl_buffer_free (&canv->buffer);

  free (canv);

  return true;
}

/* 2. Canvas setters */
bool
tdl_set_cursor_pos (tdl_canvas_t *canv, tdl_point_t pos)
{
  if (canv == NULL)
    return false;

  canv->cursor = _tdl_get_point_in_buffer (canv, pos);

  return true;
}

bool
tdl_print (tdl_canvas_t *canv, tdl_text_t text)
{
  size_t i;
  tdl_buffer_point_t *buffpt = NULL;
  tdl_point_t cur;
  
  if (canv == NULL)
    return false;

  for (i = 0; i < text.string->length; ++i)
    {
      cur = canv->cursor;
      
      ++cur.x;

      if (u8char_compare(text.string->string[i], "\n") == true)
        ++cur.y;
      else if (u8char_compare(text.string->string[i], "\t") == true)
        cur.x += 8;
      
      tdl_set_cursor_pos (canv, cur);
      
      buffpt = tdl_buffer_get_point (&canv->buffer, canv->cursor);

      u8char_copy (buffpt->character, text.string->string[i]);
      buffpt->style = text.style;

      _tdl_set_diff (&canv->diff, canv->cursor);
    }
  
  return true;
}
