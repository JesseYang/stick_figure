#pragma once

#define SNPRINTF snprintf

#include "types.h"
#include "callbacks.h"

void GLUTBackendInit(int argc, char** argv, bool WithDepth, bool WithStencil);


bool GLUTBackendCreateWindow(uint Width, uint Height, bool isFullScreen, const char* pTitle);

void GLUTBackendRun(ICallbacks* pCallbacks);

void GLUTBackendSwapBuffers();

void GLUTBackendLeaveMainLoop();

OGLDEV_KEY GLUTKeyToOGLDEVKey(uint Key);
