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

#define LKMD_TAG  "LKMD" 


#include "../include/lkmd.h" 



int
main (int __ac  , char ** __av) 
{
 

  lkmd_t  module_list ;   
  lkmd_syspath_open(LKMD_LINUX_SYSMOD, &module_list)  ; 
 
  lkmd_list_all_module_found(&module_list) ; 
  
  //int count =  lkmd_count_loaded_modules(&module_list) ;  
  
  //lkmd_log("loaded module : %i" , count );
  //
  //
  if ( lkmd_release(&module_list)  ==  _nullable ) 
  {
    puts("cleaned"); 
  }
  

  return EXIT_SUCCESS ; 
}
