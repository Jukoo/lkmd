cmake_minimum_required(VERSION 3.26) 
project(lkmd  VERSION 1.0 DESCRIPTION "advance lsmod command  'just lsmod with extrautilities flags'") 
set(CMAKE_C_STANDARD 99) 
set(CMAKE_C_STANDARD_REQUIRED True)

set(lmkd_source  src/lkmd.c) 

add_library(${PROJECT_NAME}  SHARED  ${lmkd_source}) #-fPIC -shared


if(INCLTAG) 
  message(STATUS  "Show Tag ON") 
  add_definitions(-DSHOW_LKMD_TAG) 
endif()

if(DBGPRT) 
  message(STATUS "Enable debug print macro") 
  add_definitions(-D__dbgprt)  
endif() 

add_subdirectory(tool) 

target_include_directories(${PROJECT_NAME} PRIVATE include)  #-I  

#! properties   options
set_target_properties(${PROJECT_NAME}  PROPERTIES  VERSION  ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME}  PROPERTIES  SOVERSION ${PROJECT_VERSION_MINOR})
set_target_properties(${PROJECT_NAME}  PROPERTIES  PUBLIC_HEADER  include/lkmd.h)  

