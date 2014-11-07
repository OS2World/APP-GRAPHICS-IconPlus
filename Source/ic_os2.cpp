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
 * $Id: ic_os2.cpp,v 1.8 2009/11/13 11:39:46 glass Exp $
 */

#include "ic_os2.h"
#include "ic_icon.h"
#include <memory.h>

/*------------------------------------------------------------------
 * Reads OS/2 bitmap header
 *------------------------------------------------------------------*/
os2_bitmap_header_v20 ICOS2::load_bitmap_header()
{
  os2_bitmap_header_v20 i;
  ICPalette::size_type  count;

  memset( &i, 0, sizeof(i));

  i.type           = file->read_int16();
  i.cbsize         = file->read_int32();
  i.xhotspot       = file->read_int16();
  i.yhotspot       = file->read_int16();
  i.bitmap_offset  = file->read_int32();
  i.cbfix          = file->read_int32();

  if( i.cbsize == 0x4E /* version 2.0 */ )
  {
    i.width          = file->read_int32();
    i.height         = file->read_int32();
    i.planes         = file->read_int16();
    i.bits_per_pixel = file->read_int16();
    i.compression    = file->read_int32();
    i.bitmap_size    = file->read_int32();
    i.devXres        = file->read_int32();
    i.devYres        = file->read_int32();
    i.clrUsed        = file->read_int32();
    i.clrImportant   = file->read_int32();
    i.units          = file->read_int16();
    i.reserved       = file->read_int16();
    i.recording      = file->read_int16();
    i.rendering      = file->read_int16();
    i.size1          = file->read_int32();
    i.size2          = file->read_int32();
    i.color_encoding = file->read_int32();
    i.app_identifier = file->read_int32();
  }
  else
  {
    i.width          = file->read_int16();
    i.height         = file->read_int16();
    i.planes         = file->read_int16();
    i.bits_per_pixel = file->read_int16();
  }

  i.palette = ICPalette( i.bits_per_pixel * i.planes );

  for( count = 0; count < i.palette.count(); count++ )
  {
    i.palette[count].blue  = file->read_byte();
    i.palette[count].green = file->read_byte();
    i.palette[count].red   = file->read_byte();

    if( i.cbsize == 0x4E /* version 2.0 */ )
      file->read_byte();
  }

  if( load_dump )
  {
    if( i.cbsize == 0x4E /* version 2.0 */ )
      printf( "OS/2 bitmap v2.0 header\n"  );
    else
      printf( "OS/2 bitmap v1.2 header\n"  );

    printf( "\ttype           : 0x%04X\n", i.type           );
    printf( "\tcbsize         : %u\n"    , i.cbsize         );
    printf( "\txhotspot       : %u\n"    , i.xhotspot       );
    printf( "\tyhotspot       : %u\n"    , i.yhotspot       );
    printf( "\tbitmap_offset  : %u\n"    , i.bitmap_offset  );
    printf( "\tcbfix          : %u\n"    , i.cbfix          );
    printf( "\twidth          : %u\n"    , i.width          );
    printf( "\theight         : %u\n"    , i.height         );
    printf( "\tplanes         : %u\n"    , i.planes         );
    printf( "\tbits_per_pixel : %u\n"    , i.bits_per_pixel );

    if( i.cbsize == 0x4E /* version 2.0 */ )
    {
      printf( "\tcompression    : %u\n"  , i.compression    );
      printf( "\tbitmap_size    : %u\n"  , i.bitmap_size    );
      printf( "\tdevXres        : %u\n"  , i.devXres        );
      printf( "\tdevYres        : %u\n"  , i.devYres        );
      printf( "\tclrUsed        : %u\n"  , i.clrUsed        );
      printf( "\tclrImportant   : %u\n"  , i.clrImportant   );
      printf( "\tunits          : %u\n"  , i.units          );
      printf( "\treserved       : %u\n"  , i.reserved       );
      printf( "\trecording      : %u\n"  , i.recording      );
      printf( "\trendering      : %u\n"  , i.rendering      );
      printf( "\tsize1          : %u\n"  , i.size1          );
      printf( "\tsize2          : %u\n"  , i.size2          );
      printf( "\tcolor_encoding : %u\n"  , i.color_encoding );
      printf( "\tapp_identifier : %u\n"  , i.app_identifier );
    }

    printf( "\tpalette        : " );
    for( count = 0; count < i.palette.count(); )
    {
      printf( "0x%02X%02X%02X ", i.palette[count].red,
                                 i.palette[count].green,
                                 i.palette[count].blue  );

      if(!( ++count % 6 )) printf( "\n\t\t\t " );
    }

    printf( "\n"   );
    fflush( stdout );
  }

  return i;
}

