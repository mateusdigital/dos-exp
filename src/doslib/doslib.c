#ifndef __DOS_LIB_C__
#define __DOS_LIB_C__

// -----------------------------------------------------------------------------
#include "doslib.h"


//
// File
//

// -----------------------------------------------------------------------------
u32 GetFileSize(FILE *fp)
{
    u32 curr = ftell(fp);
    fseek(fp, 0, SEEK_END);
    u32 end = ftell(fp);
    fseek(fp, curr, SEEK_SET);

    return end;
}

//
// IMG
//

// -----------------------------------------------------------------------------
bool LoadImg(FILE *fp, IMG *img)
{
    fread(&img->width,  sizeof(u16), 1, fp);
    fread(&img->height, sizeof(u16), 1, fp);

    //
    img->data = malloc(sizeof(u8) * img->width * img->height);
    if(!img->data) {
        return false;
    }

    fread(img->data, sizeof(u8), img->width * img->height, fp);
    return true;
}


//
// PAL
//

// -----------------------------------------------------------------------------
bool LoadPal(FILE *fp, PAL *pal)
{
    fread(&pal->count,  sizeof(u16), 1, fp);

    //
    pal->colors = malloc(sizeof(RGB) * pal->count);
    if(!pal->colors) {
        return false;
    }

    fread(pal->colors, sizeof(RGB), pal->count, fp);
    return true;
}

// -----------------------------------------------------------------------------
void SetPaletteColor(u8 index, u8 red, u8 green, u8 blue)
{
    outportb(0x3C8, index); // Set the color index register
    outportb(0x3C9, red  );
    outportb(0x3C9, green);
    outportb(0x3C9, blue );
}

// -----------------------------------------------------------------------------
void SetPaletteWithPal(PAL *pal)
{
    for(u16 i = 0; i < pal->count; ++i) {
        RGB *rgb = &pal->colors[i];
        SetPaletteColor(i, rgb->r, rgb->g, rgb->b);
    }
}

//
// Graphics
//

// -----------------------------------------------------------------------------
void SetVGA256()
{
    __dpmi_regs regs;
    regs.x.ax = 0x13;
    __dpmi_int(0x10, &regs);
    return;
}

// -----------------------------------------------------------------------------
void SetTextMode()
{
    __dpmi_regs regs;
    regs.x.ax = 0x03;
    __dpmi_int(0x10, &regs);
    return;
}

// -----------------------------------------------------------------------------
char VIRTUAL_SCREEN[SCREEN_HEIGHT * SCREEN_WIDTH];


//
// Time
//

// -----------------------------------------------------------------------------
unsigned long long get_milliseconds()
{
    struct timeb tb;
    ftime(&tb);
    return ((unsigned long long)tb.time * 1000) + tb.millitm;
}


#endif  // __DOS_LIB_C__