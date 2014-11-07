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
 * $Id: ic_main.cpp,v 1.14 2009/11/13 11:39:46 glass Exp $
 */

#include "ic_os2.h"
#include "ic_windows.h"
#include "ic_icl.h"
#include "ic_icon.h"
#include <stdlib.h>
#include <direct.h>

#ifdef IC_GETOPT_NEEDED
#include <getopt.h>
#endif

ICFile*    load_file;
ICFile*    save_file;
char*      save_path;
int        load_dump;
int        maps_dump;
int        lib_index = -1;
int        repack;
icon_type  save_type = os2_v12;
icon_type  load_type = unknown;
ICIconList icon_list;

/*------------------------------------------------------------------
 * Returns type of the input file
 *------------------------------------------------------------------*/
static icon_type query_load_type()
{
  UINT16 type   = load_file->read_int16();
  UINT32 cbsize = load_file->read_int16();

  load_file->seek(0);

  switch( type )
  {
    case 0:
      if( cbsize == 0x01 )
        return win_ico;
      break;

    case OS2_BFT_BITMAPARRAY:   /* OS/2 Bitmap array file */
      if( cbsize == 0x5C )      /* version 2.0 */
        return os2_v20;
      if( cbsize == 0x28 )      /* version 1.2 */
        return os2_v12;
      break;

    case OS2_BFT_COLORICON:     /* OS/2 Color icon file   */
    case OS2_BFT_ICON:          /* OS/2 B&W   icon file   */
      if( cbsize == 0x4E )      /* version 2.0 */
        return os2_v20;
      if( cbsize == 0x1A )      /* version 1.2 */
        return os2_v12;
      break;

    case 0x5A4D:                /* 'MZ' executable */
    {
      UINT16 offset;
      UINT16 type;

      load_file->seek( 0x3C   );
      offset = load_file->read_int32();
      load_file->seek( offset );
      type   = load_file->read_int16();
      load_file->seek( 0x00   );

      if( type == 0x454E )      /* 'NE'executable */
        return win_icl;

      break;
    }
  }
  return unknown;
}

/*------------------------------------------------------------------
 * Generates program error
 *------------------------------------------------------------------*/
void static error( const char* msg )
{
  fprintf( stderr, "%s\n", msg );
  exit   ( 1 );
}

/*------------------------------------------------------------------
 * Display help
 *------------------------------------------------------------------*/
void static help()
{
  printf( "Usage: icplus [<options>...] <load_file> [-i <lib_index>] [<save_file>|-p <save_path>]\n" );
  printf( "Where:\n"
          "  <load_file>  is an icon file in OS/2 format, MS Windows format or\n"
          "               MS Windows icon library file.\n"
          "  <lib_index>  is the index of an icon in icon library.\n"
          "  <save_file>  is the name of the OS/2 icon file that will be created.\n"
          "  <save_path>  is the name of the directory where file(s) will be created.\n"
          "  <options>    -2 write an OS/2 v2.0 icon file rather than a v1.2 icon file.\n"
          "               -r repack an icon file and optimise its content for OS/2.\n"
          "               -w write a Windows icon file rather than an OS/2 icon file.\n"
          "               -d dump all loaded structures.\n"
          "               -b dump all loaded bitmaps and structures.\n"
        );
  exit(1);
}

/*------------------------------------------------------------------
 * Writes icon file
 *------------------------------------------------------------------*/
void static save_icon()
{
  printf( "%d icons in file.\n", icon_list.count());

  if( !save_file->open_for_write())
    error( "Cannot create output file."  );

  switch( save_type )
  {
    case os2_v12:
    case os2_v20:
      ICOS2().save( save_file, save_type );
      break;

    case win_ico:
      ICWindows().save( save_file );
      break;

    default:
      error( "Can't wrote that type of file." );
  }
}

/*------------------------------------------------------------------
 * Program entry point
 *------------------------------------------------------------------*/
int main( int argc, char **argv )
{
  int c;

  printf( "Icon Plus Version 1.1.0. Icon conversion utility.\n"
          "Copyright (C) 2001-2009 Dmitry A.Steklenev\n\n" );

  if( argc < 2 )
    help();

  while(( c = getopt( argc, argv, "?dbp:w2i:r" )) != EOF )
  {
    switch(c)
    {
      case '?': help();
                break;
      case 'd': load_dump = 1;
                break;
      case 'b': maps_dump = 1;
                load_dump = 1;
                break;
      case '2': save_type = os2_v20;
                break;
      case 'w': save_type = win_ico;
                break;
      case 'r': repack = 1;
                break;
      case 'p': save_path = optarg;
                break;
      case 'i': lib_index = atoi( optarg );
                break;
      default:
        fprintf( stderr, "Illegal option ignored.\n" );
        break;
     }
  }

  for( ; optind < argc; optind++ )
  {
    if( !load_file )
      load_file = new ICFile( argv[optind] );
    else if( !save_file )
      save_file = new ICFile( argv[optind] );
    else
      error( "Too many filenames." );
  }

  if( !load_file )
    error( "You must specify input filename." );
  if( !load_file->open_for_read())
    error( "Cannot read input file." );

  load_type = query_load_type();

  if( save_path )
    mkdir( save_path );

  if( load_type != win_icl || lib_index >= 0 )
  {
    if( !save_file && !save_path )
      error( "You must specify input and output filenames." );

    if( save_path )
    {
      delete save_file;
      save_file = new ICFile( load_file->name(), save_path );
    }
  }

  switch( load_type )
  {
    case os2_v12:
    case os2_v20: ICOS2().load( load_file );
                  save_icon();
                  break;

    case win_ico: ICWindows().load( load_file );
                  save_icon();
                  break;
    case win_icl:
    {
      ICICL filter( load_file );
      char  name[_MAX_PATH];
      int   i = -1;

      while( filter.find( name ))
      {
        ++i;
        // If the icon index is not specified, save all the found
        // icons under names found in library.
        if( lib_index < 0 ) {
          delete save_file;
          save_file = new ICFile( name, save_path );
          save_icon();
        // Otherwise, save only an icon with the specified index.
        } else if( lib_index == i ) {
          save_icon();
          break;
        }

        icon_list.clear();
      }

      if( lib_index >= 0 && lib_index != i ) {
        error( "Can't find icon with specified index." );
      }
      break;
    }

    default:
      error( "Can't read that type of file." );
  }

  delete load_file;
  delete save_file;
  return 0;
}