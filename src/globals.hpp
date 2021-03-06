/*
 * globals.hpp
 *
 * Declaracao de tipos e variaveis globais.
 *
 * Author: Diogo Silva (dbtds@ua.pt)
 *
 */


#ifndef GLOBALS_H
#define GLOBALS_H

#define GLEW_STATIC /* Necessario se houver problemas com a lib */
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include "effects/Effects.hpp"
#include "utils/mathUtils.hpp"
#include "models/graphicmodel.hpp"
#include "visualization/themeController.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;

struct projCoord {
	float fovy;
	float aspect_ratio;
	float near_plane;
	float far_plane;
};

static string projectPath = "../src/";
extern projCoord proj;
extern int windowHandle;
extern GLuint programaGLSL;
extern GLuint vs, fs;
extern GLint attribute_coord3d, attribute_texcoord;
extern mat4x4 matrizProj, matrizModelView;
extern int currentPos;

extern vector<GraphicModel>* ss_images;
extern vector<GraphicModel> btn_effects;
extern vector<GraphicModel> btn_effectsCustom;
extern Effects matEffects;
extern GraphicModel btnSave, btnOptions, *btnPressed, btnDiscard, btnCamera;
extern GraphicModel cameraTexture;
extern bool animationActive, animationMove, animationZoom;

extern bool cameraActive;
extern int cameraLastEffect;
extern VideoCapture stream;

extern ThemeController * themect;
extern Theme * theme;
#endif
