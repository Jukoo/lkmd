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
  int lines;
  char finds[MAXBUFF] ;  
 
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
          flag_arguments->lines = strtol(optarg , (void *)0 ,  10) ; 
          if (errno != 0)
          {
            warn("required a numeric constant") ; 
            flag_arguments->lines = BAD_NUMCOVER ; 
          }
          break;
        default : 
          puts("unknow flag options") ; 
          exit(0); 
          break; 
     }
   }
   
}

void lkmd_get_live_module (); 


int
main (int __ac  , char ** __av) 
{

  flarg  flags ; 

  arguments_parser(__ac, __av ,  &flags);   

  printf("numeric -> %i\n" ,flags.lines) ; 

  lkmd_t  module_list ;   
  lkmd_syspath_open(_nullable, &module_list)  ; 
 
  //lkmd_list_all_module_found(&module_list) ; 

  char request[0xff][0xff] ={0};  
  lkmd_list_live_modules(&module_list ,  flags.lines , request ) ; 

  int i=0 ; 
  while ( i <  flags.lines ) 
  {
    if ( strlen(request[i]) == 0   || i ==  flags.lines) break ; 
    printf("%s\n",request[i]) ; 
    i++  ; 
  }

  
  
  (void *) lkmd_release(&module_list) ;  
 
  return EXIT_SUCCESS ; 
}
