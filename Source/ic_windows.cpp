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
 * $Id: ic_windows.cpp,v 1.7 2005/05/01 14:14:55 glass Exp $
 */

#include "ic_windows.h"
#include "ic_icon.h"
#include <stdlib.h>
#include <memory.h>

/*------------------------------------------------------------------
 * Reads MS Windows icon entry
 *------------------------------------------------------------------*/
win_icon_entry ICWindows::load_entry()
{
  win_icon_entry i;

  i.width       = file->read_byte ();
  i.height      = file->read_byte ();
  i.colorCount  = file->read_byte ();
  i.reserved    = file->read_byte ();
  i.planes      = file->read_int16();
  i.bitCount    = file->read_int16();
  i.bytesInRes  = file->read_int32();
  i.imageOffset = file->read_int32();

  if( load_dump )
  {
    printf( "MS Windows icon entry\n" );

    printf( "\twidth         : %u\n", i.width       );
    printf( "\theight        : %u\n", i.height      );
    printf( "\tcolorCount    : %u\n", i.colorCount  );
    printf( "\treserved      : %u\n", i.reserved    );
    printf( "\tplanes        : %u\n", i.planes      );
    printf( "\tbitCount      : %u\n", i.bitCount    );
    printf( "\tbytesInRes    : %u\n", i.bytesInRes  );
    printf( "\timageOffset   : %u\n", i.imageOffset );
    fflush( stdout );
  }

  return i;
}

/*------------------------------------------------------------------
 * Reads MS Windows bitmap header
 *------------------------------------------------------------------*/
win_bitmap_header ICWindows::load_bitmap_header()
{
  win_bitmap_header i;
  ICPalette::size_type count;

  i.size          = file->read_int32();
  i.width         = file->read_int32();
  i.height        = file->read_int32();
  i.planes        = file->read_int16();
  i.bitCount      = file->read_int16();
  i.compression   = file->read_int32();
  i.sizeImage     = file->read_int32();
  i.xPelsPerMeter = file->read_int32();
  i.yPelsPerMeter = file->read_int32();
  i.clrUsed       = file->read_int32();
  i.clrImportant  = file->read_int32();
  i.palette       = ICPalette( i.bitCount * i.planes );

  for( count = 0; count < i.palette.count(); count++ )
  {
    i.palette[count].blue  = file->read_byte();
    i.palette[count].green = file->read_byte();
    i.palette[count].red   = file->read_byte();

    file->read_byte();
  }

  if( load_dump )
  {
    printf( "MS Windows bitmap header\n" );

    printf( "\tsize          : %u\n", i.size          );
    printf( "\twidth         : %u\n", i.width         );
    printf( "\theight        : %u\n", i.height        );
    printf( "\tplanes        : %u\n", i.planes        );
    printf( "\tbitCount      : %u\n", i.bitCount      );
    printf( "\tcompression   : %u\n", i.compression   );
    printf( "\tsizeImage     : %u\n", i.sizeImage     );
    printf( "\txPelsPerMeter : %u\n", i.xPelsPerMeter );
    printf( "\tyPelsPerMeter : %u\n", i.yPelsPerMeter );
    printf( "\tclrUsed       : %u\n", i.clrUsed       );
    printf( "\tclrImportant  : %u\n", i.clrImportant  );

    printf( "\tpalette       : " );
    for( count = 0; count < i.palette.count(); )
    {
      printf( "0x%02X%02X%02X ", i.palette[count].red,
                                 i.palette[count].green,
                                 i.palette[count].blue  );

      if(!( ++count % 6 )) printf( "\n\t\t\t" );
    }

    printf( "\n"   );
    fflush( stdout );
  }

  return i;
}

/*------------------------------------------------------------------
 * Reads MS Windows bitmap data
 *------------------------------------------------------------------*/
void ICWindows::load_bitmap_data( ICBitmap& bitmap )
{
  BYTE*  bytes = bitmap.map();
  UINT32 row, col;

  for( row = 0; row < bitmap.height(); row++ )
  {
    for( col = 0; col < bitmap.byte_width(); col++ )
      *bytes++ = file->read_byte();

    /* The data for pels in each scan line is packed together
       tightly, but all scan lines are padded at the end, so that
       each one begins on a DWORD boundary. */

    while( col++ % 4 ) file->read_byte();
  }
}

/*------------------------------------------------------------------
 * Reads Windows XP bitmap data
 *------------------------------------------------------------------*/
void ICWindows::load_x32map_data( ICBitmap& bitmap, ICAlpha& alpmap )
{
  BYTE*  bytes = bitmap.map();
  BYTE*  alpha = alpmap.map();
  UINT32 row, col;

  for( row = 0; row < bitmap.height(); row++ )
    for( col = 0; col < bitmap.width(); col++ )
    {
      *bytes++ = file->read_byte();
      *bytes++ = file->read_byte();
      *bytes++ = file->read_byte();
      *alpha++ = file->read_byte();
    }
}

