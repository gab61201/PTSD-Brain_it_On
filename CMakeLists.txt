cmake_minimum_required(VERSION 3.16)
project(PTSD-Brain_it_On)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_policy(SET CMP0135 NEW)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BOX2D_TAG v3.1.1)
set(JSON_TAG v3.12.0)

FetchContent_Declare(
    box2d
    URL         https://github.com/erincatto/box2d/archive/refs/tags/${BOX2D_TAG}.zip
)

set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(box2d)

FetchContent_Declare(
    nlohmann_json
    URL         https://github.com/nlohmann/json/releases/download/${JSON_TAG}/json.tar.xz
)

FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
    ptsd

    GIT_REPOSITORY  https://github.com/ntut-open-source-club/practical-tools-for-simple-design.git
    GIT_TAG         v0.2
    GIT_SHALLOW     TRUE

    PATCH_COMMAND   git apply --ignore-whitespace ${CMAKE_CURRENT_SOURCE_DIR}/screenshot.patch
                    || git apply --ignore-whitespace --check --reverse ${CMAKE_CURRENT_SOURCE_DIR}/screenshot.patch
)

FetchContent_MakeAvailable(ptsd)

include(files.cmake)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# file(GLOB_RECURSE SRC_FILES src/*)
# file(GLOB_RECURSE HEADER_FILES include/*)

add_executable(${PROJECT_NAME}
    ${SRC_DIR}/main.cpp
    ${SRC_FILES}
    ${INCLUDE_FILES}
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources")
else()
    # Release 模式使用相對路徑
    target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="Resources")

    # 建置完成後，自動將 Resources 資料夾複製到執行檔所在的目錄
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/Resources")
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="Resources")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Resources
    )

    # 強制覆寫 PTSD 引擎的巨集，讓它也使用相對路徑尋找 Shader
    target_compile_definitions(PTSD PRIVATE PTSD_ASSETS_DIR="assets/")

    # 將 PTSD 引擎的 assets 資料夾也複製到執行檔旁邊
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ptsd_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    )
endif()

target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${DEPENDENCY_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${ptsd_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2main
    PTSD
    box2d
    nlohmann_json::nlohmann_json
)
