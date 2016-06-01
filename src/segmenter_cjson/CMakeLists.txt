cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

# --------------------------------------
# SOURCES AND INCLUDES
# --------------------------------------
set(source_files
	segmenter.cpp
	segmenterContext.cpp
	libstrus_segmenter_cjson.cpp
)

include_directories(  
	"${Intl_INCLUDE_DIRS}"
	"${Boost_INCLUDE_DIRS}"
	"${PROJECT_SOURCE_DIR}/include" 
	"${PROJECT_SOURCE_DIR}/3rdParty/cjson/include"
	"${PROJECT_SOURCE_DIR}/3rdParty/textwolf/include"
	"${strusbase_INCLUDE_DIRS}"
)
link_directories(
	"${PROJECT_SOURCE_DIR}/3rdParty/cjson/src"
)

# -------------------------------------------
# LIBRARY
# -------------------------------------------
add_library( strus_segmenter_cjson SHARED ${source_files} )
target_link_libraries( strus_segmenter_cjson strus_segmenter_utils strus_private_utils cjson )
set_target_properties(
    strus_segmenter_cjson
    PROPERTIES
    DEBUG_POSTFIX "_d"
    SOVERSION "${STRUS_MAJOR_VERSION}.${STRUS_MINOR_VERSION}"
    VERSION ${STRUS_VERSION}
)

# ------------------------------
# INSTALLATION
# ------------------------------
install( TARGETS strus_segmenter_cjson
           LIBRARY DESTINATION ${LIB_INSTALL_DIR}/strus )
