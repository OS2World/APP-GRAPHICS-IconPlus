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
 * $Id: ic_palette.cpp,v 1.5 2005/05/01 14:14:55 glass Exp $
 */

#include "ic_palette.h"
#include <stdlib.h>
#include <memory.h>

const ICRGB ICRGB::black( 0x00, 0x00, 0x00 );
const ICRGB ICRGB::white( 0xFF, 0xFF, 0xFF );

const ICPalette::size_type ICPalette::npos = ICPalette::size_type(-1);

/*------------------------------------------------------------------
 * Constructs the empty color palette
 *------------------------------------------------------------------*/
ICPalette::ICPalette()
{
  pal_size = 0;
  pal_list = 0;
  pal_true = 0;
}

/*------------------------------------------------------------------
 * Constructs the color palette
 *------------------------------------------------------------------*/
ICPalette::ICPalette( size_type bits_per_pixel )
{
  if( bits_per_pixel >= 24 )
  {
    pal_true = 1;
    pal_size = 0;
    pal_list = 0;
  }
  else
  {
    pal_true = 0;
    pal_size = 1 << bits_per_pixel;
    pal_list = new ICRGB[pal_size];
  }
}

/*------------------------------------------------------------------
 * Constructs the color palette from another palette object
 *------------------------------------------------------------------*/
ICPalette::ICPalette( const ICPalette& palette )
{
  pal_size = palette.pal_size;
  pal_true = palette.pal_true;

  if( pal_size )
  {
    pal_list = new ICRGB[pal_size];
    memcpy( pal_list, palette.pal_list, sizeof(ICRGB)*pal_size );
  }
  else
    pal_list = 0;
}

/*------------------------------------------------------------------
 * Destructs the color palette
 *------------------------------------------------------------------*/
ICPalette::~ICPalette()
{
  delete[] pal_list;
}

/*------------------------------------------------------------------
 * Returns the RGB color index
 *------------------------------------------------------------------*/
ICPalette::size_type ICPalette::find( const ICRGB& color ) const
{
  if( pal_true )
    return color;
  else
  {
    for( size_type i = 0; i < count(); i++ )
      if( pal_list[i] == color )
        return i;
  }

  return npos;
}

/*------------------------------------------------------------------
 * Assignment operator
 *------------------------------------------------------------------*/
ICPalette& ICPalette::operator=( const ICPalette& palette )
{
  if( &palette != this )
  {
    delete[] pal_list;

    pal_size = palette.pal_size;
    pal_true = palette.pal_true;

    if( pal_size )
    {
      pal_list = new ICRGB[pal_size];
      memcpy( pal_list, palette.pal_list, sizeof(ICRGB)*pal_size );
    }
    else
      pal_list = 0;
  }

  return *this;
}

/*------------------------------------------------------------------
 * Compares this palette to another palette object for equality
 *------------------------------------------------------------------*/
int ICPalette::operator==( const ICPalette& palette ) const
{
  if(( pal_size != palette.pal_size ) ||
     ( pal_true != palette.pal_true )  )
  {
    return 0;
  }

  for( size_type i = 0; i < pal_size; i++ )
    if( pal_list[i] != palette[i] )
      return 0;

  return 1;
}

/*------------------------------------------------------------------
 * Compares this palette to another palette object for inequality
 *------------------------------------------------------------------*/
int ICPalette::operator!=( const ICPalette& palette ) const
{
  return !operator==(palette);
}

/*------------------------------------------------------------------
 * Returns standard black and white color palette
 *------------------------------------------------------------------*/
ICPalette ICPalette::default_bw()
{
  ICPalette palette(1);
  palette[0] = ICRGB::black;
  palette[1] = ICRGB::white;

  return palette;
}

