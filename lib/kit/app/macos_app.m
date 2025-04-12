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

#include "macos_app.h"

#include <kit/log/log.h>

#include <stdio.h>
#include <time.h>

/***************************************************************
** MARK: CONSTANTS & MACROS
***************************************************************/

/***************************************************************
** MARK: TYPEDEFS
***************************************************************/

// Forward declaration of the CVDisplayLink callback
static CVReturn DisplayLinkCallback(CVDisplayLinkRef displayLink,
                                    const CVTimeStamp *inNow,
                                    const CVTimeStamp *inOutputTime,
                                    CVOptionFlags flagsIn,
                                    CVOptionFlags *flagsOut,
                                    void *displayLinkContext);

@interface MyOpenGLView : NSView
{
    NSOpenGLContext *glContext;
    CVDisplayLinkRef displayLink;
}
- (void)render;
- (void)updateViewport;
@end

@interface MyWindowController : NSWindowController <NSWindowDelegate>

@property (strong) MyOpenGLView *glView;

@end


MyWindowController *windowController;



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

    windowController = [[MyWindowController alloc] init]; // important to pass nil here.
    [windowController showWindow:NULL];

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

        self.glView = [[MyOpenGLView alloc] initWithFrame:window.contentView.bounds];
        self.glView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
        [window.contentView addSubview:self.glView];
    }

    return self;
}

- (void)windowDidLoad {
    [super windowDidLoad];

    printf("Window loaded\n");
}

- (void)windowDidResize:(NSNotification *)notification {
    printf("Window resized\n");
    [self.glView updateViewport]; // Call updateViewport on resize
}

@end



@implementation MyOpenGLView

- (instancetype)initWithFrame:(NSRect)frameRect {
    self = [super initWithFrame:frameRect];
    if (self) {
        // Disable layer-backed drawing (we're doing our own OpenGL rendering)
        [self setWantsLayer:NO];

        // Set to use the best resolution available (useful for Retina)
        [self setWantsBestResolutionOpenGLSurface:YES];

        // Create an NSOpenGLPixelFormat (here using a 4.1 core profile)
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

        // Create an OpenGL context manually
        glContext = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
        [glContext setView:self];

        // Set up the display link for continuous rendering
        CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);
        CVDisplayLinkSetOutputCallback(displayLink, &DisplayLinkCallback, (__bridge void *)self);

        CGLContextObj cglContext = [glContext CGLContextObj];
        CGLPixelFormatObj cglPixelFormat = [pixelFormat CGLPixelFormatObj];

        NSWindow *window = [self window];
        NSScreen *screen = [window screen];
        NSDictionary *deviceDescription = [screen deviceDescription];
        NSNumber *screenID = [deviceDescription objectForKey:@"NSScreenNumber"];
        CGDirectDisplayID displayID = [screenID unsignedIntValue];

        CVDisplayLinkSetCurrentCGDisplay(displayLink, displayID);
//        CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelFormat);
        CVDisplayLinkStart(displayLink);

        [self updateViewport]; // Initial viewport setup
    }
    return self;
}

- (void)dealloc {
    if (displayLink) {
        CVDisplayLinkStop(displayLink);
        CVDisplayLinkRelease(displayLink);
        displayLink = NULL;
    }
}

- (void)updateViewport {
    // Make the context current
    [glContext makeCurrentContext];

    // Update viewport based on the current view bounds
    NSRect bounds = [self bounds];
    glViewport(0, 0, bounds.size.width, bounds.size.height);

    [self render]; // Call render to update the view
}

// Our custom render method (invoked each frame by the display link)
- (void)render {

    printf("Rendering...\n");


    // Make the context current
    [glContext makeCurrentContext];

    // Perform OpenGL rendering here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Here you can add your shader and drawing calls…

    // Flush the rendered content to the drawable
    [glContext flushBuffer];

    // Optionally, log the GL version once:
    // const GLubyte* ver = glGetString(GL_VERSION);
    // NSLog(@"GL Version: %s", ver);
}

// Instead of relying solely on drawRect:, we can force rendering via our custom render method.
- (void)drawRect:(NSRect)dirtyRect {
    // Although not used as our primary draw trigger, it is still safe to implement.
//    [self render];
}

@end


// CVDisplayLink callback function: this runs on a high-priority background thread.
// We dispatch the render request to the main thread.
static CVReturn DisplayLinkCallback(CVDisplayLinkRef displayLink,
                                    const CVTimeStamp *inNow,
                                    const CVTimeStamp *inOutputTime,
                                    CVOptionFlags flagsIn,
                                    CVOptionFlags *flagsOut,
                                    void *displayLinkContext)
{


    
    static uint64_t lastTime = 0;
    uint64_t currentTime = mach_absolute_time();
    
    if (lastTime != 0) {
        mach_timebase_info_data_t timebase;
        mach_timebase_info(&timebase);
        
        uint64_t elapsedNano = (currentTime - lastTime) * timebase.numer / timebase.denom;
        double elapsedSeconds = (double)elapsedNano / 1e9;
        NSLog(@"Time between calls: %f seconds", elapsedSeconds);
    }
    
    lastTime = currentTime;

    /*
    @autoreleasepool {
        MyOpenGLView *view = (__bridge MyOpenGLView *)displayLinkContext;
        dispatch_async(dispatch_get_main_queue(), ^{
            [view render];
        });
    }*/
    return kCVReturnSuccess;
}