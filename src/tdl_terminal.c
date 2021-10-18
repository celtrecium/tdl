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
#include <stdio.h>

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
