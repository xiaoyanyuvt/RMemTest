#include <pthread.h>
#include <cstdint>
#include <dlfcn.h>
#include <stdio.h>
#include <R.h>
#include <Rinternals.h>
#include <Rembedded.h>
#include <Rinterface.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>

int testRMem(){
  void *hRLib=dlopen("libR.so", RTLD_LAZY);
  if(hRLib)
  {	  void *hRlpk=dlopen("libRlapack.so", RTLD_LAZY);
	  if(!hRlpk)
		  return 1;
  }else
	  return 1;
  std::cout <<"start R..."<<std::endl;

  typedef int (*pRf_initialize_R)(int ac, char **av);
pRf_initialize_R	 m_Rf_initialize_R_ptr = (pRf_initialize_R)dlsym(hRLib, "Rf_initialize_R");
  	char *localArgs[] = {"R", "--slave","--silent"};
   int lInitR = m_Rf_initialize_R_ptr(sizeof(localArgs)/sizeof(localArgs[0]), localArgs);
   if(lInitR < 0)
	   return 1;
   std::cout << "set stack limit ..."<<std::endl;
   uintptr_t *m_R_CStackLimit_ptr = (uintptr_t *)dlsym(hRLib, "R_CStackLimit");
		
   *m_R_CStackLimit_ptr = (uintptr_t) -1;

   std::cout << "after set stack limit ..."<<std::endl;
// set session to 'not interactive'
   Rboolean * m_R_Interactive_ptr = (Rboolean *)dlsym(hRLib, "R_Interactive");
		*m_R_Interactive_ptr = FALSE;
    typedef void (*p_setup_Rmainloop)(void);
    p_setup_Rmainloop	m_setup_Rmainloop_ptr;
    m_setup_Rmainloop_ptr = (p_setup_Rmainloop)dlsym(hRLib, "setup_Rmainloop");

		m_setup_Rmainloop_ptr();
   std::cout << "after setup main loop ..."<<std::endl;
 
   std::cin.get();
   SEXP pRVector;
    int lProtectCnt =0, nSize=1000000;
    typedef SEXP (*pRf_protect)(SEXP);
pRf_protect m_Rf_protect_ptr = (pRf_protect)dlsym(hRLib, "Rf_protect");

typedef SEXP (*pRf_allocVector)(SEXPTYPE, R_len_t);
pRf_allocVector	m_Rf_allocVector_ptr = (pRf_allocVector)dlsym(hRLib, "Rf_allocVector");
		
     typedef void (*pRf_unprotect)(int);
     pRf_unprotect m_Rf_unprotect_ptr = (pRf_unprotect)dlsym(hRLib, "Rf_unprotect");
		
     
   std::cout << "start to create vector ..."<<std::endl;
   m_Rf_protect_ptr(pRVector = m_Rf_allocVector_ptr(REALSXP, nSize));				
   // pRVector = m_Rf_allocVector_ptr(STRSXP, nSize);				
   std::cout << "end to create vector ..."<<std::endl;
     lProtectCnt++;
   m_Rf_unprotect_ptr(lProtectCnt);
     
     
   std::cout << "before free R..."<<std::endl;
typedef    void (*pRf_endEmbeddedR)(int fatal);
pRf_endEmbeddedR m_Rf_endEmbeddedR_ptr = (pRf_endEmbeddedR)dlsym(hRLib, "Rf_endEmbeddedR");
 m_Rf_endEmbeddedR_ptr(0);
  if(hRLib)
   {
		// free the DLL module
		dlclose(hRLib);
		hRLib=NULL;
    }
}

int main(int argc, char *argv[])
{
 std::cin.get();
  testRMem();
 std::cin.get();
  return 0;

}
