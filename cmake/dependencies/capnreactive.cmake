set(CAPNREACTIVE_ROOT "${CMAKE_SOURCE_DIR}/third-party/capnreactive")
set(CAPNREACTIVE_LIB_DIR "${CAPNREACTIVE_ROOT}/sunshine-integration/target/release")

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
endif()

add_custom_target(capnreactive_build
    COMMAND cargo build --release
    WORKING_DIRECTORY ${CAPNREACTIVE_ROOT}/sunshine-integration
    COMMENT "Building CapnReactive FFI library"
)

add_library(capnreactive STATIC IMPORTED)
set_target_properties(capnreactive PROPERTIES
    IMPORTED_LOCATION ${CAPNREACTIVE_LIB_DIR}/libsunshine_capnreactive_ffi.a
)

add_dependencies(capnreactive capnreactive_build)

target_include_directories(sunshine PRIVATE 
    ${CAPNREACTIVE_ROOT}/sunshine-integration/include
)
