cmake_minimum_required(VERSION 3.26)
project(lkmd.elfb  VERSION  1.0)

#build libgetoptusage  
add_library(getoptusage   SHARED src/getoptusage.c)  
target_include_directories(getoptusage PUBLIC  include)  

add_executable(${PROJECT_NAME} main.c) 
#++  enable  Debug symbol 
if(ENDBG) 
  message(STATUS  "Debug build") 
  set(CMAKE_BUILD_TYPE  Debug) 
endif() 

target_link_directories(${PROJECT_NAME}  PRIVATE ../libs) 
target_include_directories(${PROJECT_NAME}  PRIVATE  ../include  ./include) 
target_link_libraries(${PROJECT_NAME}  lkmd getoptusage) 
