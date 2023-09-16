/** @file  lkmd.c 
 *  @brief lsmod command like with some extra  feature 
 *  @author Umar  Ba , LoopFocus ,jUmarB@protonmail.com 
 *  
 */

#include <stdlib.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <dirent.h>  
#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h> 
#include <err.h> 
#include <string.h> 
#include <errno.h> 
#include <fcntl.h> 


#include "include/lkmd.h" 


static int kmodlive = 0;

static  char raw_kmod[MAX_LOADABLE_MDLS][MAX_LOADABLE_MDLS] ={0} ; 

static struct  __mod_t *lkmd_load_live_sysprocmod (void) 
{
    
    int  pmod_fd  = open ( LKMD_LINUX_PROCMOD , O_RDONLY ) ; 
    
    if (pmod_fd  <= ~0 ) { 
      errx ( pmod_fd  ,  "OPEN STREAM  ERROR") ; 
    }
    
    if (  dup2(pmod_fd , STDIN_FILENO)  <= ~0 ) 
    {
      lkmd_errx(LKMD_DUP_STREAM_ERR , "stream duplication error") ; 
    }
   
    struct __mod_t  * modules_collection = ( struct __mod_t *) malloc(sizeof(*modules_collection)  * MAX_LOADABLE_MDLS) ;  
    
    char inline_buffer[MAX_LOADABLE_MDLS << 3 ]  =   {0} ;
    
    int  index  = 0  ; 
    while ( (fgets(inline_buffer,  MAX_LOADABLE_MDLS << 3 , stdin)) != _nullable) 
    {
      /** parse  inline buffer and extract the needed information  and dump it into  modules_collection[index]*/
      lkmd_extract(inline_buffer , (modules_collection+index)) ;  
      index++ ; 
    }

    close (pmod_fd) ;
    kmodlive =  index ; 
    return modules_collection ;  
} 

//! just parse  inline string  
static void * lkmd_extract(const char * inbuff  , struct  __mod_t * modt) 
{

  char *local_buff = (char *)inbuff ; 
  
  char *token  = strtok(local_buff ,   " ")  ; 

  uchar_t  stage = 0; 
  while ( token != _nullable  ) 
  {
    switch (stage) { 
      case LKMD_MODULE_NAME :  
        memcpy(modt->name ,  token ,  strlen(token)) ;
        //lkmd_log("%s" , modt->name) ; 
        break ; 
      case  LKMD_MODULE_SIZE : 
        modt->size = strtol(token ,  _nullable , 0xa) ;  
        break ; 
      case  LKMD_MODULE_XUSED:
        modt->n_usedby = strtol(token,  _nullable  ,0xa) ;  
        break ; 
      case  LKMD_MODULE_USEDBY: 
        if ( *token  == '-' ) break ; 
        memcpy(modt->m_usedby , token ,  strlen(token)) ; 
        break ; 
      default  : 
        break ; 
    }
    token = strtok(_nullable , " ") ;
    stage++ ; 
  }

  return  modt ;  
}


struct __lkmd_t *  lkmd_syspath_open(const char * restrict gl_syspath , struct __lkmd_t * lkmd )    
{
 
  char sysmpath[MAX_LOADABLE_MDLS]  = LKMD_LINUX_SYSMOD ; 
  if (gl_syspath != _nullable ) { 
     memcpy(sysmpath , gl_syspath , strlen(gl_syspath)) ; 
  }

  lkmd->modules = lkmd_load_live_sysprocmod();  
  lkmd->total_of_live_module = 0 ; 
  
  int  sysmod_fd =   open (sysmpath  ,  O_RDONLY) ; 
  if (sysmod_fd <=~0)  { 
    lkmd_errx(LKMD_UKN_SYS_PATH  , "Not a Valid Path :: error code %s\n" , strerror(errno)) ; 
  }

  DIR* sysmod =  _nullable ; 
  sysmod = fdopendir(sysmod_fd) ;  
  
  if (sysmod == _nullable) {
    close(sysmod_fd) ;   
    return   _nullable ; 
  } 

 
  memcpy(lkmd->root_path ,  sysmpath  , strlen(sysmpath)) ; 
  
  struct dirent  * dirp = _nullable ; 
  uchar_t index   = 0 ; 
  
  while (  (dirp = readdir(sysmod))  != _nullable )  { 
    
    if ( dirp->d_type  ==  DT_DIR &&   dirp->d_name[0] !=  HIDDIRENT /** escape  ./ ../ and hidden directory */) { 
      
       explicit_bzero((lkmd->modules_names+index), MAX_LOADABLE_MDLS) ;
      
       uchar_t  match  = 0 ;
       while ( match <= kmodlive ) 
       { 
         /** Check live  modules*/
         if(strcmp((lkmd->modules+match)->name  , dirp->d_name) ==  0) {
           //lkmd_log("%s" ,  dirp->d_name) ; 
           lkmd->total_of_live_module++  ; 
         }
         match++ ; 
       }
       memcpy( (lkmd->modules_names+index), dirp->d_name ,  strlen(dirp->d_name) );
       memcpy((raw_kmod+index) ,dirp->d_name ,  strlen(dirp->d_name) ) ; 
       //printf("%s::\n", (char*)(lkmd->modules_names+index)) ; 
       index++ ;
    }
  }

  lkmd->modules_names[index+1]=  _nullable ;  
  lkmd->total_of_module = index; 
 

  return   lkmd ; 
}

void lkmd_list_all_module_found ( const struct __lkmd_t *  lkmd ) 
{
  /**  NOTE: depending  on parameter 
   *   -> load loaded module  or load all module from LKMD_LINUX_SYSMOD **/
  if ( lkmd  == _nullable) return  ;
  
  uchar_t index =0  ;

  //! That show all module from  LKMD_LINUX_SYSMOD 
  while  (  index <  lkmd->total_of_module)  {
   
    
    lkmd_log("%s",(char *)(raw_kmod+index)) ; 
    index++; 
  }
  
  lkmd_log("total  of all modules  : %i" , lkmd->total_of_module) ;
  lkmd_log("total  of live modules  : %i" , lkmd->total_of_live_module) ;
}

void  lkmd_list_live_modules( const struct __lkmd_t  *restrict  lkmd )   
{
  __check_nonull(lkmd); 
  int  index  = 0 ;  
 
  while (  index  < lkmd->total_of_live_module )  
  {
     lkmd_log("%s" , (char *) (lkmd->modules+index)->name) ;   
     index++; 
  }
}

int  lkmd_count_loaded_modules (  const struct __lkmd_t *  lkmd)  
{
   __check_nonull(lkmd) ; 
  
   return  lkmd->total_of_module ;  
}

int lkmd_count_live_modules(const struct __lkmd_t *restrict lkmd)
{
  __check_nonull(lkmd);
  __check_nonull(lkmd->modules); 
  return lkmd->total_of_live_module ; 
}

void  * lkmd_release( struct __lkmd_t *  restrict  lkmd )  
{
   __check_nonull(lkmd); 
   __check_nonull(lkmd->modules) ; 
   
   free(lkmd->modules) ;

   return  lkmd->modules ;  
}

/** 
 * NOTE :  
 * lkmd -s search  module  
 * lkmd -I info module  [ if no module is specified ] that check the current workdir 
 * lkmd -v version of lkmd 
 * lkmd -h help version 
 * lkmd -km  --kernel-mesg  or -dmesg 
 *
 */ 

