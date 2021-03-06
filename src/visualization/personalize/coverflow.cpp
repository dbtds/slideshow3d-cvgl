/*
 * Effect.hpp
 *
 * Author: Diogo Silva (dbtds@ua.pt)
 *
 */

#include "coverflow.hpp"
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#include "coverflow.hpp"
#include <iostream>
#include "../../globals.hpp"


Coverflow::Coverflow(vector<GraphicModel>* images) : Theme(images) {}

Theme* Coverflow::getInstance(vector<GraphicModel>* images) {
    instance = new Coverflow(images);
    return instance;
}

void Coverflow::initTheme(void) {
    animationActive = false;
    animationMove = false;
    animationZoom = false;
    currentPos = 0;

    unsigned int max;
    for (int i = 0; i < c_images->size(); i++) {
        (*c_images)[i].desl.x = i == 0 ? -2 : 0;
        (*c_images)[i].desl.y = i == 0 ? 0 : 1.5 + (i-1);
        (*c_images)[i].desl.z = 0;
        (*c_images)[i].anguloRot.x = 0;
        (*c_images)[i].anguloRot.y = 0;
        (*c_images)[i].anguloRot.z = i == 0 ? 0 : 45;
        (*c_images)[i].factorEsc.x = 1;
        max = (*c_images)[i].image.cols > (*c_images)[i].image.rows ? (*c_images)[i].image.cols : (*c_images)[i].image.rows;
        (*c_images)[i].factorEsc.y = (double) (*c_images)[i].image.cols / max;
        (*c_images)[i].factorEsc.z = (double) (*c_images)[i].image.rows / max;
    }
}

void Coverflow::pressLeft(void) {
    if (animationMove)
        return;
    if (currentPos == 0)
        return;
    glutTimerFunc(20, staticAnimation_zoomOut, currentPos);
    currentPos--;
    animationMove = true;
    glutTimerFunc(20, staticAnimation_moveLeft, currentPos);
}

void Coverflow::pressRight(void) {
	if (animationMove)
        return;
    if (currentPos == c_images->size() - 1)
        return;
    glutTimerFunc(20, staticAnimation_zoomOut, currentPos);
    currentPos++;
    animationMove = true;
    glutTimerFunc(20, staticAnimation_moveRight, currentPos);
}
void Coverflow::pressUp(void) {}
void Coverflow::pressDown(void) {}
void Coverflow::zoomIn(void) {
    if (animationActive)
        return;
    glutTimerFunc(20, staticAnimation_zoomIn, currentPos);
}
void Coverflow::zoomOut(void) {
    if (animationActive)
        return;
    glutTimerFunc(20, staticAnimation_zoomOut, currentPos);
}

void Coverflow::animation_moveRight(int pos) {
    bool changed = false;
    for (int i = 0; i < c_images->size(); i++) {
        if (i == currentPos) {
            if ((*c_images)[i].desl.y > 0) {
                (*c_images)[i].desl.y -= 0.05;
                changed = true;
            }
            if ((*c_images)[i].anguloRot.z != 0) {
                (*c_images)[i].anguloRot.z -= 3; 
                changed = true;
            }
            if ((*c_images)[i].desl.x > -2) {
                (*c_images)[i].desl.x -= 0.1;
                changed = true;
            }
        } else if (i > currentPos) {
            if ((*c_images)[i].anguloRot.z < 45) {
                (*c_images)[i].anguloRot.z += 3;
                changed = true;
            }
            if ((*c_images)[i].desl.y > (0.5 + i - currentPos)) {
                (*c_images)[i].desl.y -= 0.05;
                changed = true;
            }
        } else {
            if ((*c_images)[i].anguloRot.z > -45) {
                (*c_images)[i].anguloRot.z -= 3;
                changed = true;
            }
            if ((*c_images)[i].desl.y > (-0.5 + i - currentPos)) {
                (*c_images)[i].desl.y -= 0.05;
                changed = true;
            }
        }
    }
    glutPostRedisplay();
    if (changed)
        glutTimerFunc(5, staticAnimation_moveRight, pos);
    else {
        for (int i = 0; i < c_images->size(); i++) {
            (*c_images)[i].desl.y = i < currentPos ? -0.5 + i - currentPos : 0.5 + i - currentPos;
            (*c_images)[i].anguloRot.z = i < currentPos ? -45 : 45;
            if (i == currentPos) {
                (*c_images)[i].desl.x = -2;
                (*c_images)[i].desl.y = 0;
                (*c_images)[i].anguloRot.z = 0;
            }
        }
        animationMove = false;
    }
}

void Coverflow::animation_moveLeft(int pos) {
    bool changed = false;
    for (int i = 0; i < c_images->size(); i++) {
        if (i == currentPos) {
            if ((*c_images)[i].desl.y < 0) {
                (*c_images)[i].desl.y += 0.05;
                changed = true;
            }
            if ((*c_images)[i].anguloRot.z != 0) {
                (*c_images)[i].anguloRot.z += 3; 
                changed = true;
            }
            if ((*c_images)[i].desl.x > -2) {
                (*c_images)[i].desl.x -= 0.1;
                changed = true;
            }
        } else if (i > currentPos) {
            if ((*c_images)[i].anguloRot.z < 45) {
                (*c_images)[i].anguloRot.z += 3;
                changed = true;
            }
            if ((*c_images)[i].desl.y < (0.5 + i - currentPos)) {
                (*c_images)[i].desl.y += 0.05;
                changed = true;
            }
        } else {
            if ((*c_images)[i].anguloRot.z > -45) {
                (*c_images)[i].anguloRot.z -= 3;
                changed = true;
            }
            if ((*c_images)[i].desl.y < (-0.5 + i - currentPos)) {
                (*c_images)[i].desl.y += 0.05;
                changed = true;
            }
            
        }
    }
    glutPostRedisplay();
    if (changed) 
        glutTimerFunc(5, staticAnimation_moveLeft, pos);
    else {
        for (int i = 0; i < c_images->size(); i++) {
            (*c_images)[i].desl.y = i < currentPos ? -0.5 + i - currentPos : 0.5 + i - currentPos;
            (*c_images)[i].anguloRot.z = i < currentPos ? -45 : 45;
            if (i == currentPos) {
                (*c_images)[i].desl.x = -2;
                (*c_images)[i].desl.y = 0;
                (*c_images)[i].anguloRot.z = 0;
            }
        }
        animationMove = false;
    }
}
void Coverflow::animation_moveDown(int pos) {}
void Coverflow::animation_moveUp(int pos) {}
void Coverflow::animation_zoomIn(int pos) {
    if ((*c_images)[pos].desl.x > -4.5) { 
        animationActive = true;
        (*c_images)[pos].desl.x -= 0.1;
        glutPostRedisplay();
        glutTimerFunc(20, staticAnimation_zoomIn, pos);
    } else {
        (*c_images)[pos].desl.x = -4.5;
        animationActive = false;
    }
}
void Coverflow::animation_zoomOut(int pos) {
    int max = pos == currentPos ? -2 : 0;
    if ((*c_images)[pos].desl.x < max) { 
        animationActive = true;
        (*c_images)[pos].desl.x += 0.1;
        glutPostRedisplay();
        glutTimerFunc(20, staticAnimation_zoomOut, pos);
    } else {
        (*c_images)[pos].desl.x = max;
        animationActive = false;
    }
}