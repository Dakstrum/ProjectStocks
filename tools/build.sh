#!/bin/sh

declare build_dir=""

DebugBuild() 
{

    cmake .. -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" -DCMAKE_INSTALL_PREFIX="/mingw64/bin" -DCMAKE_BUILD_TYPE=Debug
    build_dir="build-debug-$(date +'%m-%d-%Y')"
    mkdir $build_dir

}

ReleaseBuild() 
{

    cmake .. -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" -DCMAKE_INSTALL_PREFIX="/mingw64/bin" -DCMAKE_BUILD_TYPE=Release
    build_dir="build-release-$(date +'%m-%d-%Y')"
    mkdir $build_dir

}

WindowsLibs() 
{

    declare msys_path="C:/msys64/mingw64/bin/"
    for dll in "allegro_acodec-5.2.dll" "allegro_audio-5.2.dll" "allegro_font-5.2.dll" "allegro_image-5.2.dll" "allegro_ttf-5.2.dll" "allegro_video-5.2.dll" "allegro-5.2.dll" "libdumb.dll" \
               "libFLAC-8.dll" "libjson-c-4.dll" "libogg-0.dll" "libopenal-1.dll" "libopus-0.dll" "libopusfile-0.dll" "libsqlite3-0.dll" "libtheoradec-1.dll" "libvorbis-0.dll" "libvorbisfile-3.dll" ; do

        cp "${msys_path}${dll}" $build_dir

    done

}

LinuxLibs() 
{

    echo "STUB"

}

GetSharedLibs() 
{

    if [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then

        WindowsLibs

    elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then

        LinuxLibs

    fi

}

if [ $1 == "--debug" ]; then

    echo "Building debug version"
    DebugBuild
    

elif [ $1 == "--release" ]; then

    echo "Building debug release"
    ReleaseBuild

else

    echo "Please pass either --debug or --release"
    exit 1

fi


mingw32-make -j2
GetSharedLibs
cp projectstocks.exe $build_dir
cp -r assets $build_dir