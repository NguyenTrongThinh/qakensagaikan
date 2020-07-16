/*
 * libse660 created on Mar 21, 2019 by ThinhNguyen. This library is inherited from Symbol Barcode Scanner Reader Software
 * Written by Andy Stewart
 * June 5, 2007
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
 */


#ifndef SE660_H_
#define SE660_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "string.h"

#include <linux/input.h>


char cvt_ev_char(int);
char cvt_ev_char_right_shift(int);
void debug_rcvd_event(struct input_event *);
void setEnableDebug();
void setDisableDebug();
int openScannerDevice(const char *devName);
void closeScannerDevice();
//blocking function
int startReadBlockingMode(char *content);

#define MAX_IN_SIZE 30

#ifdef __cplusplus
}
#endif

#endif /* SE660_H_ */
