//~---------------------------------------------------------------------------//
//                               *       +                                    //
//                         '                  |                               //
//                     ()    .-.,="``"=.    - o -                             //
//                           '=/_       \     |                               //
//                        *   |  '=._    |                                    //
//                             \     `=./`,        '                          //
//                          .   '=.__.=' `='      *                           //
//                 +                         +                                //
//                      O      *        '       .                             //
//                                                                            //
//  File      : main.c                                                        //
//  Project   : palview                                                       //
//  Date      : 2024-03-26                                                    //
//  License   : See project's COPYING.TXT for full info.                      //
//  Author    : mateus.digital <hello@mateus.digital>                         //
//  Copyright : mateus.digital - 2024                                         //
//                                                                            //
//  Description :                                                             //
//                                                                            //
//---------------------------------------------------------------------------~//

// -----------------------------------------------------------------------------
#include "doslib/doslib.h"


// -----------------------------------------------------------------------------
#define PROGRAM_NAME            "palview"
#define PROGRAM_VERSION         "1.0.0"
#define PROGRAM_AUTHOR          "mateus.digital"
#define PROGRAM_EMAIL           "hello@mateus.digital"
#define PROGRAM_COPYRIGHT_YEARS "2024"
#define PROGRAM_WEBSITE         "https://mateus.digital"


// -----------------------------------------------------------------------------
void help()
{
    printf("Usage:\n");
    printf("   %s <input-pal>\n", PROGRAM_NAME);
}

// -----------------------------------------------------------------------------
void version()
{
    printf("%s - %s - %s <%s>\n", PROGRAM_NAME, PROGRAM_VERSION, PROGRAM_AUTHOR, PROGRAM_EMAIL);
    printf("Copyright (c) %s - %s\n", PROGRAM_COPYRIGHT_YEARS, PROGRAM_AUTHOR);
    printf("This is a free software (GPLv3) - Share/Hack it\n");
    printf("Check %s for more :)\n", PROGRAM_WEBSITE);
}


// -----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    // Command line
    if(argc > 1 && strcmp(argv[1], "--version") == 0) {
        version();
        exit(0);
    }

    // Load the palette
    PAL pal;
    bool pal_loaded = false;

    if(argc > 1) {
        char *pal_filename = argv[1];
        FILE *pal_file = fopen(pal_filename, "rb");
        if(!pal_file) {
            printf("Failed to open pal file: %s\n", pal_filename);
            exit(1);
        }

        if(!LoadPal(pal_file, &pal)) {
            printf("Failed to load pal\n");
            exit(1);
        }

        fclose(pal_file);
        pal_loaded = true;
    }


    SetVGA256();

    // Set the palette.
    if(pal_loaded) {
        SetPaletteWithPal(&pal);
    }

    // Visualize the palette.
    u8 box_height   = 8;
    u8 box_width    = 8;
    u8 y_offset     = 0;
    u8 x_offset     = 0;
    u8 colors_count = (16 * 16) - 1;

    for(u8 color = 0; color < colors_count; ++color) {
        x_offset = (color % 16) * 8;
        if(color != 0 && color % 16 == 0) {
            y_offset += 8;
        }

        for (u16 y = 0; y < box_height; ++y) {
            for (u16 x = 0; x < box_width; ++x) {
                VirtualScreenXY(x + x_offset, y + y_offset) = color;
            }
        }
    }

    BlitVirtualScreen();

    //
    getch();
    SetTextMode();
    exit(1);

    return 0;
}
