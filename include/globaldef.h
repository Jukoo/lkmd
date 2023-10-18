/*   @file globaldef.h 
     @brief  global definition of prerprocessor macro 
     @Copyright (C) 2023 Umar Ba jUmarB@protonmail.com  , OpenWire Studio .Lab 

     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.
 */ 


#pragma once 
#ifndef  GLOBLADEF_H   
#define  GLOBLADEF_H  1 

#include <sys/types.h>

#ifndef  __USE_MISC  
#define   __USE_MISC
#endif 


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




#endif
