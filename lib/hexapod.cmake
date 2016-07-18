# Libhexapod include file

# Add library includes
include_directories(${CMAKE_CURRENT_LIST_DIR})

# Add project sources
set(LIBHEXAPOD_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/source/hexapod.c
)

# Create library
add_library(hexapod ${LIBHEXAPOD_SOURCES})

set(OPTIONAL_LIBS hexapod ${OPTIONAL_LIBS})