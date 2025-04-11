##!/bin/sh

echo ">>> BUILDING NANOKIT FOR IOS"

# check if build/macos_ios exists
if [ ! -f build/macos_ios_gen ]; then

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
    cmake -B build -G Xcode -DBUILD_IOS=ON
    
    if [ $? -eq 0 ]; then
        # create a file in the build directory called macos_ios
        touch build/macos_ios_gen
        echo "<<< GENERATING CMAKE DONE"
    else
        echo "<<< CMAKE GENERATION FAILED"
        exit 1 
    fi
fi

# run cmake build
#cmake --build build
if [-f build/macos_ios_build ]; then
    rm build/macos_ios_build
fi

cmake --build build -- -quiet

if [ $? -eq 0 ]; then
    # create a file in the build directory called macos_ios_build
    touch build/macos_ios_build
    echo "<<< BUILDING NANOKIT FOR IOS DONE"
else
    echo "<<< BUILDING NANOKIT FOR IOS FAILED"
    exit 1 
fi
