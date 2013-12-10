#ifndef _CONFIG_H_
#define _CONFIG_H_

#cmakedefine HAVE_GLES2_H 1
#cmakedefine HAVE_GLES3_H 1
#cmakedefine HAVE_CAIRO_GL_H 1

#define PACKAGE "${PROJECT_NAME}"
#define PACKAGE_NAME "${PROJECT_NAME}"
#define PACKAGE_VERSION "${PROJECT_VERSION}"
#define PACKAGE_STRING "${PROJECT_NAME} ${PROJECT_VERSION}"

#endif /* _CONFIG_H_ */
