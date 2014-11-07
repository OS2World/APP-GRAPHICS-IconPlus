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
 * $Id: ic_bitmap.h,v 1.7 2009/11/12 13:06:36 glass Exp $
 */

#ifndef IC_BITMAP_H
#define IC_BITMAP_H

#include "ic_config.h"

/**
 * Icon Plus generic bitmap class.
 *
 * The ICBitmap class creates and manages a bitmap image.
 * @author Dmitry A Steklenev
 */

class ICBitmap
{
  public:
    /** An unsigned integral type. */
    typedef UINT32 size_type;

  public:
    /** Constructs the empty generic bitmap. */
    ICBitmap();
    /** Constructs the generic bitmap. */
    ICBitmap( size_type width, size_type height, size_type bits_per_pixel );
    /** Constructs the generic bitmap from another bitmap object. */
    ICBitmap( const ICBitmap& );
    /** Destructs the generic bitmap. */
   ~ICBitmap();

  public:
    /** Returns the bitmap size in bytes. */
    size_type size() const;
    /** Returns the padded bitmap size in bytes. */
    size_type padded_size( size_type boundary ) const;
    /** Returns the bitmap width in pixels. */
    size_type width() const;
    /** Returns the bitmap height in pixels. */
    size_type height() const;
    /** Returns the number of bits per pixel. */
    size_type bits_per_pixel() const;
    /** Returns the bitmap width in bytes. */
    size_type byte_width() const;
    /** Returns the bitmap pointer. */
    BYTE* map();

    /** Returns the specified pixel. */
    UINT32 get_pixel( size_type row, size_type col ) const;
    /** Sets the specified pixel. */
    void set_pixel( size_type row, size_type col, UINT32 );

  public:
    /** Flips the generic bitmap. */
    void flip();
    /** Dumps the generic bitmap. */
    void dump();

  public:
    /** Assignment operator. */
    ICBitmap& operator=( const ICBitmap& );

  protected:
    BYTE*     map_bits;
    size_type map_width;
    size_type map_height;
    size_type map_bits_per_pixel;
    size_type map_byte_width;
    size_type map_size;
};

/**
 * Icon Plus generic bitmap mask class.
 *
 * The ICBitMask class creates and manages a black & white bitmap image.
 * @author Dmitry A Steklenev
 */

class ICBitMask : public ICBitmap
{
  public:
    /** Constructs the empty generic mask. */
    ICBitMask() : ICBitmap() {}
    /** Constructs the generic mask. */
    ICBitMask( size_type width, size_type height ) : ICBitmap( width, height, 1 ) {}
};

/**
 * Icon Plus generic alpha channel class.
 *
 * The ICAlpha class creates and manages a 8-bit alpha channel map.
 * @author Dmitry A Steklenev
 */

class ICAlpha : public ICBitmap
{
  public:
    /** Constructs the empty generic alpha channel map. */
    ICAlpha() : ICBitmap() {}
    /** Constructs the generic alpha channel map. */
    ICAlpha( size_type width, size_type height );

  public:
    /** Returns true if alpha channel is empty. */
    int empty() const;
};

/*------------------------------------------------------------------
 * Returns the bitmap size in bytes
 *------------------------------------------------------------------*/
inline ICBitmap::size_type ICBitmap::size() const
{
  return map_size;
}

/*------------------------------------------------------------------
 * Returns the bitmap width in pixels
 *------------------------------------------------------------------*/
inline ICBitmap::size_type ICBitmap::width() const
{
  return map_width;
}

/*------------------------------------------------------------------
 * Returns the bitmap width in pixels
 *------------------------------------------------------------------*/
inline ICBitmap::size_type ICBitmap::byte_width() const
{
  return map_byte_width;
}

/*------------------------------------------------------------------
 * Returns the bitmap height in pixels
 *------------------------------------------------------------------*/
inline ICBitmap::size_type ICBitmap::height() const
{
  return map_height;
}

/*------------------------------------------------------------------
 * Returns the number of bits per pixel
 *------------------------------------------------------------------*/
inline ICBitmap::size_type ICBitmap::bits_per_pixel() const
{
  return map_bits_per_pixel;
}

/*------------------------------------------------------------------
 * Returns the bitmap pointer
 *------------------------------------------------------------------*/
inline BYTE* ICBitmap::map()
{
  return map_bits;
}

#endif
