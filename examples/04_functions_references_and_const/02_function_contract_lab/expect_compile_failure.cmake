execute_process(
    COMMAND "${CXX}" -std=c++17 -Wall -Wextra -Wpedantic -fsyntax-only "${SOURCE}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE stdout
    ERROR_VARIABLE stderr)
file(WRITE "${OUTPUT}" "${stdout}${stderr}")
if(result EQUAL 0)
    message(FATAL_ERROR "Expected compilation to fail: ${SOURCE}")
endif()
message(STATUS "Expected compile failure observed; diagnostic: ${OUTPUT}")
