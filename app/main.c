/** @file  lsmod.c 
 *  @brief lsmod command like with some extra  feature 
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




#include "../include/lsmod.h" 



int
main (int __ac  , char ** __av) 
{
 

  lsmod_t  module_list ;   
  lsmod_syspath_open(LSMOD_LINUX_SYSMOD, &module_list)  ; 
 
  //lsmod_list_all_module_found(&module_list) ; 
  
  //int count =  lsmod_count_loaded_modules(&module_list) ;  
  
  //lsmod_log("loaded module : %i" , count ); 

  return EXIT_SUCCESS ; 
}
