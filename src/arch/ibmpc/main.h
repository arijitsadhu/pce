/*****************************************************************************
 * pce                                                                       *
 *****************************************************************************/

/*****************************************************************************
 * File name:     src/arch/ibmpc/main.h                                      *
 * Created:       2001-05-01 by Hampa Hug <hampa@hampa.ch>                   *
 * Last modified: 2004-11-28 by Hampa Hug <hampa@hampa.ch>                   *
 * Copyright:     (C) 1996-2004 Hampa Hug <hampa@hampa.ch>                   *
 *****************************************************************************/

/*****************************************************************************
 * This program is free software. You can redistribute it and / or modify it *
 * under the terms of the GNU General Public License version 2 as  published *
 * by the Free Software Foundation.                                          *
 *                                                                           *
 * This program is distributed in the hope  that  it  will  be  useful,  but *
 * WITHOUT  ANY   WARRANTY,   without   even   the   implied   warranty   of *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU  General *
 * Public License for more details.                                          *
 *****************************************************************************/

/* $Id$ */


#ifndef PCE_IBMPC_MAIN_H
#define PCE_IBMPC_MAIN_H 1


#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#define PCE_BRK_STOP  1
#define PCE_BRK_ABORT 2
#define PCE_BRK_SNAP  3


typedef void (*seta_uint8_f) (void *ext, unsigned long addr, unsigned char val);
typedef void (*seta_uint16_f) (void *ext, unsigned long addr, unsigned short val);
typedef unsigned char (*geta_uint8_f) (void *ext, unsigned long addr);
typedef unsigned short (*geta_uint16_f) (void *ext, unsigned long addr);

typedef void (*set_uint8_f) (void *ext, unsigned char val);
typedef void (*set_uint16_f) (void *ext, unsigned short val);
typedef unsigned char (*get_uint8_f) (void *ext);
typedef unsigned short (*get_uint16_f) (void *ext);


struct ibmpc_t;


extern char          *par_terminal;
extern char          *par_video;
extern char          *par_cpu;
extern unsigned long par_int28;


#include <lib/brkpt.h>
#include <lib/log.h>
#include <lib/hexdump.h>
#include <lib/ihex.h>
#include <lib/load.h>

#include <cpu/e8086/e8086.h>
#include <chipset/e8237.h>
#include <chipset/e8250.h>
#include <chipset/e8253.h>
#include <chipset/e8255.h>
#include <chipset/e8259.h>

#include <libini/libini.h>

#include <terminal/terminal.h>
#include <terminal/null.h>
#include <terminal/vt100.h>

#ifdef PCE_X11_USE
#include <terminal/x11.h>
#endif

#ifdef PCE_SDL_USE
#include <terminal/sdl.h>
#endif

#include <devices/memory.h>
#include <devices/nvram.h>
#include <devices/parport.h>
#include <devices/serport.h>
#include <devices/video.h>
#include <devices/mda.h>
#include <devices/hgc.h>
#include <devices/cga.h>
#include <devices/ega.h>
#include <devices/vga.h>

#include <devices/disk.h>
#include <devices/blkcow.h>
#include <devices/blkdosem.h>
#include <devices/blkimg.h>
#include <devices/blkpart.h>
#include <devices/blkpce.h>
#include <devices/blkram.h>

#include "util.h"
#include "int13.h"
#include "mouse.h"
#include "ems.h"
#include "xms.h"
#include "ibmpc.h"
#include "hook.h"
#include "msg.h"


extern ibmpc_t *par_pc;


#endif