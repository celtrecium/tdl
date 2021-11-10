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

#include "tdl/tdl_image.h"
#include "tdl/tdl_bufferpoint.h"
#include "tdl/tdl_canvas.h"
#include "tdl/tdl_geometry.h"
#include "tdl/tdl_bufferline.h"
#include "tdl/tdl_linediff.h"
#include "tdl/tdl_style.h"
#include <bits/types/FILE.h>
#include <sbvector.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <u8string.h>

#define SIGNATURE "tdlimg!"

static inline bool
_check_signature (FILE *file)
{
  char signature[sizeof(SIGNATURE)];

  fread (signature, sizeof (char), sizeof (signature) - 1, file);

  return !strcmp (signature, SIGNATURE);
}

static inline void
_write_size (FILE *file, const tdl_size_t *size)
{
  fwrite (&size->height, sizeof (size_t), 1, file);
  fwrite (&size->width, sizeof (size_t), 1, file);
}

static inline void
_read_size (FILE *file, tdl_size_t *size)
{
  fread (&size->height, sizeof (size_t), 1, file);
  fread (&size->width, sizeof (size_t), 1, file);
}

static inline void
_write_point (FILE *file, tdl_buffer_point_t *point)
{
  fwrite (point->character, sizeof (char), U8CHAR_LEN, file);
  fwrite (&point->style.attributes, sizeof (tdl_attributes_t), 1, file);
  fwrite (&point->style.color.bg, sizeof (tdl_color_t), 1, file);
  fwrite (&point->style.color.fg, sizeof (tdl_color_t), 1, file);
}

static inline void
_write_line (FILE *file, tdl_buffer_line_t *line)
{
  size_t i = 0;

  for (i = 0; i < line->line.length; ++i)
    _write_point (file, sbv_get (&line->line, tdl_buffer_point_t, i));
}

static inline void
_write_image (FILE *file, tdl_image_t *img)
{
  size_t i;

  for (i = 0; i < img->size.height; ++i)
    _write_line (file, sbv_get (&img->image, tdl_buffer_line_t, i));
}

static inline void
_read_point (tdl_buffer_point_t *point, FILE *file)
{
  fread (&point->character, sizeof (char), U8CHAR_LEN, file);
  fread (&point->style.attributes, sizeof (tdl_attributes_t), 1, file);
  fread (&point->style.color.bg, sizeof (tdl_color_t), 1, file);
  fread (&point->style.color.fg, sizeof (tdl_color_t), 1, file);
}

static inline void
_read_line (tdl_buffer_line_t *line, FILE *file)
{
  size_t i = 0;

  for (i = 0; i < line->line.length; ++i)
    _read_point (sbv_get (&line->line, tdl_buffer_point_t, i), file);
}

static inline void
_read_image (tdl_image_t *img, FILE *file)
{
  size_t i = 0;

  for (i = 0; i < img->size.height; ++i)
    _read_line (sbv_get (&img->image, tdl_buffer_line_t, i), file);
}

bool
tdl_image_save (tdl_image_t img, const char *filename)
{
  FILE *imgfile = fopen (filename, "w+");

  if (!imgfile)
    return false;
  
  fputs (SIGNATURE, imgfile);
  _write_size(imgfile, &img.size);

  _write_image(imgfile, &img);

  fclose (imgfile);

  return true;
}

static inline void
_initialize_image_buffer (tdl_image_t *img)
{
  size_t i = 0;
  sbvector_t *ptr;

  img->image = sbvector (sizeof (sbvector_t));
  sbv_resize (&img->image, img->size.height);

  for (i = 0; i < img->size.height; ++i)
    {
      ptr = sbv_get (&img->image, sbvector_t, i);
      *ptr = sbvector (sizeof (tdl_buffer_line_t));
      sbv_resize (ptr, img->size.width);
    }
}

tdl_image_t
tdl_image_load (const char *filename)
{
  tdl_image_t img;
  FILE *imgfile = fopen (filename, "r");

  if (!_check_signature (imgfile))
    {
      fclose (imgfile);
      img.size = tdl_size (0, 0);
      
      return img;
    }

  _read_size (imgfile, &img.size);

  _initialize_image_buffer (&img);
  _read_image (&img, imgfile);

  fclose (imgfile);

  return img;
}

static inline void
_copy_canvas_point (tdl_buffer_point_t *point, tdl_buffer_point_t *imgpoint)
{
  u8char_copy (imgpoint->character, point->character);
  imgpoint->style.color.bg = point->style.color.bg;
  imgpoint->style.color.fg = point->style.color.fg;
  imgpoint->style.attributes = point->style.attributes;
}

