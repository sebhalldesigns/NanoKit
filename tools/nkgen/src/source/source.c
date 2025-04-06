/***************************************************************
**
** NanoKit Tool Source File
**
** File         :  source.c
** Module       :  nkgen
** Author       :  SH
** Created      :  2025-03-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  nkgen source file writer
**
***************************************************************/


/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <xml/xml.h>

#include "source.h"

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

//static void ProcessWindow(Window* window);
//static void ProcessRootView(RootView* rootView);
//static void ProcessView(View* view);

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

void WriteSourceFile(const char* path, const char* moduleName)
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

    positionInFile = snprintf(outputBuffer, outputBufferSize, 
"/***************************************************************\n\
**\n\
** NanoKit Generated Source File\n\
**\n\
** File         :  %s\n\
** Module       :  %s\n\
**\n\
***************************************************************/\n\
\n\
",
        path,
        moduleName
        );

    positionInFile += snprintf(outputBuffer + positionInFile, outputBufferSize - positionInFile,
        "#include <stdio.h>\n\
\n");

    positionInFile += snprintf(outputBuffer + positionInFile, outputBufferSize - positionInFile,
        "void %s_Init(void)\n\
{\n\
    printf(\"%s_Init\\n\");\n\
}\n\
\n\
void %s_Destroy(void)\n\
{\n\
    printf(\"%s_Destroy\\n\");\n\
}\n\
\n",
        moduleNameUpper,
        moduleNameUpper,
        moduleNameUpper,
        moduleNameUpper
    );

    FILE *sourceFile = fopen(path, "w");
    
    if (!sourceFile) {
        fprintf(stderr, "Error: Could not open source file for writing\n");
        return;
    }

    fprintf(sourceFile, "%s", outputBuffer);
    fclose(sourceFile);

    printf("Wrote source file: %s\n", path);
}   


/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/

