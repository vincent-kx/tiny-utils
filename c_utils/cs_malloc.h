#ifndef __CS_MALLOC_H_20181016__
#define __CS_MALLOC_H_20181016__

#include <stdio.h>

#if defined(USE_TCMALLOC)

	#include <google/tcmalloc.h>
	#define malloc(size) tc_malloc(size)
	#define calloc(count,size) tc_calloc(count,size)
	#define realloc(ptr,size) tc_realloc(ptr,size)
	#define free(ptr) tc_free(ptr)
#endif

#endif