/*------------------------------------------------------------------
 * Reads OS/2 bitmap array header
 *------------------------------------------------------------------*/
os2_bitmap_array_header ICOS2::load_barray_header()
{
  os2_bitmap_array_header i;

  i.type           = file->read_int16();
  i.cbsize         = file->read_int32();
  i.next           = file->read_int32();
  i.display_width  = file->read_int16();
  i.display_height = file->read_int16();

  if( load_dump )
  {
    printf( "OS/2 bitmap array header\n" );

    printf( "\ttype           : 0x%04X\n"     , i.type           );
    printf( "\tcbsize         : %u\n"         , i.cbsize         );
    printf( "\tnext           : 0x%08X (%u)\n", i.next, i.next   );
    printf( "\tdisplay_width  : %u\n"         , i.display_width  );
    printf( "\tdisplay_height : %u\n"         , i.display_height );

    fflush( stdout );
  }

  return i;
}

/*------------------------------------------------------------------
 * Reads OS/2 bitmap data
 *------------------------------------------------------------------*/
void ICOS2::load_bitmap_data( ICBitmap& bitmap )
{
  BYTE*  bytes = bitmap.map();
  UINT32 row, col;

  for( row = 0; row < bitmap.height(); row++ )
  {
    for( col = 0; col < bitmap.byte_width(); col++ )
      *bytes++ = file->read_byte();

    /* The data for pels in each scan line is packed together
       tightly, but all scan lines are padded at the end, so that
       each one begins on a ULONG boundary. */

    while( col++ % 4 ) file->read_byte();
  }

  bitmap.flip();
}

/*------------------------------------------------------------------
 * Reads OS/2 bitmap
 *------------------------------------------------------------------*/
void ICOS2::load_bitmap()
{
  os2_bitmap_header_v20 masks_header = load_bitmap_header();
  os2_bitmap_header_v20 image_header;

  if( masks_header.type != OS2_BFT_COLORICON &&
      masks_header.type != OS2_BFT_ICON       )
  {
    fprintf( stderr, "Illegal resource type ignored: 0x%04X.\n", masks_header.type );
    return;
  }

  if( masks_header.planes         != 1 ||
      masks_header.bits_per_pixel != 1  )
  {
    fprintf( stderr, "Illegal bitmap resource ignored\n" );
    return;
  }

  if( masks_header.type == OS2_BFT_COLORICON )
      image_header = load_bitmap_header();
  else
      image_header.type =  OS2_BFT_ICON;

  if( image_header.type != OS2_BFT_COLORICON &&
      image_header.type != OS2_BFT_ICON       )
  {
    fprintf( stderr, "Illegal resource type ignored: 0x%04X.\n", image_header.type );
    return;
  }

  ICIcon* icon;

  if( masks_header.type == OS2_BFT_COLORICON )
    icon = new ICIcon( image_header.width, image_header.height, image_header.bits_per_pixel );
  else
    icon = new ICIcon( masks_header.width, masks_header.height/2, 1 );

  file->seek( masks_header.bitmap_offset );
  load_bitmap_data( icon->xormap());
  load_bitmap_data( icon->andmap());

  if( masks_header.type == OS2_BFT_COLORICON )
  {
    file->seek( image_header.bitmap_offset );
    load_bitmap_data( icon->bitmap());

    icon->move_hotspot( image_header.xhotspot, image_header.yhotspot );
    icon->palette() = image_header.palette;
  }
  else
  {
    icon->move_hotspot( masks_header.xhotspot, masks_header.yhotspot );
    icon->palette() = ICPalette::default_bw();

    // Generate colors map for b&w icons
    for( ICIcon::size_type row = 0; row < icon->width(); row++ )
      for( ICIcon::size_type col = 0; col < icon->height(); col++ )
      {
        if( icon->andmap().get_pixel( row, col ) == 0 &&
            icon->xormap().get_pixel( row, col ) == 1 )
        {
            icon->bitmap().set_pixel( row, col, 1 );
            icon->xormap().set_pixel( row, col, 0 );
        }
      }
  }

  if( maps_dump )
  {
    printf( "OS/2 XOR map\n" );
    icon->xormap().dump();
    printf( "OS/2 AND map\n" );
    icon->andmap().dump();
    printf( "OS/2 COLOR map\n" );
    icon->bitmap().dump();
  }

  icon_list.push_back( *icon );
  delete icon;
}

