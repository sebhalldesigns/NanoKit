/***************************************************************
**
** NanoKit Tool Source File
**
** File         :  main.c
** Module       :  nkgen
** Author       :  SH
** Created      :  2025-03-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  An XML code generator inspired by XAML.
**
***************************************************************/


/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <parser/parser.h>
#include <header/header.h>
#include <source/source.h>

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

int LoadFile(const char* path, char** buffer, size_t* size);

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

int main(int argc, char *argv[]) {

    if (argc != 5) {
        fprintf(stderr, "Usage: %s <moduleName> <input.xml> <output.h> <output.c>\n", argv[0]);
        return 1;
    }

    char *moduleName = argv[1];
    char *inputFile = argv[2];
    char *outputHeader = argv[3];
    char *outputSource = argv[4];

    size_t inputFileSize;
    char *inputFileBuffer;

    if (LoadFile(inputFile, &inputFileBuffer, &inputFileSize)) {
        fprintf(stderr, "Error: Could not load input file\n");
        return 1;
    }

    printf(">>> GENERATING MODULE \"%s\"\n", moduleName);
    
    /* Parse the file */
    FileContents fileContents = ParseFile(inputFileBuffer, inputFileSize, moduleName);

    /* Write the header file */
    WriteHeaderFile(outputHeader, moduleName, &fileContents);

    /* Write the source file */
    WriteSourceFile(outputSource, moduleName, &fileContents);
    
    printf("<<< DONE\n");

    return 0;
}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/

int LoadFile(const char* path, char** buffer, size_t* size)
{
    FILE *inputFileHandle = fopen(path, "r");

    if (!inputFileHandle) {
        fprintf(stderr, "Error: Could not open input file\n");
        return 1;
    }

    fseek(inputFileHandle, 0, SEEK_END);
    size_t fileSize = ftell(inputFileHandle);
    fseek(inputFileHandle, 0, SEEK_SET);

    char* fileBuffer = (char *)malloc(fileSize + 1);

    if (!fileBuffer) {
        fprintf(stderr, "Error: Could not allocate memory for input buffer\n");
        fclose(inputFileHandle);
        return 1;
    }

    fread(fileBuffer, 1, fileSize, inputFileHandle);
    fileBuffer[fileSize] = '\0';

    fclose(inputFileHandle); 

    *buffer = fileBuffer;
    *size = fileSize;

    return 0;
}
