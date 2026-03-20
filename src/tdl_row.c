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

#include "tdl/tdl_row.h"

typedef struct _header
{
  size_t capacity, count;
  bool is_clear;
} _header_t;

#define HEADER(bs) (((_header_t*) (bs)) - 1)
#define DATA(h) ((tdl_char_t*) ((h) + 1))

size_t
tdl_row_size (tdl_row_t row)
{
  if (row == NULL)
    return 0;
    
  return HEADER(row)->count;
}

bool
tdl_row_set_clear (tdl_row_t row, bool is_empty)
{
  if (row == NULL)
    return false;

  HEADER(row)->is_clear = is_empty;

  return true;
}

bool tdl_row_is_clear (tdl_row_t row) {
  if (row == NULL)
    return true;

  return HEADER(row)->is_clear;
}

tdl_row_t 
tdl_row (size_t width)
{
  _header_t header = { .capacity = width, .count = width, .is_clear = true };
  _header_t *head = malloc (sizeof (_header_t) + sizeof (tdl_char_t)*width);
  tdl_char_t tchar = tdl_char (" ", tdl_default_style);
  tdl_row_t row = NULL;
  size_t i;

  *head = header;
  row = DATA (head);

  for (i = 0; i < header.count; ++i) {
    row[i] = tchar;
  }

  return row;
}

bool
tdl_row_free (tdl_row_t row)
{
  if (!row)
    return false;

  free (HEADER (row));
  
  return true;
}

bool
tdl_row_resize (tdl_row_t *row, size_t newsize)
{
  _header_t *header = NULL;
  _header_t *tmp = NULL;
  tdl_char_t tchar;
  size_t newcapacity, i;
  
  if (row == NULL)
    return false;

  header = HEADER (*row);

  if (header->count == newsize)
    return true;

  newcapacity = header->capacity * 2;

  if (header->capacity >= newsize)
    {
      header->count = newsize;
    }
  else
    {
      while (newcapacity < newsize) newcapacity *= 2;

      tmp = realloc (header, sizeof (_header_t)
		     + sizeof (tdl_char_t) * newcapacity);

      if (tmp == NULL)
	return false;

      tmp->capacity = newcapacity;
      tmp->count = newsize;
      
      *row = DATA (tmp);
    }

  if (newsize > header->count)
    {
      tchar = tdl_char (" ", tdl_default_style);

      for (i = header->count; i < newsize; ++i)
	(*row)[i] = tchar;
    }
  
  return true;
}

bool
tdl_row_copy (tdl_row_t *dest, tdl_row_t *src)
{
  _header_t *dest_h, *src_h;
  size_t i;

  if (!tdl_row_resize (dest, HEADER(*src)->count))
    return false;

  dest_h = HEADER (*dest);
  src_h = HEADER (*src);

  dest_h->is_clear = src_h->is_clear;

  for (i = 0; src_h->count; ++i)
    (*dest)[i] = (*src)[i];
  
  return true;
}
