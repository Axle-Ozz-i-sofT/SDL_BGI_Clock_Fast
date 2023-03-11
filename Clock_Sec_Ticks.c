//------------------------------------------------------------------------------
// Name:        Clock_Sec_Ticks.c
// Purpose:     Example analog clock with per second ticks and memory image
// Title:       "Clock"
//
// Platform:    Win64, Ubuntu64
//
// Compiler:    GCC V9.x.x, MinGw-64, libc (ISO C99)
// Depends:     SDL2-devel, SDL_bgi-3.0.0,
//
// Author:      Axle
// Created:     10/03/2023
// Updated:     
// Version:     0.0.1.0 beta
// Copyright:   (c) Axle 2023
// Licence:     MIT No Attribution (MIT-0)
//------------------------------------------------------------------------------
// NOTES:
// I am uncertain about the licencing status of the DOS code base from which I
// have built this. There appears to be many copies of the same(-ish) code all
// across the internet, so I suspect it has emerged from some previous code
// source from the days of DOS and BGI graphics.h
// If you feel that I have breached copywright on your code, please let me
// know so that I can rectify or place appropriate attributions as required.
//
// Current sources used:
// http://see-programming.blogspot.com/2013/09/c-program-to-implement-analog-clock.html
// http://programmertutor16.blogspot.com/2013/10/analog-clock-in-c-simplified.html
//
// This is an updated verdion of the legacy analog BGI clock with seconds ticks.
// I have altered this version to use the fast SDL_Bgi graphics mode as well
// as clock background image in RAM.
//
//------------------------------------------------------------------------------
// Thomas Edison once said in response to a question about his missteps, "I have not failed
// 10,000 times — I've successfully found 10,000 ways that will not work".


#include <stdio.h>
#include <stdlib.h>
//#include <conio.h>
//#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <graphics.h>
//#include <SDL2/SDL_keyboard.h>


// Turn off compiler warnings for unused variables between (Windows/Linux etc.)
#define unused(x) (x) = (x)

//==============================================================================
// http://see-programming.blogspot.com/2013/09/c-program-to-implement-analog-clock.html
void minSecCalc(int radius, int midx, int midy, int secx[60], int secy[60]);
void calcPoints(int radius, int midx, int midy, int x[12], int y[12]);