/*------------------------------------------------------------------
 * Reads OS/2 icon file
 *------------------------------------------------------------------*/
void ICOS2::load( ICFile* load_file )
{
  long   curpos = load_file->tell();
  UINT16 type   = load_file->read_int16();

  file = load_file;
  file->seek( curpos );

  printf( "Reading OS/2 icon file '%s'...\n", file->name());

  if( type == OS2_BFT_BITMAPARRAY )
  {
    for(;;)
    {
      os2_bitmap_array_header array_header = load_barray_header();
      load_bitmap();

      if( array_header.next )
        file->seek( array_header.next );
      else
        break;
    }
  }
  else
    load_bitmap();
}

/*------------------------------------------------------------------
 * Repack an icon list and optimise its content for OS/2
 *------------------------------------------------------------------*/
void ICOS2::repack_list()
{
  ICIconList os2_list;

  ICIconList::iterator i16x16   = NULL;
  ICIconList::iterator i20x20   = NULL;
  ICIconList::iterator i32x32   = NULL;
  ICIconList::iterator i40x40   = NULL;
  ICIconList::iterator iLargest = NULL;
  ICIconList::iterator i;

  for( i = icon_list.begin(); i != icon_list.end(); i++ )
  {
    // Searches for a most colorful mini icon for the  96 dpi.
    if( i->width() == 16 && i->height() == 16 ) {
      if( !i16x16 || ( i->bits_per_pixel() > i16x16->bits_per_pixel())) {
        i16x16 = i;
      }
    // Searches for a most colorful mini icon for the 120 dpi.
    } else if( i->width() == 20 && i->height() == 20 ) {
      if( !i20x20 || ( i->bits_per_pixel() > i20x20->bits_per_pixel())) {
        i20x20 = i;
      }
    // Searches for a most colorful icon for the  96 dpi.
    } else if( i->width() == 32 && i->height() == 32 ) {
      if( !i32x32 || ( i->bits_per_pixel() > i32x32->bits_per_pixel())) {
        i32x32 = i;
      }
    // Searches for a most colorful icon for the 120 dpi.
    } else if( i->width() == 40 && i->height() == 40 ) {
      if( !i40x40 || ( i->bits_per_pixel() > i40x40->bits_per_pixel())) {
        i40x40 = i;
      }
    } else {
    // Searches for a most colorful and biggest icon. But this icon must be
    // not too big because OS/2 icons file can't exceed 64 K limit.
      if(( iLargest == NULL ) ||
         ( iLargest->width() <  i->width() && i->width() < 128 ) ||
         ( iLargest->width() == i->width() && iLargest->bits_per_pixel() < i->bits_per_pixel()))
      {
        iLargest = i;
      }
    }
  }

  // If OS/2 can't find a suitable bitmap for current dpi OS/2 uses the first
  // bitmap in icon file for scaling. Therefore the biggest bitmaps should be
  // placed first.

  if(( !i16x16 || !i20x20 || !i32x32 || !i40x40 ) && iLargest ) {
    os2_list.push_back( *iLargest );
  }
  if( i40x40 ) {
    os2_list.push_back( *i40x40 );
  }
  if( i32x32 ) {
    os2_list.push_back( *i32x32 );
  }
  if( i20x20 ) {
    os2_list.push_back( *i20x20 );
  }
  if( i16x16 ) {
    os2_list.push_back( *i16x16 );
  }

  icon_list.clear();
  for( i = os2_list.begin(); i != os2_list.end(); i++ ) {
    icon_list.push_back( *i );
  }
}

/*------------------------------------------------------------------
 * Writes OS/2 bitmap array header
 *------------------------------------------------------------------*/
