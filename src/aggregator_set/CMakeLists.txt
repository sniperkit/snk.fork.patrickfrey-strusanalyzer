cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# --------------------------------------
# SOURCES AND INCLUDES
# --------------------------------------
set(source_files
	libstrus_aggregator_set.cpp
)

include_directories(  
	"${Intl_INCLUDE_DIRS}"
	"${Boost_INCLUDE_DIRS}"
	"${PROJECT_SOURCE_DIR}/include"
	"${strusbase_INCLUDE_DIRS}"
)

link_directories(
	"${PROJECT_SOURCE_DIR}/src/utils"
	"${Boost_LIBRARY_DIRS}"
)

# -------------------------------------------
# LIBRARY
# -------------------------------------------
add_library( strus_aggregator_set SHARED ${source_files} )
target_link_libraries( strus_aggregator_set strusanalyzer_private_utils )
set_target_properties(
    strus_aggregator_set
    PROPERTIES
    DEBUG_POSTFIX "_d"
    SOVERSION "${STRUS_MAJOR_VERSION}.${STRUS_MINOR_VERSION}"
    VERSION ${STRUS_VERSION}
)


# ------------------------------
# INSTALLATION
# ------------------------------
install( TARGETS strus_aggregator_set
           LIBRARY DESTINATION ${LIB_INSTALL_DIR}/strus )

