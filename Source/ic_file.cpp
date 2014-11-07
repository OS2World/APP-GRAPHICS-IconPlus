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
 * $Id: ic_file.cpp,v 1.6 2005/05/01 14:14:55 glass Exp $
 */

#include "ic_file.h"
#include <stdlib.h>
#include <string.h>

/*------------------------------------------------------------------
 * Constructs the file object
 *------------------------------------------------------------------*/
ICFile::ICFile( const char* filename, const char* path )
{
  file_handle = 0;

  if( !path )
    file_name = strdup( filename );
  else
  {
    file_name = (char*)malloc( strlen(filename) + strlen(path) + sizeof(IC_PS) + 1 );
    strcpy( file_name, path     );
    strcat( file_name, IC_PS    );
    strcat( file_name, filename );
  }
}

/*------------------------------------------------------------------
 * Destructs the file object
 *------------------------------------------------------------------*/
ICFile::~ICFile()
{
  if( is_open()) close();
  free( file_name );
}

/*------------------------------------------------------------------
 * Opens file for reading
 *------------------------------------------------------------------*/
int ICFile::open_for_read()
{
  file_handle = fopen( file_name, IC_FOPEN_BREAD );
  return is_open();
}

/*------------------------------------------------------------------
 * Opens file for writing
 *------------------------------------------------------------------*/
int ICFile::open_for_write()
{
  file_handle = fopen( file_name, IC_FOPEN_BWRITE );
  return is_open();
}

/*------------------------------------------------------------------
 * Close open file
 *------------------------------------------------------------------*/
int ICFile::close()
{
  int rc = fclose( file_handle );

  if( rc == 0 )
  {
    file_handle = 0;
    return 1;
  }
  else
    return 0;
}

/*------------------------------------------------------------------
 * Changes the current file position
 *------------------------------------------------------------------*/
long ICFile::seek( long offset )
{
  clearerr( file_handle );
  return fseek( file_handle, offset, SEEK_SET );
}

/*------------------------------------------------------------------
 * Returns the current file position
 *------------------------------------------------------------------*/
long ICFile::tell()
{
  return ftell( file_handle );
}

/*------------------------------------------------------------------
 * Reads one byte
 *------------------------------------------------------------------*/
BYTE ICFile::read_byte()
{
  BYTE bytes;

  if( fread( &bytes, 1, 1, file_handle ) == 1 )
    return bytes;
  else
    return 0;
}

/*------------------------------------------------------------------
 * Reads 16bit big-endian integer
 *------------------------------------------------------------------*/
UINT16 ICFile::read_int16()
{
  BYTE bytes[2];

  if( fread( &bytes, 1, 2, file_handle ) == 2 )
    return (bytes[1] << 8) + (bytes[0]);
  else
    return 0;
}

/*------------------------------------------------------------------
 * Reads 24bit big-endian integer
 *------------------------------------------------------------------*/
UINT32 ICFile::read_int24()
{
  BYTE bytes[3];

  if( fread( &bytes, 1, 3, file_handle ) == 3 )
    return (bytes[2] << 16) + (bytes[1] << 8) + (bytes[0]);
  else
    return 0;
}

/*------------------------------------------------------------------
 * Reads 32bit big-endian integer
 *------------------------------------------------------------------*/
UINT32 ICFile::read_int32()
{
  BYTE bytes[4];

  if( fread( &bytes, 1, 4, file_handle ) == 4 )
    return (bytes[3] << 24) + (bytes[2] << 16) + (bytes[1] << 8) + (bytes[0]);
  else
    return 0;
}

/*------------------------------------------------------------------
 * Write one byte
 *------------------------------------------------------------------*/
void ICFile::write_byte( BYTE data )
{
  if( fwrite( &data, 1, 1, file_handle ) != 1 )
  {
    perror( "Write error" );
    exit  ( 1 );
  }
}

/*------------------------------------------------------------------
 * Write 16bit big-endian integer
 *------------------------------------------------------------------*/
void ICFile::write_int16( UINT16 data )
{
  BYTE bytes[2];

  bytes[1] = (data >> 8     );
  bytes[0] = (data & 0x00FF );

  if( fwrite( &bytes, 1, 2, file_handle ) != 2 )
  {
    perror( "Write error" );
    exit  ( 1 );
  }
}

/*------------------------------------------------------------------
 * Write 24bit big-endian integer
 *------------------------------------------------------------------*/
void ICFile::write_int24( UINT32 data )
{
  BYTE bytes[3];

  bytes[2] = (data & 0x00FF0000UL ) >> 16;
  bytes[1] = (data & 0x0000FF00UL ) >>  8;
  bytes[0] = (data & 0x000000FFUL );

  if( fwrite( &bytes, 1, 3, file_handle ) != 3 )
  {
    perror( "Write error" );
    exit  ( 1 );
  }
}

/*------------------------------------------------------------------
 * Write 32bit big-endian integer
 *------------------------------------------------------------------*/
void ICFile::write_int32( UINT32 data )
{
  BYTE bytes[4];

  bytes[3] = (data >> 24 );
  bytes[2] = (data & 0x00FF0000UL ) >> 16;
  bytes[1] = (data & 0x0000FF00UL ) >>  8;
  bytes[0] = (data & 0x000000FFUL );

  if( fwrite( &bytes, 1, 4, file_handle ) != 4 )
  {
    perror( "Write error" );
    exit  ( 1 );
  }
}

