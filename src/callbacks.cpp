/*
 * callbacks.cpp
 *
 * Author: Diogo Silva (dbtds@ua.pt)
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#define GLEW_STATIC /* Necessario se houver problemas com a lib */
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#include "globals.hpp"
#include "callbacks.hpp"

using namespace std;

void animation_zoomInImage(int);
void animation_zoomOutImage(int);
void animation_moveRight(int);
void animation_moveLeft(int);
bool checkButtonClick(double, double, GraphicModel*);
void animation_buttonPressed(int);

void produceModelsShading(GraphicModel *obj)
{
    glEnableVertexAttribArray(attribute_coord3d);
    glEnableVertexAttribArray(attribute_texcoord);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, obj->textureID);
    float *coordenadas = new GLfloat[obj->arrayVertices.size() * sizeof(GLfloat)];

    glVertexAttribPointer(attribute_coord3d,        // attribute
                          3,                        // number of elements per vertex, here (x,y,z)
                          GL_FLOAT,                 // the type of each element
                          GL_FALSE,                 // take our values as-is
                          0,                        // no extra data between each position
                          obj->arrayVertices.data());      // pointer to the C array
    
    glVertexAttribPointer(attribute_texcoord, // attribute
                          2,                  // number of elements per vertex, here (x,y)
                          GL_FLOAT,           // the type of each element
                          GL_FALSE,           // take our values as-is
                          0,                  // no extra data between each position
                          obj->arrayTextures.data());

    matrizModelView = IDENTITY_MATRIX;
    Translate(&matrizModelView, obj->desl.x, obj->desl.y, obj->desl.z);
    RotateAboutX(&matrizModelView, DegreesToRadians(obj->anguloRot.x));
    RotateAboutY(&matrizModelView, DegreesToRadians(obj->anguloRot.y));
    RotateAboutZ(&matrizModelView, DegreesToRadians(obj->anguloRot.z));
    /* Diminuir o tamanho do modelo para nao sair fora do view volume */
    Scale(&matrizModelView, obj->factorEsc.x, obj->factorEsc.y, obj->factorEsc.z);
    /* Matriz de projeccao e Matriz de transformacao */
    glUniformMatrix4fv(glGetUniformLocation(programaGLSL, "matrizProj"), 1, GL_FALSE, matrizProj.m);
    glUniformMatrix4fv(glGetUniformLocation(programaGLSL, "matrizModelView"), 1, GL_FALSE, matrizModelView.m);

    glUniform1i(glGetUniformLocation(programaGLSL, "mytexture"), /*GL_TEXTURE*/0);

    glDrawArrays(GL_TRIANGLES, 0, obj->numVertices);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisableVertexAttribArray(attribute_coord3d);
    glDisableVertexAttribArray(attribute_texcoord);

    delete[] coordenadas;
}
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programaGLSL);
    for (int i = 0; i < ss_images.size(); i++) 
        produceModelsShading(ss_images[i]);
    for (int i = 0; i < btn_effects.size(); i++) 
        produceModelsShading(btn_effects[i]);
    produceModelsShading(btnSave);
    produceModelsShading(btnOptions);
    produceModelsShading(btnDiscard);
    glutSwapBuffers();
}

void myKeyboard(unsigned char key, int x, int y)
{
    int i;
    GraphicModel *obj;
    switch (key)
    {
    case 'Q' :
    case 'q' :
    case 27  :  exit(EXIT_SUCCESS);
    case '+' :
        glutPostRedisplay();
        break;
    case '-' :
        glutPostRedisplay();
        break;
    case '.':
        glutPostRedisplay();
        break;
    case ',':
        glutPostRedisplay();
        break;
    case 'p':
    case 'P':
        break;
    case 'm':
    case 'M':
        break;        
    }
}
/* RANDOM */
void mySpecialKeys(int key, int x, int y)
{
    unsigned int max;

    switch (key)
    {
    case GLUT_KEY_LEFT:
        if (currentPos == 0)
            break;
        glutTimerFunc(20, animation_zoomOutImage, currentPos);
        currentPos--;
        if (animationMove)
            break;
        animationMove = true;
        glutTimerFunc(20, animation_moveLeft, currentPos);
        break;

    case GLUT_KEY_RIGHT:
        if (currentPos == ss_images.size() - 1)
            break;
        glutTimerFunc(20, animation_zoomOutImage, currentPos);
        currentPos++;
        if (animationMove)
            break;
        animationMove = true;
        glutTimerFunc(20, animation_moveRight, currentPos);
        break;

    case GLUT_KEY_UP:
        for (int i = 0; i < ss_images.size(); i++) {
            cout << ss_images[i]->desl.x << ", " << ss_images[i]->desl.y << ", "<< ss_images[i]->anguloRot.z << endl;
        }
        cout << endl;
        glutPostRedisplay();
        break;
    case GLUT_KEY_DOWN:
        glutPostRedisplay();
        break;
    }
}

