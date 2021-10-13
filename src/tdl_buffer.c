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
#include "tdl/tdl_bufferline.h"
#include "tdl/tdl_style.h"
#include "tdl/tdl_bufferpoint.h"
#include "tdl/tdl_geometry.h"
#include <sbvector.h>
#include <u8string.h>

sbvector_t
_tdl_buff_allocate (tdl_size_t size)
{
  size_t i;
  sbvector_t vec;
  
  vec = sbvector (sizeof (tdl_buffer_line_t));
  sbv_resize (&vec, size.height);

  for (i = 0; i < size.height; ++i)
    sbv_set (&vec, tdl_buffer_line_t, i, tdl_buffer_line(size.width));

  return vec;
}

bool
_tdl_buff_free (sbvector_t *vec)
{
  size_t i;
  
  for (i = 0; i < vec->length; ++i)
    if (!sbv_free (sbv_get (vec, sbvector_t, i)))
      return false;

  return sbv_free (vec);
}

static inline bool
_tdl_buff_resize_height_up (sbvector_t *vec, tdl_size_t newsize)
{
  size_t i;

  if (!sbv_resize (vec, newsize.height))
    return false;

  for (i = vec->length; i < newsize.height; ++i)
    sbv_set (vec, sbvector_t, i, sbvector (sizeof (tdl_buffer_point_t)));
  
  return true;
}

static inline bool
_tdl_buff_resize_height_downward (sbvector_t *vec, tdl_size_t newsize)
{
  size_t i;

  for (i = newsize.height; i < vec->length; ++i)
    if (!sbv_free (sbv_get (vec, sbvector_t, i)))
      return false;

  if (!sbv_resize (vec, newsize.height))
    return false;

  return true;
}

static inline bool
_tdl_buff_resize_height (sbvector_t *vec, tdl_size_t newsize,
                         tdl_size_t prevsize)
{
  
  if (newsize.height > prevsize.height)
    _tdl_buff_resize_height_up (vec, newsize);
  else if (newsize.height > prevsize.height)
    _tdl_buff_resize_height_downward (vec, newsize);

  return true;
}

static inline bool
_tdl_buff_resize_width_up (sbvector_t *vec, tdl_size_t newsize,
                           tdl_size_t prevsize)
{
  size_t i, j;
  tdl_buffer_point_t bpt;
  sbvector_t *tmpptr;

  bpt = tdl_buffer_point (
      " ", tdl_style (tdl_point_color (256, 256), TDL_NO_ATTRIBUTES));

  for (i = 0; i < vec->length; ++i)
    {
      tmpptr = sbv_get (vec, sbvector_t, i);

      if (!sbv_resize (tmpptr, newsize.width))
        return false;

      for (j = prevsize.width; j < newsize.width; ++j)
        sbv_set (tmpptr, tdl_buffer_point_t, j, bpt);
    }

  return true;
}

static inline bool
_tdl_buff_resize_width_downward (sbvector_t *vec, tdl_size_t newsize)
{
  size_t i;
  
  for (i = 0; i < vec->length; ++i)
    if (!sbv_resize (sbv_get (vec, sbvector_t, i), newsize.width))
      return false;
  
  return true;
}

static inline bool
_tdl_buff_resize_width (sbvector_t *vec, tdl_size_t newsize,
                         tdl_size_t prevsize)
{
  if (newsize.width > prevsize.width)
    _tdl_buff_resize_width_up (vec, newsize, prevsize);
  else if (newsize.width < prevsize.width)
    _tdl_buff_resize_width_downward (vec, newsize);

  return true;
}

static bool
_tdl_buff_resize (sbvector_t *vec, tdl_size_t prevsize, tdl_size_t newsize)
{
  if (!vec ||
      !_tdl_buff_resize_height (vec, newsize, prevsize) ||
      !_tdl_buff_resize_width(vec, newsize, prevsize))
    return false;

  return true;
}

tdl_buffer_t
tdl_buffer (tdl_size_t size)
{
  tdl_buffer_t buff;

  buff.size = size;

  buff.fbuff = _tdl_buff_allocate (size);
  buff.sbuff = _tdl_buff_allocate (size);

  return buff;
}

bool
tdl_buffer_free (tdl_buffer_t *buff)
{
  buff->size = tdl_size (0, 0);

  if (!buff)
    return false;
  
  if (!_tdl_buff_free (&buff->fbuff) ||
      !_tdl_buff_free (&buff->sbuff))
    return false;

  return true;
}

bool
tdl_buffer_resize (tdl_buffer_t *buff, tdl_size_t newsize)
{
  if (!buff ||
      !_tdl_buff_resize (&buff->fbuff, buff->size, newsize) ||
      !_tdl_buff_resize (&buff->sbuff, buff->size, newsize))
    return false;

  buff->size = newsize;
  
  return true;
}

tdl_buffer_point_t *
tdl_buffer_get_point (tdl_buffer_t *buff, tdl_point_t point)
{
  if (!buff)
    return NULL;

  return tdl_buffer_line_get (
      sbv_get (&buff->fbuff, tdl_buffer_line_t, (size_t)point.y),
      (size_t)point.x);
}

static inline tdl_buffer_point_t *
_tdl_sbuffer_get_point (tdl_buffer_t *buff, tdl_point_t point)
{
  return tdl_buffer_line_get (
      sbv_get (&buff->sbuff, tdl_buffer_line_t, (size_t)point.y),
      (size_t)point.x);
}

bool
tdl_buffer_check_point_mod (tdl_buffer_t *buff, tdl_point_t point)
{
  tdl_buffer_point_t *fpt = tdl_buffer_get_point (buff, point);
  tdl_buffer_point_t *spt = _tdl_sbuffer_get_point (buff, point);
  
  return tdl_style_compare (&fpt->style, &spt->style)
         & u8char_compare (fpt->character, spt->character);
}

bool
tdl_buffer_set_point (tdl_buffer_t *buff, tdl_point_t point,
                      tdl_buffer_point_t bpt)
{
  tdl_buffer_line_t *bl;
  
  if (!buff)
    return false;

  bl = sbv_get (&buff->fbuff, tdl_buffer_line_t, (size_t)point.y);

  bl->_is_empty = false;
  *tdl_buffer_line_get (bl, (size_t)point.x) = bpt;
  
  return true;
}

bool
tdl_buffer_fbuff_to_sbuff (tdl_buffer_t *buff)
{
  size_t i;
  
  if (!buff)
    return false;

  for (i = 0; i < buff->size.height; ++i)
    {
      tdl_buffer_line_copy (sbv_get (&buff->sbuff, tdl_buffer_line_t, i),
                            sbv_get (&buff->fbuff, tdl_buffer_line_t, i));
    }

  return true;
}