//http://programmertutor16.blogspot.com/2013/10/analog-clock-in-c-simplified.html
int main(int argc, char *argv[])
    {
    unused(argc);  // Turns off the compiler warning for unused argc, argv
    unused(argv);  // Turns off the compiler warning for unused argc, argv

    // request auto detection
    //int gdriver = DETECT, gmode, err, tmp;
    //int gdriver = DETECT, gmode, err;
    
    //int gdriver = SDL;
    //gdriver = SDL_800x600;  // SDL_FULLSCREEN
    
    //int j;
    int midx, midy, radius, hr, min, sec;

    int wait = 990;

    //int x[12], y[12];
    int minx[60], miny[60];
    int hrx[12], hry[12], secx[60], secy[60];

    //char str[256];
    time_t t1;
    struct tm *data;

    // ==> Use an image as the clockface
    char *Cock_face = "BlankClock.bmp";  // purzen_Clock_face_web_blue2.bmp

    // Get background image to RAM
    // Set the SDL windows options.
    setwinoptions("Load image to RAM", // char *title
                  SDL_WINDOWPOS_CENTERED, // int x
                  SDL_WINDOWPOS_CENTERED, // int y
                  SDL_WINDOW_HIDDEN);  // -1 | SDL_WINDOW_HIDDEN

    // intiiate the graphics driver and window.
    int Win_ID_1 = initwindow(600, 600);
    readimagefile(Cock_face,0,0, getmaxx(), getmaxy());// x, y, W, H
    //refresh();
    unsigned int ImgSize = imagesize(0, 0, getmaxx(), getmaxy());
    void *BackGround = malloc(ImgSize);
    getimage (0, 0, getmaxx(), getmaxy(), BackGround);

    closewindow(Win_ID_1);
    // <== Use an image as the clockface. Comment out to use a drawn circle.

    // Set the SDL windows options.
    setwinoptions ("SDL_BGI Clock (600x600) - Any key to quit", // char *title
                   SDL_WINDOWPOS_CENTERED, // int x
                   SDL_WINDOWPOS_CENTERED, // int y
                   -1); // Uint32 flags (I cant find the docs on this atm.

    int Win_ID_2 = initwindow(600, 600);  // intiiate the graphics driver and window.(1280, 1024)
    // It defaults to fast, so I don't think this is needed.
    sdlbgifast();  // sdlbgiauto(void)

    // mid position in x and y -axis
    midx = getmaxx() / 2;
    midy = getmaxy() / 2;
    radius = 250;  // will adjust the clock size. Make sure it is less than the window size :)

    // position to locate numbers in clock
    //calcPoints(radius - 30, midx, midy, x, y);  // remove comment for draw circle.
    //calcPoints_1(radius - 30, midx, midy, x, y);

    // gets position for hour needle
    //calcPoints(radius - 90, midx, midy, hrx, hry);
    calcPoints(radius - 90, midx, midy, hrx, hry);

    // gets position for minute needle
    //minSecCalc(radius - 50, midx, midy, minx, miny);
    minSecCalc(radius - 50, midx, midy, minx, miny);

    // gets position for second needle
    minSecCalc(radius - 40, midx, midy, secx, secy);

    // kbhit() has been a problimatic function in SDL_Bgi and SDL. This comes
    // about due to differences between different std library implimentations
    // ocross different platforms, Linux vs Mac, vs Windows vs CRT ...
    // It will throw a warning message but works. I don't know how as there is
    // no k_bkit() and the SDL_Bgi documentation does not expand on this.
    // I suspect we are ment to include windows.h and use _kbhit(), rather than
    // the implimentation in conio.h, SDL_bgi.h, and SDL.h if it fails to work.
    // Note! // kbhit() is for the console window, and xkbhit() is for the
    // graphics window and depends upon which window has focus..
    while (!xkbhit() && !kbhit())
        {

    // Draw the clock face
    setlinestyle(SOLID_LINE, 1, 3);  // SOLID_LINE, 0, 3
    settextstyle(TRIPLEX_FONT, 0, 3);

    // draws frame of the clock
    //circle(midx, midy, radius);  // to draw a circle clock face.
    // I have replaced the drawn circle with a clock face image.
    // comment out the below and use circle() above for drawn clock face.
    putimage (0, 0, BackGround, COPY_PUT);  // COPY_PUT, "XOR_PUT", OR_PUT


    // place the numbers  in clock (When using the draw circle above).
    /*
    for (j = 0; j < 12; j++)
        {
        if (j == 0)
            {
            sprintf(str, "%d", 12);
            }
        else
            {
            sprintf(str, "%d", j);
            }
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        moveto(x[j], y[j]);
        outtext(str);
        }
        */

        // get the current time using time() API
        t1 = time(NULL);
        data = localtime(&t1);

        // draw the minute needle in clock
        min = data->tm_min % 60;
        setcolor (BLACK);  // BLUE purzen_Clock_face_web_blue2.bmp
        line(midx, midy, minx[min], miny[min]);
        //setcolor (WHITE);

        // draw the hour needle in clock
        hr = data->tm_hour % 12;
        setcolor (BLACK);  // BLUE purzen_Clock_face_web_blue2.bmp
        line(midx, midy, hrx[hr], hry[hr]);
        //setcolor (WHITE);

        // draw the second needle in clock
        sec = data->tm_sec % 60;  //60
        setcolor (DARKGRAY);  // LIGHTGREEN 
        line(midx, midy, secx[sec], secy[sec]);
        //setcolor (WHITE);
        //swapbuffers(); swapbuffers is the same as the 4 lines below.
        // Use swpapbuffers or getvisualpage() etc
        int oldv = getvisualpage();
        int olda = getactivepage();
        setvisualpage(olda);
        setactivepage(oldv);


        // refresh(), event(), x|kbhit()
        refresh();  // Not kbhit also preforms a refresh!

        // Sleep() vs deley(): Sleep can sometime interfere with the graphics display
        // and is non standard to graphics.h but has the advantage of keeping the CPU
        // at an idle state. delay is poratable, but will run the CPU core at close
        // to 100% while the application is running.
        // See: SDL_Delay() for a better Sleep() equivilent.
        // SDL_Delay() can interfere with SDL_BGI SDL2 interaction. Use with care.
        //1000 for sec, (167 for 360 ticks/min Using alternative minSecCalc() at 1800 or 3600
        // Hint 1000ms divide by 60 (1000 / 60) == ~ 60 ticks per second (60 FPS)
        delay(wait);
        //edelay(wait); // Reports event()


    cleardevice();
    //clearviewport();
        
        }

    //getch();

    // deallocate memory allocated for graphic screen
    closewindow(Win_ID_2);
    closegraph();

    return 0;
    }


