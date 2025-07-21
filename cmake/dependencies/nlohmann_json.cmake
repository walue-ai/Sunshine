# Loads the nlohmann_json library giving the priority to the system package
# first, with a fallback to FetchContent.
include_guard(GLOBAL)

find_package(nlohmann_json 3.11 QUIET)
if(NOT nlohmann_json_FOUND)
    message(STATUS
        "nlohmann_json v3.11.x package not found in the system. "
        "Falling back to FetchContent.")
    include(FetchContent)

    if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
        # cmake-lint: disable=C0301
        cmake_policy(SET CMP0135 NEW)  # Avoid DOWNLOAD_EXTRACT_TIMESTAMP
    endif()
    if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.31.0")
        cmake_policy(SET CMP0174 NEW)  # Handle empty variables
    endif()

    fetchcontent_declare(
            json
            # cmake-lint: disable=C0301
            URL
            https://github.com/nlohmann/json/releases/download/v3.11.3/
            json.tar.xz
            URL_HASH MD5=c23a33f04786d85c29fda8d16b5f0efd
            DOWNLOAD_EXTRACT_TIMESTAMP
    )
    fetchcontent_makeavailable(json)
endif()
