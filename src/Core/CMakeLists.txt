set(CORE_INC ${PROJECT_SOURCE_DIR}/include/ArgusEngine/Core)

add_library(ArgusCore STATIC
    WindowsSharedMemory.cpp
    ${CORE_INC}/Concepts.hpp
    ${CORE_INC}/ControlHeader.hpp
    ${CORE_INC}/WindowsSharedMemory.hpp
)

add_library(Argus::Core ALIAS ArgusCore)

target_include_directories(ArgusCore 
    PUBLIC 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(ArgusCore PRIVATE argus_compiler_flags)
