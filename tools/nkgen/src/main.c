/***************************************************************
**
** NanoKit Tool Source File
**
** File         :  main.c
** Module       :  nkgen
** Author       :  SH
** Created      :  2025-03-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  An XML code generator inspired by the MSBuild
**                 XAML system.
**
***************************************************************/


/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <xml.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef struct View
{
    const char* class;

    const char* clickCallback;

    struct View* child;
    struct View* next;
} View;
 
typedef struct Window
{
    const char* title;
    int width;
    int height;

    View* rootView;
} Window;

/***************************************************************
** MARK: STATIC VARIABLES
***************************************************************/

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

int LoadFile(const char* path, char** buffer, size_t* size);
void ProcessNode(struct xml_node* node, size_t depth);

void WriteHeaderFile(const char* path, const char* moduleName);
void WriteSourceFile(const char* path, const char* moduleName);

const char* outputBuffer = NULL;
size_t outputBufferSize = 0;

Window* window = NULL;

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

    printf("Input file: %s\n", inputFile);
    printf("Output header: %s\n", outputHeader);
    printf("Output source: %s\n", outputSource);

    size_t inputFileSize;
    char *inputFileBuffer;

    if (LoadFile(inputFile, &inputFileBuffer, &inputFileSize)) {
        fprintf(stderr, "Error: Could not load input file\n");
        return 1;
    }

    printf("loaded file ok %zu\n", inputFileSize);

    struct xml_document* document = xml_parse_document(inputFileBuffer, inputFileSize);

    if (!document) {
        fprintf(stderr, "Error: Could not parse input file\n");
        return 1;
    }

    /* allocate output buffer */
    outputBuffer = (char *)malloc(1024*1024);
    outputBufferSize = 1024*1024;
    if (!outputBuffer) {
        fprintf(stderr, "Error: Could not allocate memory for output buffer\n");
        xml_document_free(document, false);
        free(inputFileBuffer);
        return 1;
    }

    struct xml_node* root = xml_document_root(document);
    
    ProcessNode(root, 0);

    WriteHeaderFile(outputHeader, moduleName);
    WriteSourceFile(outputSource, moduleName);

    /* free memory */
    xml_document_free(document, false);
    free(inputFileBuffer);


    printf("Generated files: %s, %s\n", outputHeader, outputSource);
 
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

    printf("File size: %zu\n", fileSize);

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

void ProcessNode(struct xml_node* node, size_t depth)
{
    struct xml_string* nodeClassObject = xml_node_name(node);

    /* get class */
    const char* nodeClass = calloc(xml_string_length(nodeClassObject) + 1, 1);
    xml_string_copy(nodeClassObject, nodeClass, xml_string_length(nodeClassObject));
    printf("Processing node of class: %s\n", nodeClass);

    View* view = NULL;

    size_t attributesCount = xml_node_attributes(node);


    if (depth == 0)
    {
        if (window != NULL)
        {
            fprintf(stderr, "Error: Multiple root nodes\n");
            exit(1);
        }
        else if (strcmp(nodeClass, "Window") != 0)
        {
            fprintf(stderr, "Error: Unsupported XML root class of %s\n", nodeClass);
            exit(1);

        }
        else
        {
            window = (Window *)malloc(sizeof(Window));
            window->title = "Window";
            window->width = 800;
            window->height = 600;
            window->rootView = NULL;

            for (size_t i = 0; i < attributesCount; i++) {
                struct xml_string* attributeNameObject = xml_node_attribute_name(node, i);
                struct xml_string* attributeContentObject = xml_node_attribute_content(node, i);
        
                const char* attributeName = calloc(xml_string_length(attributeNameObject) + 1, 1);
                xml_string_copy(attributeNameObject, attributeName, xml_string_length(attributeNameObject));
        
                const char* attributeContent = calloc(xml_string_length(attributeContentObject) + 1, 1);
                xml_string_copy(attributeContentObject, attributeContent, xml_string_length(attributeContentObject));
        
                printf("Attribute: %s = %s\n", attributeName, attributeContent);
        
                if (strcmp("Title", attributeName) == 0)
                {
                    window->title = attributeContent;
                }
                else if (strcmp("Width", attributeName) == 0)
                {
                    window->width = atoi(attributeContent);
                }
                else if (strcmp("Height", attributeName) == 0)
                {
                    window->height = atoi(attributeContent);
                }
        
                free(attributeName);
                free(attributeContent);
            }

        }
    }
    else
    {
        if (window != NULL && window->rootView != NULL && depth == 1)
        {
            fprintf(stderr, "Error: Multiple root views\n");
            exit(1);
        }

        view = (View *)malloc(sizeof(View));
        view->class = nodeClass;
        view->clickCallback = NULL;
        view->child = NULL;
        view->next = NULL;

        for (size_t i = 0; i < attributesCount; i++) {
            struct xml_string* attributeNameObject = xml_node_attribute_name(node, i);
            struct xml_string* attributeContentObject = xml_node_attribute_content(node, i);
    
            const char* attributeName = calloc(xml_string_length(attributeNameObject) + 1, 1);
            xml_string_copy(attributeNameObject, attributeName, xml_string_length(attributeNameObject));
    
            const char* attributeContent = calloc(xml_string_length(attributeContentObject) + 1, 1);
            xml_string_copy(attributeContentObject, attributeContent, xml_string_length(attributeContentObject));
    
            printf("Attribute: %s = %s\n", attributeName, attributeContent);
    
            if (strcmp("Click", attributeName) == 0)
            {
                view->clickCallback = attributeContent;
            }
    
            free(attributeName);
            free(attributeContent);
        }

        if (window != NULL)
        {
            if (window->rootView == NULL)
            {
                window->rootView = view;
            }
            else
            {
                View* currentView = window->rootView;
                while (currentView->next != NULL)
                {
                    currentView = currentView->next;
                }
                currentView->next = view;
            }
        }
        
    }


    size_t childrenCount = xml_node_children(node);

    for (size_t i = 0; i < childrenCount; i++) {
        ProcessNode(xml_node_child(node, i), depth + 1);
    }

    free(nodeClass);

}

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

void WriteSourceFile(const char* path, const char* moduleName)
{
    char moduleNameUpper[256];
    for (size_t i = 0; moduleName[i] != '\0'; i++) {
        moduleNameUpper[i] = (moduleName[i] >= 'a' && moduleName[i] <= 'z') ? moduleName[i] - 32 : moduleName[i];
    }
    moduleNameUpper[strlen(moduleName)] = '\0';

    int positionInFile = snprintf(outputBuffer, outputBufferSize, 
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
