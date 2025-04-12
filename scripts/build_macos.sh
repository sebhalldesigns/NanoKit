##!/bin/sh

echo ">>> BUILDING NANOKIT FOR MACOS"

# check if build/macos_ios exists
if [ ! -f build/macos_gen ]; then

    # check if build directory exists
    if [ -d build ]; then
        # remove the build directory
        echo ">>> BUILD DIRECTORY ALREADY EXISTS"
        echo "    - Removing build directory..."
        rm -rf build
        echo "    - Build directory removed."
    fi

    # create the build directory
    mkdir -p build

    # run cmake to generate the build files
    echo ">>> GENERATING CMAKE"
    cmake -B build -G Xcode
    
    if [ $? -eq 0 ]; then
        # create a file in the build directory called macos_ios
        touch build/macos_gen
        echo "<<< GENERATING CMAKE DONE"
    else
        echo "<<< CMAKE GENERATION FAILED"
        exit 1 
    fi
fi

# run cmake build
#cmake --build build
if [-f build/macos_build ]; then
    rm build/macos_build
fi

cmake --build build -- -quiet

if [ $? -eq 0 ]; then
    # create a file in the build directory called macos_ios_build
    touch build/macos_build
    echo "<<< BUILDING NANOKIT FOR MACOS DONE"
else
    echo "<<< BUILDING NANOKIT FOR MACOS FAILED"
    exit 1 
fi
