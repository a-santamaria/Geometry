CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## ============
## = Policies =
## ============

SET(
  policies
  CMP0015
  CMP0020
  CMP0042
  )
FOREACH(policy ${policies})
  IF(POLICY ${policy})
    CMAKE_POLICY(SET ${policy} NEW)
  ENDIF(POLICY ${policy})
ENDFOREACH(policy)

## ================
## = Project name =
## ================

PROJECT(convex-hull)

## ========================
## = Packages and options =
## ========================

# Find VTK
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

## ================================================
## = Do not allow to build inside the source tree =
## ================================================

IF(PROJECT_BINARY_DIR STREQUAL ${PROJECT_SOURCE_DIR})
  MESSAGE(FATAL_ERROR "Building in the source tree is not allowed.")
ENDIF(PROJECT_BINARY_DIR STREQUAL ${PROJECT_SOURCE_DIR})

## =====================================
## = Where to put executables and libs =
## =====================================

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
MARK_AS_ADVANCED(
  CMAKE_BACKWARDS_COMPATIBILITY
  EXECUTABLE_OUTPUT_PATH
  LIBRARY_OUTPUT_PATH
  )

## ==============================
## = Subdirs containing headers =
## ==============================

INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
  )

## =====================
## = Compilation rules =
## =====================
FILE(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.cxx)
ADD_EXECUTABLE(convex_hull ${SRC_FILES})
TARGET_LINK_LIBRARIES(convex_hull ${VTK_LIBRARIES})



## eof - $RCSfile$
