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


#include "tdl/tdl_shapes.h"
#include "tdl/tdl_canvas.h"
#include "tdl/tdl_geometry.h"
#include <stdlib.h>

/* 1.1. Functions for drawing shapes */
int
tdl_draw_line (tdl_canvas_t *canv, tdl_text_t text, tdl_line_t line)
{
  const int deltaX = abs(line.b.x - line.a.x);
  const int deltaY = abs(line.b.y - line.a.y);

  const int signX = line.a.x < line.b.x ? 1 : -1;
  const int signY = line.a.y < line.b.y ? 1 : -1;

  int error = deltaX - deltaY;
  int error2;

  tdl_point_t tmp_cur;
  
  if (canv == NULL)
    return EXIT_FAILURE;
  
  tmp_cur = canv->cursor;
  
  while (line.a.x != line.b.x || line.a.y != line.b.y)
    {
      tdl_set_cursor_pos (canv, line.a);
      tdl_print (canv, text);
      
      error2 = error * 2;

      if (error2 > -deltaY)
        {
          error -= deltaY;
          line.a.x += signX;
        }

      if (error2 < deltaX)
        {
          error += deltaX;
          line.a.y += signY;
        }
    }

  tdl_set_cursor_pos (canv, tmp_cur);
  
  return EXIT_SUCCESS;
}

int
tdl_draw_lines (tdl_canvas_t *canv, tdl_text_t text, tdl_line_t *lines,
                size_t n)
{
  size_t i = 0;
  
  if (canv == NULL)
    return EXIT_FAILURE;

  for (i = 0; i < n; ++i)
    tdl_draw_line (canv, text, lines[i]);

  return EXIT_SUCCESS;
}

int
tdl_draw_rectangle (tdl_canvas_t *canv, tdl_text_t text, tdl_rectangle_t rect)
{
  tdl_line_t lines[4] = {
    tdl_line (rect.point, tdl_point ((int)rect.size.width + rect.point.x + 1,
                                     rect.point.y)),
    tdl_line (rect.point,
        tdl_point (rect.point.x, rect.point.y + (int)rect.size.height + 1)),
    tdl_line (tdl_point (rect.point.x + (int)rect.size.width,
                         rect.point.y + (int)rect.size.height),
              tdl_point ((int)rect.size.width + rect.point.x, rect.point.y)),
    tdl_line (tdl_point (rect.point.x + (int)rect.size.width,
                         rect.point.y + (int)rect.size.height),
              tdl_point (rect.point.x, rect.point.y + (int)rect.size.height))
  };

  if (canv == NULL)
    return EXIT_FAILURE;

  tdl_draw_lines (canv, text, lines, 4);

  return EXIT_SUCCESS;
}

int
tdl_draw_filled_rectangle (tdl_canvas_t *canv, tdl_text_t text,
                           tdl_rectangle_t rect)
{
  size_t i = 0;
  size_t j = 0;
  
  if (canv == NULL)
    return EXIT_FAILURE;

  for (i = 0; i < rect.size.width; ++i)
    {
      for (j = 0; j < rect.size.height; ++j)
        {
          tdl_set_cursor_pos (
              canv, tdl_point ((int)i + rect.point.x, (int)j + rect.point.y));

          tdl_print (canv, text);
        }
    }

  return EXIT_SUCCESS;
}

/* 1.2. Functions for drawing an array of shapes */
int
tdl_draw_rectangles (tdl_canvas_t *canv, tdl_text_t text,
                     tdl_rectangle_t *rects, size_t n)
{
  size_t i = 0;
  
  if (canv == NULL)
    return EXIT_FAILURE;

  for (i = 0; i < n; ++i)
    tdl_draw_rectangle (canv, text, rects[i]);

  return EXIT_SUCCESS;
}

int
tdl_draw_filled_rectangles (tdl_canvas_t *canv, tdl_text_t text,
                            tdl_rectangle_t *rects, size_t n)
{
  size_t i = 0;
  
  if (canv == NULL)
    return EXIT_FAILURE;

  for (i = 0; i < n; ++i)
    tdl_draw_filled_rectangle (canv, text, rects[i]);

  return EXIT_SUCCESS;
}

