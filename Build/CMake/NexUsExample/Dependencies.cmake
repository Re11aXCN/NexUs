# 首先创建目标目录
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Resource/Image/WindowBase)

# 定义要下载的图片列表
set(IMAGE_FILES
    "Miku.gif"
    "Miku.png"
    "WorldTree.gif"
    "WorldTree.jpg"
)

# 逐个下载图片（仅当文件不存在时）
foreach(IMG_FILE IN LISTS IMAGE_FILES)
    set(DEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Resource/Image/WindowBase/${IMG_FILE}")
    
    if(NOT EXISTS ${DEST_PATH})
        message(STATUS "Downloading ${IMG_FILE}...")
        file(DOWNLOAD
            "https://raw.githubusercontent.com/Liniyous/ElaWidgetTools/main/ElaWidgetToolsExample/Resource/Image/WindowBase/${IMG_FILE}"
            "${DEST_PATH}"
            SHOW_PROGRESS
            TLS_VERIFY OFF
        )
    else()
        message(STATUS "${IMG_FILE} already exists, skipping download.")
    endif()
endforeach()