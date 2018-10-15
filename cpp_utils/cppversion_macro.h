#ifndef __CPP_VERSION_MACRO_H__
#define __CPP_VERSION_MACRO_H__

	#define SUPPORT_CPP11 (__cplusplus >= 201103L)
	#if (SUPPORT_CPP11)
		#warning C++11 supported,cpp version
	#else
		#warning C++11 not supported,cpp version
	#endif
#endif
