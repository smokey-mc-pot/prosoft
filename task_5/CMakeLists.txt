add_executable(task_5_extra_avx512 "extra_find_avx512.cpp")
add_executable(task_5_extra "extra_find.cpp")
add_executable(task_5 "main.c")
target_compile_options(task_5_extra_avx512  PRIVATE -mavx512f -mavx512vl -mavx512dq)
target_compile_options(task_5_extra  PRIVATE -mavx512f -mavx512vl -mavx512dq)



