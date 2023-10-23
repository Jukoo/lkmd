/**  @file getoptusage.h 
     @brief  build helper usage from getopt_long
             using option structure  
           
     @Copyright (C) 2023 Umar Ba jUmarB@protonmail.com  , OpenWire Studio .Lab

     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.
*/  


#pragma once 
#ifndef   GETOPT_USAGE_H 
#define   GETOPT_USAGE_H 1 

#ifndef _GETOPT_EXT_H  
#endif


#define   MXBUFF 0xff

enum {
   GETOPT_SIZE  
#define  GETOPT_SIZE(__target_data) \
  sizeof(__target_data)/sizeof(__target_data[0]) 
 
} ; 

typedef  struct  __getopt_usage_t  gopt_usage_t  ; 
struct __getopt_usage_t { 
  struct option *  opt ; 
  int opt_size  ; 
  char opt_desc[MXBUFF][MXBUFF] ; 
} ; 



/** @fn  struct __getopt_usage_t  * init (struct option *   , int size )   
 *  @brief initialize  getoptusage struct 
 *  @param   struct option * 
 *  @param   int size  :!NOTE: you'd better  user GETOPT_SIZE() macro  to get the size of  options
 *  @return  struct  __getopt_usage_t  *
 */ 
struct __getopt_usage_t*  
init( struct option * __opt , int size ); 

void 
init_ (struct  __getopt_usage_t  * gu  , struct  option *  opt) ;  

/**  @fn void dump_desclist ( struct __getopt_usage_t * , char *const *  )  
 *   @brief dump description list  in  getoptusage structure 
 *   @param  __getopt_usage_t * 
 *   @param  char * const *  
 *   
 */
void 
dump_desclist ( struct  __getopt_usage_t *  __goptu  ,  char *const *__desclist ) ; 


/** @fn  show_usage (struct __getopt_usage_t *)  
 *  @brief print  description list dumped on gopt_usage_t 
 */ 
void  show_usage (struct __getopt_usage_t *  __goptu ) ; 




#endif /** GETOPT_USAGE_H  */
