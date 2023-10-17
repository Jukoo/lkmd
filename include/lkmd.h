/** @file  lkmd.h 
 *  @brief lkmd command like with some extra  feature
 *    prototype definition and macros 
 *  @author Umar  Ba , LoopFocus ,jUmarB@protonmail.com 
 */

#if !defined  (_lkmd) 
#define        _lkmd

#include <sys/types.h> 

#define   __USE_MISC 

#define  ATOMCHG(x) x^=1  

typedef  void *  __void  ;  
#define   _nullable    (( __void  ) (0U << 1)) 
#define   _Nullable  _nullable  
#define   _void_0h   _nullable 


#define  __nrtrn         __attribute__((noreturn))
#define  __alwys_unused  __attribute__((__unused__))

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


#ifdef SHOW_LKMD_TAG  


#define  LKMD_BANNER \
  printf("\t%-15s\t\t\t%-15s\t%-15s\n" , "Modules" , "Size"  , "Used By") ;\
  puts("------------------------------------------------------------------");


#define  lkmd_log(__mesg , ...) ({\
    char smesg[MAX_LOADABLE_MDLS]  ={ 0 } ; \
    sprintf(smesg , __mesg , ##__VA_ARGS__) ; \
    fprintf(stdout , "[%s]:: %s\n" ,LKMD_TAG , smesg);\
    })

#define  lkmd_logi(index , __mesg  , ... ) ({\
    char smesg[MAX_LOADABLE_MDLS] = {0} ; \
    sprintf(smesg , __mesg ,##__VA_ARGS__);\
    fprintf(stdout , "[%s]:%i: %-15s\n" ,LKMD_TAG , index , smesg );\
    })


#else 


#define  LKMD_BANNER \
  printf("\t%-5s\t\t\t%-5s\t\t%-5s\n" , "Modules" , "Size"  , "Used By") ;\
  puts("------------------------------------------------------------------");


#define  lkmd_log(__mesg , ...) ({\
    fprintf(stdout , __mesg   , ##__VA_ARGS__);\
    })

#define  lkmd_logi(__index , __mesg  , ... ) ({\
    char smesg[MAX_LOADABLE_MDLS] =  {0} ;\
    sprintf(smesg ,  ":%i: %-15s\n", __index , __mesg);\
    fprintf(stdout , smesg , ##__VA_ARGS__);\
    })


#endif //! SHOW_LKMD_TAG  

#define  lkmd_errx(__exit_code, __mesg , ...) ( {\
    if(__exit_code == 0) warn("%s@%i : exit_code Reserved for EXIT_SUCCESS\n", __FILE__ ,__LINE__); \
    errx(__exit_code , __mesg , ##__VA_ARGS__) ; \
    })

#define  lkmd_warn( __mesg , ... ) ( { \
    warn(__mesg ,  ##__VA_ARGS__) ; \
    })

#define  __check_nonull(x) if  (x == _Nullable)  errx(~0 , "Abort") ; 
#define  __overflow_adjust(x,y) if (x > y){ warn("OverFlow Adjusting !"); x = y ;  x-=1;} 




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
  LKMD_RAW_ONLY, 
  LKMD_NONE_LIVE_ONLY
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
  //Live      - unused  //  not supported yet 
  //0x000000  - unused  //  not supported yet
} ; 


/**  raws module based on /sys/module  or  LKMD_LINUX_SYSMOD | LKMD_LINUX_SYSMOD_RAWS */
typedef struct  __lkmd_raw_t  lkmd_raw_t ; //raw modules  lkmd_raw_t  
struct  __lkmd_raw_t  {
  char *modules_names[MAX_LOADABLE_MDLS] ; 
  int   total_of_module;
  
  int   total_of_live_module; 
  struct __lkmd_live_t * modules ; 
} ; 


typedef  unsigned long int   modsize_t  ; 
typedef  unsigned char       uchar_t ; 


/** 
 *  representation of  live modules in /proc/modules 
 */ 
typedef  struct __lkmd_live_t  lmkd_live_t  ; //! live  modules  lkmd_live_t   
struct __lkmd_live_t { 
  char name[MAX_LOADABLE_MDLS >> 3]  ; 
  modsize_t  size ;
  uchar_t n_usedby ; 
  char    m_usedby[MAX_LOADABLE_MDLS <<  1 ] ; 
} ;


/**  module  request query */  

typedef  struct  __mod_request mrq_t  ; 
struct  __mod_request { 
  char  dump_register[MAX_LOADABLE_MDLS][MAX_LOADABLE_MDLS] ; 
  int   size  ; 
#if defined (LKMD_MRQ_AS_NODE) 
  /**  when  LKMD_MRQ_AS_NODE is defined the mrq  nodes are liked  */
  struct __mod_request * _next ; 
#endif
  /**  detect when allocation happen */
  struct __mod_request * check_mall ;   
} ; 

typedef  void (*lkmd_cb_getter) (const  struct __lkmd_raw_t  *  , int _size ,  char (*)[MAX_LOADABLE_MDLS] ) ;
typedef  void (*lkmd_cb_getter_mrq) (const  struct __lkmd_raw_t * ,  struct  __mod_request *  __mrqt)  ;   


typedef struct __lmkd_t  lkmd_t ; 

