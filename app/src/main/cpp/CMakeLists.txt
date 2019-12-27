# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

#添加头文件目录，相对于CMakeLists.txt也就是本文件的路径，从目录
#结构中可以看出，CMakeLists.txt和include目录是在同一级目录
include_directories(include)

#添加avcodec SHARED表示动态库 IMPORTED表示从外部导入
add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION D:/projects/FirstFfmpeg/app/libs/${ANDROID_ABI}/libavcodec.so)

#avformat
add_library(avformat SHARED IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION D:/projects/FirstFfmpeg/app/libs/${ANDROID_ABI}/libavformat.so)


add_library( # Sets the name of the library.
             native-lib

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
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

#将添加进来的库avcodec avformat链接到native-lib库中，
#因为我们最终只会生成一个libnative-lib.so库
#这个库如果没有指定路径，那么可以在app/build/cmake/debug/obj中找到
target_link_libraries( # Specifies the target library.
        native-lib
        android
        avcodec avformat
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})
