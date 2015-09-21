#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
//#include <windows.h>
#include <stdio.h>
#include "Point.h"
#include "Vector.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

/* CONST */
#define BUFSIZE 512
GLfloat RED[3] = {1.0, 0.0, 0.0};
GLfloat GREEN[3] = {0.0, 1.0, 0.0};
GLfloat BLUE[3] = {0.0, 0.0, 1.0};
GLfloat YELLOW[3] = {1.0, 1.0, 0.0};
GLfloat WHITE[3] = {1.0, 1.0, 1.0};
GLfloat BLACK[3] = {0.0, 0.0, 0.0};

float width = 800, height = 800;

int pointsSetCaptureFlag, displayResult,tmp,lineSelected = 1;
vector<Point> Points_Set;

vector<Point> Points_Line1;
vector<Point> Points_Line2;
vector<Point> Points_Line3;
vector<Point> Points_Line4;

vector<Point> Points_curve;

vector<Point> Points_curveLine1;
vector<Point> Points_curveLine2;
vector<Point> Points_curveLine3;
vector<Point> Points_curveLine4;

int leftButtonDown = 0;
GLint hits;
GLuint selectBuf[BUFSIZE];
GLint viewport[4];

//On définit les éléments du menu
enum MENU_TYPE
{
        MENU_SET_POINT,
        MENU_STOP_POINT,
        MENU_DEL_POINT,
        MENU_RENDER,
        MENU_RESET,
        MENU_BLUE,
        MENU_RED,
        MENU_GREEN,
        MENU_YELLOW,
        MENU_QUIT
};

MENU_TYPE show = MENU_SET_POINT;


//Recupere l'element passé en index dans un vector<>
template<typename T>
T getElement(vector<T> vec, int index)
{
    typename vector<T>::iterator iter = vec.begin();
    advance(iter, index);
    return *iter;
}

//Cherche l'element passé en parametre dans un vector<>
template<typename T>
int findElement(vector<T> vec, T object)
{
    typename vector<T>::iterator it;
    for(it = vec.begin(); it != vec.end(); ++it)
    {
        if(*it == object)
            return 1;
    }
    return 0;
}

//Recupere le point voulu dans le vector<>
Point* GetPoint(vector<Point> &vec, int index)
{
    vector<Point>::iterator it = vec.begin();
    advance(it, index);
    return &(*it);
}

//Dessine les points d'un vector<>
void drawPointsPoly(GLenum mode, vector<Point> &vec, GLfloat* color)
{
    if(!vec.empty())
    {
        int i = 0;
        vector<Point>::iterator it;

        for(it = vec.begin();it!=vec.end();++it)
        {
            if (mode == GL_SELECT)
                glLoadName(i);
            glColor3fv(color);
            glPointSize(10);
            glBegin(GL_POINTS);
                glVertex2f((*it).Getx(),(height - (*it).Gety()));
            glEnd();
            i++;
        }
    }
}

//Dessine la ligne formé par les points d'un vector<>
void drawPointsLine(vector<Point> &vec, GLfloat* color)
{
    if(!vec.empty())
    {
        vector<Point>::iterator it;

        glColor3fv(color);
        glPointSize(10);
        glBegin(GL_LINE_STRIP);
        for(it = vec.begin();it!=vec.end();++it)
            glVertex2f((*it).Getx(),(height - (*it).Gety()));
        glEnd();
    }
}

//Applique l'algo recursif de CastelJau
Point getCasteljauPoint(int r, int i, double t)
{
    if(r == 0)
        return getElement(Points_Set, i);

    Point p1 = getCasteljauPoint(r - 1, i, t);
    Point p2 = getCasteljauPoint(r - 1, i + 1, t);

    Point *result = new Point((int) ((1 - t) * p1.Getx() + t * p2.Getx()), (int) ((1 - t) * p1.Gety() + t * p2.Gety()), (int) ((1 - t) * p1.Getz() + t * p2.Getz()));
    return *result;
}