/*------------------------------------------------------------------
 * Reads MS Windows icon file
 *------------------------------------------------------------------*/
void ICWindows::load( ICFile* load_file )
{
  file = load_file;
  printf( "Reading MS Windows icon file '%s'...\n", file->name());

  win_icon_directory icondir;
  win_icon_entry     entry;
  win_bitmap_header  header;
  long curpos;

  icondir.reserved = file->read_int16();
  icondir.type     = file->read_int16();
  icondir.count    = file->read_int16();

  if( load_dump )
  {
    printf( "MS Windows icon directory\n" );

    printf( "\treserved      : %u\n", icondir.reserved );
    printf( "\ttype          : %u\n", icondir.type     );
    printf( "\tcount         : %u\n", icondir.count    );
    fflush( stdout );
  }

  while( icondir.count-- )
  {
    entry  = load_entry();
    curpos = file->tell();

    if( entry.imageOffset && file->seek( entry.imageOffset ) == 0 )
    {
      ICIcon* icon;
      header = load_bitmap_header();

      if( header.bitCount == 32 /* Windows XP */ )
      {
        icon = new ICIcon( header.width, abs(header.height)/2, 24 );
        load_x32map_data ( icon->bitmap(), icon->alpha());
        load_bitmap_data ( icon->andmap());
      }
      else
      {
        icon = new ICIcon( header.width, abs(header.height)/2, header.bitCount );
        load_bitmap_data ( icon->bitmap());
        load_bitmap_data ( icon->andmap());
      }

      if( header.height > 0 )
      {
        icon->bitmap().flip();
        icon->andmap().flip();
        icon->alpha ().flip();
      }

      icon->palette() = header.palette;
      ICPalette::size_type white = icon->palette().find( ICRGB::white );
      ICPalette::size_type black = icon->palette().find( ICRGB::black );

      if( maps_dump )
      {
        printf( "Color indexes:\n" );

        printf( "\twhite         : %08X\n", white );
        printf( "\tblack         : %08X\n", black );
        fflush( stdout );
      }

      // Generate generic XOR map
      for( ICIcon::size_type row = 0; row < icon->width(); row++ )
        for( ICIcon::size_type col = 0; col < icon->height(); col++ )
        {
          if( icon->andmap().get_pixel( row, col ) == 1     &&
              icon->bitmap().get_pixel( row, col ) == white &&
              icon->alpha ().get_pixel( row, col ) == 0xFF   )
          {
              icon->bitmap().set_pixel( row, col, black );
              icon->xormap().set_pixel( row, col, 1 );
          }
        }

      if( maps_dump )
      {
        printf( "MS Windows XOR map\n"   );
        icon->xormap().dump();
        printf( "MS Windows AND map\n"   );
        icon->andmap().dump();
        printf( "MS Windows ALPHA map\n" );
        icon->alpha ().dump();
        printf( "MS Windows COLOR map\n" );
        icon->bitmap().dump();
        fflush( stdout );
      }

      icon_list.push_back( *icon );
      delete icon;
    }

    file->seek( curpos );
  }
}

/*------------------------------------------------------------------
 * Writes MS Windows icon entry
 *------------------------------------------------------------------*/
void ICWindows::save_entry( const win_icon_entry& i )
{
  file->write_byte ( i.width       );
  file->write_byte ( i.height      );
  file->write_byte ( i.colorCount  );
  file->write_byte ( i.reserved    );
  file->write_int16( i.planes      );
  file->write_int16( i.bitCount    );
  file->write_int32( i.bytesInRes  );
  file->write_int32( i.imageOffset );
}

/*------------------------------------------------------------------
 * Writes MS Windows bitmap header
 *------------------------------------------------------------------*/
void ICWindows::save_bitmap_header( const win_bitmap_header& i )
{
  ICPalette::size_type count;

  file->write_int32( i.size          );
  file->write_int32( i.width         );
  file->write_int32( i.height        );
  file->write_int16( i.planes        );
  file->write_int16( i.bitCount      );
  file->write_int32( i.compression   );
  file->write_int32( i.sizeImage     );
  file->write_int32( i.xPelsPerMeter );
  file->write_int32( i.yPelsPerMeter );
  file->write_int32( i.clrUsed       );
  file->write_int32( i.clrImportant  );

  for( count = 0; count < i.palette.count(); count++ )
  {
    file->write_byte( i.palette[count].blue  );
    file->write_byte( i.palette[count].green );
    file->write_byte( i.palette[count].red   );
    file->write_byte( 0 );
  }
}

/*------------------------------------------------------------------
 * Writes MS Windows bitmap data
 *------------------------------------------------------------------*/
