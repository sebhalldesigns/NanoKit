##!/bin/sh

echo ">>> RUNNING NANOKIT ON IOS SIMULATOR"

xcrun simctl boot "iPhone 16 Pro"

# check if build/macos_ios exists
if [ ! -f build/macos_ios_build ]; then
    echo "<<< BUILD NOT FOUND"
    echo "<<< RUNNING NANOKIT ON IOS SIMULATOR FAILED"
    exit 1
else

    # check if build directory exists
    if [ -d build ]; then
        # remove the build directory
        echo ">>> BUILD DIRECTORY ALREADY EXISTS"
        echo "    - Removing build directory..."
        rm -rf build
        echo "    - Build directory removed."
    fi

    # run cmake to generate the build files
    echo ">>> RUNNING NANOKIT ON IOS SIMULATOR"
    xcrun simctl install "iPhone 16 Pro" build/NanoKit.app
    xcrun simctl launch "iPhone 16 Pro" com.nanokit.NanoKit
    if [ $? -eq 0 ]; then
        echo "<<< RUNNING NANOKIT ON IOS SIMULATOR DONE"
    else
        echo "<<< RUNNING NANOKIT ON IOS SIMULATOR FAILED"
        exit 1 
    fi
fi