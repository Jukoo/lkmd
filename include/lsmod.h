/** @file  lsmod.h 
 *  @brief lsmod command like with some extra  feature
 *    prototype definition and macros 
 *  @author Umar  Ba , LoopFocus ,jUmarB@protonmail.com 
 */
//! NOTE :  i use  strace  command to get some idea how the 
//!         command  work  

#if !defined  (_lsmod_extra_plus) 
#define        _lsmod_extra_plus

#define   _nullable  (void *) (0U << 1)  
#define   _Nullable  _nullable 

#define  __nrtrn    __attribute__((noreturn)) 
 
/**
 * Give default value when MAX_LOADABLE_MDLS is not set 
 * compile time flags  
 */ 

#ifndef  MAX_LOADABLE_MDLS
#define  MAX_LOADABLE_MDLS ( sizeof(void*) << 6 )  
#endif   //! MAX_LOADABLE_MDLS 


#define  lsmod_log(__mesg , ...) ({\
    char smesg[MAX_LOADABLE_MDLS]  ={ 0 } ; \
    sprintf(smesg , __mesg , ##__VA_ARGS__) ; \
    fprintf(stdout , "[%s]:: %s\n" ,"LSMOD" , smesg);\
    })

#define  lsmod_errx(__exit_code, __mesg , ...) ( {\
    if(__exit_code == 0) warn("%s@%i : exit_code Reserved for ESUCESS\n", __FILE__ ,__LINE__); \
    errx(__exit_code , __mesg , ##__VA_ARGS__) ; \
    }) 

#define  __check_nonull(x) if  (x == _Nullable)  \
                                   errx(~0 , "Abort") ; 

#ifndef  __gnu_linux__ 
#error "Reserved for GNU/Linux"  
#endif  //! __gnu_linux__

#define d_gnu_linux_sys_paths_modules  "/sys/module/" 
#define f_gnu_linux_sys_proc_modules  "/proc/modules" 

enum { 
  LSMOD_LINUX_SYSMOD,
#define  LSMOD_LINUX_SYSMOD  d_gnu_linux_sys_paths_modules 
  LSMOD_LINUX_PROCMOD 
#define  LSMOD_LINUX_PROCMOD f_gnu_linux_sys_proc_modules
} ; 


//! lsmod internal error code  
enum { 
  LSMOD_UKN_SYS_PATH   = ~2 ,
  LSMOD_DUP_STREAM_ERR = ~3 , 

} ;

enum { 
  LSMOD_MODULE_NAME , 
  LSMOD_MODULE_SIZE , 
  LSMOD_MODULE_XUSED, 
  LSMOD_MODULE_USEDBY , 
  //Live      - unused 
  //0x000000  - unused   
} ; 

typedef struct  __lsmod_t  lsmod_t ; 
struct  __lsmod_t  {
  char root_path[0x14] ; 
  char *modules_names[MAX_LOADABLE_MDLS] ; 
  int   total_of_module;
  
  struct __mod_t * modules ; 
} ; 


typedef  unsigned long int   modsize_t  ; 
typedef  unsigned char       uchar_t ; 

typedef  struct __mod_t  mod_t  ; 
struct __mod_t { 
  char name[MAX_LOADABLE_MDLS >> 3]  ; 
  modsize_t  size ;
  uchar_t n_usedby ; 
  char    m_usedby[MAX_LOADABLE_MDLS <<  1 ] ; 
} ; 

/** @fn  void *lsmod_syspath_open  (const char *  , struct __lsmod_t* ) ; 
 *  @brief read  proc module file (LSMOD_LINUX_PROCMOD)  to get  loaded  module name
 *  this function is used  to  make match between current loaded module 
 *  and modules sys path directory (LSMOD_LINUX_SYSMOD  
 *  @param void 
 *  @return  array of  mod_t *  struct 
 */ 
struct __mod_t *  lsmod_load_live_sysprocmod(void) ;


void *  lsmod_syspath_open (const char  *  __restrict__  __gnu_linux__syspath  , struct __lsmod_t *)  ; 


static  void *  lsmod_extract( const char * __buff  ,  struct __mod_t *  ) ; 
int  lsmod_count_loaded_modules (const struct  __lsmod_t * lsmod ) ; 
void   lsmod_list_all_module_found (const  struct __lsmod_t *__restrict__  lsmod) ;  

#endif /** _lsmod_extra_plus*/ 
