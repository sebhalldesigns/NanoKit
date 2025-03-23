/***************************************************************
**
** NanoKit Tool Source File
**
** File         :  parser.c
** Module       :  nkgen
** Author       :  SH
** Created      :  2025-03-23 (YYYY-MM-DD)
** License      :  MIT
** Description  :  nkgen XML Parser
**
***************************************************************/


/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include <xml.h>

#include "parser.h"

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

FileContents ParseFile(char* contents, size_t size, const char* moduleName)
{
    struct xml_document* document = xml_parse_document(contents, size);

    if (!document) {
        fprintf(stderr, "Error: Could not parse input file\n");
        exit(1);
    }

    FileContents fileContents = {0};

    return ;
}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/


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

