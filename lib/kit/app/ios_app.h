/***************************************************************
**
** NanoKit Platform Abstraction Layer Header File
**
** File         :  ios_app.h
** Module       :  app
** Author       :  SH
** Created      :  2025-04-12 (YYYY-MM-DD)
** License      :  MIT
** Description  :  iOS App Header
**
***************************************************************/

#ifndef IOS_APP_H
#define IOS_APP_H

/***************************************************************
** MARK: INCLUDES
***************************************************************/

/* Objective-C imports */
#import <UIKit/UIKit.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/


/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;

@end

/***************************************************************
** MARK: FUNCTION DEFS
***************************************************************/

#endif /* IOS_APP_H */