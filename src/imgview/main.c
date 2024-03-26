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
//  Project   : imgview                                                       //
//  Date      : 2024-03-25                                                    //
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
#define PROGRAM_NAME            "imgview"
#define PROGRAM_VERSION         "1.0.0"
#define PROGRAM_AUTHOR          "mateus.digital"
#define PROGRAM_EMAIL           "hello@mateus.digital"
#define PROGRAM_COPYRIGHT_YEARS "2024"
#define PROGRAM_WEBSITE         "https://mateus.digital"


// -----------------------------------------------------------------------------
void help()
{
    printf("Usage:\n");
    printf("   %s <input-file>\n", PROGRAM_NAME);
}

// -----------------------------------------------------------------------------
void version()
{
    printf("%s - %s - %s <%s>\n", PROGRAM_NAME, PROGRAM_VERSION, PROGRAM_AUTHOR, PROGRAM_EMAIL);
    printf("Copyright (c) %s - %s\n", PROGRAM_COPYRIGHT_YEARS, PROGRAM_AUTHOR);
    printf("This is a free software (GPLv3) - Share/Hack it\n");
    printf("Check %s for more :)\n", PROGRAM_WEBSITE);
}

void setPalette(int index, int red, int green, int blue) {
    outportb(0x3C8, index);         // Set the color index register
    outportb(0x3C9, red & 0xFF);    // Set the red component
    outportb(0x3C9, green & 0xFF);  // Set the green component
    outportb(0x3C9, blue & 0xFF);   // Set the blue component
}


// -----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    //
    if(argc != 2) {
        help();
        exit(1);
    }
    if(strcmp(argv[1], "--version") == 0) {
        version();
        exit(0);
    }

    //
    char *filename = argv[1];
    FILE *file = fopen(filename, "rb");
    if(!file) {
        printf("Failed to open file: %s\n", filename);
        exit(1);
    }

    IMG img;
    if(!LoadImg(file, &img)) {
        printf("Failed to load img");
        exit(1);
    }

    //
    SetVGA256();
    setPalette(0x0a, 255, 255, 255);
    setPalette(0x08, 255, 0, 255);
    
    for (u16 y = 0; y < img.height; ++y) {
        for (u16 x = 0; x < img.width; ++x) {
            VIRTUAL_SCREEN[y * SCREEN_WIDTH + x] = img.data[y * img.width + x];
        }
    }

    BlitVirtualScreen();

    //
    getch();
    SetTextMode();
    exit(1);

    unsigned long long start_time, end_time;
    unsigned long long frame_time;
    char key;
    while (1)
    {
        start_time = get_milliseconds();
        int color = 0;
        for (int i = 0; i < SCREEN_HEIGHT; ++i)
        {
            for (int j = 0; j < SCREEN_WIDTH; ++j)
            {
                VIRTUAL_SCREEN[i * SCREEN_WIDTH + j] = color;
            }
            color++;
        }

        BlitVirtualScreen();
        end_time = get_milliseconds();

         if (kbhit()) {
            key = getch();
            if (key == 27) { // 27 is the ASCII code for the "Esc" key
                break; // Exit the loop if "Esc" key is pressed
            }
        }

        // Calculate the time taken to render this frame
        frame_time = end_time - start_time;

        // If frame was rendered too quickly, delay to maintain desired frame rate
        unsigned long long delay_time = DESIRED_FRAME_TIME - frame_time;

        if (frame_time < DESIRED_FRAME_TIME)
        {

            printf("%ld\n", delay_time);
            delay(delay_time);
        }
    }

    SetTextMode();
    return 0;
}
