set(CAPNREACTIVE_ROOT "${CMAKE_SOURCE_DIR}/third-party/capnreactive")
# cmake-lint: disable=C0301
set(CAPNREACTIVE_LIB_DIR
    "${CAPNREACTIVE_ROOT}/sunshine-integration/target/release")

if(SUNSHINE_ENABLE_CAPNREACTIVE)
    find_package(Git QUIET)
    if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMOD_RESULT)
    endif()

    if(EXISTS "${CAPNREACTIVE_ROOT}")

        add_custom_target(capnreactive_build
            COMMAND cargo build --release
            # cmake-lint: disable=C0301
            WORKING_DIRECTORY ${CAPNREACTIVE_ROOT}/sunshine-integration
            COMMENT "Building CapnReactive FFI library"
        )

        add_library(capnreactive STATIC IMPORTED)
        set_target_properties(capnreactive PROPERTIES
            IMPORTED_LOCATION
            ${CAPNREACTIVE_LIB_DIR}/libsunshine_capnreactive_ffi.a)

        add_dependencies(capnreactive capnreactive_build)

        # cmake-lint: disable=C0301
        target_include_directories(sunshine PRIVATE
            ${CAPNREACTIVE_ROOT}/sunshine-integration/include
        )

        target_compile_definitions(sunshine PRIVATE
            SUNSHINE_CAPNREACTIVE_ENABLED)
        target_link_libraries(sunshine PRIVATE capnreactive)

        message(STATUS
            "CapnReactive integration enabled")
        message(STATUS
            "libdisplaydevice disabled due to CMake version conflict")
    else()
        message(WARNING
            "CapnReactive requested but submodule not found.")
        message(WARNING
            "Disabling CapnReactive support.")
        # cmake-lint: disable=C0301
        set(SUNSHINE_ENABLE_CAPNREACTIVE OFF CACHE BOOL
            "CapnReactive support disabled due to missing submodule" FORCE)
    endif()
else()
    message(STATUS "CapnReactive integration disabled")
endif()
