/** @file  lkmd.c 
 *  @brief lkmd command like with some extra  feature 
 *  @author Umar  Ba , LoopFocus ,jUmarB@protonmail.com 
 *  
 *  @todo  : 
 *    1 ) list all modules /sys/module ( directory)
 */

#include <stdlib.h> 
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <dirent.h>  
#include <errno.h> 
#include <err.h> 
#include <stdio.h>
#include <getopt.h>
#include <string.h> 


#include "lkmd.h"
#include "getoptusage.h" 


#define VERSION  "1.0"
#define MAXBUFF   0xfa

enum { 
  BAD_NUMCOVER = ~3 
}; 

typedef  struct  __flargs  flarg ; 
struct __flargs { 

  char _finds[MAXBUFF] ;          //NOT IMPLTED  
  char _interval[MAXBUFF >> 4] ;   //NOT IMPLTED  

  int  _lines;
  int  _lives; 
  int  _nonelives;                 //NOT IMPLTED  
  int  _human_readable ;           //NOT IMPLTED  
  int  _kmesg ;                    //NOT IMPLTED  

  struct  __getopt_usage_t * goptu  ; 

} ; 


#define LKMD_TAG  "LKMD" 

struct option   excepted_option [] = { 
    {  "version" , no_argument  , 0  , 'v'}  ,
    {  "help" , no_argument  , 0  , 'h'} ,
    {  "lines" , no_argument  , 0  , 'l'} 
} ;   


char *desc[]=  {
  "lmkd lsmod like just  lmkd  [option]" ,
  "show current version of the programme",
  "print this help and exit" ,
  "how many n lines you want to print"
}; 





void  arguments_parser ( int  argcount , char * const  * argvalues , struct __flargs *   flag_arguments) 
{
  int option=0 ; 
  const char *short_flags = "vhl:" ;

  flag_arguments->goptu =  init_with_desc(excepted_option , GETOPT_SIZE(excepted_option) ,  desc) ; 
  if (flag_arguments->goptu == _void_0h ) 
  {
    errx(-1 , "getoptusage fail") ; 
  }
  
  while ( (option = getopt_long(argcount , argvalues , short_flags , flag_arguments->goptu->opt , (void *)0 )) !=  ~0  ) 
  {
    switch ( option ) 
    {
        case 'v' : 
          fprintf(stdout , "%s\n", VERSION) ;
          exit(0) ; 
          break; 
        case 'h' : 
          show_usage(flag_arguments->goptu , argvalues) ; 
          exit(1) ; 
          break ;
        case  'l' : 
          flag_arguments->_lines = strtol(optarg , (void *)0 ,  10) ; 
          if (errno != 0)
          {
            warn("required a numeric constant") ; 
            flag_arguments->_lines = BAD_NUMCOVER ; 
          }
          break;
        default : 
          puts("unknow flag options") ; 
          exit(0);
          break; 
     }
   }

  endof_getoptusage(flag_arguments->goptu) ;  
   
}



int  __alwys_unused  main (int __ac  , char ** __av) 
{

  flarg  flags ;
 

  //int options_size = sizeof(excepted_option) / sizeof(excepted_option[0])  ;
  //printf("size of lopt  %i \n" , options_size );  
  
//parse_lopt_for_usage(&lopt ,  options_size) ; 

  arguments_parser(__ac, __av ,  &flags);   

 
  struct __lkmd_t * kmod = lkmd_syspath_open(_void_0h) ; 

 
  mrq_t  * request  =  mrq_set(_Nullable , flags._lines) ; 
  

  

  //lkmd_get_raw_modules_mrq( &module_list  , flags.lines  , &request ) ; 
  //lkmd_get_live_modules(&module_list ,  flags.lines,  request.dump_register) ; 
  //lkmd_get_live_modules_mrq(&module_list , flags.lines ,&request) ; 

  //lkmd_get_from_cb(&module_list ,  flags.lines , request.dump_register , lkmd_get_live_modules) ;  

  lkmd_show_lkmod(kmod,  request->size) ; 
  
  //char *a = lkmd_get_from_mrq(kmod , LKMD_RAW_ONLY ,  &request ) ; //flags.lines ,  request.dump_register) ;  

  //lkmd_splice(a , -2 );
  //dbgprt ;
 

  //lkmd_splice_show(a); 


  //lkmd_list_dumper_contains( flags.lines , request.dump_register) ; 
  

  
 
  mrq_checkmall(request) ; 
  (void *) lkmd_release(kmod) ;  
 
  lkmd_exit(EXIT_SUCCESS) ;  
}