void ICOS2::save_barray_header( const os2_bitmap_array_header& i )
{
  file->write_int16( i.type           );
  file->write_int32( i.cbsize         );
  file->write_int32( i.next           );
  file->write_int16( i.display_width  );
  file->write_int16( i.display_height );
}

/*------------------------------------------------------------------
 * Writes OS/2 bitmap header
 *------------------------------------------------------------------*/
void ICOS2::save_bitmap_header( const os2_bitmap_header_v20& i )
{
  ICPalette::size_type count;

  file->write_int16( i.type          );
  file->write_int32( i.cbsize        );
  file->write_int16( i.xhotspot      );
  file->write_int16( i.yhotspot      );
  file->write_int32( i.bitmap_offset );
  file->write_int32( i.cbfix         );

  if( i.cbsize == 0x4E /* version 2.0 */ )
  {
    file->write_int32( i.width          );
    file->write_int32( i.height         );
    file->write_int16( i.planes         );
    file->write_int16( i.bits_per_pixel );
    file->write_int32( i.compression    );
    file->write_int32( i.bitmap_size    );
    file->write_int32( i.devXres        );
    file->write_int32( i.devYres        );
    file->write_int32( i.clrUsed        );
    file->write_int32( i.clrImportant   );
    file->write_int16( i.units          );
    file->write_int16( i.reserved       );
    file->write_int16( i.recording      );
    file->write_int16( i.rendering      );
    file->write_int32( i.size1          );
    file->write_int32( i.size2          );
    file->write_int32( i.color_encoding );
    file->write_int32( i.app_identifier );
  }
  else
  {
    file->write_int16( i.width          );
    file->write_int16( i.height         );
    file->write_int16( i.planes         );
    file->write_int16( i.bits_per_pixel );
  }

  for( count = 0; count < i.palette.count(); count++ )
  {
    file->write_byte( i.palette[count].blue  );
    file->write_byte( i.palette[count].green );
    file->write_byte( i.palette[count].red   );

    if( i.cbsize == 0x4E /* version 2.0 */ )
      file->write_byte(0);
  }
}

/*------------------------------------------------------------------
 * Writes OS/2 bitmap data
 *------------------------------------------------------------------*/
void ICOS2::save_bitmap_data( ICBitmap& bitmap )
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
       each one begins on a ULONG boundary. */

    while( col++ % 4 ) file->write_byte(0);
  }

  bitmap.flip();
}

/*------------------------------------------------------------------
 * Wrote OS/2 icon file
 *------------------------------------------------------------------*/
