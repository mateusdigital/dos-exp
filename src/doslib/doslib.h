#ifndef __DOS_LIB_H__
#define __DOS_LIB_H__

// -----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <dos.h>
#include <conio.h>

#include <go32.h>
#include <dpmi.h>
#include <sys/farptr.h>
#include <sys/nearptr.h>
#include <sys/timeb.h>

//
// Types
//

// -----------------------------------------------------------------------------
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef u8 bool;

#define true (!(0))
#define false (0)


//
// File
//

// -----------------------------------------------------------------------------
u32 GetFileSize(FILE *fp);

//
// IMG
//

// -----------------------------------------------------------------------------
typedef struct tagIMG {
    u16 width;
    u16 height;
    u8 *data;
} IMG;

// -----------------------------------------------------------------------------
bool LoadImg(FILE *fp, IMG *img);


//
// Graphics
//

// -----------------------------------------------------------------------------
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 200

// -----------------------------------------------------------------------------
void SetVGA256  ();
void SetTextMode();

// -----------------------------------------------------------------------------
extern char VIRTUAL_SCREEN[SCREEN_HEIGHT * SCREEN_WIDTH];
#define BlitVirtualScreen() dosmemput(VIRTUAL_SCREEN, sizeof(VIRTUAL_SCREEN), 0xA0000)


//
// Time
//

// -----------------------------------------------------------------------------
unsigned long long get_milliseconds();

#define DESIRED_FRAME_RATE 30
#define DESIRED_FRAME_TIME (1000 / DESIRED_FRAME_RATE)


#endif  // __DOS_LIB_H__