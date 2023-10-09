/** @file  lkmd.h 
 *  @brief lkmd command like with some extra  feature
 *    prototype definition and macros 
 *  @author Umar  Ba , LoopFocus ,jUmarB@protonmail.com 
 */

#if !defined  (_lkmd) 
#define        _lkmd

typedef  void *  __void  ;  
#define   _nullable    (( __void  ) (0U << 1)) 
#define   _Nullable  _nullable  
#define   _void_0h   _nullable 


#define  __nrtrn    __attribute__((noreturn)) 

/**  debug print */ 
#ifdef   __dbgprt  
#define  dbgprt  printf("<< dbprt  mark @ \x1b[1;36m %s \x1b[0m : __line__ number  \x1b[1;32m %i \x1b[0m \n", __FILE__ , __LINE__) ; 
#else 
#define  dbgprt  
#endif //__dbgprt   

/**
 * Give default value when MAX_LOADABLE_MDLS is not set 
 * compile time flags  
 */ 
#ifndef  MAX_LOADABLE_MDLS
#define  MAX_LOADABLE_MDLS ( sizeof(__void)  << 6 )  
#endif   

#define LKMD_TAG   "LKMD"

#define HIDDIRENT  (0x2e&0xff)  


#define  lkmd_log(__mesg , ...) ({\
    char smesg[MAX_LOADABLE_MDLS]  ={ 0 } ; \
    sprintf(smesg , __mesg , ##__VA_ARGS__) ; \
    fprintf(stdout , "[%s]:: %s\n" ,LKMD_TAG , smesg);\
    })

#define  lkmd_errx(__exit_code, __mesg , ...) ( {\
    if(__exit_code == 0) warn("%s@%i : exit_code Reserved for ESUCESS\n", __FILE__ ,__LINE__); \
    errx(__exit_code , __mesg , ##__VA_ARGS__) ; \
    })

#define  lkmd_warn( __mesg , ... ) ( { \
    warn(__mesg ,  ##__VA_ARGS__) ; \
    })

#define  __check_nonull(x) if  (x == _Nullable)  errx(~0 , "Abort") ; 
#define  __overflow_adjust(x,y) if (x > y){ warn("OverFlow Adjusting !"); x = y ;} 




#define d_gnu_linux_sys_paths_modules  "/sys/module/" 
#define f_gnu_linux_sys_proc_modules  "/proc/modules" 

enum { 
  LKMD_LINUX_SYSMOD,
#define  LKMD_LINUX_SYSMOD  d_gnu_linux_sys_paths_modules 
  LKMD_LINUX_SYSMOD_RAWS, 
#define LKMD_LINUX_SYSMOD_RAWS LKMD_LINUX_SYSMOD 
  LKMD_LINUX_PROCMOD 
#define  LKMD_LINUX_PROCMOD f_gnu_linux_sys_proc_modules
} ; 

enum { 
  LKMD_LINUX_RAW_MODULE ,
  LKMD_LINUX_MATCH_LIVE_MODULE
};


enum { 
  LKMD_LIVE_ONLY , 
  LKMD_RAW_ONLY 
}; 

//! lkmd internal error code  
enum { 
  LKMD_UKN_SYS_PATH   = ~2 ,
  LKMD_DUP_STREAM_ERR = ~3 , 

} ;

enum { 
  LKMD_MODULE_NAME , 
  LKMD_MODULE_SIZE , 
  LKMD_MODULE_XUSED, 
  LKMD_MODULE_USEDBY , 
  //Live      - unused 
  //0x000000  - unused   
} ; 


/**  raws module based on /sys/module  or  LKMD_LINUX_SYSMOD | LKMD_LINUX_SYSMOD_RAWS */
typedef struct  __lkmd_t  lkmd_t ; //raw modules  lkmd_raw_t  
struct  __lkmd_t  {
  char root_path[0x14] ; 
  char *modules_names[MAX_LOADABLE_MDLS] ; 
  int   total_of_module;
  
  int   total_of_live_module; 
  struct __mod_t * modules ; 
} ; 


typedef  unsigned long int   modsize_t  ; 
typedef  unsigned char       uchar_t ; 


/** 
 *  representation of  live modules in /proc/modules 
 */ 
