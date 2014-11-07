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
 * $Id: ic_palette.h,v 1.7 2009/11/12 13:06:37 glass Exp $
 */

#ifndef IC_PALETTE_H
#define IC_PALETTE_H

#include "ic_config.h"

/**
 * Icon Plus generic RGB color class.
 * @author Dmitry A Steklenev
 */

class ICRGB {

  public:
    /** Constructs the black RGB color. */
    ICRGB();
    /** Constructs the RGB color. */
    ICRGB( BYTE r, BYTE g, BYTE b );

    /** Converts the RGB color to unsigned long. */
    operator unsigned long() const;

    /** Compares this color to another color object for equality. */
    int operator==( const ICRGB& ) const;
    /** Compares this color to another color object for inequality. */
    int operator!=( const ICRGB& ) const;

  public:
    static const ICRGB white; //@- White.
    static const ICRGB black; //@- Black.

  public:
    BYTE blue;  //@- Blue component of the RGB color.
    BYTE green; //@- Green component of the RGB color.
    BYTE red;   //@- Red component of the RGB color.
};

#if !defined(__ccdoc__)
/*------------------------------------------------------------------
 * Constructs the black RGB color
 *------------------------------------------------------------------*/
inline ICRGB::ICRGB()

: red  (0),
  green(0),
  blue (0)
{}

/*------------------------------------------------------------------
 * Constructs the RGB color
 *------------------------------------------------------------------*/
inline ICRGB::ICRGB( BYTE r, BYTE g, BYTE b )

: red  (r),
  green(g),
  blue (b)
{}

/*------------------------------------------------------------------
 * Converts the RGB color to unsigned long
 *------------------------------------------------------------------*/
inline ICRGB::operator unsigned long() const
{
  return blue | (green << 8) | (red << 16);
}

/*------------------------------------------------------------------
 * Compares this color to another color object for equality
 *------------------------------------------------------------------*/
inline int ICRGB::operator==( const ICRGB& color ) const
{
  return red   == color.red   &&
         green == color.green &&
         blue  == color.blue;
}

/*------------------------------------------------------------------
 * Compares this color to another color object for inequality
 *------------------------------------------------------------------*/
inline int ICRGB::operator!=( const ICRGB& color ) const
{
  return red   != color.red   ||
         green != color.green ||
         blue  != color.blue;
}
#endif

/**
 * Icon Plus generic color palette class.
 *
 * The ICPalette class creates and manages a RGB color palette.
 * @author Dmitry A Steklenev
 */

class ICPalette
{
  public:
    /** An unsigned integral type. */
    typedef unsigned long size_type;

    /** The largest possible value of type size_type. */
    static const size_type npos;

  public:
    /** Constructs the empty color palette. */
    ICPalette();
    /** Constructs the color palette. */
    ICPalette( size_type size   );
    /** Constructs the color palette from another palette object. */
    ICPalette( const ICPalette& );
    /** Destructs the color palette. */
   ~ICPalette();

  public:
    /** Returns the number of RGB colors stored in the palette. */
    size_type count() const;
    /** Returns the RGB color index. */
    size_type find( const ICRGB& ) const;
    /** Returns true (1) if palette is in truecolor mode. */
    int is_truecolor() const;

  public:
    /** Assignment operator. */
    ICPalette& operator=( const ICPalette& );
    /** Returns a reference to the RGB color. */
    ICRGB& operator[]( size_type pos );
    /** Returns a constant reference to the RGB color. */
    const ICRGB& operator[]( size_type pos ) const;
    /** Compares this palette to another palette object for equality. */
    int operator==( const ICPalette& ) const;
    /** Compares this palette to another palette object for inequality. */
    int operator!=( const ICPalette& ) const;

  public:
    /** Returns standard black and white color palette. */
    static ICPalette default_bw();

  private:
    ICRGB*    pal_list;
    size_type pal_size;
    int       pal_true;
};

/*------------------------------------------------------------------
 * Returns the number of RGB colors
 *------------------------------------------------------------------*/
inline ICPalette::size_type ICPalette::count() const
{
  return pal_size;
}

/*------------------------------------------------------------------
 * Returns true if the palette is in truecolor mode
 *------------------------------------------------------------------*/
inline int ICPalette::is_truecolor() const
{
  return pal_true;
}

/*------------------------------------------------------------------
 * Returns a reference to the RGB color
 *------------------------------------------------------------------*/
inline ICRGB& ICPalette::operator[]( size_type pos )
{
  return pal_list[pos];
}

/*------------------------------------------------------------------
 * Returns a constant reference to the RGB color
 *------------------------------------------------------------------*/
inline const ICRGB& ICPalette::operator[]( size_type pos ) const
{
  return pal_list[pos];
}

#endif
