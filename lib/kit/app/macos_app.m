/***************************************************************
**
** NanoKit Platform Abstraction Layer Source File
**
** File         :  macos_app.m
** Module       :  app
** Author       :  SH
** Created      :  2025-04-11 (YYYY-MM-DD)
** License      :  MIT
** Description  :  macOS App Implementation
**
***************************************************************/

/***************************************************************
** MARK: INCLUDES
***************************************************************/

#include "platform_app.h"

#include <kit/log/log.h>

#include <stdio.h>

/* Objective-C imports */
#import <Cocoa/Cocoa.h>
#import <OpenGL/gl3.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

@interface MyOpenGLView : NSOpenGLView
@property (nonatomic, assign) BOOL isResizing;
@property (nonatomic, assign) int width;
@property (nonatomic, assign) int height;
@end

@interface MyWindowController : NSWindowController <NSWindowDelegate>

@property (strong) MyOpenGLView *glView;

@end


@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (strong) MyWindowController *windowController;

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification;
- (void)applicationWillTerminate:(NSNotification *)aNotification;

@end


/***************************************************************
** MARK: STATIC VARIABLES
***************************************************************/

static WindowEventCallback windowEventCallback = NULL;
static ApplicationEventCallback appEventCallback = NULL;

/***************************************************************
** MARK: STATIC FUNCTION DEFS
***************************************************************/

/***************************************************************
** MARK: PUBLIC FUNCTIONS
***************************************************************/

int RunLoop(ApplicationEventCallback appCallback, WindowEventCallback windowCallback)
{
    if (!appCallback || !windowCallback)
    {
        LogError("No callback provided");
        return -1;
    }

    appEventCallback = appCallback;
    windowEventCallback = windowCallback;

    @autoreleasepool {
        NSApplication *application = [NSApplication sharedApplication];
        AppDelegate *appDelegate = [[AppDelegate alloc] init];
        application.delegate = appDelegate;
        return NSApplicationMain(0, NULL);
    }

    return 0;

}

/***************************************************************
** MARK: STATIC FUNCTIONS
***************************************************************/

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    if (appEventCallback) {

        ApplicationEvent event;
        event.type = APPLICATION_EVENT_LAUNCHED;

        appEventCallback(event);
    }

    self.windowController = [[MyWindowController alloc] init]; // important to pass nil here.
    [self.windowController showWindow:self];


}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    /* Clean up code here */

}

@end

@implementation MyWindowController


- (instancetype)init {

    NSRect frame = NSMakeRect(100, 100, 800, 600);
    NSUInteger style = NSWindowStyleMaskTitled | NSWindowStyleMaskClosable |
                         NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable;
    
    NSWindow *window = [[NSWindow alloc] initWithContentRect:frame
                                                   styleMask:style
                                                     backing:NSBackingStoreBuffered
                                                       defer:NO];
    self = [super initWithWindow:window];
    if (self) {
        [window setTitle:@"OpenGL 3.2 Core Profile Window"];
        [window center];
        window.delegate = self;  // Set delegate to capture window events.
        
        // Create the OpenGL view and add it to the window's content.
        self.glView = [[MyOpenGLView alloc] initWithFrame:window.contentView.bounds];
        self.glView.layerContentsPlacement = NSViewLayerContentsPlacementTopLeft;
        self.glView.layerContentsRedrawPolicy = NSViewLayerContentsRedrawDuringViewResize;

        //self.glView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
        [window.contentView addSubview:self.glView];
    }

    return self;
}

- (void)windowDidLoad {
    [super windowDidLoad];

    printf("Window loaded\n");
}

- (void)windowDidResize:(NSNotification *)notification {

    [self.glView reshape];
    [self.glView setNeedsDisplayInRect:self.glView.bounds];
    printf("Window resized\n");
}

@end



@implementation MyOpenGLView

// Override the initializer to set up an OpenGL 3.2 core profile context.
- (instancetype)initWithFrame:(NSRect)frameRect {
    // Define attributes for a 3.2 core profile context
    NSOpenGLPixelFormatAttribute attrs[] = {
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
        NSOpenGLPFAColorSize,     24,
        NSOpenGLPFAAlphaSize,     8,
        NSOpenGLPFADepthSize,     24,
        NSOpenGLPFAStencilSize,   8,
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAAccelerated,
        0
    };
    
    NSOpenGLPixelFormat *pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    if (!pixelFormat) {
        NSLog(@"No appropriate pixel format found");
        return nil;
    }
    
    self = [super initWithFrame:frameRect pixelFormat:pixelFormat];
    if (self) {
        // Register for notifications on surface size changes if needed.
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(viewResized:)
                                                     name:NSViewFrameDidChangeNotification
                                                   object:self];
    }
    return self;
}

- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

// Override reshape to update viewport dimensions.
- (void)reshape {
    [super reshape];
    NSRect bounds = [self bounds];
    glViewport(0, 0, bounds.size.width, bounds.size.height);
}

// Called when view is resized.
- (void)viewResized:(NSNotification *)notification {
    [self reshape];
    [self setNeedsDisplay:YES];
}

// Override drawRect: for rendering.
- (void)drawRect:(NSRect)dirtyRect {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    const GLubyte* version = glGetString(GL_VERSION);
    NSLog(@"OpenGL version: %s", version);
    
    // Insert any additional OpenGL rendering code here.
    
    // Flush the buffer after drawing.
    [[self openGLContext] flushBuffer];
}

// Optionally override input event methods.
- (void)mouseDown:(NSEvent *)event {
    NSLog(@"Mouse down at %@", NSStringFromPoint([event locationInWindow]));
}

- (void)keyDown:(NSEvent *)event {
    NSLog(@"Key down: %@", [event charactersIgnoringModifiers]);
}

@end