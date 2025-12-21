# CMakeList.txt : CMake project for cake-ngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (Carrot CXX)
set(CMAKE_CXX_STANDARD 17)

# Dependencies
set(RAYLIB_ROOT "D:\User Folders\Desktop\personal\cake-ngine\raylib-master") 

target_include_directories(Carrot PRIVATE "${RAYLIB_ROOT}/src")

# Add source to this project's executable.
add_executable(Carrot)

target_link_libraries(Carrot PRIVATE "${RAYLIB_ROOT}/lib/raylib.dll") 


if (CMAKE_VERSION VERSION_GREATER 3.12)
  #set_property(TARGET Carrot PROPERTY CXX_STANDARD 20)
  #target_link_libraries(${PROJECT_NAME} PRIVATE libs)
endif()

# TODO: Add tests and install targets if needed.
