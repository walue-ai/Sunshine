# CapnReactive target configuration - must be included after sunshine target creation

if(SUNSHINE_ENABLE_CAPNREACTIVE AND CAPNREACTIVE_CONFIGURED)
    set(CAPNREACTIVE_ROOT "${CMAKE_SOURCE_DIR}/third-party/capnreactive")
    
    # cmake-lint: disable=C0301
    target_include_directories(sunshine PRIVATE
        ${CAPNREACTIVE_ROOT}/sunshine-integration/include
    )

    target_compile_definitions(sunshine PUBLIC
        SUNSHINE_CAPNREACTIVE_ENABLED)
    target_link_libraries(sunshine capnreactive)
    
    message(STATUS "CapnReactive target configuration applied to sunshine")
endif()
