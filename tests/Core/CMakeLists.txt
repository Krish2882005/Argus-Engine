add_executable(ArgusCoreTests 
    WindowsSharedMemoryTest.cpp
)

target_link_libraries(ArgusCoreTests
    PRIVATE
    Argus::Core
    GTest::gtest_main
    argus_compiler_flags
)

gtest_discover_tests(ArgusCoreTests)
