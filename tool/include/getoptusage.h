/**  @file getoptusage.h 
     @brief  build helper usage from getopt_long
             using option structure  
           
     @Copyright (C) 2023 Umar Ba jUmarB@protonmail.com  OpenWire Studio .Lab

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

#define   MXBUFF 0xff
#define   RBN_MXBUFF ( MXBUFF >> 4 )

enum {
   GETOPT_SIZE  
#define  GETOPT_SIZE(__target_data) \
  sizeof(__target_data)/sizeof(__target_data[0]) 
} ;

#ifdef  SHOWUPWUR  
  #define  __must_check  __attribute__((warn_unused_result)) 
#else 
  #define  __must_check 
#endif

#ifdef GETOPT_SYNOPSIS
/***/
#define SYNOPSIS_INDEX 1
#define __condcheck(x,y)  x <= y 
#else 
#define __condcheck(x,y)  x < y 
#endif 

typedef  struct  __getopt_usage_t  gopt_usage_t  ; 
struct __getopt_usage_t { 
  struct option *  opt ; 
  int opt_size  ; 
  char opt_desc[MXBUFF][MXBUFF] ;

#ifdef GETOPT_SYNOPSIS 
  union  { 
     char synopsis[MXBUFF] ;
     //... 
  }; 
#endif 

} ;


/** @fn  struct __getopt_usage_t  * init (struct option *   , int size )   
 *  @brief initialize  getoptusage struct 
 *  @param   struct option * 
 *  @param   int size  :!NOTE: you'd better  user GETOPT_SIZE() macro  to get the size of  options
 *  @return  struct  __getopt_usage_t  *
 */ 
struct __getopt_usage_t*  
init( struct option * __opt , int size );

/** @fn  struct __getopt_usage_t * init_(struct option * , int size , char * const * dl)
 *  @brief like the init function above +  dump_desclist to feed  opt_desc 
 *  @param   struct option * 
 *  @param   int size  :!NOTE: you'd better  use GETOPT_SIZE() macro  to get the size of  options
 *  @param   char * const *  dl  when (void *) 0 or NULL the init function above is used by default   
 *  @return  struct  __getopt_usage_t  *
 */ 

struct __getopt_usage_t* 
init_( struct option * __opt , int size  ,char * const * __description_list ) ; 

/** good naming Macro shortcut*/
#define  init_no_desc    init 
#define  init_with_desc  init_ 

/** @fn extern inline void endof_getoptusage ( struct __getopt_usage_t  *  __goptu ) 
 *  @brief release allocated resources 
 *  @param struct __getopt_usage_t  *  
 */ 
extern  inline void endof_getoptusage  ( struct __getopt_usage_t *  __restrict__   __goptu  ) {
   if(__goptu  == (void *)0) return  ; 
   free(__goptu) ; 
} 


/**  @fn void dump_desclist ( struct __getopt_usage_t * , char *const *  )  
 *   @brief dump description list  in  getoptusage structure 
 *   @param  __getopt_usage_t * 
 *   @param  char * const *  
 */
void dump_desclist ( struct  __getopt_usage_t  * goptu  ,    char  * const *desclist ) ; 


/** @fn  show_usage (struct __getopt_usage_t *)  
 *  @brief print  description list dumped on gopt_usage_t 
 */ 
void  show_usage (struct __getopt_usage_t *  __goptu ,  char * const *  __argv ) ; 


/** @brief basename program  manipulation to prettyfy */ 
static char * __must_check rootbn (char  *const *  __argv ,char *__root_basename_dumper)  ; 
static char * __must_check rootbn_prettyfy ( char * basename ) ; 


#endif /** GETOPT_USAGE_H  */
