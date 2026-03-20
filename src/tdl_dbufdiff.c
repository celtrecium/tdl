#include "tdl/tdl_rowdiff.h"
#include "tdl/tdl_dbufdiff.h"

typedef struct _header
{
  size_t capacity, count;
} _header_t;

#define HEADER(dbd) (((_header_t*) (dbd)) - 1)
#define DATA(h) ((tdl_rowdiff_t*) ((h) + 1))

#define INIT_CAP 128

tdl_dbufdiff_t
tdl_dbufdiff (void)
{
  _header_t header = { .capacity = INIT_CAP, .count = 0 };
  _header_t *head =
    malloc (sizeof (_header_t) + sizeof (tdl_rowdiff_t) * INIT_CAP);
  
  *head = header;

  return DATA (head);
}

size_t
tdl_dbufdiff_size (tdl_dbufdiff_t dbdiff)
{
  if (dbdiff == NULL)
    return 0;

  return HEADER(dbdiff)->count;
}

bool
tdl_dbufdiff_set (tdl_dbufdiff_t *dbdiff, tdl_rowdiff_t rdiff)
{
  _header_t *header, *tmp;
  size_t i;

  if (dbdiff == NULL || *dbdiff == NULL)
    return false;

  header = HEADER(*dbdiff);

  for (i = 0; i < header->count; ++i)
    {
      if ((*dbdiff)[i].row_number == rdiff.row_number)
	{
	  tdl_rowdiff_combine(&(*dbdiff)[i], rdiff);

	  return true;
	}
    }
  
  if (header->count + 1 > header->capacity) {
    tmp = realloc (header, sizeof (_header_t)
		   + sizeof (tdl_rowdiff_t)*header->capacity*2);

    if (tmp == NULL)
      return false;

    header = tmp;
    *dbdiff = DATA (header);
  }

  (*dbdiff)[header->count++] = rdiff;
  
  return true;
}

bool
tdl_dbufdiff_clear (tdl_dbufdiff_t dbdiff)
{
  if (dbdiff == NULL)
    return false;

  HEADER(dbdiff)->count = 0;
  
  return true;
}

bool
tdl_dbufdiff_free (tdl_dbufdiff_t dbdiff)
{
  if (dbdiff == NULL)
    return false;

  free (HEADER(dbdiff));
  
  return true;
}