static inline void
_copy_canvas_line (sbslice_t line, tdl_buffer_line_t *imgline)
{
  size_t i = 0;

  for (i = 0; i < line.length; ++i)
    _copy_canvas_point (sbslice_get (&line, tdl_buffer_point_t, i),
                        sbv_get (&imgline->line, tdl_buffer_point_t, i));
}

static inline void
_copy_cropped_canvas (tdl_canvas_t *canv, tdl_image_t *img,
                               tdl_rectangle_t *rect)
{
  sbslice_t canv_slice_height
      = sbslice (&canv->buffer.fbuff, (size_t)rect->point.y,
                 (size_t)rect->point.y + rect->size.height);
  size_t i = 0;

  for (i = 0; i < canv_slice_height.length; ++i)
    _copy_canvas_line (
        sbslice (sbslice_get (&canv_slice_height, sbvector_t, i),
                 (size_t)rect->point.x,
                 (size_t)rect->point.x + rect->size.width),
        sbv_get (&img->image, tdl_buffer_line_t, i));
}

tdl_image_t
tdl_image_crop_from_canvas (tdl_canvas_t *canv, tdl_rectangle_t rect)
{
  tdl_image_t img;

  if (!canv)
    return (tdl_image_t){ sbvector (0), tdl_size (0, 0) };

  img.size.width = rect.size.width;
  img.size.height = rect.size.height;
  
  _initialize_image_buffer (&img);

  _copy_cropped_canvas (canv, &img, &rect);

  return img;
}

bool
tdl_image_free (tdl_image_t *img)
{
  size_t i = 0;

  for (i = 0; i < img->size.height; ++i)
    tdl_buffer_line_free (sbv_get (&img->image, tdl_buffer_line_t, i));

  return true;
}

static void
_tdl_set_diff (sbvector_t *sbv, tdl_point_t modpt)
{
  size_t i = 0;
  tdl_ldiff_t *ldiff;

  for (i = 0; i < sbv->length; ++i)
    {
      if ((ldiff = sbv_get (sbv, tdl_ldiff_t, i))->line_number
          == (size_t)modpt.y)
        {
          tdl_ldiff_set (ldiff, (size_t)modpt.x);
          
          return;
        }
    }

  sbv_push (sbv, tdl_ldiff_t,
            tdl_ldiff ((size_t)modpt.y, (size_t)modpt.x, (size_t)modpt.x));
}

static inline void
_set_line_diff (tdl_canvas_t *canv, tdl_point_t *begin, tdl_point_t *end)
{
      _tdl_set_diff (&canv->diff, *begin);
      _tdl_set_diff (&canv->diff, *end);
      sbv_get (&canv->buffer.fbuff, tdl_buffer_line_t, (size_t)begin->y)
          ->_is_empty
          = false;
}

static inline void
_set_rectangle_diff (tdl_canvas_t *canv, tdl_rectangle_t rect)
{
  size_t i = 0;
  tdl_point_t begin = (tdl_point_t){ rect.point.x, rect.point.y };
  tdl_point_t end
      = (tdl_point_t){ rect.point.x + (int)rect.size.width, rect.point.y };

  for (i = 0; i < rect.size.height; ++i, ++begin.y, ++end.y)
    _set_line_diff (canv, &begin, &end);
}

static inline void
_copy_buffer_point (tdl_buffer_point_t *dest, tdl_buffer_point_t *src)
{
  u8char_copy(dest->character, src->character);
  dest->style.attributes = src->style.attributes;
  dest->style.color.bg = src->style.color.bg;
  dest->style.color.fg = src->style.color.fg;
}


static inline void
_copy_line_to_canvas (tdl_buffer_line_t *line, tdl_buffer_line_t *imgline,
                      tdl_point_t pos)
{
  sbslice_t line_slice = sbslice (&line->line, (size_t)pos.x,
                                  imgline->line.length + (size_t)pos.y);
  size_t i = 0;

  for (i = 0; i < line_slice.length; ++i)
    _copy_buffer_point (sbslice_get (&line_slice, tdl_buffer_point_t, i),
                        sbv_get (&imgline->line, tdl_buffer_point_t, i));
}

bool
tdl_image_print_to_canvas (tdl_canvas_t *canv, tdl_image_t img,
                           tdl_point_t pos)
{
  size_t i = 0;
  sbslice_t canv_slice = sbslice (&canv->buffer.fbuff, (size_t)pos.y,
                                  img.size.height + (size_t)pos.y);

  if (!canv || !img.size.height || !img.size.width)
    return false;

  _set_rectangle_diff (canv, tdl_rectangle (pos, img.size));

  for (i = 0; i < canv_slice.length; ++i)
    _copy_line_to_canvas (sbslice_get (&canv_slice, tdl_buffer_line_t, i),
                          sbv_get (&img.image, tdl_buffer_line_t, i), pos);


  return true;
}
