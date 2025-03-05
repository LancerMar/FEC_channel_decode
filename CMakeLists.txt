cmake_minimum_required(VERSION 3.16)
Project(FEC_channel_decode)

set(SHARED_LIBS_SOURCES
    src/FEC_channel_decode_api.cpp
)

set(EXE_SOURCES
    main.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(FEC_channel_decode SHARED ${SHARED_LIBS_SOURCES})

target_include_directories(FEC_channel_decode 
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(FEC_channel_decode
    PRIVATE
        FEC_channel_decode
)

add_executable(TEST_lib_exe_FEC_channel_decode ${EXE_SOURCES})
target_include_directories(TEST_lib_exe_FEC_channel_decode 
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(TEST_lib_exe_FEC_channel_decode
    PRIVATE
        FEC_channel_decode
)