void onMouse(int button, int state, int x, int y)
{
    switch (button) {
        case 3:
            if (animationActive)
                return;
            glutTimerFunc(20, animation_zoomInImage, currentPos);
            break;
        case 4:
            if (animationActive)
                return;
            glutTimerFunc(20, animation_zoomOutImage, currentPos);
            break;
        case 0:
            if (state != GLUT_DOWN)
                return;

            GLint viewport[4];
            GLdouble modelview[16], projection[16];
            GLfloat winX, winY, winZ;
            GLdouble posX, posY, posZ;
         
            glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
            glGetDoublev(GL_PROJECTION_MATRIX, projection);
            glGetIntegerv(GL_VIEWPORT, viewport);
         
            winX = (float)x;
            winY = (float)viewport[3] - (float)y;
            for (int i = 0; i < 16; i++) 
                projection[i] = matrizProj.m[i];

            glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
            gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

            for (int i = 0; i < btn_effects.size(); i++) {
                if (checkButtonClick(posY, posZ, btn_effects[i])) {
                    if (i < matEffects.getNumberEffects()) {
                        matEffects.applyEffect(i, &ss_images[currentPos]->image, matEffects.readParameters(i));
                        ss_images[currentPos]->textureID = loadImage(&ss_images[currentPos]->image);
                    }
                }
            }
            if (checkButtonClick(posY, posZ, btnSave)) {
                string s = ss_images[currentPos]->filepath;
                string::size_type i = s.rfind('.', s.length());
                if (i != string::npos)
                    s.replace(i, 0, "_output");
                saveImage(s, ss_images[currentPos]->image);
                cout << "Saved file as: " << s << endl;
            } else if (checkButtonClick(posY, posZ, btnOptions)) {
                printf("Pressed options button\n");
            } else if (checkButtonClick(posY, posZ, btnDiscard)) {
                ss_images[currentPos]->image = ss_images[currentPos]->original;
                ss_images[currentPos]->textureID = loadImage(&ss_images[currentPos]->image);
            }
            break;
    }
    glutPostRedisplay();
}
bool checkButtonClick(double posY, double posZ, GraphicModel *obj) {
    if (posZ < obj->desl.z + obj->factorEsc.z && posZ > obj->desl.z - obj->factorEsc.z
        && posY < obj->desl.y + obj->factorEsc.y && posY > obj->desl.y - obj->factorEsc.y)
    {
        btnPressed = obj;
        glutTimerFunc(20, animation_buttonPressed, 1);
        return true;
    }
    return false;
}
void animation_buttonPressed(int status) {
    if ((status == 1 && btnPressed->desl.x < -4.57) ||
        (status == 0 && btnPressed->desl.x > -4.6)) {
        btnPressed->desl.x += 0.01 * (status * 2 - 1);
        glutPostRedisplay();
        if (btnPressed->desl.x >= -4.57 && status == 1)
            glutTimerFunc(10, animation_buttonPressed, 0);
        else if (!(btnPressed->desl.x <= -4.6 && status == 0))
            glutTimerFunc(10, animation_buttonPressed, status);
    }
}
void animation_zoomInImage(int pos) {
    if (ss_images[pos]->desl.x > -4.5) { 
        animationActive = true;
        ss_images[pos]->desl.x -= 0.1;
        glutPostRedisplay();
        glutTimerFunc(20, animation_zoomInImage, pos);
    } else {
        ss_images[pos]->desl.x = -4.5;
        animationActive = false;
    }
}
void animation_zoomOutImage(int pos) {
    int max = pos == currentPos ? -2 : 0;
    if (ss_images[pos]->desl.x < max) { 
        animationActive = true;
        ss_images[pos]->desl.x += 0.1;
        glutPostRedisplay();
        glutTimerFunc(20, animation_zoomOutImage, pos);
    } else {
        ss_images[pos]->desl.x = max;
        animationActive = false;
    }
}

