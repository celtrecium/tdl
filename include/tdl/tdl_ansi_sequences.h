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


#ifndef TDL_ANSI_SEQUENCES_H
#define TDL_ANSI_SEQUENCES_H

#define ESC         "\033["
#define ATTRIBUTE   "m"
#define ERASE_LINE  "K"
#define BG_COLOR    "48;5;"
#define FG_COLOR    "38;5;"
#define CURSOR_POS  "H"
#define CURSOR_UP   "A"
#define CURSOR_DOWN "B"
#define CURSOR_FWD  "C"
#define CURSOR_BACK "D"
#define COLUMN      "G"
#define CLEAR       ESC "49;39" ATTRIBUTE ESC CURSOR_POS ESC "2J"
#define ALTERNATE_SCREEN_ENABLE  ESC "?1049h"
#define ALTERNATE_SCREEN_DISABLE ESC "?1049l"
#define CURSOR_ENABLE  ESC "?25h"
#define CURSOR_DISABLE ESC "?25l"
#define GET_CURSOR_POS ESC "6n"

#endif  /* TDL_ANSI_SEQUENCES_H */