extern  inline void  __nrtrn lkmd_exit( int  __sc  ) { 
  _Exit(__sc) ; 
}

/**  @fn struct   __mod_request  *  mrq_set ( void * , int new_init_size  )  
 *   @brief  initialize the mrq_t  and set new value to size 
 *           that be used  to match  dump_register  size 
 *          
 *   @param  void *    when   null that make and new allocation  of mrq 
 *           otherwise it just use it as an normal no allocation 
 *   @param  int new_init_size  for size
 *   @return struct __mod_request *  ; 
 *
 */ 
struct __mod_request * mrq_set(  void  *   , int new_size )  ;  

void mrq_checkmall  ( struct __mod_request * mrq )  ; 

/** @fn  __lkmd_live_t  *lkmd_load_live_sysprocmod (void) ; 
 *  @brief read  proc module file (LKMD_LINUX_PROCMOD) and feed  _lkmd_live_t  
 *  @param void 
 *  @return  __lkmd_live_t * 
 */ 
static struct __lkmd_live_t *  lkmd_load_live_sysprocmod(void)  /** __attribute__((malloc))**/;

/** @fn  __lkmd_t * lmkd_syspath_open(const char * , struct __lkmd_t *)  
 *  @brief open  (LKMD_LINUX_SYSMOD) and  feed  __lkmd_raw_t  data structure   
 *  @param  struct __lkmd_t * 
 *  @return struct __lkmd_t *  
 * 
 *  @todo : give an apropriate name 
 */ 
struct __lkmd_t *  lkmd_syspath_open (struct __lkmd_t * __lmkd )  ; 

/** @fn static void * lkmd_extract( const char * , struct __lkmd_live_t * )  
 *  @brief extract the needed information  from  the inline buffer 
 *  @param  const char  * __inline_buffer 
 *  @param  struct __lkmd_live_t *  ( see  struct  __lkmd_live_t data structure ) 
 *  @return  void *  
 *
 */ 
static  void *  lkmd_extract( const char * __inline_buffer  ,  struct __lkmd_live_t *  ) ; 

/** @fn  void lkmd_get_raw_modules( const struct __lkmd_raw_t *  , int , char (*)[MAX_LOADABLE_MDLS])  
 *  @brief list all modules found in LKMD_LINUX_SYSMOD 
 *
 *  @param const struct __lkmd_raw_t *
 *  @param int size    
 *  @param char (*)[MAX_LOADABLE_MDLS] 
 */ 
void   lkmd_get_raw_modules (const  struct __lkmd_raw_t * _lkmd ,  int size ,  char (*dp)[MAX_LOADABLE_MDLS]) ;  
struct __mod_request *lkmd_get_raw_modules_mrq(const struct  __lkmd_raw_t * _lkmd , struct __mod_request * ) ;  

/** @fn  void lkmd_get_live_modules( const struct __lkmd_live_t *  , int , char (*)[MAX_LOADABLE_MDLS])  
 *  @brief list all modules found in LKMD_LINUX_PROCMOD   
 *
 *  @param const struct __lkmd_live_t *
 *  @param int size    
 *  @param char (*)[MAX_LOADABLE_MDLS] 
 */ 
void lkmd_get_live_modules(const struct __lkmd_raw_t * __restrict__  lkmd , int m_size , char  (*__dumper)[MAX_LOADABLE_MDLS] ) ;
struct __mod_request *lkmd_get_live_modules_mrq (const struct __lkmd_raw_t* __restrict__ lkmd , struct __mod_request *  ) ;  


void  lkmd_get_from_cb(const struct  __lkmd_raw_t  * _lkmd , int size ,  char (*dp)[MAX_LOADABLE_MDLS]  , lkmd_cb_getter );   

char * lkmd_get (const struct __lkmd_t *  ,  int type , int size ,  char (*dp)[MAX_LOADABLE_MDLS]); 

char * lkmd_get_from_mrq ( const struct __lkmd_t * ,int type , struct  __mod_request * ) ;  

/** @fn lkmd_count_loaded_modules  (const struct __lkmd_raw_t * ) 
 *  @brief count all modules available 
 *  
 *  @param const struct __lkmd_raw_t * 
 *  @return  int  - number of all modules  even not loaded 
 */ 

int  lkmd_count_loaded_modules (const struct  __lkmd_raw_t * __restrict__ __lkmd ) ;
int  lkmd_count_live_modules ( const struct __lkmd_raw_t * __restrict__ __lkmd ) ; 

void lkmd_show_lkmod(const struct __lkmd_t * __restrict__ __kmod  ,  int nrows )  ; 

/** @fn lkmd_release ( struct __lkmd_raw_t *)
 *  @brief clean allocated  lmkd_live_t a.k.a  struct  __lkmd_live_t 
 *
 *  @param struct  __lkmd_raw_t * 
 *  @return void * should be null  for Success 
 */ 
void * lkmd_release(struct __lkmd_t * __restrict__ __lkmdlive); 

//!show dumper on stdout ,  
void lkmd_list_dumper_contains ( const  unsigned char  size ,  const char (*dumper)[size]) ; 

void lkmd_splice(char * __restrict__   ,  int  blimit) ; 

void lkmd_splice_show( const char  * __restrict__ ) ;  



#endif /** _lkmd*/ 
