
#ifndef __KEYCODEMAP_H__
#define __KEYCODEMAP_H__

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include "config.h"

#define KEY_SW1_SCANCODE	0xfffd
#define KEY_SW2_SCANCODE	0xffdf
#define KEY_SW3_SCANCODE	0xfdff
#define KEY_SW4_SCANCODE	0xfffb
#define KEY_SW5_SCANCODE	0xffbf
#define KEY_SW6_SCANCODE	0xfbff
#define KEY_SW7_SCANCODE	0xfff7



enum{
	KEY_UP_VALUE=0x00,
	KEY_DOWN_VALUE,
	KEY_PRESS_VALUE
};
struct KeyMap{
	UINT16 iKeyScanCode;
	UINT16 iKeyCode;
};

typedef struct KeyMap KeyMap_s;
#endif




