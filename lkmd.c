/** @file  lkmd.c 
 *  @brief lsmod command like with some extra  feature 
 *  @author Umar  Ba , LoopFocus ,jUmarB@protonmail.com 
 *  
 */

#include <stdlib.h> 
#include <strings.h>
#include <sys/cdefs.h>
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

// ! should be removed later then ... 
static int kmodlive = 0;

static  char raw_kmod[MAX_LOADABLE_MDLS][MAX_LOADABLE_MDLS] ={0} ;

static char resquest[0xff][0xff] ={0} ;  

static struct  __lkmd_live_t *lkmd_load_live_sysprocmod (void) 
{
    
    int  pmod_fd  = open ( LKMD_LINUX_PROCMOD , O_RDONLY ) ; 
    
    if (pmod_fd  <= ~0 ) { 
      errx ( pmod_fd  ,  "OPEN STREAM  ERROR") ; 
    }
    
    if (  dup2(pmod_fd , STDIN_FILENO)  <= ~0 ) 
    {
      lkmd_errx(LKMD_DUP_STREAM_ERR , "stream duplication error") ; 
    }
   
    struct __lkmd_live_t  * modules_collection = ( struct __lkmd_live_t *) malloc(sizeof(*modules_collection)  * MAX_LOADABLE_MDLS) ;  
    
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
static void * lkmd_extract(const char * inbuff  , struct  __lkmd_live_t * modt) 
{

  char *local_buff = (char *)inbuff ; 
  char sep[2]= { 0x20  , 0 } ;  
  
  char *token  = strtok(local_buff ,  sep)  ; 

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


struct __lkmd_t *  lkmd_syspath_open(struct __lkmd_t * restrict lkmd_subt )    
{

  lkmd_subt = malloc(sizeof(struct  __lkmd_raw_t )) ; 
  
  if (lkmd_subt  == _void_0h ) 
    lkmd_errx(~1, "lkmd_syspath_open ! __init : bab allocation"); 
  

  struct __lkmd_raw_t  *lkmd =  (struct  __lkmd_raw_t*)  lkmd_subt ;  
  
  
  char sysmpath[MAX_LOADABLE_MDLS]  = LKMD_LINUX_SYSMOD ; 


  lkmd->modules = lkmd_load_live_sysprocmod();  
  lkmd->total_of_live_module = 0 ; 
  
  int  sysmod_fd =   open (LKMD_LINUX_SYSMOD ,  O_RDONLY) ; 
  if (sysmod_fd <=~0)  { 
    lkmd_errx(LKMD_UKN_SYS_PATH  , "Not a Valid Path :: error code %s\n" , strerror(errno)) ; 
  }

  DIR* sysmod =  _nullable ; 
    
  sysmod = fdopendir(sysmod_fd) ;  
  
  if (sysmod == _nullable) {
    close(sysmod_fd) ;   
    return   _nullable ; 
  } 

  
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
           //+ just  to make sure current loaded modules are  in present in raw modules 
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
 

  return   lkmd_subt;  
}

void lkmd_get_raw_modules( const struct __lkmd_raw_t *  lkmd ,  int m_size   , char  (*dumper)[MAX_LOADABLE_MDLS] )  
{
  /**  NOTE: depending  on parameter 
   *   -> load loaded module  or load all module from LKMD_LINUX_SYSMOD **/
 
  __check_nonull(lkmd) ;
  __overflow_adjust(m_size ,  MAX_LOADABLE_MDLS) ; 
  uchar_t index =0  ;
  

  //! That show all module from  LKMD_LINUX_SYSMOD  |  LKMD_LINUX_PROCMOD_RAWS  
  while  ( index  <  m_size  )  
  {   
    
    memcpy(dumper[index] , (raw_kmod+index) , strlen(raw_kmod[index]) ) ; 
    index++; 
  }
    //lkmd_log("%s",(char *)(raw_kmod+index)) ; 
  
}


struct __mod_request * lkmd_get_raw_modules_mrq (  const struct  __lkmd_raw_t * lkmd  , struct  __mod_request * mrq )  { 
  __check_nonull(lkmd); 
  __overflow_adjust(mrq->size,  MAX_LOADABLE_MDLS) ; 

  uint index = 0 ; 
 
  while  ( index < mrq->size ) 
  { 
     memcpy( (mrq->dump_register+index) ,  (raw_kmod+index) , strlen(raw_kmod[index])+1) ; 
     index++ ; 
  }

  return mrq; 
}

void  lkmd_get_live_modules( const struct __lkmd_raw_t  *  lkmd ,   int m_size , char  (*dumper)[MAX_LOADABLE_MDLS])     
{
  __check_nonull(lkmd->modules);
  __overflow_adjust(m_size ,  MAX_LOADABLE_MDLS) ; 

  int  index  = 0 ;  
  
  while ( index < m_size ) 
  {
    
    char *module_names =  (lkmd->modules+index)->name ; 
    memcpy( (dumper+index),  module_names, strlen(module_names)) ;  
    index++ ;  
  }



}


struct __mod_request  * lkmd_get_live_modules_mrq(const struct __lkmd_raw_t * lmkd  , struct __mod_request  * mrq)   
{
  __check_nonull(lmkd->modules);
  __overflow_adjust(mrq->size,  MAX_LOADABLE_MDLS) ; 

  int index = 0 ; 
  while ( index <  mrq->size) 
  {
    memcpy((mrq->dump_register+index), (lmkd->modules+index)->name, 0xff); 
    index++ ; 
  }

  memset ((mrq->dump_register+index+1) , 0 ,   0xff-mrq->size) ; 
  return mrq ;
}


void  lkmd_get_from_cb (const struct __lkmd_raw_t  * lkmd , int  size  , char  (*dp)[MAX_LOADABLE_MDLS],  lkmd_cb_getter  lkmd_get_cb )  
{
  __check_nonull(lkmd) ; 
   
  lkmd_get_cb(lkmd , size ,  dp) ; 
}



char *lkmd_get (const struct __lkmd_t * lkmod  , int type , int size , char (*dp)[MAX_LOADABLE_MDLS]) { 
 
  
  struct __lkmd_raw_t  *lkmd = (struct __lkmd_raw_t *) lkmod ;   
  
  
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

char * lkmd_get_from_mrq (const struct __lkmd_t *  lkmod  , int type , struct __mod_request  * mrq) { 
  
  return lkmd_get(lkmod ,  type , mrq->size ,mrq->dump_register) ;  
}

int  lkmd_count_loaded_modules (  const struct __lkmd_raw_t *  lkmd)  
{
   __check_nonull(lkmd) ; 
  
   return  lkmd->total_of_module ; 
}

int lkmd_count_live_modules(const struct __lkmd_raw_t *restrict lkmd)
{
  __check_nonull(lkmd);
  __check_nonull(lkmd->modules); 
  return lkmd->total_of_live_module ; 
}

void  * lkmd_release( struct __lkmd_t *  restrict  kmod  )  
{
   __check_nonull(kmod);
   
   struct __lkmd_raw_t  *lkmd = ( struct __lkmd_raw_t* )kmod ; 
   __check_nonull(lkmd) ;  
   __check_nonull(lkmd->modules) ; 
   
   free(lkmd->modules) ;  
   free(kmod) ;  


   return  lkmd->modules ; // ! it's should be nil  
}

void lkmd_show_lkmod( const struct __lkmd_t   * kmodlive , int nrows ) 
{
  __check_nonull(kmodlive);

  struct __lkmd_raw_t *  lmkd = (struct __lkmd_raw_t *)  kmodlive ; 
  

  LKMD_BANNER ; 

  int rows_index = 0 ; 
  if(nrows   <= 0 ) 
    nrows =  lmkd->total_of_live_module ;   

  while  ( rows_index  < nrows )  
  {
    //! display 
    lkmd_logi(rows_index+1 , "%-15s\t\t%-15li\t%i %-15s", (lmkd->modules+rows_index)->name , (lmkd->modules+rows_index)->size , (lmkd->modules+rows_index)->n_usedby ,  (lmkd->modules+rows_index)->m_usedby); 
    rows_index++ ; 
  }
  
  
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


void lkmd_splice(char *restrict scp_lkmd ,  int   blimit) 
{
  __check_nonull(scp_lkmd); 
 
  uint j_index = 0 ;
  uint z_index = 0 ;

  while (  (scp_lkmd+z_index)[0] != 0 ) 
  {
    if(blimit  ==0)
    { 
      lkmd_logi( j_index+1  ,"%s", (scp_lkmd+z_index) )  ;
      
      //!like jump  , each index  is set to MAX_LOADABLE_MDLS  bytes 
      //[0] ="data<0:MAX_LOADABLE_MDLS>...[1] ="data<1:MAX_LOADABLE_MDLS>..." 
      //z_index +MAX_LOADABLE_MDLS> ....  ,  
      z_index+=MAX_LOADABLE_MDLS ; 
      j_index++ ; 
      continue ; 
    }
  
    int  abs_limit =  abs((int)blimit)  ; 
    if (j_index <  abs_limit  ) 
    { 
      z_index+=MAX_LOADABLE_MDLS ; 
      j_index++ ; 
    }else {        
       /** 
        * !FIXME : TODO  : 
        *  -------------- 
        *  like  splice function in other programming  language 
        * 
       if (blimit <= ~0) 
       {  
         char *scp_transaction = (char *)(scp_lkmd+z_index) ;  
        
         explicit_bzero(scp_lkmd,  MAX_LOADABLE_MDLS) ;  

         memcpy((char *)  scp_lkmd , scp_transaction  ,  MAX_LOADABLE_MDLS) ; 
        
       }else 
       */ 

         memset ( (scp_lkmd +  z_index) ,  0 , MAX_LOADABLE_MDLS)  ; 

      break ; 
    } 
  }

  
}


void lkmd_splice_show ( const  char *  scp_lkmd  )
{ 
  char *scp=(char*) scp_lkmd ; 
  lkmd_splice(scp ,0)  ; 
}


/**
 * TODO :  
 * []  show live module  like  lsmod   
 * NOTE :
 * lkmd -n --lines   x  
 * lmkd -g --goto    x 
 * lmkd -i --interval  x:y   [ even if  the  y number  is greater than x   ]
 * lmkd -h --human-readable  < human readable> 
 * lkmd -f --find  module  
 * lkmd -I --info info module  [ if no module is specified ] that check the current workdir ] 
 * lkmd -v --version version of lkmd 
 * lkmd -h help version 
 * lkmd -m --kmesg --kernel-mesg  or -dmesg 
 *
 */ 

