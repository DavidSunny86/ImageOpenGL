/*!*********************************************************************************************************************
\File         OGLESHelloAPI_Windows.cpp
\Title        OpenGL ES HelloAPI Tutorial
\Author       PowerVR by Imagination, Developer Technology Team
\Copyright    Copyright (c) Imagination Technologies Limited.
\brief		  Basic Tutorial that shows step-by-step how to initialize OpenGL ES 2.0, use it for drawing a triangle and terminate it.
              Entry Point: WinMain
***********************************************************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <string>
#include <opencv.hpp>
#include "src/CSkinAdjust.h"

#define GL_GLEXT_PROTOTYPES
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <vector>

using namespace std;
using namespace cv;
static EGLConfig eglConf;
static EGLSurface eglSurface;
static EGLContext eglCtx;
static EGLDisplay eglDisp;
static void InitEGL()
{
	// EGL config attributes
	const EGLint confAttr[] =
	{
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,// very important!
		EGL_SURFACE_TYPE,EGL_PBUFFER_BIT,//EGL_WINDOW_BIT EGL_PBUFFER_BIT we will create a pixelbuffer surface
		EGL_RED_SIZE,   8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE,  8,
		EGL_ALPHA_SIZE, 8,// if you need the alpha channel
		EGL_DEPTH_SIZE, 8,// if you need the depth buffer
		EGL_STENCIL_SIZE,8,
		EGL_NONE
	};
	// EGL context attributes
	const EGLint ctxAttr[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,// very important!
		EGL_NONE
	};
	// surface attributes
	// the surface size is set to the input frame size
	const EGLint surfaceAttr[] = {
		EGL_WIDTH,512,
		EGL_HEIGHT,512,
		EGL_NONE
	};
	EGLint eglMajVers, eglMinVers;
	EGLint numConfigs;

	eglDisp = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if (eglDisp == EGL_NO_DISPLAY)
	{
		return;
	}
	if (!eglInitialize(eglDisp, &eglMajVers, &eglMinVers))
	{
		// Unable to initialize EGL. Handle and recover
		return;
	}
	// choose the first config, i.e. best config
	if (!eglChooseConfig(eglDisp, confAttr, &eglConf, 1, &numConfigs))
	{
		return;
	}
	// create a pixelbuffer surface
	eglSurface = eglCreatePbufferSurface(eglDisp, eglConf, surfaceAttr);
	if (eglSurface == EGL_NO_SURFACE)
	{
		switch (eglGetError())
		{
		case EGL_BAD_ALLOC:
			// Not enough resources available. Handle and recover
			printf("Not enough resources available");
			break;
		case EGL_BAD_CONFIG:
			// Verify that provided EGLConfig is valid
			printf("provided EGLConfig is invalid");
			break;
		case EGL_BAD_PARAMETER:
			// Verify that the EGL_WIDTH and EGL_HEIGHT are
			// non-negative values
			printf("provided EGL_WIDTH and EGL_HEIGHT is invalid");
			break;
		case EGL_BAD_MATCH:
			// Check window and EGLConfig attributes to determine
			// compatibility and pbuffer-texture parameters
			printf("Check window and EGLConfig attributes");
			break;
		}
	}
	eglCtx = eglCreateContext(eglDisp, eglConf, EGL_NO_CONTEXT, ctxAttr);
	if (eglCtx == EGL_NO_CONTEXT)
	{
		EGLint error = eglGetError();
		if (error == EGL_BAD_CONFIG)
		{
			printf("EGL_BAD_CONFIG");
		}
	}
	if (!eglMakeCurrent(eglDisp, eglSurface, eglSurface, eglCtx))
	{
		printf("MakeCurrent failed");
	}
	printf("initialize success!");
}

void  releaseEgl(){
	eglMakeCurrent(eglDisp, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroyContext(eglDisp, eglCtx);
	eglDestroySurface(eglDisp, eglSurface);
	eglTerminate(eglDisp);

	eglDisp = EGL_NO_DISPLAY;
	eglSurface = EGL_NO_SURFACE;
	eglCtx = EGL_NO_CONTEXT;
}

