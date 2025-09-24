cmake_minimum_required(VERSION 3.5)

project(NXPacketIO VERSION 2.0.0 LANGUAGES CXX)

# 配置中间文件目录（包含平台后缀）
set(CMAKE_OBJECT_OUTPUT_DIRECTORY "${NEXUS_BIN_DIR}/Intermediate/${PROJECT_NAME}_$<CONFIG>_${PLATFORM_SUFFIX}")

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_definitions(-DNXPACKETIO_LIBRARY)
option(NXPACKETIO_BUILD_STATIC_LIB "Build static library." OFF)

FILE(GLOB ORIGIN *.h *.cpp)
FILE(GLOB PacketIO PacketIO/*.h PacketIO/*.cpp)
FILE(GLOB Genio Genio/*.h Genio/*.cpp)
FILE(GLOB Util Util/*.h Util/*.cpp)
FILE(GLOB XIO XIO/*.h XIO/*.cpp)

source_group(PacketIO FILES ${PacketIO})
source_group(Genio FILES ${Genio})
source_group(Util FILES ${Util})
source_group(XIO FILES ${XIO})

set(PROJECT_SOURCES
    ${ORIGIN}
    ${PacketIO}
    ${Genio}
    ${Util}
    ${XIO}
)

if (NXPACKETIO_BUILD_STATIC_LIB)
    set(LIB_TYPE "STATIC")
else ()
    set(LIB_TYPE "SHARED")
endif ()

add_library(${PROJECT_NAME} ${LIB_TYPE}
    ${PROJECT_SOURCES}
)

if (MINGW)
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif ()
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:/ZI>"
        "$<$<CONFIG:Debug>:/Od>"
        "$<$<CONFIG:Release>:/O2>"
        "$<$<CONFIG:RelWithDebInfo>:/Zi>"
        "$<$<CONFIG:RelWithDebInfo>:/O2>"
        "$<$<CONFIG:MinSizeRel>:/Zi>"
        "$<$<CONFIG:MinSizeRel>:/O1>"
    )
    
    target_link_options(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:/DEBUG>"
        "$<$<CONFIG:RelWithDebInfo>:/DEBUG:FULL>"
        "$<$<CONFIG:Release>:/DEBUG:NONE>"
        "$<$<CONFIG:MinSizeRel>:/DEBUG:FASTLINK>"
    )
    
    set_target_properties(${PROJECT_NAME} PROPERTIES
        DEBUG_POSTFIX "d"
        COMPILE_PDB_NAME ${PROJECT_NAME}
        COMPILE_PDB_NAME_DEBUG ${PROJECT_NAME}d
        COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    )
endif ()

FILE(GLOB ORIGIN_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
FILE(GLOB XIO_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/XIO/*.h)
FILE(GLOB GENIO_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/GenIO/*.h)
FILE(GLOB PACKETIO_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/PacketIO/*.h)
FILE(GLOB UTIL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Util/*.h)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/GenIO
    ${CMAKE_CURRENT_SOURCE_DIR}/PacketIO
    ${CMAKE_CURRENT_SOURCE_DIR}/Util
    ${CMAKE_CURRENT_SOURCE_DIR}/XIO
)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        ws2_32
    )
endif ()

if(COMMAND func_install_target_files)
    func_install_target_files(${PROJECT_NAME})
endif()

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${ORIGIN_HEADERS} DESTINATION include)
install(FILES ${XIO_HEADERS} DESTINATION include/XIO)
install(FILES ${GENIO_HEADERS} DESTINATION include/GenIO)
install(FILES ${PACKETIO_HEADERS} DESTINATION include/PacketIO)
install(FILES ${UTIL_HEADERS} DESTINATION include/Util)

set(INCLUDE_DIRS include)
set(LIBRARIES ${PROJECT_NAME})
set(LIB_DIR lib)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION 2.0.0
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION lib/cmake
    PATH_VARS INCLUDE_DIRS LIBRARIES LIB_DIR
)

install(
    FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake 
          ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake 
    DESTINATION lib/cmake
)
