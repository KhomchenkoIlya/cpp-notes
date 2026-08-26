file(MAKE_DIRECTORY "${OUTPUT_DIR}")
execute_process(
    COMMAND "${CXX}" -std=c++17 -Wall -Wextra -Wpedantic
            -I "${INCLUDE_DIR}" -c "${SOURCE}" -o "${OUTPUT_DIR}/negative.o"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE output
    ERROR_VARIABLE error)
file(WRITE "${OUTPUT_DIR}/bad_reporter.log" "${output}${error}")
if(result EQUAL 0)
    message(FATAL_ERROR "Expected compilation to fail, but it succeeded")
endif()
if("${output}${error}" STREQUAL "")
    message(FATAL_ERROR "Compiler failed without a diagnostic")
endif()
message(STATUS "Expected reporter requirement failure observed")
