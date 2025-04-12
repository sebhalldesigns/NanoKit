/***************************************************************
**
** NanoKit Library Header File
**
** File         :  dockpanel.h
** Module       :  library
** Author       :  SH
** Created      :  2025-04-12 (YYYY-MM-DD)
** License      :  MIT
** Description  :  NanoKit DockPanel header file
**
***************************************************************/

#ifndef DOCKPANEL_H
#define DOCKPANEL_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include <view/view.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

typedef struct 
{
    nkView *View;          /* view */
    bool LastChildFill;   /* Last child fill */
} nkDockPanel;

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

nkDockPanel *nkDockPanel_Create(void);

void nkDockPanel_Destroy(nkDockPanel *dockPanel);

#endif /* DOCKPANEL_H */