cmake_minimum_required(VERSION 3.26) 
project(lkmd  VERSION 1.0 DESCRIPTION "advance lsmod command  'just lsmod with utilities flags'") 
set(CMAKE_C_STANDARD 99) 
set(CMAKE_C_STANDARD_REQUIRED True)

add_library(${PROJECT_NAME}  SHARED lkmd.c) #-fPIC

add_subdirectory(app) 

target_include_directories(${PROJECT_NAME} PRIVATE include)  #-I  

#! properties   options
set_target_properties(${PROJECT_NAME}  PROPERTIES  VERSION  ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME}  PROPERTIES  SOVERSION ${PROJECT_VERSION_MINOR})
set_target_properties(${PROJECT_NAME}  PROPERTIES  PUBLIC_HEADER include/lkmd.h) 

