/* $Revision: 2 $ */
#ifndef VERSION

#ifdef V4_COMPAT
#define VERSION "MATLAB 4 compatible"
#else
#define VERSION "MATLAB R12 native"
#endif /* V4_COMPAT */

#endif /* VERSION */

#ifdef ARRAY_ACCESS_INLINING
#define INLINE " (inlined)"
#else
#define INLINE
#endif /* ARRAY_ACCESS_INLINING */

#ifdef MCC_GENERATED
#define COMPILERGEN " (MATLAB Compiler Generated)"
#else
#define COMPILERGEN
#endif /* MCC_GENERATED */

static const char *version = VERSION INLINE COMPILERGEN;

#ifdef __cplusplus
extern "C" {
#endif

const char *mexVersion (void);

const char *mexVersion (void) {
    /* mex version information */
    return version;
}

#ifdef __cplusplus
}
#endif

