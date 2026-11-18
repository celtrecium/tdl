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

#include <tdl/tdl_buffer.h>
#include <tdl/tdl_row.h>
#include <tdl/tdl_style.h>
#include <tdl/tdl_scroll_reg.h>

typedef struct _header
{
  size_t capacity, count;
  tdl_size_t size;
} _header_t;

#define HEADER(b) (((_header_t*) (b)) - 1)
#define DATA(h) ((tdl_row_t*) ((h) + 1))

tdl_buffer_t
tdl_buffer (tdl_size_t size)
{
  _header_t header = { .capacity = size.height, .count = size.height, .size = size };
  _header_t *head = malloc (sizeof (_header_t) + sizeof (tdl_row_t)*size.height);
  tdl_buffer_t buffer = NULL;
  size_t i;

  *head = header;
  buffer = DATA (head);

  for (i = 0; i < header.count; ++i) {
    buffer[i] = tdl_row (size.width);
  }

  return buffer;
}

tdl_size_t
tdl_buffer_size (tdl_buffer_t *buffer)
{
  if (buffer == NULL)
    return (tdl_size_t) { .width = 0, .height = 0 };

  return HEADER(buffer)->size;
}

bool
tdl_buffer_resize(tdl_buffer_t *buffer, tdl_size_t newsize)
{
  _header_t *header = NULL;
  _header_t *tmp = NULL;
  size_t newcapacity, i;
  
  header = HEADER (*buffer);

  if (header->size.height == newsize.height
      && header->size.width == newsize.width)
    return true;

  newcapacity = header->capacity * 2;

  if (header->count >= newsize.height)
    {
      for (i = 0; i < newsize.height; ++i)
        if (!tdl_row_resize (&((*buffer)[i]), newsize.width))
          return false;

      for (i = newsize.height; i < header->count; ++i)
        {
          tdl_row_free ((*buffer)[i]);
          (*buffer)[i] = NULL;
        }
    }
  else
    {
      if (header->capacity < newsize.height)
        {
          while (newcapacity < newsize.height) newcapacity *= 2;

          tmp = realloc (header, sizeof (_header_t)
                         + sizeof (tdl_row_t) * newcapacity);

          if (tmp == NULL)
            return false;
          
          header = tmp;
          *buffer = DATA (header);

          header->capacity = newcapacity;
        }

      for (i = 0; i < header->count; ++i)
        if (!tdl_row_resize (&((*buffer)[i]), newsize.width))
          return false;

      for (i = header->count; i < newsize.height; ++i)
        (*buffer)[i] = tdl_row (newsize.width);
    }

  header->count = newsize.height;
  header->size = newsize;
  
  return true;
}

bool
tdl_buffer_copy (tdl_buffer_t *dest, tdl_buffer_t *src)
{
  tdl_size_t src_size;
  size_t i;
  
  src_size = HEADER(*src)->size;
  
  if (!tdl_buffer_resize (dest, src_size))
    return false;

  for (i = 0; i < src_size.height; ++i)
    if(!tdl_row_copy (&((*dest)[i]), &((*src)[i])))
      return false;
  
  return true;
}

void
tdl_buffer_free (tdl_buffer_t buffer)
{
  size_t i;
  _header_t *header = HEADER (buffer);

  for (i = 0; i < header->count; ++i) 
    tdl_row_free (buffer[i]);

  free (header);
}

void
tdl_buffer_clear_row (tdl_buffer_t buff, size_t row_n)
{
  size_t i;
  size_t w = tdl_row_size (buff[row_n]);
  u8codept_t empty_codept = { ' ', 0 };
  tdl_char_t empty_char = tdl_char (empty_codept, tdl_default_style);

  tdl_row_set_clear(buff[row_n], true);

  for (i = 0; i < w; ++i)
    buff[row_n][i] = empty_char;
}

void
tdl_buffer_clear (tdl_buffer_t buff)
{
  size_t i, h = HEADER(buff)->count;

  for (i = 0; i < h; ++i)
    tdl_buffer_clear_row (buff, i);
}

void
tdl_buffer_insert_rows_up (tdl_buffer_t buff, tdl_point_t cur,
                           tdl_scroll_reg_t reg, size_t n_rows)
{
  size_t i, j;
  tdl_row_t emptyrow;

  if (n_rows == 0)
    return;
  
  if (cur.y - (int) n_rows < (int) reg.top)
    n_rows = (size_t) cur.y - reg.top;

  for (i = 0; i < n_rows; ++i)
    tdl_row_clear (buff[i + reg.top]);

  for (i = 0; i < n_rows; ++i)
    {
      emptyrow = buff[reg.top];

      for (j = reg.top + 1; j <= (size_t) cur.y; ++j)
        buff[j - 1] = buff[j];

      buff[cur.y] = emptyrow;
    }
}

void
tdl_buffer_insert_rows_down (tdl_buffer_t buff, tdl_point_t cur,
                             tdl_scroll_reg_t reg, size_t n_rows)
{
  size_t i;
  int j;
  tdl_row_t emptyrow;

  if (n_rows == 0)
    return;

  if (reg.top + n_rows - 1 > reg.bottom)
    n_rows = reg.bottom - reg.top;

  for (i = 0; i < n_rows; ++i)
    tdl_row_clear (buff[reg.bottom - i]);

  for (i = 0; i < n_rows; ++i)
    {
      emptyrow = buff[reg.bottom];

      for (j = (int) reg.bottom - 1; j >= cur.y; --j)
        buff[j + 1] = buff[j];

      buff[cur.y] = emptyrow;
    }
}

bool
tdl_buffer_insert_blank_char (tdl_buffer_t buff, tdl_point_t point)
{
  _header_t *buff_h = HEADER (buff);

  if (point.x >= (int) buff_h->size.width ||
      point.y >= (int) buff_h->size.height)
    return false;

  tdl_row_insert_blank_char (buff[point.y], (size_t) point.x);

  return true;

}

bool
tdl_buffer_delete_char (tdl_buffer_t buff, tdl_point_t point)
{
  _header_t *buff_h = HEADER (buff);

  if (point.x >= (int) buff_h->size.width ||
      point.y >= (int) buff_h->size.height)
    return false;

  tdl_row_delete_char (buff[point.y], (size_t) point.x);

  return true;
}
