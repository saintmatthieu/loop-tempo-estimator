# Add the include directories for minimp3
add_library(minimp3 INTERFACE)

# Include directories for minimp3 headers
target_include_directories(minimp3 INTERFACE
    ${PROJECT_SOURCE_DIR}/minimp3
)