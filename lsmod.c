/** @file  lsmod.c 
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


#include "include/lsmod.h" 


static int modlive = 0;  

struct  __mod_t *lsmod_load_live_sysprocmod (void) 
{
    int  pmod_fd  = open ( LSMOD_LINUX_PROCMOD , O_RDONLY ) ; 
    
    if (pmod_fd  <= ~0 ) { 
      errx ( pmod_fd  ,  "OPEN STREAM  ERROR") ; 
    }
    
    if (  dup2(pmod_fd , STDIN_FILENO)  <= ~0 ) 
    {
      //lsmod_err(LSMOD_STREAM_DUP_ERR , "stream duplication error") ; 
      errx ( -2 ,  "stream duplication error" ) ; 
    }
    
    struct __mod_t  * modules_collection = ( struct __mod_t *) malloc(sizeof(*modules_collection)  * MAX_LOADABLE_MDLS) ;  
    
    char inline_buffer[MAX_LOADABLE_MDLS << 3 ]  =   {0} ;
    
    int  index  = 0  ; 
    while ( (fgets(inline_buffer,  MAX_LOADABLE_MDLS << 3 , stdin)) != _nullable) 
    {
      lsmod_extract(inline_buffer , (modules_collection+index)) ;  
      index++ ; 
    }

    close (pmod_fd) ;
    modlive =  index ; 
    return modules_collection ;  
} 


static void * lsmod_extract(const char * inbuff  , struct  __mod_t * modt) 
{

  char *local_buff = (char *)inbuff ; 
  
  char *token  = strtok(local_buff , " ")  ; 

  uchar_t  stage = 0; 
  while ( token != _nullable  ) 
  {
    switch (stage ) { 
      case LSMOD_MODULE_NAME :  
        memcpy(modt->name ,  token ,  strlen(token)) ;
        //lsmod_log("%s" , modt->name) ; 
        break ; 
      case  LSMOD_MODULE_SIZE : 
        modt->size = strtol(token ,  _nullable , 0xa) ;  
        break ; 
      case  LSMOD_MODULE_XUSED:
        modt->n_usedby = strtol(token,  _nullable  ,0xa) ;  
        break ; 
      case  LSMOD_MODULE_USEDBY: 
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


void *  lsmod_syspath_open(const char * restrict  gl_syspath , struct __lsmod_t * lsmod )    
{

 
  mod_t  * modules = lsmod_load_live_sysprocmod()  ; 
  
  int  sysmod_fd =   open (gl_syspath ,  O_RDONLY) ; 
  if (sysmod_fd <=~0)  { 
    errx(LSMOD_UKN_SYS_PATH  , "Not a Valid Path :: error code %s\n" , strerror(errno)) ; 
  }

 
  /* 
  int  i = 0 ; 
  
  while (  i <   modlive)     
  {
    printf("name -> %s \n" , (modules+i)->name) ; 
    i++ ;  
  }
  */ 

  DIR* sysmod =  _nullable ; 
  sysmod = fdopendir(sysmod_fd) ;  
  
  if (sysmod == _nullable) {
    close(sysmod_fd) ;   
    return   _nullable ; 
  }  
 
  memcpy(lsmod->root_path ,  gl_syspath , strlen(gl_syspath)) ; 
  
  struct dirent  * dirp = _nullable ;  
  
  uchar_t index   = 0 ;  
  //!  TODO  : create a generic function that read contents of directory 
  while (  (dirp = readdir(sysmod))  != _nullable )  { 
    
    if ( dirp->d_type  ==  DT_DIR ) { 
       //! catch only directory 
       explicit_bzero((lsmod->modules_names+index), MAX_LOADABLE_MDLS) ;
      
       uchar_t  match  = 0 ;
       while ( match < modlive ) 
       {
         if(strcmp((modules+match)->name  , dirp->d_name) !=  0) continue ; 
         lsmod_log("match -> %s" ,  dirp->d_name) ; 
         match++ ; 
       }
       memcpy( (lsmod->modules_names+index), dirp->d_name ,  strlen(dirp->d_name) ); 
       printf ("^s---> %s\n" , (lsmod->modules_names+index)); 
       index++ ;
       //printf ("^s---> %s\n" , (lsmod->modules_names+index)); 
    }
  }
 
  lsmod->modules_names[index+1]= _nullable ; 
 
  lsmod->total_of_module = index; 
  
  
  return   lsmod ; 
}

void lsmod_list_all_module_found ( const struct __lsmod_t * restrict   lsmod  ) 
{
  if ( lsmod  == _nullable) return  ; 
  
  uchar_t index =0  ;
  while  (  lsmod->modules_names[index]   != _nullable )    {
    
    lsmod_log("%s" , (lsmod->modules_names+index)) ; 
    index++; 
    
  }
  
  printf("-----\ntotal of kmod : %i \n", index) ; 
}


int  lsmod_count_loaded_modules (  const struct __lsmod_t *  lsmod)  
{
   __check_nonull(lsmod) ; 
  
   return  lsmod->total_of_module ;  
}


