cmake_minimum_required(VERSION 3.12)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_MODULE_PATH
        ${CMAKE_SOURCE_DIR}/cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake
        ${CMAKE_MODULE_PATH}
        ${ECM_MODULE_PATH})

set(HTTP_SRCS
        http/mongoose.c
        http/mongoose.h
        http/mjson.c
        http/mjson.h)

set(SERVICES_SRCS
        services/generate_keys.c
        services/generate_keys.h services/transform.c services/transform.h)

set(INITIALIZATION_SRCS
        initialization/initialization.c
        initialization/initialization.h)

set(SRCS
        ${HTTP_SRCS}
        ${INITIALIZATION_SRCS}
        ${SERVICES_SRCS}
        entry.c)

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} MeowCryptoUtils)
