cmake_minimum_required(VERSION 3.5)

project(NexUs VERSION 2.0.0 LANGUAGES CXX)

add_definitions(-DNEXUS_LIBRARY)
option(NEXUS_BUILD_STATIC_LIB "Build static library." OFF)

FILE(GLOB ORIGIN Source/include/*.h Source/*.cpp)
FILE(GLOB PRIVATE Source/private/*.h Source/private/*.cpp)
FILE(GLOB DEVELOPER Source/DeveloperComponents/*.h Source/DeveloperComponents/*.cpp)

# 以下内容已 discarded 无需关心
# 手动生成moc，需要在mocs_compilation.cpp #include moc_NXDef.cpp
# set(MOC_HEADERS 
#     ${CMAKE_CURRENT_SOURCE_DIR}/Source/include/NXDef.h
# )
# if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#     qt6_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})
# else()
#     qt5_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})
# endif()

source_group(include FILES ${ORIGIN})
source_group(private FILES ${PRIVATE})
source_group(DeveloperComponents FILES ${DEVELOPER})

set(PROJECT_SOURCES
    ${ORIGIN}
    ${PRIVATE}
    ${DEVELOPER}
    ${CMAKE_CURRENT_SOURCE_DIR}/NexUs.qrc
)

if (NEXUS_BUILD_STATIC_LIB)
    set(LIB_TYPE "STATIC")
else ()
    set(LIB_TYPE "SHARED")
endif ()

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(${PROJECT_NAME} ${LIB_TYPE}
        ${PROJECT_SOURCES}
    )
else ()
    add_library(${PROJECT_NAME} ${LIB_TYPE}
        ${PROJECT_SOURCES}
    )
endif ()

FILE(GLOB EXPORT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Source/include/*.h)
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source/private>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Source/DeveloperComponents>
    $<INSTALL_INTERFACE:${PROJECT_NAME}/include>
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

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        D3D11
        DXGI
    )
else ()
    target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
    )
endif ()

if(COMMAND func_install_target_files)
    func_install_target_files(${PROJECT_NAME})
endif()

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION ${PROJECT_NAME}/lib
    LIBRARY DESTINATION ${PROJECT_NAME}/lib
    RUNTIME DESTINATION ${PROJECT_NAME}/bin
)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/NexUsExample
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/NexUsExample
)
if (MSVC AND NOT LIB_TYPE STREQUAL "STATIC")
    install(
        FILES $<TARGET_PDB_FILE:${PROJECT_NAME}>
        DESTINATION ${CMAKE_INSTALL_PREFIX}/NexUsExample OPTIONAL)
endif ()

install(FILES ${EXPORT_HEADERS} DESTINATION ${PROJECT_NAME}/include)

set(INCLUDE_DIRS include)
set(LIBRARIES ${PROJECT_NAME})
set(LIB_DIR lib)

install(
    EXPORT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION ${PROJECT_NAME}/lib/cmake
)

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
    INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
)

install(
    FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake DESTINATION ${PROJECT_NAME}/lib/cmake
)
