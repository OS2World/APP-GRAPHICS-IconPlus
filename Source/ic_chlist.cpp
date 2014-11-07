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
 * $Id: ic_chlist.cpp,v 1.3 2005/05/01 14:14:55 glass Exp $
 */

#include "ic_chlist.h"
#include <string.h>

/*------------------------------------------------------------------
 * Constructs the strings list
 *------------------------------------------------------------------*/
ICChList::ICChList()
{
  lst_strings = 0;
  lst_size    = 0;
}

/*------------------------------------------------------------------
 * Destructs the strings list
 *------------------------------------------------------------------*/
ICChList::~ICChList()
{
  for( iterator i = begin(); i != end(); i++ )
    delete *i;

  delete[] lst_strings;
}

/*------------------------------------------------------------------
 * Inserts the string as the last item in the list
 *------------------------------------------------------------------*/
void ICChList::push_back( const char* string )
{
  char** new_list = new char*[lst_size+1];
  char** old_list = lst_strings;
  size_type i;

  for( i = 0; i < lst_size; i++ )
    new_list[i] = old_list[i];

  new_list[i] = new char[strlen(string)+1];
  strcpy( new_list[i], string );

  lst_strings = new_list;
  lst_size   += 1;

  delete[] old_list;
}

