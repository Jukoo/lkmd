/** @file  lkmd.c 
 *  @brief lsmod command like with some extra  feature 
 *  @author Umar  Ba , LoopFocus ,jUmarB@protonmail.com 
 *  
 */

#include <endian.h>
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

static char resquest[0xff][0xff] ={0} ;  

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

void lkmd_get_raw_modules( const struct __lkmd_t *  lkmd ,  int m_size   , char  (*dumper)[MAX_LOADABLE_MDLS] )  
{
  /**  NOTE: depending  on parameter 
   *   -> load loaded module  or load all module from LKMD_LINUX_SYSMOD **/
 
  __check_nonull(lkmd) ; 
  __overflow_adjust(m_size , lkmd->total_of_module) ;  
  uchar_t index =0  ;
  

  //! That show all module from  LKMD_LINUX_SYSMOD 
  while  ( index  <  m_size  )  
  {   
    
    memcpy(dumper[index] , (raw_kmod+index) , strlen(raw_kmod[index]) ) ; 
    index++; 
  }
    //lkmd_log("%s",(char *)(raw_kmod+index)) ; 
  
}


struct __mod_request * lkmd_get_raw_modules_mrq (  const struct  __lkmd_t * lkmd  , int requested_size , struct  __mod_request * mrq )  { 
  __check_nonull(lkmd); 

  uint index = 0 ; 
 
  while  ( index < requested_size ) 
  { 
     memcpy( (mrq->dump_register+index) ,  (raw_kmod+index) , strlen(raw_kmod[index])+1) ; 
     index++ ; 
  }

  return mrq; 
}

void  lkmd_get_live_modules( const struct __lkmd_t  *  lkmd ,   int m_size , char  (*dumper)[MAX_LOADABLE_MDLS])     
{
  __check_nonull(lkmd->modules);

  int  index  = 0 ;  
  
  while ( index < m_size ) 
  {
    
    char *module_names =  (lkmd->modules+index)->name ; 
    memcpy( (dumper+index),  module_names, strlen(module_names)) ;  
    index++ ;  
  } 

}


struct __mod_request  * lkmd_get_live_modules_mrq(const struct __lkmd_t * lmkd  , int request_size ,  struct __mod_request  * mrq)   
{
  __check_nonull(lmkd->modules);
  
  int index = 0 ; 
  while ( index <  request_size) 
  {
    memcpy((mrq->dump_register+index), (lmkd->modules+index)->name, 0xff); 
    index++ ; 
  }

  memset ((mrq->dump_register+index+1) , 0 ,   0xff-request_size) ; 
  return mrq ;
}


void  lkmd_get_from_cb (const struct __lkmd_t  * lkmd , int  size  , char  (*dp)[MAX_LOADABLE_MDLS],  lkmd_cb_getter  lkmd_get_cb )  
{
  __check_nonull(lkmd) ; 
   
  lkmd_get_cb(lkmd , size ,  dp) ; 
}

/* 
void  lkmd_get ( const struct  __lkmd_t * lkmd , int type  , int size , char (*dp)[MAX_LOADABLE_MDLS] )   
{

  switch(type) 
  {
    case LKMD_RAW_ONLY : 
      lkmd_get_raw_modules(lkmd ,  size , dp) ;  
      break ; 
    case LKMD_LIVE_ONLY  : 
      lkmd_get_live_modules(lkmd, size , dp) ; 
      break ; 
    default : 
      //! thow error instead  ; 
      return ; 
      
  }
  
}*/ 

char *lkmd_get (const struct __lkmd_t * lkmd  , int type , int size , char (*dp)[MAX_LOADABLE_MDLS]) { 
  
  switch(type) 
  {
    case LKMD_RAW_ONLY : 
      lkmd_get_raw_modules(lkmd ,  size , dp) ;  
      break ; 
    case LKMD_LIVE_ONLY  : 
      lkmd_get_live_modules(lkmd, size , dp) ; 
      break ; 
    default : 
      //! thow error instead  ; 
      break ;   
      
  }

  return   (char *) dp ; 
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

void lkmd_list_dumper_contains(const unsigned char size, const char (*dumper)[MAX_LOADABLE_MDLS]) 
{
  unsigned char  index = 0 ; 
  while  ( index < size  ) 
  {
    lkmd_log("%s" , (char *)(dumper+index)) ; 
    index++ ; 
  }

}


void lkmd_enumerate_spc(char *restrict scp_lkmd ,  int limite) 
{
  __check_nonull(scp_lkmd); 
 
  uint j_index = 0 ;
  uint z_index = 0 ;

  char *scp_transaction  = malloc(MAX_LOADABLE_MDLS) ; 

  while (  (scp_lkmd+z_index)[0] != 0 ) 
  {
    if(limite ==0)
    {
      lkmd_log("%s", (scp_lkmd+z_index) )  ; 
      z_index+=MAX_LOADABLE_MDLS ; 
      continue ; 
    }
  
    
    if (j_index <  limite  ) 
    { 
      //modify change  
      
      lkmd_log("%s", (scp_lkmd+z_index) )  ;
      
      char  *tmp =  (scp_lkmd+z_index) ;
      memset((scp_transaction+j_index) , 0  , MAX_LOADABLE_MDLS) ; 
      memcpy((scp_transaction+j_index)  ,tmp , strlen(tmp) ); 
      
      z_index+=MAX_LOADABLE_MDLS ; 
      j_index++ ; 
    }else { 
      
      memset (  (scp_lkmd+z_index) ,  0 , MAX_LOADABLE_MDLS)  ; 
      memcpy(scp_lkmd , scp_transaction , MAX_LOADABLE_MDLS ) ;  
      break ; 
    } 
  }
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

