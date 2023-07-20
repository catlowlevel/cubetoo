cmake_minimum_required(VERSION 3.0.0)
project(minhook VERSION 0.1.0 LANGUAGES C)

#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32") #generate 32bit
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Check if the target platform is 32-bit
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(HDE_SOURCE_FILE hde/hde32.c)
else()
    set(HDE_SOURCE_FILE hde/hde64.c)
endif()

add_library(minhook STATIC hook.c trampoline.c buffer.c ${HDE_SOURCE_FILE})
