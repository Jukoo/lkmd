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

#define VERSION  "1.0"
#define MAXBUFF   0xfa
#define X_BN 

#ifndef  X_BN 
#define BASENAME(ac , av) ({\
    char * bn_elfbin= av[ac^1] ;\
    char  bn[0x14] = {0} ; \
    memcpy( bn, (bn_elfbin+2) , 0x14);\
    printf("%s\n",bn);})
#else  
#define BASENAME(ac_as_exit_code , __mesg , ...) \
  errx(ac_as_exit_code , __mesg , ##__VA_ARGS__)
#endif 


typedef struct  _long_options_t  lopt_t   ; 
struct  _long_options_t  { 
  struct  option   *opt ;  
  char opt_desc[0xff][0xff] ;   
} ; 

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
 
} ; 


#define LKMD_TAG  "LKMD" 

struct option   excepted_option [] = { 
  {  "version" , no_argument  , 0  , 'v'}  ,
    {  "help" , no_argument  , 0  , 'h'} ,
    {  "lines" , no_argument  , 0  , 'l'} 

} ;   

struct _long_options_t  lopt= {  
  
  excepted_option ,  

  { 
    "show current version of the programme" , 
    "print this help and exit" , 
    "how many n lines you want to print"
  }
}; 
 

void  parse_lopt_for_usage ( const struct  _long_options_t  *long_opt ,  size_t   long_opt_size )   
{
  int  index  =0 ; 
  while (  index < long_opt_size)   
  {
    fprintf(stdout , "options  ->  %s\n" , (long_opt->opt +index)->name);
    fprintf(stdout  , " %s \n " , long_opt->opt_desc[index]) ; 

    index++ ; 
  }
}


void  arguments_parser ( int  argcount , char * const  * argvalues , struct __flargs *   flag_arguments) 
{
  errno = 0 ; 
  if ( argcount   <=1  )
  {     //display usage 
        //
     BASENAME(argcount  , "usage") ; 
  }
  int option=0 ; 
  const char *short_flags = "vhn:" ;

  
  while ( (option = getopt_long(argcount , argvalues , short_flags , lopt.opt , (void *)0 )) !=  ~0  ) 
  {
    switch ( option ) 
    {
        case 'v' : 
          fprintf(stdout , "%s\n", VERSION) ;
          exit(0) ; 
          break; 
        case 'h' : 
          puts("should display usage of the programme") ; 
          exit(0) ; 
          break ;
        case  'n' : 
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
   
}



int  __alwys_unused  main (int __ac  , char ** __av) 
{

  flarg  flags ;
 

  int options_size = sizeof(excepted_option) / sizeof(excepted_option[0])  ;
  printf("size of lopt  %i \n" , options_size );  
  
  parse_lopt_for_usage(&lopt ,  options_size) ; 

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
