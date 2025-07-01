cmake_minimum_required(VERSION 3.5)

project(NXPacketIO VERSION 2.0.0 LANGUAGES CXX)

# 显式设置支持的配置类型（关键修改）
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel" CACHE STRING "" FORCE)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

# 检测平台架构并设置平台后缀
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PLATFORM_SUFFIX "x64")
else()
    set(PLATFORM_SUFFIX "x86")
endif()

# 设置基础路径
set(NEXUS_ROOT_DIR "${CMAKE_SOURCE_DIR}/..")
set(NEXUS_BIN_DIR "${NEXUS_ROOT_DIR}/Bin")

# 配置输出目录（包含平台后缀）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${NEXUS_BIN_DIR}/${PROJECT_NAME}_$<CONFIG>_${PLATFORM_SUFFIX}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${NEXUS_BIN_DIR}/${PROJECT_NAME}_$<CONFIG>_${PLATFORM_SUFFIX}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${NEXUS_BIN_DIR}/${PROJECT_NAME}_$<CONFIG>_${PLATFORM_SUFFIX}")

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
    set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
     # 设置编译器选项：为所有配置生成调试信息
    target_compile_options(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:/Zi>"
        "$<$<CONFIG:Release>:/Zi>"
        "$<$<CONFIG:RelWithDebInfo>:/Zi>"
        "$<$<CONFIG:MinSizeRel>:/Zi>"
    )
    
    # 设置链接器选项：为所有配置生成调试信息
    target_link_options(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:/DEBUG>"
        "$<$<CONFIG:Release>:/DEBUG:FULL>"
        "$<$<CONFIG:RelWithDebInfo>:/DEBUG:FULL>"
        "$<$<CONFIG:MinSizeRel>:/DEBUG:FASTLINK>"
    )
    
    # 设置PDB文件输出位置
    set_target_properties(${PROJECT_NAME} PROPERTIES
        COMPILE_PDB_NAME ${PROJECT_NAME}
        COMPILE_PDB_NAME_DEBUG ${PROJECT_NAME}d  # 调试版本的PDB名称
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

# 使用构建后命令将DLL复制到NexUs输出目录
if(WIN32 AND NOT NXPACKETIO_BUILD_STATIC_LIB)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory 
            "${NEXUS_BIN_DIR}/NexUs_$<CONFIG>_${PLATFORM_SUFFIX}"
        # 复制动态库文件（.dll）
        COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_FILE:${PROJECT_NAME}>"
            "${NEXUS_BIN_DIR}/NexUs_$<CONFIG>_${PLATFORM_SUFFIX}/"
        # 复制导入库文件（.lib）
        COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_LINKER_FILE:${PROJECT_NAME}>"
            "${NEXUS_BIN_DIR}/NexUs_$<CONFIG>_${PLATFORM_SUFFIX}/"
        # 复制PDB文件（如果存在）
        COMMAND ${CMAKE_COMMAND} -E copy
            "$<TARGET_PDB_FILE:${PROJECT_NAME}>"
            "${NEXUS_BIN_DIR}/NexUs_$<CONFIG>_${PLATFORM_SUFFIX}/"
        COMMENT "Copying ${PROJECT_NAME} outputs to NexUs directory"
    )
endif()

# 安装配置保持不变（可选）
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
