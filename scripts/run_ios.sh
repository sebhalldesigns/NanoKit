##!/bin/sh

echo ">>> RUNNING NANOKIT ON IOS SIMULATOR"

xcrun simctl boot "iPhone 16 Pro"

# check if build/macos_ios exists
if [ ! -f build/macos_ios_build ]; then
    echo "<<< BUILD NOT FOUND"
    echo "<<< RUNNING NANOKIT ON IOS SIMULATOR FAILED"
    exit 1
else

    # run cmake to generate the build files
    echo ">>> RUNNING NANOKIT ON IOS SIMULATOR"
    xcrun simctl install "iPhone 16 Pro" build/Debug-iphonesimulator/NanoKit-Dev.app
    xcrun simctl launch --console-pty "iPhone 16 Pro" com.company.app
    if [ $? -eq 0 ]; then
        echo "<<< RUNNING NANOKIT ON IOS SIMULATOR DONE"
    else
        echo "<<< RUNNING NANOKIT ON IOS SIMULATOR FAILED"
        exit 1 
    fi
fi