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



enum { 
  BAD_NUMCOVER = ~3 
}; 

typedef  struct  __flargs  flarg ; 
struct __flargs { 

  char _finds[MAXBUFF] ;          //NOT IMPLTED  
  char _interval[MAXBUFF >> 4] ;   //NOT IMPLTED  

  int  _lines;
  int  _goto;                      //NOT IMPLTED  
  int  _human_readable ;           //NOT IMPLTED  
  int  _kmesg ;                    //NOT IMPLTED  
 
} ; 


#define LKMD_TAG  "LKMD" 

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

  static struct option  lopt [] = {
    {  "version" , no_argument  , 0  , 'v'} , 
    {"help", no_argument , 0 , 'h'}, 
    {"lines" ,  required_argument ,  0 , 'n'} 
  }; 
  
  while ( (option = getopt_long(argcount , argvalues , short_flags , lopt , (void *)0 )) !=  ~0  ) 
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



int
main (int __ac  , char ** __av) 
{

  flarg  flags ; 


  arguments_parser(__ac, __av ,  &flags);   

 
  struct __lkmd_t * kmod  = _nullable ; 
  kmod =lkmd_syspath_open(_nullable ,  kmod) ; 
 
  mrq_t  request ; 

  //lkmd_get_raw_modules_mrq( &module_list  , flags.lines  , &request ) ; 
  //lkmd_get_live_modules(&module_list ,  flags.lines,  request.dump_register) ; 
  //lkmd_get_live_modules_mrq(&module_list , flags.lines ,&request) ; 

  //lkmd_get_from_cb(&module_list ,  flags.lines , request.dump_register , lkmd_get_live_modules) ;  

  lkmd_show_lkmod(kmod,  flags._lines) ; 
  //char *a = lkmd_get(kmod , LKMD_RAW_ONLY ,flags.lines ,  request.dump_register) ;  

  //lkmd_splice(a , -2 );
  //dbgprt ;
  //lkmd_splice_show(a); 


  //lkmd_list_dumper_contains( flags.lines , request.dump_register) ; 
  

  
  
  (void *) lkmd_release(kmod) ;  
 
  return EXIT_SUCCESS ; 
}
