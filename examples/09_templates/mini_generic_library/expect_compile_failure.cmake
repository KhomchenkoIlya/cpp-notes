file(MAKE_DIRECTORY "${OUTPUT_DIR}")
execute_process(
    COMMAND "${CXX}" -std=c++17 -Wall -Wextra -Wpedantic
            -I "${INCLUDE_DIR}" -c "${SOURCE}" -o "${OUTPUT_DIR}/negative.o"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error)
file(WRITE "${OUTPUT_DIR}/no_less.log" "${output}${error}")
if(result EQUAL 0)
    message(FATAL_ERROR "Expected compilation to fail, but it succeeded")
endif()
if("${output}${error}" STREQUAL "")
    message(FATAL_ERROR "Compiler failed without a diagnostic")
endif()
message(STATUS "Expected requirement failure observed; diagnostic saved to no_less.log")
