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

#include "tdl/tdl_terminal.h"
#include "tdl/tdl_ansi_sequences.h"
#include "tdl/tdl_geometry.h"
#include <stdio.h>
#include <termios.h>

#if defined(__unix__)
#  include <termios.h>
#  include <sys/ioctl.h>
#elif defined(_WIN32) || defined(__CYGWIN__)
#  include <windows.h>
#endif

tdl_size_t
tdl_terminal_get_size (void)
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

void
tdl_terminal_clear (void)
{
  puts (CLEAR);
}

void
tdl_terminal_set_alternate_screen (bool is_enabled)
{
  if (is_enabled)
    puts (ALTERNATE_SCREEN_ENABLE);
  else
    puts (ALTERNATE_SCREEN_DISABLE);
}

void
tdl_terminal_set_cursor (bool is_enabled)
{
  if (is_enabled)
    puts (CURSOR_ENABLE);
  else
    puts (CURSOR_DISABLE);
}

tdl_point_t
tdl_terminal_get_cursor_pos (void)
{
  tdl_point_t position = tdl_point (0, 0);
  char buf[30] = { 0 };
  int i, pow;
  char ch;

  struct termios term, restore;

  tcgetattr (0, &term);
  tcgetattr (0, &restore);
  term.c_lflag &= (tcflag_t) ~(ICANON|ECHO);
  tcsetattr (0, TCSANOW, &term);

  fputs (GET_CURSOR_POS, stdout);

  for( i = 0, ch = 0; ch != 'R'; i++ )
    {
      ch = (char)fgetc (stdin);

      buf[i] = ch;
    }

  if (i < 2)
    {
      tcsetattr (0, TCSANOW, &restore);

      return position;
    }

  for (i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10)
    position.x += (buf[i] - '0') * pow;

  for ( i-- , pow = 1; buf[i] != '['; i--, pow *= 10)
    position.y += (buf[i] - '0') * pow;

  tcsetattr(0, TCSANOW, &restore);
  
  return tdl_point(position.x - 1, position.y - 1);
}
