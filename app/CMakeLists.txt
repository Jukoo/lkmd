cmake_minimum_required(VERSION 3.26)
project(lkmd.elfbin  VERSION  1.0)

add_executable(${PROJECT_NAME} main.c) 

#++  enable  Debug symbol 
if(ENDBG) 
  message(STATUS  "Debug build") 
  set(CMAKE_BUILD_TYPE  Debug) 
endif() 

target_link_directories(${PROJECT_NAME}  PRIVATE ../build) 
target_include_directories(${PROJECT_NAME}  PRIVATE  ../include)

target_link_libraries(${PROJECT_NAME}  lkmd)
