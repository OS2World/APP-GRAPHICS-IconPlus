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
 * $Id: ic_bitmap.cpp,v 1.8 2005/05/01 14:14:55 glass Exp $
 */

#include "ic_bitmap.h"
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

/*------------------------------------------------------------------
 * Constructs the empty generic bitmap
 *------------------------------------------------------------------*/
ICBitmap::ICBitmap()

: map_width         ( 0 ),
  map_height        ( 0 ),
  map_bits_per_pixel( 0 ),
  map_byte_width    ( 0 ),
  map_size          ( 0 ),
  map_bits          ( 0 )
{}

/*------------------------------------------------------------------
 * Constructs the generic bitmap
 *------------------------------------------------------------------*/
ICBitmap::ICBitmap( size_type width, size_type height, size_type bits_per_pixel )

: map_width         ( width          ),
  map_height        ( height         ),
  map_bits_per_pixel( bits_per_pixel )
{
  map_byte_width = (width*bits_per_pixel+7)/8;
  map_size = map_byte_width*height;
  map_bits = new BYTE[map_size];
  memset( map_bits, 0, map_size );
}

/*------------------------------------------------------------------
 * Constructs the generic bitmap from another bitmap object
 *------------------------------------------------------------------*/
ICBitmap::ICBitmap( const ICBitmap& bitmap )
{
  map_width          = bitmap.map_width;
  map_height         = bitmap.map_height;
  map_bits_per_pixel = bitmap.map_bits_per_pixel;
  map_byte_width     = bitmap.map_byte_width;
  map_size           = bitmap.map_size;

  map_bits = new BYTE[map_size];
  memcpy( map_bits, bitmap.map_bits, map_size);
}

/*------------------------------------------------------------------
 * Destructs the generic bitmap
 *------------------------------------------------------------------*/
ICBitmap::~ICBitmap()
{
  delete map_bits;
}

/*------------------------------------------------------------------
 * Assignment operator
 *------------------------------------------------------------------*/
ICBitmap& ICBitmap::operator=( const ICBitmap& bitmap )
{
  if( &bitmap != this )
  {
    delete map_bits;

    map_width          = bitmap.map_width;
    map_height         = bitmap.map_height;
    map_bits_per_pixel = bitmap.map_bits_per_pixel;
    map_byte_width     = bitmap.map_byte_width;
    map_size           = bitmap.map_size;

    map_bits = new BYTE[map_size];
    memcpy( map_bits, bitmap.map_bits, map_size);
  }

  return *this;
}

/*------------------------------------------------------------------
 * Returns the padded bitmap size in bytes
 *------------------------------------------------------------------*/
ICBitmap::size_type ICBitmap::padded_size( size_type boundary ) const
{
  size_type width = byte_width();

  while( width % boundary != 0 )
    width++;

  return width * height();
}

/*------------------------------------------------------------------
 * Returns the specified pixel
 *------------------------------------------------------------------*/
UINT32 ICBitmap::get_pixel( size_type row, size_type col ) const
{
  // The first byte of the specified location.
  size_type pos = row*map_byte_width + col*map_bits_per_pixel/8;
  // The first bit in first byte of the specified location.
  size_type bit = col*map_bits_per_pixel%8;

  UINT32 pixel = 0;
  UINT32 mask  = map_bits_per_pixel < 32 ? ((1 << map_bits_per_pixel) - 1) : -1;

  for( int i = 0; i <= (int)((map_bits_per_pixel-1)/8); i++ )
  {
    pixel = (pixel << 8) | map_bits[ pos + i ];
  }

  pixel >>= ((map_bits_per_pixel+7)/8)*8 - bit - map_bits_per_pixel;
  return pixel & mask;
}

/*------------------------------------------------------------------
 * Sets the specified pixel
 *------------------------------------------------------------------*/
void ICBitmap::set_pixel( size_type row, size_type col, UINT32 pixel )
{
  // The first byte of the specified location.
  size_type pos = row*map_byte_width + col*map_bits_per_pixel/8;
  // The first bit in first byte of the specified location.
  size_type bit = col*map_bits_per_pixel%8;

  UINT32 mask = map_bits_per_pixel < 32 ? ((1 << map_bits_per_pixel) - 1) : -1;

  pixel <<= ((map_bits_per_pixel+7)/8)*8 - bit - map_bits_per_pixel;
  mask  <<= ((map_bits_per_pixel+7)/8)*8 - bit - map_bits_per_pixel;

  for( int i = (map_bits_per_pixel-1)/8; i >= 0; i-- )
  {
    map_bits[pos+i] = map_bits[pos+i] & ~(mask & 0xFF ) | (pixel & 0xFF );
    pixel >>= 8;
    mask  >>= 8;
  }
}

/*------------------------------------------------------------------
 * Flips the generic bitmap
 *------------------------------------------------------------------*/
void ICBitmap::flip()
{
  BYTE*  flipped = new BYTE[map_size];
  BYTE*  dst_row = flipped;
  BYTE*  src_row = map_bits + map_size - map_byte_width;

  size_type i;

  while( src_row >= map_bits )
  {
    for( i = 0; i < map_byte_width; i++ )
      *dst_row++ = *src_row++;

    src_row -= 2*map_byte_width;
  }

  memcpy( map_bits, flipped, map_size );
  delete flipped;
}

/*------------------------------------------------------------------
 * Dumps the generic bitmap
 *------------------------------------------------------------------*/
void ICBitmap::dump()
{
  char format[8];
  size_type row, col;

  sprintf( format, "%%0%uX ", map_bits_per_pixel/4 );

  for( row = 0; row < map_height; row++ )
  {
    printf( "\t" );

    for( col = 0; col < map_width; col++ )
      printf( format, get_pixel( row, col ));

    printf( "\n" );
  }
}

/*------------------------------------------------------------------
 * Constructs the generic alpha channel map
 *------------------------------------------------------------------*/
ICAlpha::ICAlpha( size_type width, size_type height )

: ICBitmap( width, height, 8 )

{
  memset( map_bits, 0xFF, map_size );
}

/*------------------------------------------------------------------
 * Returns true if alpha channel is empty
 *------------------------------------------------------------------*/
int ICAlpha::empty() const
{
  BYTE* map = map_bits;

  for( size_type i = 0; i < map_size; i++ )
    if( *map++ != (BYTE)0xFF )
      return 0;

  return 1;
}

