add_definitions(-g -std=c++11 -std=gnu99)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../
)

set(c2p_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/../sds.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../zmalloc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../functions.c
)

add_executable(test_c2p
    test_c2p.cpp
    ${c2p_SRCS}
)

add_executable(test_qt
    test_qt.cpp
    ${c2p_SRCS}
)

target_link_libraries(test_qt
    Qt5::Core
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../PinyinData.txt DESTINATION ${CHINESE2PINYIN_DIR})
