execute_process(
    COMMAND "${CXX}" -std=c++17 -Wall -Wextra -Wpedantic -c "${SOURCE}" -o "${OBJECT}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE stdout
    ERROR_VARIABLE stderr)
file(WRITE "${LOG}" "${stdout}${stderr}")
if(result EQUAL 0)
    message(FATAL_ERROR "Expected compilation to fail for ${SOURCE}")
endif()
if(NOT "${stdout}${stderr}" MATCHES "${PATTERN}")
    message(FATAL_ERROR "Compilation failed for an unexpected reason; see ${LOG}")
endif()
message(STATUS "Expected compile failure observed: ${PATTERN}")
