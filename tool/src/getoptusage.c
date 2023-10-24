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
#include <libgen.h>  

#include "getoptusage.h"
#include <getopt.h>


struct  __getopt_usage_t *  init ( struct  option *  opt   , int size  )  
{

  if ( opt == (void *) 0   ) return  (void *) 0 ; 
   
  struct __getopt_usage_t *  goptu = (struct __getopt_usage_t*) malloc(sizeof(*goptu)) ;  
  
  goptu->opt = opt ; 
  goptu->opt_size  =   size  ;
#ifdef GETOPT_SYNOPSIS 
   explicit_bzero(goptu->synopsis,MXBUFF);  
#endif 
  return goptu;  
}


struct __getopt_usage_t * init_(struct option *  opt , int size ,  char * const *  desclist)  
{
   if (desclist == (void *)0) 
   { 
     warn("using init_with_desc aka <init_>   but no description specified !") ; 
     return init(opt  , size) ; 
   } 

   struct __getopt_usage_t *  goptu = (struct __getopt_usage_t *) malloc(sizeof(*goptu)) ; 
   goptu->opt = opt ; 
   goptu->opt_size  =   size  ;
#ifdef GETOPT_SYNOPSIS 
   explicit_bzero(goptu->synopsis,MXBUFF);  
#endif 
   dump_desclist(goptu ,  desclist) ;   

   return goptu; 

    
}

void dump_desclist ( struct  __getopt_usage_t  * goptu  ,    char  * const *desclist ) 
{
  int  desclist_index = 0 ; 
  while ( desclist_index  <=goptu->opt_size ) 
  {

    memcpy (
        (goptu->opt_desc+desclist_index) , 
        desclist[desclist_index] ,
        strlen(desclist[desclist_index]))  ; 
    desclist_index++ ;
  }
}

void
show_usage ( struct __getopt_usage_t * goptu , char * const *  argv   )  
{
   if (goptu == (void *) 0 )  
   {
     errx(~0 ,"nil <nothing to show>") ; 
   }
 
   char rbasename[RBN_MXBUFF] = {0}  ;
   if ( argv !=  (void *) 0   ) 
   {
     (void*) rootbn(argv ,  rbasename);
     memcpy(rbasename , __xpg_basename(rbasename)  , strlen(rbasename)) ; 
     fprintf(stdout , "%s Usage:\n" , rbasename ) ; 
   }
   /**
    *  TODO :  add possiblity to add description and a little synopsis 
    */
   int index  = 0 ;
   int index_options = 0 ; 
#ifdef   GETOPT_SYNOPSIS  
   index = 1;  
   memcpy(goptu->synopsis ,  goptu->opt_desc[index^SYNOPSIS_INDEX] , MXBUFF) ; 
   fprintf(stdout ,  "\t%s\n" ,  goptu->synopsis); 
   index_options =1  ; 
#endif 

   puts("Options:") ;
   while(__condcheck(index , goptu->opt_size)) 
   {
      fprintf(stdout  , "\t-%-1c, --%-2s\t" ,  goptu->opt[index - index_options].val ,  goptu->opt[index-index_options].name); 
      fprintf(stdout, "%-10s\n", goptu->opt_desc[index]); 
      index++ ;  
   }
}


static char * __must_check  rootbn(char * const * argv ,  char  *rb_dump ) 
{
  char *localbasname =(char *) (*argv+0) ; 
   
  if (strlen(localbasname) >  RBN_MXBUFF)    
  {
     errx(-23 ,  "root basename too long !");  
  }

  rootbn_prettyfy(localbasname) ;  
  memcpy(rb_dump , (localbasname) ,  RBN_MXBUFF) ; 
  return rb_dump ; 
  
}


static char * __must_check  rootbn_prettyfy (  char *basename ) 
{
  
  char dot_start = 0x2e ; 
  if ( (*basename+0) ==dot_start  &&  (*basename+1) ==  dot_start++) 
  {
    return memcpy(basename ,   (basename+2) , strlen(basename)) ; 
  } 
  
  return  basename ; 
}
 