void ICWindows::save_bitmap_data( ICBitmap& bitmap )
{
  bitmap.flip();

  BYTE*  bytes = bitmap.map();
  UINT32 row, col;

  for( row = 0; row < bitmap.height(); row++ )
  {
    for( col = 0; col < bitmap.byte_width(); col++ )
      file->write_byte( *bytes++ );

    /* The data for pels in each scan line is packed together
       tightly, but all scan lines are padded at the end, so that
       each one begins on a DWORD boundary. */

    while( col++ % 4 ) file->write_byte(0);
  }

  bitmap.flip();
}

/*------------------------------------------------------------------
 * Writes Windows XP bitmap data
 *------------------------------------------------------------------*/
void ICWindows::save_x32map_data( ICBitmap& bitmap, ICAlpha& alpmap )
{
  bitmap.flip();
  alpmap.flip();

  BYTE*  bytes = bitmap.map();
  BYTE*  alpha = alpmap.map();
  UINT32 row, col;

  for( row = 0; row < bitmap.height(); row++ )
    for( col = 0; col < bitmap.width(); col++ )
    {
      file->write_byte( *bytes++ );
      file->write_byte( *bytes++ );
      file->write_byte( *bytes++ );
      file->write_byte( *alpha++ );
    }

  bitmap.flip();
  alpmap.flip();
}

/*------------------------------------------------------------------
 * Writes MS Windows icon file
 *------------------------------------------------------------------*/
void ICWindows::save( ICFile* save_file )
{
  file = save_file;
  printf( "Wrote MS Windows icon file...\n" );

  win_icon_directory icondir;

  icondir.reserved = 0;
  icondir.type     = 1;
  icondir.count    = 0;

  /* Prepare headers and bitmaps offsets */

  win_icon_info* info_list = new win_icon_info[icon_list.count()];
  win_icon_info* info = info_list;
  win_icon_info* last = info_list;

  UINT32 next_bitmap_offset = 0; /* Initially at calculation of bitmap offsets
                                    the size of the file header is not taken.
                                    It will be corrected at saving headers. */
  ICIconList::iterator i;

  for( i  = icon_list.begin();
       i != icon_list.end  (); i++, info++, last++ )
  {
    UINT16 bmp_bits = i->bitmap().bits_per_pixel();
    UINT32 bmp_size = i->bitmap().padded_size(4) +
                      i->andmap().padded_size(4);

    if( bmp_bits == 24 && !i->alpha().empty() /* Windows XP */ )
    {
      bmp_bits = 32;
      bmp_size = i->bitmap().size() +
                 i->alpha ().size() +
                 i->andmap().padded_size(4);
    }

    memset( info, 0, sizeof(win_icon_info));

    info->bitmap.size        = 40;
    info->bitmap.width       = i->bitmap().width ();
    info->bitmap.height      = i->bitmap().height()*2;
    info->bitmap.planes      = 1;
    info->bitmap.bitCount    = bmp_bits;
    info->bitmap.sizeImage   = bmp_size;
    info->bitmap.clrUsed     = i->palette().count();
    info->bitmap.palette     = i->palette();

    info->entry.width        = i->bitmap().width ();
    info->entry.height       = i->bitmap().height();
    info->entry.colorCount   = i->palette().count();
    info->entry.reserved     = 0;
    info->entry.planes       = 1;
    info->entry.bitCount     = bmp_bits;
    info->entry.imageOffset  = next_bitmap_offset;
    info->entry.bytesInRes   = info->bitmap.size      +
                               info->bitmap.sizeImage +
                               i->palette().count()*4;
    ++icondir.count;
    next_bitmap_offset += info->entry.bytesInRes;
    info->icon = i;
  }

  next_bitmap_offset = 6 + icondir.count*16;

  /* Write headers */

  file->write_int16( icondir.reserved );
  file->write_int16( icondir.type     );
  file->write_int16( icondir.count    );

  for( info = info_list; info < last; info++ )
  {
    info->entry.imageOffset += next_bitmap_offset;
    save_entry( info->entry );
  }

  /* Write bitmaps */

  for( info = info_list; info < last; info++ )
  {
    ICIcon icon = *info->icon;
    ICPalette::size_type white = icon.palette().find( ICRGB::white );

    // Generate MS Windows XOR map (add inversion)
    for( ICIcon::size_type row = 0; row < icon.width(); row++ )
      for( ICIcon::size_type col = 0; col < icon.height(); col++ )
      {
        if( icon.andmap().get_pixel( row, col ) == 1 &&
            icon.xormap().get_pixel( row, col ) == 1  )
            icon.bitmap().set_pixel( row, col, white  );
      }

    save_bitmap_header( info->bitmap );

    if( info->bitmap.bitCount == 32 /* Windows XP */ )
    {
      save_x32map_data( icon.bitmap(), icon.alpha());
      save_bitmap_data( icon.andmap());
    }
    else
    {
      save_bitmap_data( icon.bitmap());
      save_bitmap_data( icon.andmap());
    }
  }
}

