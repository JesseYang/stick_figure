#pragma once

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define INVALID_OGL_VALUE 0xffffffff

#define GLCheckError() (glGetError() == GL_NO_ERROR)