/*****************************************************************************
 * pce                                                                       *
 *****************************************************************************/

/*****************************************************************************
 * File name:     src/devices/serport.c                                      *
 * Created:       2003-09-04 by Hampa Hug <hampa@hampa.ch>                   *
 * Last modified: 2004-02-19 by Hampa Hug <hampa@hampa.ch>                   *
 * Copyright:     (C) 2003-2004 Hampa Hug <hampa@hampa.ch>                   *
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


#include <stdio.h>
#include <stdlib.h>

#include "serport.h"


void ser_init (serport_t *ser, unsigned long base)
{
  ser->io = base;

  e8250_init (&ser->uart);

  ser->uart.setup_ext = ser;
  ser->uart.setup = (e8250_setup_f) &ser_uart_setup;
  ser->uart.send_ext = ser;
  ser->uart.send = (e8250_send_f) &ser_uart_out;
  ser->uart.recv_ext = ser;
  ser->uart.recv = (e8250_recv_f) &ser_uart_inp;

  mem_blk_init (&ser->port, base, 8, 0);
  ser->port.ext = &ser->uart;
  ser->port.get_uint8 = (mem_get_uint8_f) &e8250_get_uint8;
  ser->port.set_uint8 = (mem_set_uint8_f) &e8250_set_uint8;
  ser->port.get_uint16 = (mem_get_uint16_f) &e8250_get_uint16;
  ser->port.set_uint16 = (mem_set_uint16_f) &e8250_set_uint16;

  ser->bps = 2400;
  ser->databits = 8;
  ser->stopbits = 1;
  ser->parity = E8250_PARITY_N;

  ser->dtr = 0;
  ser->rts = 0;

  ser->fp = NULL;
  ser->fp_close = 0;

  /* ready to receive data */
  e8250_set_dsr (&ser->uart, 1);
  e8250_set_cts (&ser->uart, 1);
}

serport_t *ser_new (unsigned long base)
{
  serport_t *ser;

  ser = malloc (sizeof (serport_t));
  if (ser == NULL) {
    return (NULL);
  }

  ser_init (ser, base);

  return (ser);
}

void ser_free (serport_t *ser)
{
  e8250_free (&ser->uart);
  mem_blk_free (&ser->port);

  if (ser->fp_close) {
    fclose (ser->fp);
  }
}

void ser_del (serport_t *ser)
{
  if (ser != NULL) {
    ser_free (ser);
    free (ser);
  }
}

mem_blk_t *ser_get_reg (serport_t *ser)
{
  return (&ser->port);
}

int ser_set_fp (serport_t *ser, FILE *fp, int close)
{
  if (ser->fp_close) {
    fclose (ser->fp);
  }

  ser->fp = fp;
  ser->fp_close = (fp != NULL) && close;

  return (0);
}

int ser_set_fname (serport_t *ser, const char *fname)
{
  FILE *fp;

  fp = fopen (fname, "wb");
  if (fp == NULL) {
    return (1);
  }

  if (ser_set_fp (ser, fp, 1)) {
    fclose (fp);
    return (1);
  }

  return (0);
}

void ser_uart_setup (serport_t *ser, unsigned char val)
{
  int           chg;
  unsigned long bps;
  unsigned      data, stop, parity;
  int           dtr, rts;

  chg = 0;

  bps = e8250_get_bps (&ser->uart);
  if (bps != ser->bps) {
    ser->bps = bps;
    chg = 1;
  }

  data = e8250_get_databits (&ser->uart);
  if (data != ser->databits) {
    ser->databits = data;
    chg = 1;
  }

  stop = e8250_get_stopbits (&ser->uart);
  if (stop != ser->stopbits) {
    ser->stopbits = stop;
    chg = 1;
  }

  parity = e8250_get_parity (&ser->uart);
  if (parity != ser->parity) {
    ser->parity = parity;
    chg = 1;
  }

  if (chg) {
/*    ser_line_setup (ser); */
  }

  chg = 0;

  dtr = e8250_get_dtr (&ser->uart);
  if (dtr != ser->dtr) {
    ser->dtr = dtr;
    chg = 1;
  }

  rts = e8250_get_rts (&ser->uart);
  if (rts != ser->rts) {
    ser->rts = rts;
    chg = 1;
  }

  if (chg) {
/*    ser_modem_setup (ser); */
  }
}

/* 8250 output buffer is not empty */
void ser_uart_out (serport_t *ser, unsigned char val)
{
  unsigned char c;

  if (ser->fp == NULL) {
    return;
  }

  while (1) {
    if (e8250_get_out (&ser->uart, &c)) {
      break;
    }

    fputc (c, ser->fp);
  }

  fflush (ser->fp);
}

/* 8250 input buffer is not full */
void ser_uart_inp (serport_t *ser, unsigned char val)
{
}

void ser_receive (serport_t *ser, unsigned char val)
{
  e8250_receive (&ser->uart, val);
}

void ser_clock (serport_t *ser, unsigned n)
{
  e8250_clock (&ser->uart, n);
}
