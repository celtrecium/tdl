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

#include <tdl/tdl_dbufdiff.h>

typedef struct _header
{
  size_t capacity, count;
} _header_t;

#define HEADER(dbd) (((_header_t*) (dbd)) - 1)
#define DATA(h) ((tdl_rowdiff_t*) ((h) + 1))

#define INIT_CAP 128

tdl_dbufdiff_t
tdl_dbufdiff (size_t size)
{
  _header_t header = { .capacity = size, .count = size };
  _header_t *head =
    malloc (sizeof (_header_t) + sizeof (tdl_rowdiff_t) * size);
  tdl_dbufdiff_t data;
  size_t i;

  *head = header;
  data = DATA (head);

  for (i = 0; i < size; ++i)
    data[i] = tdl_rowdiff();

  return data;
}

size_t
tdl_dbufdiff_size (tdl_dbufdiff_t dbdiff)
{
  if (dbdiff == NULL)
    return 0;

  return HEADER(dbdiff)->count;
}

bool
tdl_dbufdiff_resize (tdl_dbufdiff_t *dbdiff, size_t newsize)
{
  _header_t *header = NULL;
  _header_t *tmp = NULL;
  tdl_rowdiff_t rdiff = tdl_rowdiff();

  size_t newcapacity, i, prevsize;
  
  if (dbdiff == NULL)
    return false;

  header = HEADER ((*dbdiff));

  if (header->count == newsize)
    return true;

  newcapacity = header->capacity * 2;
  prevsize = header->count;

  if (header->capacity >= newsize)
    {
      header->count = newsize;
    }
  else
    {
      while (newcapacity < newsize) newcapacity *= 2;

      tmp = realloc (header, sizeof (_header_t)
                     + sizeof (tdl_rowdiff_t) * newcapacity);

      if (tmp == NULL)
        return false;

      tmp->capacity = newcapacity;
      tmp->count    = newsize;

      *dbdiff = DATA (tmp);
      header  = HEADER ((*dbdiff));
    }

  if (newsize > prevsize)
    {
      for (i = prevsize; i < newsize; ++i)
        (*dbdiff)[i] = rdiff;
    }
  
  return true;
}

void
tdl_dbufdiff_clear (tdl_dbufdiff_t dbdiff)
{
  size_t i;
  size_t size = HEADER(dbdiff)->count;
  tdl_rowdiff_t empty_rdiff = tdl_rowdiff();

  for (i = 0; i < size; ++i)
    dbdiff[i] = empty_rdiff;
}

void
tdl_dbufdiff_free (tdl_dbufdiff_t dbdiff)
{
  free (HEADER(dbdiff));
}
