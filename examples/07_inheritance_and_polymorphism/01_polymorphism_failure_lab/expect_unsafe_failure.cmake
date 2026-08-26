execute_process(
    COMMAND "${PROGRAM}"
    RESULT_VARIABLE result
    OUTPUT_VARIABLE stdout
    ERROR_VARIABLE stderr)
file(WRITE "${LOG}" "stdout:\n${stdout}\nstderr:\n${stderr}")
if(result EQUAL 0)
    message(FATAL_ERROR "Unsafe deletion unexpectedly exited with code 0")
endif()
if(NOT "${stdout}${stderr}" MATCHES "new-delete-type-mismatch|AddressSanitizer")
    message(FATAL_ERROR "No relevant AddressSanitizer diagnostic; see ${LOG}")
endif()
message(STATUS "Expected AddressSanitizer failure observed")
