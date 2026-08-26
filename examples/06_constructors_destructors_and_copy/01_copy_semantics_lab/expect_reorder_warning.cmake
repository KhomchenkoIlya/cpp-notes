execute_process(
    COMMAND "${CXX}" -std=c++17 -Wall -Wextra -Wpedantic -Wreorder
            -c "${SOURCE}" -o "${CMAKE_CURRENT_BINARY_DIR}/initializer_order_warning.o"
    RESULT_VARIABLE compiler_status
    OUTPUT_VARIABLE compiler_stdout
    ERROR_VARIABLE compiler_stderr)
file(WRITE "${OUTPUT}" "${compiler_stdout}${compiler_stderr}")
if(NOT compiler_status EQUAL 0)
    message(FATAL_ERROR "warning example unexpectedly failed to compile; see ${OUTPUT}")
endif()
if(NOT compiler_stderr MATCHES "reorder|will be initialized after|field .* will be initialized after")
    message(FATAL_ERROR "compiler emitted no recognizable initialization-order warning; see ${OUTPUT}")
endif()
