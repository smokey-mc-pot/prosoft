set(_USE_CLANG FALSE)

if (WIN32)
    find_program(_CLANG_C NAMES clang.exe clang)
    find_program(_CLANG_CXX NAMES clang++.exe clang++)

    if (_CLANG_C AND _CLANG_CXX)
      set(CMAKE_C_COMPILER "${_CLANG_C}" CACHE STRING "" FORCE)
      set(CMAKE_CXX_COMPILER "${_CLANG_CXX}" CACHE STRING "" FORCE)
      set(_USE_CLANG TRUE)

    else()
        find_program(_MSVC_CL NAMES cl.exe)

        if (NOT _MSVC_CL)
          message(FATAL_ERROR "Clang and MSVC toolchain not found on your Windows machine")
        endif()

        set(CMAKE_C_COMPILER "${_MSVC_CL}" CACHE STRING "" FORCE)
        set(CMAKE_CXX_COMPILER "${_MSVC_CL}" CACHE STRING "" FORCE)

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /O2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /OPT:REF /OPT:ICF")
    endif()

elseif (UNIX)
    find_program(_CLANG_C NAMES clang)
    find_program(_CLANG_CXX NAMES clang++)

    if (_CLANG_C AND _CLANG_CXX)
        set(CMAKE_C_COMPILER "${_CLANG_C}" CACHE STRING "" FORCE)
        set(CMAKE_CXX_COMPILER "${_CLANG_CXX}" CACHE STRING "" FORCE)
        set(_USE_CLANG TRUE)

    else()
        find_program(_GCC NAMES gcc)
        find_program(_GXX NAMES g++)

        if (NOT _GCC OR NOT _GXX)
          message(FATAL_ERROR "Clang and GCC toolchain not found on your UNIX machine")
        endif()

        set(CMAKE_C_COMPILER "${_GCC}" CACHE STRING "" FORCE)
        set(CMAKE_CXX_COMPILER "${_GXX}" CACHE STRING "" FORCE)

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    endif()
endif()

if (_USE_CLANG)
    include(${CMAKE_CURRENT_LIST_DIR}/common_flags.cmake)

    if (COMMAND apply_clang_common_flags)
      apply_clang_common_flags()
    endif()

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

