cmake_minimum_required(VERSION 3.5)

project(NXFramework VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel" CACHE STRING "" FORCE)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PLATFORM_SUFFIX "x64")
else()
    set(PLATFORM_SUFFIX "x86")
endif()

set(NEXUS_ROOT_DIR "${CMAKE_SOURCE_DIR}")
set(NEXUS_BIN_DIR "${NEXUS_ROOT_DIR}/Bin")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${NEXUS_BIN_DIR}/NexUs_$<CONFIG>_${PLATFORM_SUFFIX}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${NEXUS_BIN_DIR}/NexUs_$<CONFIG>_${PLATFORM_SUFFIX}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${NEXUS_BIN_DIR}/NexUs_$<CONFIG>_${PLATFORM_SUFFIX}")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

#在这里配置QT路径 例如 D:/Qt/6.6.2/msvc2019_64 D:/Qt/5.15.2/msvc2019_64 /home/liniyous/Qt/5.15.2/gcc_64
SET(QT_SDK_DIR E:/Qt/6.6.2/msvc2019_64 CACHE PATH "QT SDK DIR" FORCE)
message("在CMAKE中配置 QT_SDK_DIR 选项指定QT路径; 当前路径: ${QT_SDK_DIR}")
SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/Install CACHE PATH "Installation path" FORCE)

option(BUILD_NXPACKETIO "Build NXPacketIO" ON)
list(APPEND CMAKE_PREFIX_PATH ${QT_SDK_DIR})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_subdirectory(NexUs)
if (WIN32 AND BUILD_NXPACKETIO)
    add_definitions(-DNXPACKETIO_LIBRARY_ALREADY_BUILT)
    add_subdirectory(NXPacketIO)
endif ()
add_subdirectory(NexUsExample)

# 跨平台文件复制函数（保留用于构建时复制）
function(func_copy_target_files TARGET_NAME)
    # 获取目标文件的扩展名
    get_target_property(TARGET_TYPE ${TARGET_NAME} TYPE)
    
    # 添加构建后复制命令
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
        COMMENT "Ensuring output directory exists for ${TARGET_NAME}"
    )
    
    # 复制主要目标文件
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:${TARGET_NAME}>"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
        COMMENT "Copying ${TARGET_NAME} main file"
    )
    
    # 复制导入库文件（仅Windows共享库）
    if(WIN32 AND TARGET_TYPE STREQUAL "SHARED_LIBRARY")
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_LINKER_FILE:${TARGET_NAME}>"
                "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
            COMMENT "Copying ${TARGET_NAME} import library"
        )
    endif()
    
    # 复制PDB文件（仅Windows且MSVC）- 排除exp、idb、ilk文件
    if(WIN32 AND MSVC)
        # 只复制PDB文件，不复制其他调试相关文件
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_PDB_FILE:${TARGET_NAME}>"
                "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/"
            COMMENT "Copying ${TARGET_NAME} PDB file"
        )
    endif()
endfunction()

function(func_install_target_files TARGET_NAME DESTINATION_DIR)
    get_target_property(TARGET_TYPE ${TARGET_NAME} TYPE)
    install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION ${DESTINATION_DIR}
        LIBRARY DESTINATION ${DESTINATION_DIR}
        ARCHIVE DESTINATION ${DESTINATION_DIR}
    )
    if(WIN32 AND MSVC AND NOT TARGET_TYPE STREQUAL "STATIC_LIBRARY")
        install(FILES $<TARGET_PDB_FILE:${TARGET_NAME}>
            DESTINATION ${DESTINATION_DIR}
            OPTIONAL
        )
    endif()
endfunction()

add_custom_target(copy_all_files ALL)
add_dependencies(copy_all_files NexUs)
if (WIN32 AND BUILD_NXPACKETIO)
    add_dependencies(copy_all_files NXPacketIO)
endif ()
add_dependencies(copy_all_files NexUsExample)

if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.NexUs)
endif ()