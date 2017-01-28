# Libhexapod cmake include file
# https://github.com/ryankurte/libhexapod
# Copyright 2017 Ryan Kurte

# Add library includes
include_directories(${CMAKE_CURRENT_LIST_DIR})

# Add project sources
set(LIBHEXAPOD_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/source/hexapod.c
    ${CMAKE_CURRENT_LIST_DIR}/source/vector.c
)

# Create library
add_library(hexapod ${LIBHEXAPOD_SOURCES})

set(OPTIONAL_LIBS hexapod ${OPTIONAL_LIBS})
