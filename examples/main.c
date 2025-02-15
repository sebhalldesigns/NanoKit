#include <stdio.h>

#include <pal/api/thread/thread.h>
#include <pal/api/event/event.h>
#include <pal/api/window/window.h>

#include <extern/nanovg/src/nanovg.h>

WindowHandle window1;
int segoe;

void Callback(WindowHandle window, Event event)
{
    switch (event.type)
    {
        case EVENT_WINDOW_RESIZE:
        {
            printf("WINDOW RESIZE: %zu x %zu\n", event.windowResize.width, event.windowResize.height);
        } break;

        case EVENT_MOUSE_MOVE:
        {
            printf("MOUSE MOVE: %d, %d\n", event.mouseMove.x, event.mouseMove.y);

            if (window == window1)
            {
                BeginRender(window);

                NVGcontext *nvg = GetNanoVGContext(window);
                if (nvg)
                {
                    Size size = GetWindowSize(window);
                    nvgBeginFrame(nvg, size.width, size.height, 1.0f);
                    nvgBeginPath(nvg);
                    nvgRect(nvg, 0, 0, size.width, size.height);
                    nvgFillColor(nvg, nvgRGBA(255, 255, 255, 255));
                    nvgFill(nvg);

                    int width = size.width;
                    int height = size.height;

                    nvgFontSize(nvg, 12.0f);
                    nvgFontFace(nvg, "segoe");
                    nvgTextAlign(nvg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                    nvgFillColor(nvg, nvgRGBA(0, 0, 0, 255));
                    nvgFontBlur(nvg, 0.0f);  // Ensure zero blur

                    const char *text = "AAAAAAAA";

                    for (int x = 0; x < width; x += 100)
                    {
                        for (int y = 0; y < height; y += 10)
                        {
                            nvgText(nvg, x, y, text, NULL);
                        }
                    }

                    nvgBeginPath(nvg);
                    nvgRect(nvg, event.mouseMove.x - 5, event.mouseMove.y - 5, 10, 10);
                    nvgFillColor(nvg, nvgRGBA(255, 200, 200, 255));
                    nvgFill(nvg);

                    nvgEndFrame(nvg);
                }
                
                EndRender(window);
            }
        } break;

        case EVENT_WINDOW_REDRAW:
        {
            printf("WINDOW REDRAW\n");

            if (window == window1)
            {
                NVGcontext *nvg = GetNanoVGContext(window);
                if (nvg)
                {
                    Size size = GetWindowSize(window);
                    nvgBeginFrame(nvg, size.width, size.height, 1.0f);
                    nvgBeginPath(nvg);
                    nvgRect(nvg, 0, 0, size.width, size.height);
                    nvgFillColor(nvg, nvgRGBA(255, 255, 255, 255));
                    nvgFill(nvg);

                    int width = size.width;
                    int height = size.height;

                    nvgFontSize(nvg, 12.0f);
                    nvgFontFace(nvg, "segoe");
                    nvgTextAlign(nvg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                    nvgFillColor(nvg, nvgRGBA(0, 0, 0, 255));
                    nvgFontBlur(nvg, 0.0f);  // Ensure zero blur

                    const char *text = "AAAAAAAA";

                    for (int x = 0; x < width; x += 100)
                    {
                        for (int y = 0; y < height; y += 10)
                        {
                            nvgText(nvg, x, y, text, NULL);
                        }
                    }

                    nvgEndFrame(nvg);
                }
                
            }
            
        
        } break;
    }

}

int main()
{   
    printf("FOUND %d processors\n", GetNumberOfProcessors());

    window1 = InitWindow("Hello, World اَلْعَرَبِيَّةُ 汉语 😊😊!", 800, 600);
    NVGcontext *nvg = GetNanoVGContext(window1);

    segoe = nvgCreateFont(nvg, "segoe", "C:/Windows/Fonts/segoeui.ttf");



    WindowHandle window2 = InitWindow("Hello, World 2️⃣!", 800, 600);

    //FreeWindow(window);

    return RunLoop(Callback);
}