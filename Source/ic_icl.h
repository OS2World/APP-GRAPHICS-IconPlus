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
 * $Id: ic_icl.h,v 1.5 2009/11/12 13:06:37 glass Exp $
 */

#ifndef IC_ICL_H
#define IC_ICL_H

#include "ic_windows.h"
#include "ic_chlist.h"

#if !defined(__ccdoc__)

struct win_executable
{
  UINT16 type;      /* Signature of the executable file. */
  UINT32 ne_offset; /* Offset of New Executable header from beginning of file. */
  UINT16 ne_type;   /* Signature of the New Executable file. */
  UINT16 rs_offset; /* Offset of beginning of resource table relative to beginning
                       of New Executable header. */
  UINT16 rs_shift;  /* Alignment shift count for resource data. */
};

struct win_resource_directory
{
  UINT16 type;      /* This is an integer type if the high-order bit is
                       set (8000h); otherwise, it is an offset to the type string,
                       the offset is relative to the beginning of the resource
                       table. A zero type ID marks the end of the resource type
                       information blocks. */
  UINT16 count;     /* Number of resources of this type. */
  UINT32 reserved;  /* Reserved for run-time use. */
  long   offset;    /* This is not a part of the standard header! */
};

#define RT_ICON       0x8003
#define RT_GROUP_ICON 0x800E

struct win_resource_entry
{
  UINT16 offset;    /* File offset to the contents of the resource data,
                       relative to beginning of file. The offset is in terms
                       of the alignment shift count value specified at
                       beginning of the resource table. */
  UINT16 size;      /* Length of the resource in the file (in bytes)/ */
  UINT16 flags;     /* Resource flag word. */
  UINT16 id;        /* Resource ID. This is an integer type if the high-order
                       bit is set (8000h), otherwise it is the offset to the
                       resource string, the offset is relative to the
                       beginning of the resource table. */
  UINT32 reserved;  /* Reserved for run-time use. */
};

#endif

/**
 * Loads MS Windows icon library files.
 * @author Dmitry A Steklenev
 */

class ICICL : public ICWindows
{
  public:

    /** Construct the ICICL object. */
    ICICL( ICFile* load_file )
    {
      init(load_file);
    }

    /** Finds the next icon in library. */
    virtual char* find( char* save_name );

  private:
    virtual void init( ICFile* load_file );
    virtual win_icon_entry load_entry();

  private:
    ICFile*  file;
    ICChList name_list;

    win_executable         header;
    win_resource_directory group;
    win_resource_directory icons;
};

#endif
