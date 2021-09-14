# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("asan")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        asan

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        asan

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})

# Enable address sanitizer only for debug builds
# Ideally we would use debug type properties and generator expressions,
# But with add_custom_command that becomes kind of convoluted

if (SANITIZE)

    set(LIBNAME asan)
    message("Enabling sanitizer in native library ${LIBNAME}")

    # A function to retrieve the architecture from the ABI
    # (neither ANDROID_ARCH_NAME nor CMAKE_SYSTEM_PROCESSOR work for this)

    function(get_architecture ABI ARCH)
        if (ABI STREQUAL armeabi-v7a)
            set(${ARCH} arm PARENT_SCOPE)
        elseif(ABI STREQUAL arm64-v8a)
            set(${ARCH} aarch64 PARENT_SCOPE)
        elseif(ABI STREQUAL x86)
            set(${ARCH} i686 PARENT_SCOPE)
        elseif(ABI STREQUAL x86_64)
            set(${ARCH} x86_64 PARENT_SCOPE)
        else()
            message(FATAL_ERROR "Unsupported ABI")
        endif()
    endfunction()

    # OR
    #    target_compile_options(${LIBNAME} PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    #    target_link_libraries(${LIBNAME} PRIVATE -fsanitize=address)

    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address")
    SET (CMAKE_ANDROID_ARM_MODE ARM)

    # Asan libs are "somewhere" in the toolchain's root, we try to find the
    # right one for the current architecture and copy it to the libs output dir
    # (so that it will be packed in the apk):

    get_architecture(${ANDROID_ABI} ARCH)
    file(GLOB_RECURSE ASAN_SRC ${ANDROID_TOOLCHAIN_ROOT}/*.asan*${ARCH}*-android.so)
    set(ASAN_DEST ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

    add_custom_command(
            TARGET ${LIBNAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${ASAN_SRC} ${ASAN_DEST}
    )

endif()