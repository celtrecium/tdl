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

#ifndef TDL_TERMINAL_H
#define TDL_TERMINAL_H

#include <stdlib.h>
#include <stdbool.h>

void tdl_terminal_clear (void);
void tdl_terminal_set_alternate_screen (bool is_enabled);
void tdl_terminal_set_cursor (bool is_enabled);

#endif  /* TDL_TERMINAL_H */
