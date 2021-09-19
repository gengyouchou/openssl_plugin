cmake_minimum_required(VERSION 3.6)

# configure import libs

add_library(lib_crypto SHARED IMPORTED)
add_library(lib_ssl SHARED IMPORTED)
set_target_properties(lib_crypto PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/jniLibs/libs/android/clang/${ANDROID_ABI}/libcrypto.so)
set_target_properties(lib_ssl PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/jniLibs/libs/android/clang/${ANDROID_ABI}/libssl.so)


find_library( # Defines the name of the path variable that stores the
              # location of the NDK library.
              log-lib

              # Specifies the name of the NDK library that
              # CMake needs to locate.
              log )


add_library( # Sets the name of the library.
        native-lib             # Sets the library as a shared library.
        SHARED             # Provides a relative path to your source file(s).
        src/main/cpp/native-lib.cpp 
        src/main/cpp/envelope.cpp 
        )


target_include_directories(native-lib PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/main/cpp/include/)
    

target_link_libraries(
                      native-lib
                      android
                      lib_crypto
                      lib_ssl
                      ${log-lib})
