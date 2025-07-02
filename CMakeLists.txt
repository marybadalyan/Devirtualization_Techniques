cmake_minimum_required(VERSION 3.10)
project(Optimization_levels CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ANALYSIS_DIR "${CMAKE_BINARY_DIR}/analysis")
file(MAKE_DIRECTORY ${ANALYSIS_DIR})

add_executable(final_test main.cpp)

# Removed Release configuration for compile definitions
# target_compile_definitions(final_test PRIVATE
#     $<$<CONFIG:Release>:FINAL>  
# )

add_executable(no_final_test main.cpp)

# Removed Release configuration for compile definitions
# target_compile_definitions(no_final_test PRIVATE
#     $<$<CONFIG:Release>:DISABLE_FINAL>  
# )

add_executable(final_test_lto main.cpp)

# Removed Release configuration for compile definitions
# target_compile_definitions(final_test_lto PRIVATE
#     $<$<CONFIG:Release>:FINAL> 
# )

set_target_properties(final_test_lto PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)

if(MSVC)
    add_custom_command(
        TARGET final_test POST_BUILD
        COMMAND cmd /c "dumpbin /DISASM \"$<TARGET_OBJECTS:final_test>\" > \"${ANALYSIS_DIR}/assembly_final.txt\""
        COMMENT "Dumping assembly for final_test "
    )
    add_custom_command(
        TARGET no_final_test POST_BUILD
        COMMAND cmd /c "dumpbin /DISASM \"$<TARGET_OBJECTS:no_final_test>\" > \"${ANALYSIS_DIR}/assembly_no_final.txt\""
        COMMENT "Dumping assembly for no_final_test "
    )
    add_custom_command(
        TARGET final_test_lto POST_BUILD
        COMMAND cmd /c "dumpbin /DISASM \"$<TARGET_OBJECTS:final_test_lto>\" > \"${ANALYSIS_DIR}/assembly_final_lto.txt\""
        COMMENT "Dumping assembly for final_test "
    )
else()
    add_custom_command(
        TARGET final_test POST_BUILD
        COMMAND objdump -d -M intel "$<TARGET_FILE:final_test>" > "${ANALYSIS_DIR}/assembly_final.txt"
        COMMENT "Dumping assembly for final_test "
    )
    add_custom_command(
        TARGET no_final_test POST_BUILD
        COMMAND objdump -d -M intel "$<TARGET_FILE:no_final_test>" > "${ANALYSIS_DIR}/assembly_no_final.txt"
        COMMENT "Dumping assembly for no_final_test "
    )
    add_custom_command(
        TARGET final_test_lto POST_BUILD
        COMMAND objdump -d -M intel "$<TARGET_FILE:final_test_lto>" > "${ANALYSIS_DIR}/assembly_final_lto.txt"
        COMMENT "Dumping assembly for final_test "
    )
endif()
