# CMake requirement for minimum version
cmake_minimum_required (VERSION 2.8)

# project information
project (g729a_ipp)

aux_source_directory(. DIR_SRCS)

set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set (PROJECT_SOURCE_FILES
	${PROJECT_SOURCE_DIR}/g729a_ipp_interface.c
	)

link_directories (${PROJECT_SOURCE_DIR}/ipp_libs)

add_library (g729a_ipp ${PROJECT_SOURCE_FILES})
target_link_libraries(g729a_ipp 
  ippcore
  ipps
  ippsc
  ippvm
  svml
  imf
  intlc
  lc
  speech_g729fp
  speech_vadi
)
