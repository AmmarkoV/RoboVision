# Install script for directory: /home/ammar/Documents/Programming/RoboVision/3dpartylibs/OpenKinect/fakenect

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/fakenect/libfreenect.so.0.0.1")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/fakenect/libfreenect.so.0.0.1"
         RPATH "")
  ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/fakenect/libfreenect.so.0.0.1")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/fakenect" TYPE SHARED_LIBRARY FILES
    "/home/ammar/Documents/Programming/RoboVision/3dpartylibs/OpenKinect/build/lib/fakenect/libfreenect.so.0.0.1"
    "/home/ammar/Documents/Programming/RoboVision/3dpartylibs/OpenKinect/build/lib/fakenect/libfreenect.so.0.0"
    "/home/ammar/Documents/Programming/RoboVision/3dpartylibs/OpenKinect/build/lib/fakenect/libfreenect.so"
    )
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/fakenect/libfreenect.so.0.0.1")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/fakenect/libfreenect.so.0.0.1")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/fakenect/libfreenect.so.0.0.1")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

