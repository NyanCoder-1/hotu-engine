#ifndef __UTILITY_ALIGN_H__
#define __UTILITY_ALIGN_H__

// align_if from cglm
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L)
# include <stdalign.h>
#endif
#if defined(_MSC_VER)
/* do not use alignment for older visual studio versions */
/* also ARM32 also causes similar error, disable it for now on ARM32 too */
#  if _MSC_VER < 1913 || _M_ARM /*  Visual Studio 2017 version 15.6  */
#    define ALIGN(X) /* no alignment */
#  else
#    define ALIGN(X) __declspec(align(X))
#  endif
#else
#  define ALIGN(X) __attribute((aligned(X)))
#endif

#ifdef __AVX__
#define ALIGN_MAT ALIGN(32)
#else
#define ALIGN_MAT ALIGN(16)
#endif

#endif
