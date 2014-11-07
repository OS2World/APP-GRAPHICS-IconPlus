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
 * $Id: ic_icl.cpp,v 1.4 2005/05/14 07:00:00 glass Exp $
 */

#include "ic_icl.h"
#include <stdlib.h>
#include <string.h>

/*------------------------------------------------------------------
 * Initializes MS Windows Icon Library
 *------------------------------------------------------------------*/
void ICICL::init( ICFile* load_file )
{
  file = load_file;
  fprintf( stderr, "Reading MS Windows icon library '%s'...\n", file->name());

  // Read executable file header and find his resource table entry.

  header.type = file->read_int16();
  file->seek( 0x3C );
  header.ne_offset = file->read_int32();
  file->seek( header.ne_offset );
  header.ne_type   = file->read_int16();
  file->seek( header.ne_offset + 0x24 );
  header.rs_offset = file->read_int16();
  file->seek( header.ne_offset + header.rs_offset );
  header.rs_shift  = file->read_int16();

  if( header.type != 0x5A4D || header.ne_type != 0x454E || !header.rs_offset )
  {
    fprintf( stderr, "Can't read that type of file." );
    return;
  }

  if( load_dump )
  {
    printf( "MS Windows ICL file header\n" );
    printf( "\ttype       : %08X\n", header.type      );
    printf( "\tne_offset  : %08X\n", header.ne_offset );
    printf( "\tne_type    : %08X\n", header.ne_type   );
    printf( "\trs_offset  : %08X\n", header.rs_offset );
    printf( "\trs_shift   : %u\n"  , header.rs_shift  );
    fflush( stdout );
  }

  // Search group of icons and icons definitions in resource table.

  win_resource_directory rsdir;
  group.count = 0;
  icons.count = 0;

  for(;;)
  {
    rsdir.type = file->read_int16();

    if( !rsdir.type /* End of the resource table */ )
      break;

    rsdir.count    = file->read_int16();
    rsdir.reserved = file->read_int32();

    if( load_dump && rsdir.type )
    {
      printf( "MS Windows resource directory\n" );
      printf( "\ttype       : 0x%08X\n", rsdir.type     );
      printf( "\tcount      : %u\n"    , rsdir.count    );
      printf( "\treserved   : 0x%08X\n", rsdir.reserved );
      fflush( stdout );
    }

    if( rsdir.type == RT_GROUP_ICON )
    {
      group = rsdir;
      group.offset = file->tell();
    }
    if( rsdir.type == RT_ICON )
    {
      icons = rsdir;
      icons.offset = file->tell();
    }

    file->seek( file->tell() + rsdir.count*12 );
  }

  // Load resource type and name strings

  BYTE len;

  for( len = file->read_byte(); len; len = file->read_byte())
  {
    char  string[257];
    char* sp = string;

    while( len-- ) *sp++ = file->read_byte();
    *sp = 0;

    name_list.push_back( string );
  }

  if( load_dump )
  {
    printf( "MS Windows resource strings table\n" );

    for( ICChList::size_type i = 0; i < name_list.count(); i++ )
      printf( "\t%04X: '%s'\n", i, name_list.begin()[i] );

    fflush( stdout );
  }
}

/*------------------------------------------------------------------
 * Finds next icon in MS Windows Icon Library
 *------------------------------------------------------------------*/
char* ICICL::find( char* save_name )
{
  if( group.count-- )
  {
    win_resource_entry rsentry;
    file->seek( group.offset );

    rsentry.offset   = file->read_int16();
    rsentry.size     = file->read_int16();
    rsentry.flags    = file->read_int16();
    rsentry.id       = file->read_int16();
    rsentry.reserved = file->read_int32();

    if( load_dump )
    {
      printf( "MS Windows resource entry\n" );
      printf( "\toffset     : 0x%08X\n", rsentry.offset   );
      printf( "\tsize       : %u\n"    , rsentry.size     );
      printf( "\tflags      : 0x%08X\n", rsentry.flags    );
      printf( "\tid         : 0x%08X\n", rsentry.id       );
      printf( "\treserved   : 0x%08X\n", rsentry.reserved );
      fflush( stdout );
    }

    group.offset = file->tell();
    file->seek( rsentry.offset << header.rs_shift );
    load( file );

    if( name_list.count()
        && name_list.count() > ( rsentry.id & 0x7FFFU )
        && stricmp( *name_list.begin(), "ICL" ) == 0 )
    {
      sprintf( save_name, "%s.ico", name_list.begin()[rsentry.id & 0x7FFF]);
    }
    else {
      sprintf( save_name, "icon%04u.ico", rsentry.id & 0x7FFFU );
    }

    return save_name;
  } else {
    return NULL;
  }
}

/*------------------------------------------------------------------
 * Reads MS Windows icon entry
 *------------------------------------------------------------------*/
win_icon_entry ICICL::load_entry()
{
  win_icon_entry i;
  win_resource_entry rsentry;

  i.width       = file->read_byte ();
  i.height      = file->read_byte ();
  i.colorCount  = file->read_byte ();
  i.reserved    = file->read_byte ();
  i.planes      = file->read_int16();
  i.bitCount    = file->read_int16();
  i.bytesInRes  = file->read_int32();
  i.imageOffset = file->read_int16(); /* This is 16bit resource ID */

  if( load_dump )
  {
    printf( "MS Windows icon entry\n" );

    printf( "\twidth         : %u\n"  , i.width       );
    printf( "\theight        : %u\n"  , i.height      );
    printf( "\tcolorCount    : %u\n"  , i.colorCount  );
    printf( "\treserved      : %u\n"  , i.reserved    );
    printf( "\tplanes        : %u\n"  , i.planes      );
    printf( "\tbitCount      : %u\n"  , i.bitCount    );
    printf( "\tbytesInRes    : %u\n"  , i.bytesInRes  );
    printf( "\tid            : %08X\n", i.imageOffset );
    fflush( stdout );
  }

  if( icons.count )
  {
    long curpos = file->tell();
    file->seek( icons.offset );

    for( UINT16 count = 0; count < icons.count; count++ )
    {
      rsentry.offset   = file->read_int16();
      rsentry.size     = file->read_int16();
      rsentry.flags    = file->read_int16();
      rsentry.id       = file->read_int16();
      rsentry.reserved = file->read_int32();

      if( i.imageOffset == ( rsentry.id & 0x7FFFU ))
      {
        i.imageOffset = rsentry.offset << header.rs_shift;

        if( load_dump )
        {
          printf( "\timageOffset   : %u\n", i.imageOffset );
          fflush( stdout );
        }

        file->seek( curpos );
        return i;
      }
    }
    file->seek( curpos );
  }

  i.imageOffset = 0;
  return i;
}

