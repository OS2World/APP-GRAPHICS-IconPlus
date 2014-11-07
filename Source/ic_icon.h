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
 * $Id: ic_icon.h,v 1.8 2009/11/13 11:39:46 glass Exp $
 */

#ifndef IC_ICON_H
#define IC_ICON_H

#include "ic_palette.h"
#include "ic_bitmap.h"

/**
 * Icon Plus generic icon class.
 *
 * The ICIcon class creates and manages a generic icon.
 * <p>
 * The following table shows how these bitmaps are used for a
 * color icon or pointer:
 * <pre>
 *   AND XOR COLOR
 *    1   1    x   Invert screen
 *    1   0    x   Transparency
 *    0   0    x   Use color x
 *    0   1    x   Use color x

 * </pre>
 *
 * @author Dmitry A Steklenev
 */

class ICIcon
{
  public:
    /** An unsigned integral type. */
    typedef UINT32 size_type;

  public:
    /** Constructs the empty generic icon. */
    ICIcon();
    /** Constructs the generic icon. */
    ICIcon( size_type width, size_type height, size_type bits_per_pixel );
    /** Constructs the generic icon from another icon object. */
    ICIcon( const ICIcon& );
    /** Destructs the generic icon. */
   ~ICIcon();

  public:
    /** Returns the icon width in pixels. */
    size_type width() const;
    /** Returns the icon height in pixels. */
    size_type height() const;
    /** Returns the icon of bits per pixel. */
    size_type bits_per_pixel() const;

    /** Returns a reference to the icon bitmap. */
    ICBitmap&  bitmap () { return ico_clr_map; }
    /** Returns a reference to the icon bitmap palette. */
    ICPalette& palette() { return ico_clr_pal; }
    /** Returns a reference to the icon "and" mask. */
    ICBitMask& andmap () { return ico_and_map; }
    /** Returns a reference to the icon "xor" mask. */
    ICBitMask& xormap () { return ico_xor_map; }
    /** Returns a reference to the alpha channel map. */
    ICAlpha&   alpha  () { return ico_alp_map; }

    /** Returns the width of icon hotspot. */
    size_type  xhotspot() const;
    /** Returns the height of icon hotspot. */
    size_type  yhotspot() const;
    /** Sets the icon hotspot. */
    void move_hotspot( size_type x, size_type y );

    /** Returns true if this icon is black and white. */
    int is_bw() const;

  public:
    /** Assignment operator. */
    ICIcon& operator=( const ICIcon& );

  private:
    ICBitmap  ico_clr_map;
    ICPalette ico_clr_pal;
    ICBitMask ico_xor_map;
    ICBitMask ico_and_map;
    ICAlpha   ico_alp_map;

    size_type ico_xhotspot;
    size_type ico_yhotspot;
};


/*------------------------------------------------------------------
 * Returns the icon width in pixels
 *------------------------------------------------------------------*/
inline ICIcon::size_type ICIcon::width() const
{
  return ico_clr_map.width();
}

/*------------------------------------------------------------------
 * Returns the icon height in pixels
 *------------------------------------------------------------------*/
inline ICIcon::size_type ICIcon::height() const
{
  return ico_clr_map.height();
}

/*------------------------------------------------------------------
 * Returns the number of bits per pixel
 *------------------------------------------------------------------*/
inline ICIcon::size_type ICIcon::bits_per_pixel() const
{
  return ico_clr_map.bits_per_pixel();
}

/*------------------------------------------------------------------
 * Returns the width of icon hotspot
 *------------------------------------------------------------------*/
inline ICIcon::size_type ICIcon::xhotspot() const
{
  return ico_xhotspot;
}

/*------------------------------------------------------------------
 * Returns the height of icon hotspot
 *------------------------------------------------------------------*/
inline ICIcon::size_type ICIcon::yhotspot() const
{
  return ico_yhotspot;
}

/*------------------------------------------------------------------
 * Sets the icon hotspot
 *------------------------------------------------------------------*/
inline void ICIcon::move_hotspot( size_type x, size_type y )
{
  ico_xhotspot = x;
  ico_yhotspot = y;
}

/*------------------------------------------------------------------
 * Returns true if this icon is black and white
 *------------------------------------------------------------------*/
inline int ICIcon::is_bw() const
{
  return ico_clr_pal == ICPalette::default_bw();
}

/**
 * Icon Plus generic icons list.
 *
 * The ICIconList class creates and manages a generic icons list.
 * @author Dmitry A Steklenev
 */

class ICIconList
{
  public:
    /** An unsigned integral type. */
    typedef unsigned long size_type;

  public:
    /** Constructs the icons list. */
    ICIconList();
    /** Destructs the icons list. */
   ~ICIconList();

  public:
    /** Iterator used to iterate over the icons. */
    typedef ICIcon* iterator;
    /** Returns an iterator pointing to the beginning of the icons list. */
    iterator  begin() const;
    /** Returns an iterator pointing to the end of the icons list. */
    iterator  end  () const;
    /** Returns the number of icons stored in the list. */
    size_type count() const;

    /** Inserts the icon as the last item in the list. */
    void push_back ( const ICIcon& icon );
    /** Inserts the icon as the first item in the list. */
    void push_front( const ICIcon& icon );
    /** Removes all icons from the list. */
    void clear();

  private:
    ICIcon*   lst_icons;
    size_type lst_size;
};

/** List of loaded icons. */
extern ICIconList icon_list;

/*------------------------------------------------------------------
 * Returns an iterator pointing to the beginning of the icons list
 *------------------------------------------------------------------*/
inline ICIconList::iterator ICIconList::begin() const
{
  return lst_icons;
}

/*------------------------------------------------------------------
 * Returns an iterator pointing to the end of the icons list
 *------------------------------------------------------------------*/
inline ICIconList::iterator ICIconList::end() const
{
  return lst_icons + lst_size;
}

/*------------------------------------------------------------------
 * Returns the number of icons stored in the list
 *------------------------------------------------------------------*/
inline ICIconList::size_type ICIconList::count() const
{
  return lst_size;
}

#endif
