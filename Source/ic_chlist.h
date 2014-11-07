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
 * $Id: ic_chlist.h,v 1.4 2009/11/12 13:06:37 glass Exp $
 */

#ifndef IC_CHLIST_H
#define IC_CHLIST_H

/**
 * Icon Plus character strings list.
 *
 * The ICIconList class creates and manages a character strings list.
 * @author Dmitry A Steklenev
 */

class ICChList
{
  public:
    /** An unsigned integral type. */
    typedef unsigned long size_type;

  public:
    /** Constructs the strings list. */
    ICChList();
    /** Destructs strings list. */
   ~ICChList();

  public:
    /** Iterator used to iterate over the strings. */
    typedef char** iterator;
    /** Returns an iterator pointing to the beginning of the strings list. */
    iterator  begin() const;
    /** Returns an iterator pointing to the end of the strings list. */
    iterator  end  () const;
    /** Returns the number of strings stored in the list. */
    size_type count() const;

    /** Inserts the strings as the last item in the list. */
    void push_back( const char* strings );

  private:
    char**    lst_strings;
    size_type lst_size;
};

/*------------------------------------------------------------------
 * Returns an iterator pointing to the beginning of the strings list
 *------------------------------------------------------------------*/
inline ICChList::iterator ICChList::begin() const
{
  return lst_strings;
}

/*------------------------------------------------------------------
 * Returns an iterator pointing to the end of the strings list
 *------------------------------------------------------------------*/
inline ICChList::iterator ICChList::end() const
{
  return lst_strings + lst_size;
}

/*------------------------------------------------------------------
 * Returns the number of strings stored in the list
 *------------------------------------------------------------------*/
inline ICChList::size_type ICChList::count() const
{
  return lst_size;
}

#endif
