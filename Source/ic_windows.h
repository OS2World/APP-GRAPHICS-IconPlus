/* Icon Plus
 * Copyright (C) 2001-2005 Dmitry A.Steklenev
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * $Id: ic_windows.h,v 1.7 2009/11/12 13:06:37 glass Exp $
 */

#ifndef IC_WINDOWS_H
#define IC_WINDOWS_H

#include "ic_config.h"
#include "ic_file.h"
#include "ic_palette.h"
#include "ic_bitmap.h"

#if !defined(__ccdoc__)

struct win_icon_directory
{
  UINT16 reserved;      /* Reserved; must be zero. */
  UINT16 type;          /* Specifies the resource type. This member is set to 1. */
  UINT16 count;         /* Specifies the number of entries in the directory. */
};

struct win_icon_entry
{
  BYTE   width;         /* Specifies the width of the icon, in pixels. */
  BYTE   height;        /* Specifies the height of the icon, in pixels. */
  BYTE   colorCount;    /* Specifies the number of colors in the icon. */
  BYTE   reserved;      /* Reserved; must be zero. */
  UINT16 planes;        /* Specifies the number of color planes in the icon bitmap. */
  UINT16 bitCount;      /* Specifies the number of bits in the icon bitmap. */
  UINT32 bytesInRes;    /* Specifies the size of the resource, in bytes. */
  UINT32 imageOffset;   /* Specifies the offset, in bytes, from the beginning
                           of the file to the icon image or the resource ID. */
};

struct win_bitmap_header
{
  UINT32 size;          /* Specifies the number of bytes required by the structure */
  INT32  width;         /* Specifies the width of the bitmap, in pixels. */
  INT32  height;        /* Specifies the height of the bitmap, in pixels. If biHeight
                           is positive, the bitmap is a bottom-up DIB and its origin
                           is the lower left corner. If biHeight is negative, the
                           bitmap is a top-down DIB and its origin is the upper
                           left corner. */
  UINT16 planes;        /* Specifies the number of planes for the target device */
  UINT16 bitCount;      /* Specifies the number of bits per pixel. */
  UINT32 compression;   /* Specifies the type of compression */
  UINT32 sizeImage;     /* Specifies the size, in bytes, of the image.
                           This may be set to 0 for uncompressed bitmaps. */
  UINT32 xPelsPerMeter; /* Specifies the horizontal resolution, in pixels per meter. */
  UINT32 yPelsPerMeter; /* Specifies the vertical resolution, in pixels per meter. */
  UINT32 clrUsed;       /* Specifies the number of color indices in the color table
                           that are actually used by the bitmap */
  UINT32 clrImportant;  /* Specifies the number of color indices that are considered
                           important for displaying the bitmap */

  ICPalette palette;    /* This is not a part of the standard header! */
};

struct win_icon_info
{
  win_icon_entry    entry;
  win_bitmap_header bitmap;

  class ICIcon* icon;
};

#endif

/**
 * Loads and saves MS Windows icon files.
 * @author Dmitry A Steklenev
 */

class ICWindows
{
  public:
    /** Loads icon file. */
    virtual void load( ICFile* load_file );
    /** Saves icon file. */
    virtual void save( ICFile* save_file );

  private:
    virtual win_icon_entry    load_entry();
    virtual win_bitmap_header load_bitmap_header();

    virtual void load_x32map_data  ( ICBitmap&, ICAlpha& );
    virtual void save_x32map_data  ( ICBitmap&, ICAlpha& );
    virtual void load_bitmap_data  ( ICBitmap& );
    virtual void save_bitmap_data  ( ICBitmap& );
    virtual void save_entry        ( const win_icon_entry&    );
    virtual void save_bitmap_header( const win_bitmap_header& );

  private:
    ICFile* file;
};

#endif

