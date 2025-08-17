cmake_minimum_required(VERSION 3.15)
project(prosoft)

include(cmake/common_flags.cmake) # папка с общими флагами для clang
include(cmake/unique_flags.cmake) # выбор платформы и компилятора

set(CMAKE_CXX_STANDARD 23) # можно и 20 вроде, ничего такого из 23 не использовал
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NUM_OF_TASKS 6) # количество задачек

foreach(i RANGE 1 ${NUM_OF_TASKS}) # решил сделать циклом, чтобы не прописывать каждый раз add_subdirectory, вдруг было бы очень много задачек)
    set(TASK_NAME "task_${i}")

    if (EXISTS "${CMAKE_SOURCE_DIR}/${TASK_NAME}/CMakeLists.txt")
        add_subdirectory(${TASK_NAME})
    endif()
endforeach()

