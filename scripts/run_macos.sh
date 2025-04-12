##!/bin/sh

echo ">>> RUNNING NANOKIT FOR MACOS"

# check if build/macos_ios exists
if [ -f build/macos_build ]; then

    build/Debug/NanoKit-Dev.app/Contents/MacOS/NanoKit-Dev 

    if [ $? -eq 0 ]; then
        echo "<<< BUILDING NANOKIT FOR MACOS DONE"
        return 0
    else
        echo "<<< BUILDING NANOKIT FOR MACOS FAILED"
        exit 1 
    fi

else 
    echo "<<< RUNNING NANOKIT FOR MACOS FAILED"

fi
