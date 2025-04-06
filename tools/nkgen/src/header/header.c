/***************************************************************
**
** NanoKit Tool Source File
**
** File         :  header.c
** Module       :  nkgen
** Author       :  SH
** Created      :  2025-03-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  nkgen header file writer
**
***************************************************************/


/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <xml/xml.h>

#include "header.h"

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

/***************************************************************
** MARK: STATIC VARIABLES
***************************************************************/

static char* outputBuffer = NULL;
static size_t outputBufferSize = 0;
static size_t positionInFile = 0;

static char moduleNameBuffer[256];
static char moduleNameUpper[256];

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

static void ProcessWindow(Window* window);
static void ProcessRootView(RootView* rootView);
static void ProcessView(View* view);

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

void WriteHeaderFile(const char* path, const char* moduleName, FileContents* fileContents)
{

    if (outputBuffer != NULL) 
    {
        free(outputBuffer);
    }

    outputBufferSize = 1024 * 1024; // 1 MB
    outputBuffer = (char*)malloc(outputBufferSize);
    if (outputBuffer == NULL) {
        fprintf(stderr, "Error: Could not allocate memory for output buffer\n");
        exit(1);
    }

    sprintf(moduleNameBuffer, "%s", moduleName);
    
    for (size_t i = 0; moduleName[i] != '\0'; i++) {
        moduleNameUpper[i] = (moduleName[i] >= 'a' && moduleName[i] <= 'z') ? moduleName[i] - 32 : moduleName[i];
    }
    moduleNameUpper[strlen(moduleName)] = '\0';

    const char* moduleType;

    if (fileContents->rootNodeType == ROOT_NODE_WINDOW)
    {
        moduleType = "nkWindow";
    }
    else
    {
        moduleType = "nkView";
    }

    positionInFile = snprintf(outputBuffer, outputBufferSize, 
"/***************************************************************\n\
**\n\
** NanoKit Generated Header File\n\
**\n\
** File         :  %s\n\
** Module       :  %s\n\
**\n\
***************************************************************/\n\
\n\
#ifndef %s_XML_H\n\
#define %s_XML_H\n\
\n\
#include <NanoKit.h>\n\
\n\
/* Module Functions - Implementations Generated from XML */\n\
%s* %s_Create(void);\n\
void %s_Destroy(%s*);\n\
\n\
/* Callback Functions - Implemented in User Code */\n\
",
        path,
        moduleName,
        moduleNameUpper,
        moduleNameUpper,
        moduleType,
        moduleName,
        moduleName,
        moduleType
        );


    /* PRINT OUT CALLBACK FUNCTIONS */

    if (fileContents->rootNodeType == ROOT_NODE_WINDOW)
    {

        ProcessWindow((Window*)fileContents->rootNode);
    }
    else
    {
        ProcessRootView((RootView*)fileContents->rootNode);
    }

    positionInFile += snprintf(outputBuffer + positionInFile, outputBufferSize - positionInFile,
        "\n\
#endif /*%s_XML_H*/\n",
        moduleNameUpper
    );

    FILE *headerFile = fopen(path, "w");
    
    if (!headerFile) {
        fprintf(stderr, "Error: Could not open header file for writing\n");
        return;
    }

    fprintf(headerFile, "%s", outputBuffer);
    fclose(headerFile);

    printf("    - Wrote header file: %s\n", path);
}


/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/


static void ProcessWindow(Window* window)
{
    if (window->Content != NULL)
    {
        ProcessView(window->Content);
    }
}

static void ProcessRootView(RootView* rootView)
{
    if (rootView->Content != NULL)
    {
        ProcessView(rootView->Content);
    }
}

static void ProcessView(View* view)
{
    if (view->ClickFunction != NULL)
    {
        positionInFile += snprintf(outputBuffer + positionInFile, outputBufferSize - positionInFile,
"#ifdef %s_BUILD\n\
    void %s(void);\n\
#endif\n\
\n",
            moduleNameUpper,
            view->ClickFunction
        );
    }

    if (view->Content != NULL)
    {
        ProcessView(view->Content);
    }

    if (view->Next != NULL)
    {
        ProcessView(view->Next);
    }
    
}