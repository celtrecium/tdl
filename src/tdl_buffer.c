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

#include "tdl/tdl_buffer.h"
#include "tdl/tdl_row.h"
#include "tdl/tdl_style.h"

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

  if (header->count == newsize.height)
    return true;

  newcapacity = header->capacity * 2;

  if (header->count >= newsize.height)
    {
      for (i = 0; i < newsize.height; ++i)
	if (!tdl_row_resize (&(*buffer[i]), newsize.width))
	  return false;

      for (i = newsize.height; i < header->count; ++i)
	tdl_row_free (*buffer[i]);
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
	if (!tdl_row_resize (&(*buffer[i]), newsize.width))
	  return false;

      for (i = header->count; i < newsize.height; ++i)
	*buffer[i] = tdl_row (newsize.width);
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
    if(!tdl_row_copy (&(*dest[i]), &(*src[i])))
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
  tdl_char_t empty_char = tdl_char (" ", tdl_default_style);

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