//Rempli un vec en ayant appliqué l'algorithme de Casteljau sur les points
void DeCasteljau(vector<Point> &points)
{
    for (double t = 0; t <= 1; t += 0.01)
       Points_curve.push_back(getCasteljauPoint(points.size()-1, 0, t));
}

//Sauvegarde les différentes lignes
void saveLine()
 {
     switch(lineSelected)
     {
        case 1:
            Points_Line1 = Points_Set;
            break;
        case 2:
            Points_Line2 = Points_Set;
            break;
        case 3:
            Points_Line3 = Points_Set;
            break;
        case 4:
            Points_Line4 = Points_Set;
            break;
     }
 }

void saveRender()
 {
     switch(lineSelected)
     {
        case 1:
            Points_curveLine1 = Points_curve;
            break;
        case 2:
            Points_curveLine2 = Points_curve;
            break;
        case 3:
            Points_curveLine3 = Points_curve;
            break;
        case 4:
            Points_curveLine4 = Points_curve;
            break;
     }
 }

 void clearRender()
 {
     Points_curve.clear();
     saveRender();
 }

//Gere les clics de la souris
void mouseButtonPressed(int button, int state, int x, int y)
{
    switch(button)
    {
      case GLUT_LEFT_BUTTON://clic gauche -> ajout de points
            if (GLUT_DOWN == state)
            {
                if(pointsSetCaptureFlag)//Si le mode ajout de points est activé
                {
                    Point p(x,y,0);
                    if(!findElement(Points_Set, p))//On verifie que le point n'est pas déja dans la liste
                        Points_Set.push_back(p);
                        saveLine();
                }

            }
            break;
    }
    if(displayResult)
    {
        clearRender();
        DeCasteljau(Points_Set);
        saveRender();
    }
    glutPostRedisplay();
    if(GLUT_LEFT_BUTTON == button)
    {
        if(GLUT_DOWN == state)
            leftButtonDown = 1;
        if(GLUT_UP == state)
            leftButtonDown = 0;
    }

    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
        return;

    glGetIntegerv(GL_VIEWPORT, viewport);

    glSelectBuffer(BUFSIZE, selectBuf);
    (void) glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3] - y),
                 10, 10, viewport);
        glOrtho(0.0, width, 0.0, height, 1.0, -1.0);
        drawPointsPoly(GL_SELECT, Points_Set, RED);
    glPopMatrix();
    glFlush();

    hits = glRenderMode(GL_RENDER);
}

void mouseMove(int x, int y){
    if(!pointsSetCaptureFlag && leftButtonDown){
        Point *p;
        GLuint *ptr;
        ptr = (GLuint *) selectBuf;
        ptr++;
        ptr++;
        ptr++;
        if((1 <= hits) && ( 0 <= *ptr && Points_Set.size() > *ptr))
        {
            p = GetPoint(Points_Set, *ptr);
            (*p).Setx(x);
            (*p).Sety(y);
            if(displayResult)
            {
                clearRender();
                DeCasteljau(Points_Set);
                saveRender();
            }
            saveLine();
            glutPostRedisplay();
        }
    }
}

void specialKeyboard(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_LEFT:
            break;
        case GLUT_KEY_RIGHT:
            break;
        default:
             cout << "Input not recognized" << endl;
    }
    glutPostRedisplay();
}

