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
 * $Id: ic_config.h,v 1.9 2009/11/13 11:39:46 glass Exp $
 */

#ifndef IC_CONFIG_H
#define IC_CONFIG_H

typedef unsigned char      BYTE;   //@- A byte.
typedef unsigned short int UINT16; //@- 16-bit unsigned integer in the range 0 through 65 535.
typedef unsigned long int  UINT32; //@- 32-bit unsigned integer in the range 0 through 4 294 967 295.
typedef short int          INT16;  //@- 16-bit signed integer in the range -32 768 through 32 767.
typedef int                INT32;  //@- 32-bit signed integer in the range -2 147 483 648 through 2 147 483 647.

#if defined(__EMX__) || defined(__IBMCPP__) || defined(__BORLANDC__) || defined(__WATCOMC__)
  #define IC_FOPEN_BWRITE "wb"
  #define IC_FOPEN_BREAD  "rb"
#else
  /**
   * Defines mode for opening a binary file for reading
   * via <i>fopen</i> function.
   */

  #define IC_FOPEN_BWRITE "w"

  /**
   * Defines mode for creating an empty binary file
   * for writing via <i>fopen</i> function.
   */

  #define IC_FOPEN_BREAD  "r"
#endif

#if defined(__TOS_OS2__) || defined(__TOS_WIN__)
  #define IC_PC '\\'
  #define IC_PS "\\"
#else
  /** Defines path separator character. */
  #define IC_PC '/'
  /** Defines path separator string. */
  #define IC_PS "/"
#endif

/** Defines known icons types. */

enum icon_type { unknown, /* Unknown format           */
                 os2_v12, /* OS/2 v1.2  icons format  */
                 os2_v20, /* OS/2 v2.0  icons format  */
                 win_ico, /* MS Windows icons format  */
                 win_icl  /* MS Windows icons library */
               };

extern int load_dump; //@- Enables dump of all loaded structures.
extern int maps_dump; //@- Enables dump of all loaded bitmaps.
extern int repack;    //@- Repack an icon file and optimise its content for OS/2.

#endif
