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


#include "lkmd.h" 



int
main (int __ac  , char ** __av) 
{
 

  lkmd_t  module_list ;   
  lkmd_syspath_open(_nullable, &module_list)  ; 
 
  //lkmd_list_all_module_found(&module_list) ; 
  lkmd_list_live_modules(&module_list) ; 

  
  
  (void *) lkmd_release(&module_list) ;  
 
  return EXIT_SUCCESS ; 
}
