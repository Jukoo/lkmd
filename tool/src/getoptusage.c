/**  @file getoptusage.h 
     @brief  build helper usage from getopt_long using options structure 

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


#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdio.h> 
#include <err.h> 


#include "getoptusage.h"
#include <getopt.h>


struct  __getopt_usage_t *  init ( struct  option *  opt   , int size  )  
{
  
  if ( opt == (void *) 0   ) return  (void *) 0 ; 
 
   
  struct __getopt_usage_t *  goptu = (struct __getopt_usage_t*) malloc(sizeof(*goptu)) ;  
  
  goptu->opt = opt ; 
  goptu->opt_size  =   size  ;  

  return goptu;  
}



void dump_desclist ( struct  __getopt_usage_t  * goptu  ,    char  * const *desclist ) 
{
  int  desclist_index = 0 ; 
  while ( desclist_index  < goptu->opt_size ) 
  {

    memcpy (
        (goptu->opt_desc+desclist_index) , 
        desclist[desclist_index] ,
        strlen(desclist[desclist_index]))  ; 

    desclist_index++ ;
  }
  
  
} 

void
show_usage ( struct __getopt_usage_t * goptu  )  
{
   if (goptu == (void *) 0 )  
   {
     errx(~0 ,"nil <nothing to show>") ; 
   } 
   int index  = 0 ;
    
   while(index < goptu->opt_size) 
   {
      fprintf(stdout  , "\t--%-3s , -%-3c :" ,  goptu->opt[index].name , goptu->opt[index].val) ; 
      fprintf(stdout, "%-10s\n", goptu->opt_desc[index]); 
      index++ ; 
   }
}