//Gere les touches du clavier
void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case 'e': //Execute l'algorithme de Casteljau
            tmp = lineSelected;
            if(!Points_Line1.empty()){
                lineSelected = 1;
                clearRender();
                DeCasteljau(Points_Line1);
                saveRender();
            }
            if(!Points_Line2.empty()){
                lineSelected = 2;
                clearRender();
                DeCasteljau(Points_Line2);
                saveRender();
            }
            if(!Points_Line3.empty()){
                lineSelected = 3;
                clearRender();
                DeCasteljau(Points_Line3);
                saveRender();
            }
            if(!Points_Line4.empty()){
                lineSelected = 4;
                clearRender();
                DeCasteljau(Points_Line4);
                saveRender();
            }
            lineSelected = tmp;
            displayResult = 1;
            break;
        case 'p': //Passe en mode ajout de points
            pointsSetCaptureFlag = 1;
            break;
        case 'q': //quitte l'application
            exit(0);
            break;
        case 'r': //reset les données
            pointsSetCaptureFlag = 0;
            displayResult = 0;
            Points_Set.clear();
            Points_Line1.clear();
            Points_Line2.clear();
            Points_Line3.clear();
            Points_Line4.clear();
            Points_curveLine1.clear();
            Points_curveLine2.clear();
            Points_curveLine3.clear();
            Points_curveLine4.clear();
            break;
        case 's': //Stop le mode ajout de points
            pointsSetCaptureFlag = 0;
            break;
        default:
             cout << "Input not recognized" << endl;
    }
}

//Gère les fonctions des différentes sections du menu
void menu(int item)
{
        switch (item)
        {
        case MENU_SET_POINT:
            pointsSetCaptureFlag = 1;
            break;
        case MENU_STOP_POINT:
            pointsSetCaptureFlag = 0;
            break;
        case MENU_DEL_POINT:
            if(!Points_Set.empty()){
                Points_Set.pop_back();
                saveLine();
                clearRender();
            }
            break;
        case MENU_RESET:
            pointsSetCaptureFlag = 0;
            displayResult = 0;
            Points_Set.clear();
            Points_Line1.clear();
            Points_Line2.clear();
            Points_Line3.clear();
            Points_Line4.clear();
            Points_curveLine1.clear();
            Points_curveLine2.clear();
            Points_curveLine3.clear();
            Points_curveLine4.clear();
            break;
        case MENU_RENDER:
            tmp = lineSelected;
            if(!Points_Line1.empty()){
                lineSelected = 1;
                clearRender();
                DeCasteljau(Points_Line1);
                saveRender();
            }
            if(!Points_Line2.empty()){
                lineSelected = 2;
                clearRender();
                DeCasteljau(Points_Line2);
                saveRender();
            }
            if(!Points_Line3.empty()){
                lineSelected = 3;
                clearRender();
                DeCasteljau(Points_Line3);
                saveRender();
            }
            if(!Points_Line4.empty()){
                lineSelected = 4;
                clearRender();
                DeCasteljau(Points_Line4);
                saveRender();
            }
            lineSelected = tmp;
            displayResult = 1;
            break;
        case MENU_BLUE:
            saveLine();
            saveRender();
            Points_Set = Points_Line1;
            lineSelected = 1;
            if(displayResult && !Points_Set.empty())
            {
                clearRender();
                DeCasteljau(Points_Set);
                saveRender();
            }
            break;
        case MENU_RED:
            saveLine();
            saveRender();
            Points_Set = Points_Line2;
            lineSelected = 2;
            if(displayResult && !Points_Set.empty())
            {
                clearRender();
                DeCasteljau(Points_Set);
                saveRender();
            }
            break;
        case MENU_GREEN:
            saveLine();
            saveRender();
            Points_Set = Points_Line3;
            lineSelected = 3;
            if(displayResult && !Points_Set.empty())
            {
                clearRender();
                DeCasteljau(Points_Set);
                saveRender();
            }
            break;
        case MENU_YELLOW:
            saveLine();
            saveRender();
            Points_Set = Points_Line4;
            lineSelected = 4;
            if(displayResult && !Points_Set.empty())
            {
                clearRender();
                DeCasteljau(Points_Set);
                saveRender();
            }
            break;
        case MENU_QUIT:
            exit(0);
        default:
            cout << "Input not recognized" << endl;
            break;
        }
        glutPostRedisplay();

        return;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawPointsPoly(GL_RENDER, Points_Line1, BLUE);
    drawPointsLine(Points_Line1, BLUE);

    drawPointsPoly(GL_RENDER, Points_Line2, RED);
    drawPointsLine(Points_Line2, RED);

    drawPointsPoly(GL_RENDER, Points_Line3, GREEN);
    drawPointsLine(Points_Line3, GREEN);

    drawPointsPoly(GL_RENDER, Points_Line4, YELLOW);
    drawPointsLine(Points_Line4, YELLOW);

    if(displayResult){
        if(!Points_Line1.empty())
            drawPointsLine(Points_curveLine1, RED);
        if(!Points_Line2.empty())
            drawPointsLine(Points_curveLine2, RED);
        if(!Points_Line3.empty())
            drawPointsLine(Points_curveLine3, RED);
        if(!Points_Line4.empty())
            drawPointsLine(Points_curveLine4, RED);
    }
    glutSwapBuffers();
}

