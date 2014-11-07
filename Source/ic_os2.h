/* Icon Plus
 * Copyright (C) 2001-2009 Dmitry A.Steklenev
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
 * $Id: ic_os2.h,v 1.7 2009/11/13 11:39:46 glass Exp $
 */

#ifndef IC_OS2_H
#define IC_OS2_H

#include "ic_config.h"
#include "ic_file.h"
#include "ic_palette.h"
#include "ic_bitmap.h"

#if !defined(__ccdoc__)

struct os2_bitmap_array_header
{
  UINT16 type;            /* Type of structure. */
  UINT32 cbsize;          /* Size of the structure in bytes. */
  UINT32 next;            /* Offset of the next bitmap array structure from
                             the start of the file. */
  UINT16 display_width;   /* Device width, in pels. */
  UINT16 display_height;  /* Device height, in pels. */
};

struct os2_bitmap_file_header
{
  UINT16 type;            /* Type of resource the file contains. */
  UINT32 cbsize;          /* Size of the structure in bytes. */
  INT16  xhotspot;        /* Width of hotspot for icons and pointers. */
  INT16  yhotspot;        /* Height of hotspot for icons and pointers. */
  UINT32 bitmap_offset;   /* Offset in bytes to beginning of bitmap pel
                             data in the file, from the start of the definition. */
};

struct os2_bitmap_header_v12 : public os2_bitmap_file_header
{
  UINT32 cbfix;           /* Size of the structure in bytes. */
  UINT16 width;           /* Bitmap width in pels. */
  UINT16 height;          /* Bitmap height in pels. */
  UINT16 planes;          /* Number of bit planes. */
  UINT16 bits_per_pixel;  /* Number of bits per pel within a plane. */

  ICPalette palette;      /* This is not a part of the standard OS/2 header! */
};

struct os2_bitmap_header_v20 : public os2_bitmap_file_header
{
  UINT32 cbfix;           /* Size of the structure in bytes. */
  UINT32 width;           /* Bitmap width in pels. */
  UINT32 height;          /* Bitmap height in pels. */
  UINT16 planes;          /* Number of bit planes. */
  UINT16 bits_per_pixel;  /* Number of bits per pel within a plane. */
  UINT32 compression;     /* Compression scheme used to store the bit map. */
  UINT32 bitmap_size;     /* Length of bitmap storage data, in bytes.
                             If the bit map is uncompressed, zero
                             (the default) can be specified for this. */
  UINT32 devXres;         /* Horizontal component of the resolution of target device. */
  UINT32 devYres;         /* Vertical component of the resolution of target device. */
  UINT32 clrUsed;         /* Number of color indexes used. */
  UINT32 clrImportant;    /* Minimum number of color indexes for satisfactory
                             appearance of the bit map. */
  UINT16 units;           /* Units of measure. */
  UINT16 reserved;        /* Reserved. */
  UINT16 recording;       /* Recording algorithm. */
  UINT16 rendering;       /* Halftoning algorithm. */
  UINT32 size1;           /* Size value 1. */
  UINT32 size2;           /* Size value 2. */
  UINT32 color_encoding;  /* Color encoding. */
  UINT32 app_identifier;  /* Reserved for application use. */

  ICPalette palette;      /* This is not a part of the standard OS/2 header! */
};

#define OS2_BFT_BITMAPARRAY   0x4142
#define OS2_BFT_BITMAP        0x4D42
#define OS2_BFT_ICON          0x4349
#define OS2_BFT_POINTER       0x4540
#define OS2_BFT_COLORICON     0x4943
#define OS2_BFT_COLORPOINTER  0x5043

struct os2_icon_info
{
  os2_bitmap_array_header header;
  os2_bitmap_header_v20   xorand;
  os2_bitmap_header_v20   bitmap;

  class ICIcon* icon;
};

#endif

/**
 * Loads and saves OS/2 icon files.
 * @author Dmitry A Steklenev
 */

class ICOS2
{
  public:
    /** Loads icon file. */
    virtual void load( ICFile* load_file  );
    /** Saves icon file. */
    virtual void save( ICFile* save_file, icon_type );

  private:
    virtual os2_bitmap_array_header load_barray_header();
    virtual os2_bitmap_header_v20   load_bitmap_header();

    virtual void load_bitmap       ();
    virtual void load_bitmap_data  ( ICBitmap& );
    virtual void save_bitmap_data  ( ICBitmap& );
    virtual void save_barray_header( const os2_bitmap_array_header& i );
    virtual void save_bitmap_header( const os2_bitmap_header_v20&   i );

    /** Repack an icon list and optimise its content for OS/2. */
    virtual void repack_list();

  private:
    ICFile* file;
};

#endif