typedef  struct __mod_t  mod_t  ; //! live  modules  lkmd_live_t   
struct __mod_t { 
  char name[MAX_LOADABLE_MDLS >> 3]  ; 
  modsize_t  size ;
  uchar_t n_usedby ; 
  char    m_usedby[MAX_LOADABLE_MDLS <<  1 ] ; 
} ;


typedef  void (*lkmd_cb_getter) (const  struct __lkmd_t  *  , int ,  char (*)[MAX_LOADABLE_MDLS] ) ; 


typedef  struct  __mod_request mrq_t  ; 
struct  __mod_request { 
  char  dump_register[MAX_LOADABLE_MDLS][MAX_LOADABLE_MDLS] ; 
  int   size  ;  
} ; 


/** @fn  void *lkmd_syspath_open  (const char *  , struct __lkmd_t* ) ; 
 *  @brief read  proc module file (LKMD_LINUX_PROCMOD)  to get  loaded  module name
 *  this function is used  to  make match between current loaded module 
 *  and modules sys path directory (LKMD_LINUX_SYSMOD  
 *  @param void 
 *  @return  array of  mod_t *  struct 
 */ 
static struct __mod_t *  lkmd_load_live_sysprocmod(void) ;

/** @fn  l  
 *
 */ 
struct __lkmd_t *  lkmd_syspath_open (const char  * __restrict__  __syspath  , struct __lkmd_t *)  ; 

/** @fn static void * lkmd_extract( const char * , struct __mod_t * )  
 *  @brief extract the need information from __buff  parameter 
 *         which is a single line  and dump it into  __mod_t structure 
 *  @param  const char  * __inline_buffer 
 *  @param  struct __mod_t *  ( see  struct  __mod_t data structure ) 
 *  @return  void *  
 *
 */ 
static  void *  lkmd_extract( const char * __inline_buffer  ,  struct __mod_t *  ) ; 

/** @fn  void lkmd_list_all_module_found( const struct __lkmd_t * ) 
 *  @brief list all modules found in the host even the unloaded  modules 
 *
 *  @param const struct __lkmd_t *  
 *  @return void 
 */ 
void   lkmd_get_raw_modules (const  struct __lkmd_t * _lkmd ,  int size ,  char (*dp)[MAX_LOADABLE_MDLS]) ; 

struct __mod_request *lkmd_get_raw_modules_mrq(const struct  __lkmd_t * _lkmd ,  int requested_size , struct __mod_request * ) ;  

void lkmd_get_live_modules(const struct __lkmd_t * __restrict__  lkmd , int m_size , char  (*__dumper)[MAX_LOADABLE_MDLS] ) ;
//char (*(lkmd_get_live_modules_wr) (const struct __lkmd_t* __restrict__ lkmd , int __request_size ))[0xff] ; 
struct __mod_request *lkmd_get_live_modules_mrq (const struct __lkmd_t* __restrict__ lkmd , int __request_size ,   struct __mod_request *  ) ;  


void  lkmd_get_from_cb(const struct  __lkmd_t  * _lkmd , int size ,  char (*dp)[MAX_LOADABLE_MDLS]  , lkmd_cb_getter );   

//void  lkmd_get(const struct __lkmd_t *  ,  int type , int size ,  char (*dp)[MAX_LOADABLE_MDLS]) ; 
char  * lkmd_get (const struct __lkmd_t *  ,  int type , int size ,  char (*dp)[MAX_LOADABLE_MDLS]);  

/** @fn lkmd_count_loaded_modules  (const struct __lkmd_t * ) 
 *  @brief count all modules available 
 *  
 *  @param const struct __lkmd_t * 
 *  @return  int  - number of all modules  even not loaded 
 */ 

int  lkmd_count_loaded_modules (const struct  __lkmd_t * __restrict__ __lkmd ) ;
int  lkmd_count_live_modules ( const struct __lkmd_t * __restrict__ __lkmd ) ; 


/** @fn lkmd_release ( struct __lkmd_t *)
 *  @brief clean allocated  mod_t a.k.a  struct  __mod_t 
 *
 *  @param struct  __lkmd_t * 
 *  @return void * should be null  for Success 
 */ 
void * lkmd_release(struct __lkmd_t * __restrict__ __lkmd); 

//!show dumper on stdout ,  
void lkmd_list_dumper_contains ( const  unsigned char  size ,  const char __dumper[][size] ) ; 

void lkmd_enumerate_spc(char * __restrict__   ,  int limit) ;   //spc  signle pointor char 
                                                       

#endif /** _lkmd*/ 
