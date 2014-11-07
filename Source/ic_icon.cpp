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
 * $Id: ic_icon.cpp,v 1.8 2009/11/13 11:39:46 glass Exp $
 */

#include "ic_icon.h"

/*------------------------------------------------------------------
 * Constructs the empty generic icon
 *------------------------------------------------------------------*/
ICIcon::ICIcon()

: ico_clr_map (   ),
  ico_clr_pal (   ),
  ico_xor_map (   ),
  ico_and_map (   ),
  ico_alp_map (   ),
  ico_xhotspot( 0 ),
  ico_yhotspot( 0 )
{}

/*------------------------------------------------------------------
 * Constructs the generic icon
 *------------------------------------------------------------------*/
ICIcon::ICIcon( size_type width, size_type height, size_type bits_per_pixel )

: ico_clr_map ( width, height, bits_per_pixel ),
  ico_xor_map ( width, height  ),
  ico_and_map ( width, height  ),
  ico_alp_map ( width, height  ),
  ico_clr_pal ( bits_per_pixel ),
  ico_xhotspot( width /2 ),
  ico_yhotspot( height/2 )
{}

/*------------------------------------------------------------------
 * Constructs the generic icon from another icon object
 *------------------------------------------------------------------*/
ICIcon::ICIcon( const ICIcon& icon )

: ico_clr_map ( icon.ico_clr_map  ),
  ico_clr_pal ( icon.ico_clr_pal  ),
  ico_xor_map ( icon.ico_xor_map  ),
  ico_and_map ( icon.ico_and_map  ),
  ico_alp_map ( icon.ico_alp_map  ),
  ico_xhotspot( icon.ico_xhotspot ),
  ico_yhotspot( icon.ico_yhotspot )
{}

/*------------------------------------------------------------------
 * Destructs the generic icon
 *------------------------------------------------------------------*/
ICIcon::~ICIcon() {}

/*------------------------------------------------------------------
 * Assignment operator
 *------------------------------------------------------------------*/
ICIcon& ICIcon::operator=( const ICIcon& icon )
{
  if( &icon != this )
  {
    ico_clr_map  = icon.ico_clr_map;
    ico_clr_pal  = icon.ico_clr_pal;
    ico_xor_map  = icon.ico_xor_map;
    ico_and_map  = icon.ico_and_map;
    ico_alp_map  = icon.ico_alp_map;
    ico_xhotspot = icon.ico_xhotspot;
    ico_yhotspot = icon.ico_yhotspot;
  }

  return *this;
}

/*------------------------------------------------------------------
 * Constructs the icons list
 *------------------------------------------------------------------*/
ICIconList::ICIconList()
{
  lst_icons = 0;
  lst_size  = 0;
}

/*------------------------------------------------------------------
 * Destructs the icons list
 *------------------------------------------------------------------*/
ICIconList::~ICIconList()
{
  delete[] lst_icons;
}

/*------------------------------------------------------------------
 * Inserts the icon as the last item in the list
 *------------------------------------------------------------------*/
void ICIconList::push_back( const ICIcon& icon )
{
  ICIcon* new_list = new ICIcon[lst_size+1];
  ICIcon* old_list = lst_icons;
  size_type i;

  for( i = 0; i < lst_size; i++ ) {
    new_list[i] = old_list[i];
  }

  new_list[i] = icon;
  lst_icons = new_list;
  lst_size++;

  delete[] old_list;
}

/*------------------------------------------------------------------
 * Inserts the icon as the first item in the list
 *------------------------------------------------------------------*/
void ICIconList::push_front( const ICIcon& icon )
{
  ICIcon* new_list = new ICIcon[lst_size+1];
  ICIcon* old_list = lst_icons;
  size_type i;

  for( i = 0; i < lst_size; i++ )
    new_list[i+1] = old_list[i];

  new_list[0] = icon;
  lst_icons = new_list;
  lst_size++;

  delete[] old_list;
}


/*------------------------------------------------------------------
 * Removes all icons from the list
 *------------------------------------------------------------------*/
void ICIconList::clear()
{
  lst_icons = 0;
  lst_size  = 0;
  delete[] lst_icons;
}