//==============================================================================
// http://see-programming.blogspot.com/2013/09/c-program-to-implement-analog-clock.html
/*
   * Calculates position for minute and second needle movement
   * Each quadrant has 90 degrees.  So, we need to split each
   * quadrant into 15 parts(6 degree each) to get the minute
   * and second needle movement
*/
void minSecCalc(int radius, int midx, int midy, int secx[60], int secy[60])
    {
    //int i, j = 0, x, y;
    int i, j = 0;
    //char str[32];

    /* 15 position(min/sec - 12 to 3) in first quadrant of clock  */
    secx[j] = midx, secy[j++] = midy - radius;

    for (i = 96; i < 180; i = i + 6)
        {
        secx[j] = midx - (radius * cos((i * 3.14) / 180));
        secy[j++] = midy - (radius * sin((i * 3.14) / 180));
        }

    /* 15 positions(min or sec - 3 to 6) in second quadrant of clock */
    secx[j] = midx + radius, secy[j++] = midy;
    for (i = 186; i < 270; i = i + 6)
        {
        secx[j] = midx - (radius * cos((i * 3.14) / 180));
        secy[j++] = midy - (radius * sin((i * 3.14) / 180));
        }

    /* 15 positions(min or sec - 6 to 9) in third quadrant of clock */
    secx[j] = midx, secy[j++] = midy + radius;
    for (i = 276; i < 360; i = i + 6)
        {
        secx[j] = midx - (radius * cos((i * 3.14) / 180));
        secy[j++] = midy - (radius * sin((i * 3.14) / 180));
        }

    /* 15 positions(min or sec - 9 to 12) in fourth quadrant of clock */
    secx[j] = midx - radius, secy[j++] = midy;
    for (i = 6; i < 90; i = i + 6)
        {
        secx[j] = midx - (radius * cos((i * 3.14) / 180));
        secy[j++] = midy - (radius * sin((i * 3.14) / 180));
        }

    return;
    }

//==============================================================================
// http://see-programming.blogspot.com/2013/09/c-program-to-implement-analog-clock.html
/*
   * find the points at 0, 30, 60,.., 360 degrees
   * on the given circle.  x value correponds to
   * radius * cos(angle) and y value corresponds
   * to radius * sin(angle).  Numbers in the clock
   * are written using the above manipulated x and
   * y values.  And the hour needle movement
   * is based on this
*/

void calcPoints(int radius, int midx, int midy, int x[12], int y[12])
    {
    int x1, y1;

    /* 90, 270, 0, 180 degrees */
    x[0] = midx, y[0] = midy - radius;
    x[6] = midx, y[6] = midy + radius;
    x[3] = midx + radius, y[3] = midy;
    x[9] = midx - radius, y[9] = midy;

    /* 30, 150, 210, 330 degrees */
    x1 = (int) ((radius / 2) * sqrt(3));
    y1 = (radius / 2);
    x[2] = midx + x1, y[2] = midy - y1;
    x[4] = midx + x1, y[4] = midy + y1;
    x[8] = midx - x1, y[8] = midy + y1;
    x[10] = midx - x1, y[10] = midy - y1;

    /* 60, 120, 210, 300 degrees */
    x1 = radius / 2;
    y1 = (int) ((radius / 2)  * sqrt(3));
    x[1] = midx + x1, y[1] = midy - y1;
    x[5] = midx + x1, y[5] = midy + y1;
    x[7] = midx - x1, y[7] = midy + y1;
    x[11] = midx - x1, y[11] = midy - y1;

    return;
    }
//==============================================================================


