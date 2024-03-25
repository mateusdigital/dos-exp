#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <go32.h>
#include <dpmi.h>
#include <sys/farptr.h>
#include <sys/nearptr.h>
#include <sys/timeb.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200

void set_graphics_mode()
{
    union REGS in_regs, out_regs;
    in_regs.h.ah = 0x00; // Set video mode
    in_regs.h.al = 0x13; // VGA 320x200 256-color mode
    int86(0x10, &in_regs, &out_regs);
}

void vga256()
{
    __dpmi_regs regs;
    regs.x.ax = 0x13;
    __dpmi_int(0x10, &regs);
    return;
}

char virtscreen[SCREEN_WIDTH * SCREEN_HEIGHT];

unsigned long long get_milliseconds()
{
    struct timeb tb;
    ftime(&tb);
    return ((unsigned long long)tb.time * 1000) + tb.millitm;
}
// Desired frame rate
#define DESIRED_FRAME_RATE 30
// Desired frame time in milliseconds
#define DESIRED_FRAME_TIME (1000 / DESIRED_FRAME_RATE)

int main()
{
    // set_graphics_mode();
    vga256();

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
                virtscreen[i * SCREEN_WIDTH + j] = color;
            }
            color++;
        }

        dosmemput(virtscreen, sizeof(virtscreen), 0xA0000);
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
    return 0;
}
