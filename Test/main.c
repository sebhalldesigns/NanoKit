// MAIN.C NANOKIT TEST
// NOVEMBER 2024, SEB HALL

#include <stdio.h>
#include <NanoGraph.h>
#include <NanoWin.h>
#include <NanoDraw.h>
#include <NanoDrawing.h>

nDrawCommand cancelCommands[5];
nDrawCommand nextCommands[5];
nDrawCommand welcomeCommands[5];
nDrawCommand welcomeCommands2[5];

const char* cancelText = "Cancel";
const char* nextText = "Next >";
const char* welcomeText = "Welcome to the NanoKit Test!";
const char* welcomeText2 = "This is a test of the NanoKit windowing and drawing";
const char* welcomeText3 = "libraries. Lorem ipsum dolor sit amet, consectetur";
const char* welcomeText4 = "adipiscing elit. Sed do eiusmod tempor incididunt";

int main(int argc, char** argv) 
{
    nWindow_h window1 = NanoWin_CreateWindow(500, 360, "NanoKit Test - Window 1");
    //nWindow_h window2 = NanoWin_CreateWindow(800, 600, "NanoKit Test - Window 2");
    nDrawingContext_h context1 = NanoDraw_GetContextForWindow(window1);

    nTypeface_h arial24 = NanoDraw_CreateTypeface(context1, "C:\\Windows\\Fonts\\arial.ttf", 24.0f);
    nTypeface_h arial16 = NanoDraw_CreateTypeface(context1, "C:\\Windows\\Fonts\\arial.ttf", 16.0f);
    nTypeface_h inter16 = NanoDraw_CreateTypeface(context1, "Resources/Fonts/Inter/Inter-Regular.ttf", 16.0f);
    nTypeface_h inter24 = NanoDraw_CreateTypeface(context1, "Resources/Fonts/Inter/Inter-Regular.ttf", 24.0f);


    nGraphNode_h rootNode = NanoGraph_CreateRootNode();
    rootNode->name = "rootNode";
    rootNode->parentLayout = LAYOUT_DOCK;
    rootNode->backgroundColor = COLOR_WHITE;

    nGraphNode_h bottomBar = NanoGraph_InsertNode(rootNode);
    bottomBar->name = "bottomBar";
    bottomBar->childDockPosition = DOCK_BOTTOM;
    bottomBar->userRect.height = 50;
    bottomBar->backgroundColor = COLOR_LIGHT_GRAY;
    bottomBar->parentLayout = LAYOUT_DOCK;

        // buttons 
        nTextSize cancelSize = NanoDraw_GetTextSize(context1, inter16, cancelText);
        nTextSize nextSize = NanoDraw_GetTextSize(context1, inter16, nextText);

        nGraphNode_h cancelButton = NanoGraph_InsertNode(bottomBar);
        cancelButton->name = "button";
        cancelButton->childDockPosition = DOCK_RIGHT; 
        cancelButton->childHorizontalAlignment = HORIZONTAL_ALIGNMENT_CENTER;
        cancelButton->childVerticalAlignment = VERTICAL_ALIGNMENT_CENTER;
        cancelButton->userRect.width = cancelSize.width + 10;
        cancelButton->userRect.height = cancelSize.height + 10;
        cancelButton->backgroundColor = COLOR_WHITE;

        cancelCommands[0].operation = SET_FILL_COLOR;
        cancelCommands[0].parameters.colorData.color.r = 0.0f;
        cancelCommands[0].parameters.colorData.color.g = 0.0f;
        cancelCommands[0].parameters.colorData.color.b = 0.0f;
        cancelCommands[0].parameters.colorData.color.a = 1.0f;

        cancelCommands[1].operation = SET_TYPEFACE;
        cancelCommands[1].parameters.typefaceData.typeface = inter16;

        cancelCommands[2].operation = DRAW_TEXT;
        cancelCommands[2].parameters.drawTextData.x = 5;
        cancelCommands[2].parameters.drawTextData.y = 5;
        cancelCommands[2].parameters.drawTextData.text = cancelText;

        cancelButton->drawing.commands = cancelCommands;
        cancelButton->drawing.commandCount = 3;
        
        nGraphNode_h nextButton = NanoGraph_InsertNode(bottomBar);
        nextButton->name = "nextButton";
        nextButton->childDockPosition = DOCK_RIGHT; 
        nextButton->childHorizontalAlignment = HORIZONTAL_ALIGNMENT_CENTER;
        nextButton->childVerticalAlignment = VERTICAL_ALIGNMENT_CENTER;
        nextButton->userRect.width = cancelSize.width + 10;
        nextButton->userRect.height = cancelSize.height + 10;
        nextButton->backgroundColor = COLOR_WHITE;

        nextCommands[0].operation = SET_FILL_COLOR;
        nextCommands[0].parameters.colorData.color.r = 0.0f;
        nextCommands[0].parameters.colorData.color.g = 0.0f;
        nextCommands[0].parameters.colorData.color.b = 0.0f;
        nextCommands[0].parameters.colorData.color.a = 1.0f;

        nextCommands[1].operation = SET_TYPEFACE;
        nextCommands[1].parameters.typefaceData.typeface = inter16;

        nextCommands[2].operation = DRAW_TEXT;
        nextCommands[2].parameters.drawTextData.x = 5;
        nextCommands[2].parameters.drawTextData.y = 5;
        nextCommands[2].parameters.drawTextData.text = nextText;

        nextButton->drawing.commands = nextCommands;
        nextButton->drawing.commandCount = 3;

        nGraphNode_h bottomFill = NanoGraph_InsertNode(bottomBar);
        bottomFill->name = "bottomFill";


    nGraphNode_h topBar = NanoGraph_InsertNode(rootNode);
    topBar->name = "topBar";
    topBar->childDockPosition = DOCK_LEFT;
    topBar->userRect.width = 150;
    topBar->backgroundColor = COLOR_DARK_BLUE;

    nGraphNode_h mainContent = NanoGraph_InsertNode(rootNode);
    mainContent->name = "mainContent";
    mainContent->backgroundColor = COLOR_WHITE;
    mainContent->parentLayout = LAYOUT_DOCK;
    
    nGraphNode_h labelStack = NanoGraph_InsertNode(mainContent);
    labelStack->name = "labelStack";
    labelStack->parentLayout = LAYOUT_STACK;
    labelStack->parentStackOrientation = STACK_VERTICAL;

        nTextSize welcomeSize = NanoDraw_GetTextSize(context1, inter24, welcomeText);
        nTextSize welcomeSize2 = NanoDraw_GetTextSize(context1, inter16, welcomeText2);

        nGraphNode_h welcomeLabel = NanoGraph_InsertNode(labelStack);
        welcomeLabel->name = "welcomeLabel";
        //welcomeLabel->backgroundColor = COLOR_YELLOW;
        welcomeLabel->userRect.height = welcomeSize.height + 10 + 15;

        welcomeCommands[0].operation = SET_FILL_COLOR;
        welcomeCommands[0].parameters.colorData.color.r = 0.0f;
        welcomeCommands[0].parameters.colorData.color.g = 0.0f;
        welcomeCommands[0].parameters.colorData.color.b = 0.0f;
        welcomeCommands[0].parameters.colorData.color.a = 1.0f;

        welcomeCommands[1].operation = SET_TYPEFACE;
        welcomeCommands[1].parameters.typefaceData.typeface = inter24;

        welcomeCommands[2].operation = DRAW_TEXT;
        welcomeCommands[2].parameters.drawTextData.x = 5;
        welcomeCommands[2].parameters.drawTextData.y = 10;
        welcomeCommands[2].parameters.drawTextData.text = welcomeText;

        welcomeLabel->drawing.commands = welcomeCommands;
        welcomeLabel->drawing.commandCount = 3;

        nGraphNode_h welcomeLabel2 = NanoGraph_InsertNode(labelStack);
        welcomeLabel2->name = "welcomeLabel2";
       // welcomeLabel2->backgroundColor = COLOR_ORANGE;
        welcomeLabel2->userRect.height = 10 + ((welcomeSize2.height + 5)*3);

        welcomeCommands2[0].operation = SET_FILL_COLOR;
        welcomeCommands2[0].parameters.colorData.color.r = 0.0f;
        welcomeCommands2[0].parameters.colorData.color.g = 0.0f;
        welcomeCommands2[0].parameters.colorData.color.b = 0.0f;
        welcomeCommands2[0].parameters.colorData.color.a = 1.0f;

        welcomeCommands2[1].operation = SET_TYPEFACE;
        welcomeCommands2[1].parameters.typefaceData.typeface = inter16;

        welcomeCommands2[2].operation = DRAW_TEXT;
        welcomeCommands2[2].parameters.drawTextData.x = 5;
        welcomeCommands2[2].parameters.drawTextData.y = 5;
        welcomeCommands2[2].parameters.drawTextData.text = welcomeText2;

        welcomeCommands2[3].operation = DRAW_TEXT;
        welcomeCommands2[3].parameters.drawTextData.x = 5;
        welcomeCommands2[3].parameters.drawTextData.y = welcomeSize2.height + 10;
        welcomeCommands2[3].parameters.drawTextData.text = welcomeText3;

        welcomeCommands2[4].operation = DRAW_TEXT;
        welcomeCommands2[4].parameters.drawTextData.x = 5;
        welcomeCommands2[4].parameters.drawTextData.y = (welcomeSize2.height*2) + 15;
        welcomeCommands2[4].parameters.drawTextData.text = welcomeText4;
        

        welcomeLabel2->drawing.commands = welcomeCommands2;
        welcomeLabel2->drawing.commandCount = 5;


    
    NanoWin_SetRootNode(window1, rootNode);

    NanoWin_ShowWindow(window1);

    while (1)
    {
        NanoWin_PollEvents();

        _sleep(0);
    }

    return 0;
}