void idle()
{
    glutPostRedisplay();
}

void setup()
{
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0,0, width, height);
    glMatrixMode(GL_PROJECTION);
    glOrtho(0.0, width, 0.0, height, 1.0, -1.0);

    glutSwapBuffers();
}

//Recupere le point avec l'abscisse la plus petite du vector<>
Point getMinX(vector<Point> &vec)
{
    if(vec.empty())
        return Point();

    Point Xmin = vec.front();
    vector<Point>::iterator it;
    for(it = vec.begin();it!=vec.end();++it)
        if((*it).Getx() < Xmin.Getx())
            Xmin = (*it);

    return Xmin;
}

//Recupere le point avec l'abscisse la plus grande du vector<>
Point getMaxX(vector<Point> &vec)
{
    if(vec.empty())
        return Point();

    Point Xmax = vec.front();
    vector<Point>::iterator it;
    for(it = vec.begin();it!=vec.end();++it)
        if((*it).Getx() > Xmax.Getx())
            Xmax = (*it);

    return Xmax;
}

//Calcul le vecteur entre deux points
Vector calcVect(Point &P1, Point &P2)
{
    float x = (P2.Getx()-P1.Getx());
    float y = (P2.Gety()-P1.Gety());
    float z = (P2.Getz()-P1.Getz());
    Vector v (x, y, z); // create new vector, (xb-xa, yb-ya, zb-za)
    return v;
}

//Calcul la normal
Vector calcNormal(Point &F1, Point &F2)
{
    Vector seg = calcVect(F1, F2);                                   // segment to calculate normal
    Vector normal(-seg.Gety(), seg.Getx(), 0);                       // normal1 equals (-b, a)

    return normal;
}

//Calcul le produit scalaire
float scalar(Vector vect1, Vector vect2)
{
    return (vect1.Getx()*vect2.Getx()) + (vect1.Gety()*vect2.Gety());
}

//Calcul le milieu d'un segment
Point getMiddleLine(Point &A, Point &B)
{
    double x = A.Getx() + ( ( B.Getx() - A.Getx() ) * 0.5 );
    double y = A.Gety() + ( ( B.Gety() - A.Gety() ) * 0.5 );
    double z = A.Getz() + ( ( B.Getz() - A.Getz() ) * 0.5 );
    return Point(x, y, z);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width,height);
    glutCreateWindow("Bezier Curves");
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutMouseFunc(mouseButtonPressed);
    glutMotionFunc(mouseMove);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    setup();

    int submenu;

    submenu = glutCreateMenu(menu);
	glutAddMenuEntry("Line 1",MENU_BLUE);
	glutAddMenuEntry("Line 2",MENU_RED);
	glutAddMenuEntry("Line 3",MENU_GREEN);
	glutAddMenuEntry("Line 4",MENU_YELLOW);

    glutCreateMenu(menu);
    glutAddMenuEntry("Create new points", MENU_SET_POINT);
    glutAddMenuEntry("Move points", MENU_STOP_POINT);
    glutAddMenuEntry("Delete last point", MENU_DEL_POINT);
    glutAddSubMenu("Switch line", submenu);
    glutAddMenuEntry("Render curve", MENU_RENDER);
    glutAddMenuEntry("Reset scene", MENU_RESET);
    glutAddMenuEntry("Quit", MENU_QUIT);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop();
    return 0;
}
