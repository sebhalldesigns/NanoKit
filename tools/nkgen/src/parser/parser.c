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

#include <xml/xml.h>

#include "parser.h"

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

struct xml_string {
	uint8_t const* buffer;
	size_t length;
};

/***************************************************************
** MARK: STATIC VARIABLES
***************************************************************/

static Window *window = NULL;
static RootView *rootView = NULL;

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

static void ProcessNode(struct xml_node* node, size_t depth, ParentType parentType, void* parent);


/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

FileContents ParseFile(char* contents, size_t size, const char* moduleName)
{
    window = NULL;
    rootView = NULL;

    struct xml_document* document = xml_parse_document(contents, size);

    if (!document) {
        fprintf(stderr, "Error: Could not parse input file\n");
        exit(1);
    }

    ProcessNode(xml_document_root(document), 0, PARENT_TYPE_NONE, NULL);

    FileContents fileContents = {0};

    if (window != NULL && rootView == NULL)
    {
        fileContents.rootNodeType = ROOT_NODE_WINDOW;
        fileContents.rootNode = (void*)window;
    }
    else if (window == NULL && rootView != NULL)
    {
        fileContents.rootNodeType = ROOT_NODE_VIEW;
        fileContents.rootNode = (void*)rootView;
    }
    else
    {
        fprintf(stderr, "Error: No valid XML structure found\n");
        exit(1);
    }

    return fileContents;  
}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/


static void ProcessNode(struct xml_node* node, size_t depth, ParentType parentType, void* parent)
{
    struct xml_string* nodeClassObject = xml_node_name(node);

    /* get class */
    const char* nodeClass = calloc(xml_string_length(nodeClassObject) + 1, 1);
    xml_string_copy(nodeClassObject, nodeClass, xml_string_length(nodeClassObject));
    //printf("Processing node of class: %s\n", nodeClass);

    
    View* view = NULL;
    ParentType viewParentType = parentType;
    void* viewParent = parent;

    size_t attributesCount = xml_node_attributes(node);

    if (depth == 0)
    {
        if (window != NULL || rootView != NULL)
        {
            fprintf(stderr, "Error: Multiple root nodes\n");
            exit(1);
        }
        else if (strcmp(nodeClass, "Window") == 0)
        {
            window = (Window *)malloc(sizeof(Window));
            window->Class = nodeClass;
            window->Title = "Window";
            window->Width = 800;
            window->Height = 600;
            window->Content = NULL;
            viewParentType = PARENT_TYPE_WINDOW;
            viewParent = window;

            for (size_t i = 0; i < attributesCount; i++) {
                struct xml_string* attributeNameObject = xml_node_attribute_name(node, i);
                struct xml_string* attributeContentObject = xml_node_attribute_content(node, i);
        
                const char* attributeName = calloc(xml_string_length(attributeNameObject) + 1, 1);
                xml_string_copy(attributeNameObject, attributeName, xml_string_length(attributeNameObject));
        
                /* code to handle items with spaces in between */
                const char* attributeContentObjectString = attributeContentObject->buffer;
                
                size_t attributeContentLength = 0;

                while (attributeContentObjectString[attributeContentLength] != '\0' && attributeContentObjectString[attributeContentLength] != '\"')
                {
                    attributeContentLength++;
                }

                const char* attributeContent = calloc(attributeContentLength + 1, 1);
                sprintf(attributeContent, "%.*s", (int)attributeContentLength, attributeContentObjectString);
        
                //printf("Attribute: %s = %s\n", attributeName, attributeContent);
        
                if (strcmp("Title", attributeName) == 0)
                {
                    window->Title = calloc(attributeContentLength + 1, 1);
                    sprintf(window->Title, "%.*s", (int)attributeContentLength, attributeContentObjectString);

                }
                else if (strcmp("Width", attributeName) == 0)
                {
                    window->Width = atoi(attributeContent);
                }
                else if (strcmp("Height", attributeName) == 0)
                {
                    window->Height = atoi(attributeContent);
                }
        
                free(attributeName);
                free(attributeContent);
            }
        }
        else if (strcmp(nodeClass, "View") == 0)
        {
            rootView = (RootView *)malloc(sizeof(RootView));
            rootView->Class = nodeClass;
            rootView->Content = NULL;
            viewParentType = PARENT_TYPE_ROOT_VIEW;
            viewParent = rootView;

            for (size_t i = 0; i < attributesCount; i++) {
                struct xml_string* attributeNameObject = xml_node_attribute_name(node, i);
                struct xml_string* attributeContentObject = xml_node_attribute_content(node, i);
        
                const char* attributeName = calloc(xml_string_length(attributeNameObject) + 1, 1);
                xml_string_copy(attributeNameObject, attributeName, xml_string_length(attributeNameObject));
        
                const char* attributeContent = calloc(xml_string_length(attributeContentObject) + 1, 1);
                xml_string_copy(attributeContentObject, attributeContent, xml_string_length(attributeContentObject));
        
                //printf("Attribute: %s = %s\n", attributeName, attributeContent);
                
                free(attributeName);
                free(attributeContent);
            }
        }
        else
        {
            fprintf(stderr, "Error: Unsupported XML root class of %s\n", nodeClass);
            exit(1);
        }
    }
    else
    {
        if (window != NULL && window->Content != NULL && depth == 1)
        {
            fprintf(stderr, "Error: Multiple root views\n");
            exit(1);
        }

        view = (View *)malloc(sizeof(View));
        view->Class = nodeClass;
        view->ClickFunction = NULL;
        view->Content = NULL;
        view->Next = NULL;
        view->ParentType = viewParentType;
        view->Parent = viewParent;

        viewParentType = PARENT_TYPE_VIEW;
        viewParent = view;

        for (size_t i = 0; i < attributesCount; i++) {
            struct xml_string* attributeNameObject = xml_node_attribute_name(node, i);
            struct xml_string* attributeContentObject = xml_node_attribute_content(node, i);
    
            const char* attributeName = calloc(xml_string_length(attributeNameObject) + 1, 1);
            xml_string_copy(attributeNameObject, attributeName, xml_string_length(attributeNameObject));
    
            const char* attributeContent = calloc(xml_string_length(attributeContentObject) + 1, 1);
            xml_string_copy(attributeContentObject, attributeContent, xml_string_length(attributeContentObject));
    
           // printf("Attribute: %s = %s\n", attributeName, attributeContent);
    
            if (strcmp("Click", attributeName) == 0)
            {
                
                view->ClickFunction = calloc(xml_string_length(attributeContentObject) + 1, 1);
                xml_string_copy(attributeContentObject, view->ClickFunction, xml_string_length(attributeContentObject));
            }
    
            free(attributeName);
            free(attributeContent);
        }

        if (window != NULL)
        {
            if (window->Content == NULL)
            {
                window->Content = view;
            }
            else
            {
                View* currentView = window->Content;
                while (currentView->Next != NULL)
                {
                    currentView = currentView->Next;
                }
                currentView->Next = view;
            }
        }
        
    }

    size_t childrenCount = xml_node_children(node);

    for (size_t i = 0; i < childrenCount; i++) {
        ProcessNode(xml_node_child(node, i), depth + 1, viewParentType, viewParent);
    }

    free(nodeClass);

}