void ICOS2::save( ICFile* save_file, icon_type version )
{
  file = save_file;

  if( version == os2_v20 )
    printf( "Wrote OS/2 version 2.0 icon file '%s'...\n", file->name());
  else
    printf( "Wrote OS/2 version 1.2 icon file '%s'...\n", file->name());

  if( repack ) {
    repack_list();
  }

  /* Prepare headers and bitmaps offsets */

  os2_icon_info* info_list = new os2_icon_info[icon_list.count()];
  os2_icon_info* info = info_list;
  os2_icon_info* last = info_list;

  UINT32 next_bitmap_offset = 0; /* Initially at calculation of bitmap offsets
                                    the size of the file header is unknown.
                                    It will be corrected at saving headers. */
  ICIconList::iterator i;

  for( i  = icon_list.begin();
       i != icon_list.end  (); i++, info++, last++ )
  {
    memset( info, 0, sizeof(os2_icon_info));

    // Corrects icon maps according alpha map.
    ICPalette::size_type black = i->palette().find( ICRGB::black );

    for( ICIcon::size_type row = 0; row < i->width(); row++ ) {
      for( ICIcon::size_type col = 0; col < i->height(); col++ )
      {
        if( i->alpha ().get_pixel( row, col ) < 0x10 ) {
            i->andmap().set_pixel( row, col, 1 );
            i->bitmap().set_pixel( row, col, black );
        }
      }
    }

    info->header.type             = OS2_BFT_BITMAPARRAY;
    info->header.cbsize           = version == os2_v12 ? 0x28 : 0x5C;
    info->header.display_width    = 0;
    info->header.display_height   = 0;
    info->header.next             = info == info_list ? 0 : (info-1)->header.next;

    info->xorand.type             = i->is_bw() ? OS2_BFT_ICON : OS2_BFT_COLORICON;
    info->xorand.cbsize           = version == os2_v12 ? 0x1A : 0x4E;
    info->xorand.xhotspot         = i->xhotspot();
    info->xorand.yhotspot         = i->yhotspot();
    info->xorand.cbfix            = version == os2_v12 ? 0x0C : 0x40;
    info->xorand.width            = i->andmap().width ();
    info->xorand.height           = i->andmap().height() + i->xormap().height();
    info->xorand.planes           = 1;
    info->xorand.bits_per_pixel   = 1;
    info->xorand.bitmap_offset    = next_bitmap_offset;
    info->xorand.clrUsed          = 0;
    info->xorand.clrImportant     = 2;
    info->xorand.palette          = ICPalette::default_bw();

    if( info->xorand.width == 40 && info->xorand.height == 80 )
    {
      info->header.display_width  = 1024;
      info->header.display_height = 768;
      info->xorand.devXres        = 3622;
      info->xorand.devYres        = 3622;
    }

    next_bitmap_offset += i->andmap().padded_size(4) +
                          i->xormap().padded_size(4);
    if( !i->is_bw())
    {
      info->bitmap.type           = OS2_BFT_COLORICON;
      info->bitmap.cbsize         = version == os2_v12 ? 0x1A : 0x4E;
      info->bitmap.xhotspot       = i->xhotspot();
      info->bitmap.yhotspot       = i->yhotspot();
      info->bitmap.cbfix          = version == os2_v12 ? 0x0C : 0x40;
      info->bitmap.width          = i->bitmap().width ();
      info->bitmap.height         = i->bitmap().height();
      info->bitmap.planes         = 1;
      info->bitmap.bits_per_pixel = i->bitmap().bits_per_pixel();
      info->bitmap.bitmap_offset  = next_bitmap_offset;
      info->bitmap.palette        = i->palette();

      /* I don't know why, but the OS/2 icons editor doesn't
         fill in these fields:

      info->bitmap.clrUsed        = i->palette().count();
      info->bitmap.clrImportant   = i->palette().count();

      */

      if( info->bitmap.width == 40 && info->bitmap.height == 40 )
      {
        info->bitmap.devXres = 3622;
        info->bitmap.devYres = 3622;
      }

      next_bitmap_offset += i->bitmap().padded_size(4);
    }

    if( i->is_bw())
    {
      info->header.next += 14 + info->xorand.cbsize
                              + info->xorand.palette.count()*(version == os2_v12 ? 3 : 4);
    }
    else
    {
      info->header.next += 14 + info->xorand.cbsize
                              + info->xorand.palette.count()*(version == os2_v12 ? 3 : 4)
                              + info->bitmap.cbsize
                              + info->bitmap.palette.count()*(version == os2_v12 ? 3 : 4);
    }

    if( i + 1 == icon_list.end())
    {
      next_bitmap_offset = info->header.next;
      info->header.next  = 0;
    }

    info->icon = i;
  }

  /* Write headers */

  for( info = info_list; info < last; info++ )
  {
    save_barray_header( info->header );

    info->xorand.bitmap_offset += next_bitmap_offset;
    info->bitmap.bitmap_offset += next_bitmap_offset;

    if( info->xorand.type == OS2_BFT_COLORICON )
    {
      save_bitmap_header( info->xorand );
      save_bitmap_header( info->bitmap );
    }
    else
      save_bitmap_header( info->xorand );
  }

  /* Write bitmaps */

  for( info = info_list; info < last; info++ )
  {
    if( info->xorand.type == OS2_BFT_COLORICON )
    {
      save_bitmap_data( info->icon->xormap());
      save_bitmap_data( info->icon->andmap());
      save_bitmap_data( info->icon->bitmap());
    }
    else
    {
      ICIcon icon = *info->icon;

      // Generate OS/2 XOR mask for B&W icons
      for( ICIcon::size_type row = 0; row < icon.width(); row++ )
        for( ICIcon::size_type col = 0; col < icon.height(); col++ )
        {
          if( icon.andmap().get_pixel( row, col ) == 0 )
              icon.xormap().set_pixel( row, col, icon.bitmap().get_pixel( row, col ));
        }

      save_bitmap_data( icon.xormap());
      save_bitmap_data( icon.andmap());
    }
  }
}

