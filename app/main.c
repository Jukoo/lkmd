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

#define BASENAME(ac , av) ({\
    char * bn_elfbin= av[ac^1] ;\
    char  bn[0x14] = {0} ; \
    memcpy( bn, (bn_elfbin+2) , 0x14);\
    printf("%s\n",bn);})

#define basename(ac, av) BASENAME(ac,av) 



#define LKMD_TAG  "LKMD" 

void  arguments_parser ( int  argcount , char * const  * argvalues ) 
{

  printf("%i\n",   argcount)  ; 
  if ( argcount   <=1  )
  {     //display usage 
    
     basename(argcount , argvalues) ; 
     exit(1) ; 
  }
  int option=0 ; 
  const char *short_flags = ":vh" ;
  while ( (option = getopt(argcount , argvalues , short_flags)) !=  ~0  ) 
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

  arguments_parser(__ac, __av);   


  lkmd_t  module_list ;   
  lkmd_syspath_open(_nullable, &module_list)  ; 
 
  //lkmd_list_all_module_found(&module_list) ; 
  lkmd_list_live_modules(&module_list) ; 

  
  
  (void *) lkmd_release(&module_list) ;  
 
  return EXIT_SUCCESS ; 
}
