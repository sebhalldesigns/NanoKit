/***************************************************************
**
** NanoKit Platform Abstraction Layer Header File
**
** File         :  macos_app.h
** Module       :  app
** Author       :  SH
** Created      :  2025-02-15 (YYYY-MM-DD)
** License      :  MIT
** Description  :  Win32 App Header
**
***************************************************************/

#ifndef MACOS_APP_H
#define MACOS_APP_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

/* Objective-C imports */
#import <Cocoa/Cocoa.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/


/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

@interface AppDelegate : NSObject <NSApplicationDelegate>

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification;
- (void)applicationWillTerminate:(NSNotification *)aNotification;

@end

@interface WindowDelegate : NSObject <NSWindowDelegate>

@end

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/


#endif /* MACOS_APP_H */