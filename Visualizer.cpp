#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <unistd.h>
#include "GL/freeglut.h"
#include "GL/gl.h"

int length;
int delay;
int* arr;
void (*sort)(int*, int);
static int window, returnsubmenucolor1, value=0, returnmenu;

void randomizeArray(int* arr, int length)
{
    for(int i = length - 1; i > 0; --i)
    {
        std::swap(arr[i], arr[rand() % (i+1)]);
    }
}


//used for debugging
void printArray(int* arr, int length)
{
    for(int i = 0; i < length; ++i)
    {
        printf("%i", arr[i]);

        if(i < length - 1)
        {
            printf(",");
        }
    }

    printf("\n");
}


void renderFunction()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    float l = (float) length;
    float widthAdder = 1/l;

    for(int i = 0; i < length; ++i)
    {
        glBegin(GL_POLYGON);

        // + 1 so value of 0 has height of 1
        float arrayIndexHeightRation = 2*(arr[i] + 1)/l;
        float widthIndexAdder = 2*i/l;

        float leftX   = -1 + widthIndexAdder;
        float rightX  = leftX + widthAdder;
        float bottomY = -1;
        float topY    = bottomY + arrayIndexHeightRation;

        // bottom left
        glColor4f(1,0,0,0);
        glVertex2f(leftX, bottomY);

        // bottom right
        glColor4f(0,1,0,0);
        glVertex2f(rightX, bottomY);

        // top right
        glColor4f(0,0,1,0);
        glVertex2f(rightX, topY);

        // top left
        glColor4f(0,0,0,1);
        glVertex2f(leftX, topY);

        glEnd();
    }

    glFlush();
}

void swap(int index1, int index2)
{
    std::swap(arr[index1], arr[index2]);
    renderFunction();
    usleep(delay);
}

void keyboardEvent(unsigned char c, int x, int y)
{
    if(c == 27)
    {
        // exit on escape key pressed
        exit(0);
        free(arr);
    }
    else if(c == 115)
    {
        // start on `s` key pressed
        sort(arr, length);
    }
}


void menu(int n){
    if (n == 0)
    {
        glutDestroyWindow(window);
        exit(0);
    }
    else if(n==1)
    {
        sort(arr, length);
    }
    glutPostRedisplay();
}


void createMenu(void) {
    returnmenu = glutCreateMenu(menu); //function to call menu function and return value
    glutAddMenuEntry("Start", 1);
    glutAddMenuEntry("Quit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void setUpGlutAndArray(int argc, char** argv, void (*sortingAlgorithm)(int*, int))
{
    sort = sortingAlgorithm;
    arr = (int*) malloc(sizeof(int) * length);
    for(int i = 0; i < length; ++i)
    {
        arr[i] = i;
    }

    randomizeArray(arr, length);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(length,length);
    glutInitWindowPosition(100,100);
    window=glutCreateWindow("Sort Visualization");
    createMenu();

    glutDisplayFunc(renderFunction);
    glutKeyboardFunc(keyboardEvent);

    glutMainLoop();
}