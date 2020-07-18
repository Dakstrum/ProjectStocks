
declare build_dir=""
declare operating_system=""
declare c_compiler="gcc"
declare cxx_compiler="g++"
declare build_type="debug-build"

if [ "$(uname -s)" == "Darwin" ]; then

	operating_system="Darwin"

else

	operating_system="$(expr substr $(uname -s) 1 10)"

fi

echo "Detected operating system = ${operating_system}"

for arg in "$@"
do
    if [ $arg == "--release" ]; then

        build_type="release-build"

    elif [ $arg == "--clang" ]; then

        c_compiler="clang"
        cxx_compiler="clang++"

    fi
done

DebugBuild() 
{


    build_dir="debug-build-$(date +'%m-%d-%Y')"
    mkdir $build_dir

    if [ $operating_system == "MINGW64_NT" ]; then

        cmake .. -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" -DCMAKE_INSTALL_PREFIX="/mingw64/bin" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=$c_compiler -DCMAKE_CXX_COMPILER=$cxx_compiler

    elif [ $operating_system == "Linux" ] || [ $operating_system == "Darwin" ]; then

        cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=$c_compiler -DCMAKE_CXX_COMPILER=$cxx_compiler

    fi

}

ReleaseBuild() 
{

    build_dir="release-build-$(date +'%m-%d-%Y')"
    mkdir $build_dir

    if [ $operating_system == "MINGW64_NT" ]; then

        cmake .. -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" -DCMAKE_INSTALL_PREFIX="/mingw64/bin" -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_C_COMPILER=$c_compiler -DCMAKE_CXX_COMPILER=$cxx_compiler

    elif [ $operating_system == "Linux" ] || [ $operating_system == "Darwin" ]; then

        cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=$c_compiler -DCMAKE_CXX_COMPILER=$cxx_compiler

    fi

}

GetSharedLibs() 
{

    if [ $operating_system == "MINGW64_NT" ]; then

        declare msys_path="C:/msys64/mingw64/bin/"
        for dll in "allegro_acodec-5.2.dll" "allegro_audio-5.2.dll" "allegro_font-5.2.dll" "allegro_image-5.2.dll" "allegro_ttf-5.2.dll" "allegro_video-5.2.dll" "allegro-5.2.dll" "allegro_primitives-5.2.dll" "libdumb.dll" \
                   "libFLAC-8.dll" "libjson-c-5.dll" "libogg-0.dll" "libopenal-1.dll" "libopus-0.dll" "libopusfile-0.dll" "libsqlite3-0.dll" "libtheoradec-1.dll" "libvorbis-0.dll" "libvorbisfile-3.dll" \
                   "libpng16-16.dll" "libwebp-7.dll" "libgcc_s_seh-1.dll" "libstdc++-6.dll" "libjpeg-8.dll" "libfreetype-6.dll" "libwinpthread-1.dll" "zlib1.dll" "libbz2-1.dll" "libharfbuzz-0.dll" \
                   "libgraphite2.dll" "libglib-2.0-0.dll" "libpcre-1.dll" "libintl-8.dll" "libiconv-2.dll"; do

            if test -f "${msys_path}${dll}"; then
                cp "${msys_path}${dll}" $build_dir
            else
                echo "Did not find ${msys_path}${dll}"
            fi

        done

    elif [ $operating_system == "Linux" ]; then

        for lib in $(readelf -d projectstocks | grep 'Shared library' | awk '{print $5}' | tr -d []) "libdumb-0.9.3.so" "libopusfile.so.0" "libjpeg.so.62"; do

            #cp -L $(locate $lib | head -n 1) "lib/${lib}"

            if test -f "/lib64/${lib}"; then
                cp -L "/lib64/${lib}" "lib/${lib}"
            fi

        done
        rm lib/libc*
        rm lib/libm*

    fi

}

SetupCmake()
{

    if [ $build_type == "debug-build" ]; then
        echo "Building debug version"
        DebugBuild
    else
        echo "Building release version"
        ReleaseBuild
    fi

}

BuildProject()
{

    if [ $operating_system == "MINGW64_NT" ]; then

        mingw32-make -j2

    elif [ $operating_system == "Linux" ] || [ $operating_system == "Darwin" ]; then

        make -j2

    fi

}

main()
{

    SetupCmake
    BuildProject
    GetSharedLibs

    if [ $operating_system == "MINGW64_NT" ]; then

        cp projectstocks.exe $build_dir

    elif [ $operating_system == "Linux" ]; then

        cp projectstocks $build_dir
        cp -r lib $build_dir

    fi

    if [ $operating_system == "MINGW64_NT" ] || [ $operating_system == "Linux" ]; then

        cp -r assets $build_dir

    fi

}


main
