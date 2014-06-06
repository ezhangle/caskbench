#ifndef _CONFIG_H_
#define _CONFIG_H_

#cmakedefine HAVE_GLES2_H 1
#cmakedefine HAVE_GLES3_H 1
#cmakedefine HAVE_GLX_H 1
#cmakedefine HAVE_CAIRO_GL_H 1
#cmakedefine USE_SKIA 1
#cmakedefine USE_CAIRO 1
#cmakedefine USE_EGL 1
#cmakedefine USE_GLX 0
#cmakedefine USE_CAIROGLES 0

#define PACKAGE "${PROJECT_NAME}"
#define PACKAGE_NAME "${PROJECT_NAME}"
#define PACKAGE_VERSION "${PROJECT_VERSION}"
#define PACKAGE_STRING "${PROJECT_NAME} ${PROJECT_VERSION}"

// Workarounds for skia packaging bug
#define SK_ATOMICS_PLATFORM_H "ports/SkAtomics_sync.h"
#define SK_MUTEX_PLATFORM_H   "ports/SkMutex_pthread.h"
#define SK_ENABLE_INST_COUNT 0

#endif /* _CONFIG_H_ */
