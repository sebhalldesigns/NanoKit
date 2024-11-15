// MAIN.C NANOKIT TEST
// NOVEMBER 2024, SEB HALL

#include <stdio.h>
#include <NanoGraph.h>
#include <NanoWin.h>

int main(int argc, char** argv) 
{
    nWindow_h window1 = NanoWin_CreateWindow(800, 600, "NanoKit Test - Window 1");
    nWindow_h window2 = NanoWin_CreateWindow(800, 600, "NanoKit Test - Window 2");

    while (1)
    {
        NanoWin_PollEvents();
    }

    return 0;
}