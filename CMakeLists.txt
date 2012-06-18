PROJECT(bricolage)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_VERBOSE_MAKEFILE OFF)
 
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE)

IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

#IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "DEBUG")
#ENDIF()
SET(CMAKE_CXX_FLAGS_DEBUG "-g2")

# Postfix for debug builds
SET(CMAKE_DEBUG_POSTFIX "_d")

FIND_PACKAGE(Qt4 REQUIRED)

MACRO (MY_QT4_EXTRACT_OPTIONS _qt4_files _qt4_options)
  SET(${_qt4_files})
  SET(${_qt4_options})
  SET(_QT4_DOING_OPTIONS FALSE)
  FOREACH(_currentArg ${ARGN})
    IF ("${_currentArg}" STREQUAL "OPTIONS")
      SET(_QT4_DOING_OPTIONS TRUE)
    ELSE ("${_currentArg}" STREQUAL "OPTIONS")
      IF(_QT4_DOING_OPTIONS) 
        LIST(APPEND ${_qt4_options} "${_currentArg}")
      ELSE(_QT4_DOING_OPTIONS)
        LIST(APPEND ${_qt4_files} "${_currentArg}")
      ENDIF(_QT4_DOING_OPTIONS)
    ENDIF ("${_currentArg}" STREQUAL "OPTIONS")
  ENDFOREACH(_currentArg) 
ENDMACRO (MY_QT4_EXTRACT_OPTIONS)

MACRO (MY_QT4_WRAP_UI outfiles outdir)
  MY_QT4_EXTRACT_OPTIONS(ui_files ui_options ${ARGN})
  FOREACH (it ${ui_files})
    GET_FILENAME_COMPONENT(outfile ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    SET(outfile ${outdir}/ui_${outfile}.h)
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
      COMMAND ${QT_UIC_EXECUTABLE}
      ARGS ${ui_options} -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile})
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH (it)
ENDMACRO (MY_QT4_WRAP_UI)

MACRO (MY_QT4_ADD_RESOURCES outfiles outdir)
  QT4_EXTRACT_OPTIONS(rcc_files rcc_options ${ARGN})
  FOREACH (it ${rcc_files})
    GET_FILENAME_COMPONENT(outfilename ${it} NAME_WE)
    GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
    GET_FILENAME_COMPONENT(rc_path ${infile} PATH)
    SET(outfile ${outdir}/qrc_${outfilename}.cxx)
    #  parse file for dependencies 
    #  all files are absolute paths or relative to the location of the qrc file
    FILE(READ "${infile}" _RC_FILE_CONTENTS)
    STRING(REGEX MATCHALL "<file[^<]+" _RC_FILES "${_RC_FILE_CONTENTS}")
    SET(_RC_DEPENDS)
    FOREACH(_RC_FILE ${_RC_FILES})
      STRING(REGEX REPLACE "^<file[^>]*>" "" _RC_FILE "${_RC_FILE}")
      IF(NOT IS_ABSOLUTE "${_RC_FILE}")
        SET(_RC_FILE "${rc_path}/${_RC_FILE}")
      ENDIF(NOT IS_ABSOLUTE "${_RC_FILE}")
      SET(_RC_DEPENDS ${_RC_DEPENDS} "${_RC_FILE}")
    ENDFOREACH(_RC_FILE)
    # Since this cmake macro is doing the dependency scanning for these files,
    # let's make a configured file and add it as a dependency so cmake is run
    # again when dependencies need to be recomputed.
    QT4_MAKE_OUTPUT_FILE("${infile}" "" "qrc.depends" out_depends)
    CONFIGURE_FILE("${infile}" "${out_depends}" COPY_ONLY)
    ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
      COMMAND ${QT_RCC_EXECUTABLE}
      ARGS ${rcc_options} -name ${outfilename} -o ${outfile} ${infile}
      MAIN_DEPENDENCY ${infile}
      DEPENDS ${_RC_DEPENDS} "${out_depends}")
    SET(${outfiles} ${${outfiles}} ${outfile})
  ENDFOREACH (it)
ENDMACRO (MY_QT4_ADD_RESOURCES)

# all the QT modules that you need besides core and gui
SET(QT_USE_QTWEBKIT TRUE)
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTSCRIPT TRUE)
# qtsql enabling
SET(QT_USE_QTSQL TRUE)
INCLUDE (${QT_USE_FILE})

# convenience paths
GET_FILENAME_COMPONENT(TOP_LEVEL . ABSOLUTE)
SET(LIBRARY_DIR ${TOP_LEVEL})
SET(APPLICATION_DIR ${TOP_LEVEL})

# Path to put executables in
SET(EXECUTABLE_OUTPUT_PATH ${TOP_LEVEL}/build/bin)

# Path to put libraries in
SET(LIBRARY_OUTPUT_PATH ${TOP_LEVEL}/build/lib)

# library directory locations
SET(BENTO_DIR ${LIBRARY_DIR})
SET(BENTO_INCLUDE_DIR ${BENTO_DIR}/include)
SET(BENTO_SOURCE_DIR ${BENTO_DIR}/src)

# testing directory locations
SET(BENTOTESTING_DIR ${APPLICATION_DIR}/Test)

INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR} ${BENTO_INCLUDE_DIR})
ADD_DEFINITIONS(${QT_DEFINITIONS})
# message(STATUS "${QT_LIBRARIES}")

# Bento Library
FILE(GLOB BENTO_SOURCE_FILES ${BENTO_SOURCE_DIR}/*)
FILE(GLOB BENTO_RESOURCES ${BENTO_DIR}/*.qrc)
MY_QT4_ADD_RESOURCES(BENTO_RESOURCES_RCC ${BENTO_SOURCE_DIR} ${BENTO_RESOURCES})
ADD_LIBRARY(Bento SHARED ${BENTO_SOURCE_FILES} ${BENTO_RESOURCES_RCC})
TARGET_LINK_LIBRARIES(Bento ${QT_LIBRARIES})

# Bento Test
FILE(GLOB BENTOTESTING_SOURCE_FILES ${BENTOTESTING_DIR}/src/*.cpp)
FILE(GLOB BENTOTESTING_HEADER_FILES ${BENTOTESTING_DIR}/src/window.h)
FILE(GLOB BENTOTESTING_FORMS ${BENTOTESTING_DIR}/*.ui)
QT4_WRAP_CPP(BENTOTESTING_HEADER_MOCS ${BENTOTESTING_HEADER_FILES})
MY_QT4_WRAP_UI(BENTOTESTING_FORM_HEADERS ${BENTOTESTING_DIR}/src ${BENTOTESTING_FORMS})
ADD_EXECUTABLE(BentoTest ${BENTOTESTING_SOURCE_FILES} ${BENTOTESTING_HEADER_FILES} ${BENTOTESTING_HEADER_MOCS} ${BENTOTESTING_FORM_HEADERS})
TARGET_LINK_LIBRARIES(BentoTest Bento ${QT_LIBRARIES})
