project(project_name CXX C ASM)
set(USER_SOURCES src/main.cpp)
include_directories(include)
add_executable(elf_name ${USER_SOURCES})
