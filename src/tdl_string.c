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

#include "tdl_string.h"
#include <string.h>

#define UTF8_CODEPOINT_4B 0xF0
#define UTF8_CODEPOINT_3B 0xE0
#define UTF8_CODEPOINT_2B 0xC0

static const int cpoint_len[3][2] =
  {
    { UTF8_CODEPOINT_2B, 2 },
    { UTF8_CODEPOINT_3B, 3 },
    { UTF8_CODEPOINT_4B, 4 }
  };

static size_t
tdl_get_utf8char_len (char *chptr)
{
  size_t i = 0;
  
  for (i = 0; i < 3; ++i)
    if ((cpoint_len[i][0] & *chptr) == cpoint_len[i][0])
      return (size_t) cpoint_len[i][1];  
    else if (i == 2)
      return 1;

  return 1;
}

static void
tdl_copy_cstring_to_tdlstring (tdl_string_t tdlstr, char *cstr)
{
  size_t i = 0;
  size_t j = 0;
  size_t k = 0;
  size_t len = strlen (cstr);

  memset (tdlstr.string, 0, sizeof (tdl_utf8char_t) * tdlstr.length);
  
  for (i = 0; i < len; i += k)
    {
      k = tdl_get_utf8char_len (cstr + i);
      memcpy (tdlstr.string[j++], cstr + i, k);
    }
}

size_t
tdl_strlen (char *string)
{
  size_t length = 0;
  size_t i = 0;

  for (; string[i]; ++length)
    i += tdl_get_utf8char_len (string + i);

  return length;
}

tdl_string_t
tdl_string (char *string)
{
  tdl_string_t ret = {NULL, 0};
  
  ret.length += tdl_strlen (string);
  ret.string = malloc (ret.length * sizeof (tdl_utf8char_t));

  tdl_copy_cstring_to_tdlstring (ret, string);

  return ret;
}

int
tdl_string_free (tdl_string_t *str)
{
  free (str->string);

  str->string = NULL;
  str->length = 0;

  return EXIT_SUCCESS;
}

tdl_string_t
tdl_string_set (tdl_string_t *string, char *newstr)
{
  size_t newsize = tdl_strlen (newstr);

  if (newsize <= string->length)
    tdl_copy_cstring_to_tdlstring (*string, newstr);
  else
    {
      string->string = realloc (string->string, newsize * sizeof (tdl_utf8char_t));
      string->length = newsize;

      tdl_copy_cstring_to_tdlstring (*string, newstr);
    }
  return *string;
}
