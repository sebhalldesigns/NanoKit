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

#include <xml.h>

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

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

void WriteHeaderFile(const char* path, const char* moduleName)
{
    char moduleNameUpper[256];
    for (size_t i = 0; moduleName[i] != '\0'; i++) {
        moduleNameUpper[i] = (moduleName[i] >= 'a' && moduleName[i] <= 'z') ? moduleName[i] - 32 : moduleName[i];
    }
    moduleNameUpper[strlen(moduleName)] = '\0';

    int positionInFile = snprintf(outputBuffer, outputBufferSize, 
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
/* Module Functions - Implementations Generated from XML */\n\
void %s_Init(void);\n\
void %s_Destroy(void);\n\
\n\
/* Callback Functions - Implemented in User Code */\n\
",
        path,
        moduleName,
        moduleNameUpper,
        moduleNameUpper,
        moduleNameUpper,
        moduleNameUpper
        );

    for (View* currentView = window->rootView; currentView != NULL; currentView = currentView->next)
    {
        if (currentView->clickCallback != NULL)
        {
            positionInFile += snprintf(outputBuffer + positionInFile, outputBufferSize - positionInFile,
"#ifdef %s_BUILD\n\
    void %s(void);\n\
#endif\n\
\n",
                moduleNameUpper,
                currentView->clickCallback
            );
        }
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

    printf("Wrote header file: %s\n", path);
}


/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/
