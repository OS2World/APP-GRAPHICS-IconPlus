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
 * $Id: ic_file.h,v 1.7 2009/11/12 13:06:37 glass Exp $
 */

#ifndef IC_FILE_H
#define IC_FILE_H

#include "ic_config.h"
#include <stdio.h>

/**
 * Icon Plus file input/output.
 *
 * The ICFile class creates and manages a file.
 * @author Dmitry A Steklenev
 */

class ICFile
{
  public:
    /** Constructs the file object. */
    ICFile( const char* filename, const char* path = 0 );
    /** Destructs the file object. */
   ~ICFile();

  public:
    /** Opens file for reading. */
    int open_for_read ();
    /** Opens file for writing. */
    int open_for_write();
    /** Close open file. */
    int close();
    /** Returns true if file is open. */
    int is_open() const;

  public:
    /** Reads one byte. */
    BYTE   read_byte ();
    /** Reads 16-bit big-endian integer. */
    UINT16 read_int16();
    /** Reads 24-bit big-endian integer. */
    UINT32 read_int24();
    /** Reads 32-bit big-endian integer. */
    UINT32 read_int32();

    /** Write one byte. */
    void write_byte ( BYTE   );
    /** Write 16-bit big-endian integer. */
    void write_int16( UINT16 );
    /** Write 24-bit big-endian integer. */
    void write_int24( UINT32 );
    /** Write 32-bit big-endian integer. */
    void write_int32( UINT32 );

  public:
    /** Returns the file name. */
    const char* name() const;
    /** Changes the current file position. */
    long seek( long offset );
    /** Returns the current file position. */
    long tell();

  private:
    char* file_name;
    FILE* file_handle;
};

/*------------------------------------------------------------------
 * Returns true if file is open
 *------------------------------------------------------------------*/
inline int ICFile::is_open() const
{
  return file_handle != 0;
}

/*------------------------------------------------------------------
 * Returns the file name
 *------------------------------------------------------------------*/
inline const char* ICFile::name() const
{
  return file_name;
}

#endif
