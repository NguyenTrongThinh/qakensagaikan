/*
 * se660.c
 *
 *  Created on: Mar 21, 2019
 *      Author: thinhnguyen
 */

#include "libse660/inc/se660.h"
#include <stdio.h>
#include <stdint.h>

int fd = -1;
int DEBUG = 0;

#define DEBUG_PRINT(fmt, ...) \
        do { if (DEBUG) printf("%s:%d:%s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

int openScannerDevice(const char *devName){
	fd = open(devName, O_RDONLY);
	if (fd < 0) {
		DEBUG_PRINT("%s %s", "Error opening file descriptor . Exiting", devName);
	}
	return fd;
}

void closeScannerDevice() {
	 close(fd);
}
void setEnableDebug() {
	DEBUG = 1;
}
void setDisableDebug() {
	DEBUG = 0;
}
int startReadBlockingMode(char *content) {
	struct input_event ev[64];    /* each scan causes more than one event...get them all */
	int bytes_read = 0;           /* number of bytes read by the read function */
	int i;                        /* loop variable */
	char scanner_in[128];          /* set this bigger if scanned input is more than 30 characters */
	char scan_char;               /* this holds one char at a time...will be concatenated into scanner_in[] */
	int wr_ptr = 0;               /* points into scanner_in so we know where to put the next scan_char */
	char rightShiftPressed = 0;
	strcpy(content, "");

	do {
		bytes_read = read(fd, &ev, sizeof(struct input_event) * 64);

		if (bytes_read < 0) {
			DEBUG_PRINT("%s", "ERROR: can't properly read from device\n");
			return -1;
		}
		for (i=0; i < (int) (bytes_read / sizeof(struct input_event)); i++) {

		  /* Look for a "key press" event */
		  if ((ev[i].type == EV_KEY) && (ev[i].value == 1)) {
			if (ev[i].code == KEY_RIGHTSHIFT) {
				rightShiftPressed = 1;
			} else if (ev[i].code != KEY_LEFTSHIFT) {
				if (rightShiftPressed)
				{
					scan_char = cvt_ev_char_right_shift(ev[i].code);         /* Extract the character from the event */
					rightShiftPressed = 0;
				}
				else
					scan_char = cvt_ev_char(ev[i].code);         /* Extract the character from the event */

                debug_rcvd_event(&ev[i]);
                DEBUG_PRINT("Scan char: %c\n", scan_char);

                  if (ev[i].code != KEY_ENTER) {
					scanner_in[wr_ptr++] = scan_char;
				  }
				  else {
					scanner_in[wr_ptr] = '\0';
					strcpy(content, scanner_in);
					wr_ptr = 0;
					DEBUG_PRINT("Input from Scanner: \"%s\"\n", content);
				  }
			}
		  }
		}
	} while (strlen(content) <= 0);
	return strlen(content);
}

char cvt_ev_char_right_shift(int foo) {
	char bar;

	  switch (foo) {
	    case KEY_0: bar = ')'; break;
	    case KEY_1: bar = '!'; break;
	    case KEY_2: bar = '@'; break;
	    case KEY_3: bar = '#'; break;
	    case KEY_4: bar = '$'; break;
	    case KEY_5: bar = '%'; break;
	    case KEY_6: bar = '^'; break;
	    case KEY_7: bar = '&'; break;
	    case KEY_8: bar = '*'; break;
	    case KEY_9: bar = '('; break;

	    case KEY_A: bar = 'A'; break;
	    case KEY_B: bar = 'B'; break;
	    case KEY_C: bar = 'C'; break;
	    case KEY_D: bar = 'D'; break;
	    case KEY_E: bar = 'E'; break;
	    case KEY_F: bar = 'F'; break;
	    case KEY_G: bar = 'G'; break;
	    case KEY_H: bar = 'H'; break;
	    case KEY_I: bar = 'I'; break;
	    case KEY_J: bar = 'J'; break;
	    case KEY_K: bar = 'K'; break;
	    case KEY_L: bar = 'L'; break;
	    case KEY_M: bar = 'M'; break;
	    case KEY_N: bar = 'N'; break;
	    case KEY_O: bar = 'O'; break;
	    case KEY_P: bar = 'P'; break;
	    case KEY_Q: bar = 'Q'; break;
	    case KEY_R: bar = 'R'; break;
	    case KEY_S: bar = 'S'; break;
	    case KEY_T: bar = 'T'; break;
	    case KEY_U: bar = 'U'; break;
	    case KEY_V: bar = 'V'; break;
	    case KEY_W: bar = 'W'; break;
	    case KEY_X: bar = 'X'; break;
	    case KEY_Y: bar = 'Y'; break;
	    case KEY_Z: bar = 'Z'; break;
	    case KEY_MINUS: bar = '_'; break;
	    case KEY_DOT: bar = '>'; break;
	    case KEY_SLASH: bar = '?'; break;
	    case KEY_EQUAL: bar = '+'; break;
	    case KEY_ENTER: bar = '\n'; break;
	    default: bar = '?';
	  }

	  return bar;
}

char cvt_ev_char(int foo) {

  char bar;

  switch (foo) {
    case KEY_0: bar = '0'; break;
    case KEY_1: bar = '1'; break;
    case KEY_2: bar = '2'; break;
    case KEY_3: bar = '3'; break;
    case KEY_4: bar = '4'; break;
    case KEY_5: bar = '5'; break;
    case KEY_6: bar = '6'; break;
    case KEY_7: bar = '7'; break;
    case KEY_8: bar = '8'; break;
    case KEY_9: bar = '9'; break;

    case KEY_A: bar = 'a'; break;
    case KEY_B: bar = 'b'; break;
    case KEY_C: bar = 'c'; break;
    case KEY_D: bar = 'd'; break;
    case KEY_E: bar = 'e'; break;
    case KEY_F: bar = 'f'; break;
    case KEY_G: bar = 'g'; break;
    case KEY_H: bar = 'h'; break;
    case KEY_I: bar = 'i'; break;
    case KEY_J: bar = 'j'; break;
    case KEY_K: bar = 'k'; break;
    case KEY_L: bar = 'l'; break;
    case KEY_M: bar = 'm'; break;
    case KEY_N: bar = 'n'; break;
    case KEY_O: bar = 'o'; break;
    case KEY_P: bar = 'p'; break;
    case KEY_Q: bar = 'q'; break;
    case KEY_R: bar = 'r'; break;
    case KEY_S: bar = 's'; break;
    case KEY_T: bar = 't'; break;
    case KEY_U: bar = 'u'; break;
    case KEY_V: bar = 'v'; break;
    case KEY_W: bar = 'w'; break;
    case KEY_X: bar = 'x'; break;
    case KEY_Y: bar = 'y'; break;
    case KEY_Z: bar = 'z'; break;
    case KEY_MINUS: bar = '-'; break;
    case KEY_DOT: bar = '.'; break;
    case KEY_SLASH: bar = '/'; break;
    case KEY_SPACE: bar = ' '; break;
    case KEY_EQUAL: bar = '='; break;
    case KEY_ENTER: bar = '\n'; break;

    default: bar = '?';
  }

  return bar;

}

void debug_rcvd_event(struct input_event *ev) {

  char type_str[15];

  switch (ev->type) {
    case EV_SYN:  strcpy(type_str, "EV_SYN"); break;
    case EV_KEY:  strcpy(type_str, "EV_KEY"); break;
    case EV_REL:  strcpy(type_str, "EV_REL"); break;
    case EV_ABS:  strcpy(type_str, "EV_ABS"); break;
    case EV_MSC:  strcpy(type_str, "EV_MSC"); break;
    case EV_SW:   strcpy(type_str, "EV_SW"); break;
    case EV_LED:  strcpy(type_str, "EV_LED"); break;
    case EV_SND:  strcpy(type_str, "EV_SND"); break;
    case EV_REP:  strcpy(type_str, "EV_REP"); break;
    case EV_FF:   strcpy(type_str, "EV_FF"); break;
    case EV_PWR:  strcpy(type_str, "EV_PWR"); break;
    case EV_MAX:  strcpy(type_str, "EV_MAX"); break;

    case EV_FF_STATUS: strcpy(type_str, "EV_FF_STATUS"); break;

    default: strcpy(type_str, "UNK");

  }

  DEBUG_PRINT("Event: time %ld.%06ld, type %s, code %d, value %d\n",
	 ev->time.tv_sec, ev->time.tv_usec, type_str,
	 ev->code, ev->value);

}