void animation_moveRight(int pos) {
    bool changed = false;
    for (int i = 0; i < ss_images.size(); i++) {
        if (i == currentPos) {
            if (ss_images[i]->desl.y > 0) {
                ss_images[i]->desl.y -= 0.05;
                changed = true;
            }
            if (ss_images[i]->anguloRot.z != 0) {
                ss_images[i]->anguloRot.z -= 3; 
                changed = true;
            }
            if (ss_images[i]->desl.x > -2) {
                ss_images[i]->desl.x -= 0.1;
                changed = true;
            }
        } else if (i > currentPos) {
            if (ss_images[i]->anguloRot.z < 45) {
                ss_images[i]->anguloRot.z += 3;
                changed = true;
            }
            if (ss_images[i]->desl.y > (0.5 + i - currentPos)) {
                ss_images[i]->desl.y -= 0.05;
                changed = true;
            }
        } else {
            if (ss_images[i]->anguloRot.z > -45) {
                ss_images[i]->anguloRot.z -= 3;
                changed = true;
            }
            if (ss_images[i]->desl.y > (-0.5 + i - currentPos)) {
                ss_images[i]->desl.y -= 0.05;
                changed = true;
            }
        }
    }
    glutPostRedisplay();
    if (changed)
        glutTimerFunc(5, animation_moveRight, pos);
    else {
        for (int i = 0; i < ss_images.size(); i++) {
            ss_images[i]->desl.y = i < currentPos ? -0.5 + i - currentPos : 0.5 + i - currentPos;
            ss_images[i]->anguloRot.z = i < currentPos ? -45 : 45;
            if (i == currentPos) {
                ss_images[i]->desl.x = -2;
                ss_images[i]->desl.y = 0;
                ss_images[i]->anguloRot.z = 0;
            }
        }
        animationMove = false;
    }
}

void animation_moveLeft(int pos) {
    bool changed = false;
    for (int i = 0; i < ss_images.size(); i++) {
        if (i == currentPos) {
            if (ss_images[i]->desl.y < 0) {
                ss_images[i]->desl.y += 0.05;
                changed = true;
            }
            if (ss_images[i]->anguloRot.z != 0) {
                ss_images[i]->anguloRot.z += 3; 
                changed = true;
            }
            if (ss_images[i]->desl.x > -2) {
                ss_images[i]->desl.x -= 0.1;
                changed = true;
            }
        } else if (i > currentPos) {
            if (ss_images[i]->anguloRot.z < 45) {
                ss_images[i]->anguloRot.z += 3;
                changed = true;
            }
            if (ss_images[i]->desl.y < (0.5 + i - currentPos)) {
                ss_images[i]->desl.y += 0.05;
                changed = true;
            }
        } else {
            if (ss_images[i]->anguloRot.z > -45) {
                ss_images[i]->anguloRot.z -= 3;
                changed = true;
            }
            if (ss_images[i]->desl.y < (-0.5 + i - currentPos)) {
                ss_images[i]->desl.y += 0.05;
                changed = true;
            }
            
        }
    }
    glutPostRedisplay();
    if (changed) 
        glutTimerFunc(5, animation_moveLeft, pos);
    else {
        for (int i = 0; i < ss_images.size(); i++) {
            ss_images[i]->desl.y = i < currentPos ? -0.5 + i - currentPos : 0.5 + i - currentPos;
            ss_images[i]->anguloRot.z = i < currentPos ? -45 : 45;
            if (i == currentPos) {
                ss_images[i]->desl.x = -2;
                ss_images[i]->desl.y = 0;
                ss_images[i]->anguloRot.z = 0;
            }
        }
        animationMove = false;
    }
}

void registarCallbackFunctions(void)
{
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecialKeys);
    glutMouseFunc(onMouse);
}
