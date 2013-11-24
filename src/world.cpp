
#include "world.h"

#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>

    void DrawNode::draw ()const {
        glPushMatrix();
        glTranslatef(x,y,z);
        glBegin(GL_QUADS);

            glColor3f(159.0f/256.0f,182.0f/256.0f,205.0f/256.0f);

            /* top of cube */
            glNormal3f(0.f, 1.f, 0.f);
            glVertex3f(1.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, 1.0f, 1.0f);
            glVertex3f(1.0f, 1.0f, 1.0f);

            /* bottom of cube */
            glNormal3f(0.f, -1.f, 0.f);
            glVertex3f(1.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(1.0f, 0.0f, 0.0f);

            /* front of cube */
    
            glNormal3f(0.f, 0.f, 1.f);
            glVertex3f(1.0f, 1.0f, 1.0f);
            glVertex3f(0.0f, 1.0f, 1.0f);
            glVertex3f(0.0f, 0.0f, 1.0f);
            glVertex3f(1.0f, 0.0f, 1.0f);

            /* back of cube */
            glNormal3f(0.f, 0.f, -1.f);
            glVertex3f(0.0f, 1.0f, 0.0f);
            glVertex3f(1.0f, 1.0f, 0.0f);
            glVertex3f(1.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);

            /* right side of cube */
            glNormal3f(1.f, 0.f, 0.f);
            glVertex3f(1.0f, 1.0f, 0.0f);
            glVertex3f(1.0f, 1.0f, 1.0f);
            glVertex3f(1.0f, 0.0f, 1.0f);
            glVertex3f(1.0f, 0.0f, 0.0f);

            /* left side of cube */
            glNormal3f(-1.f, 0.f, 0.f);
            glVertex3f(0.0f, 1.0f, 1.0f);
            glVertex3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 1.0f);
        glEnd();
        glPopMatrix();
    }



///////////////////////////////////////////////////////////////////////////////////////////////7
///////////////////////////////////////////////////////////////////////////////////////////////7
///////////////////////////////////////////////////////////////////////////////////////////////7

inline double findnoise2(double x,double y) {
    int n=(int)x+(int)y*57;
    n=(n<<13)^n;
    int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
    return 1.0-((double)nn/1073741824.0);
}


World::World(){

//    nodeTree.addElement(DrawNode (1.0f, 0.0f, -7.0f));
//    nodeTree.addElement(DrawNode (-1.0f, 0.0f, -7.0f));
//    nodeTree.addElement(DrawNode (0.0f, 0.0f, 0.0f));

    for (float i =-100; i < 100; ++i){
        std::cout << ".";
        for (float j =-100; j < 100; ++j){
            nodeTree.addElement(DrawNode (i, findnoise2(i,j)*7.f, j));
        }
    }
    std::cout << " map done " << std::endl;

}

World::~World(){
}

bool World::update(){

    return false;
}

void World::draw() const{

    // draw floor lines
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    for (float i=-100; i < 100; i+=10){
        glColor3f(0.0f, .5f, 0.0f);
        glVertex3f(i, 0.f, 100.0f);
        glVertex3f(i, 0.f,-100.0f);
    }
    for (float i=-100; i < 100; i+=10){
        glColor3f(0.0f, .5f, 0.0f);
        glVertex3f(-100.f, 0.f, i);
        glVertex3f( 100.f, 0.f, i);
    }
    glEnd();
    glEnable(GL_LIGHTING);

    struct DrawVisitor : public DrawingVisitor <DrawVisitor, DrawNode>{
        void visitElem (const DrawNode& elem){
            elem.draw();
        }
    } vis;
    glPushMatrix();
    vis.TraverseTree(nodeTree);
    glPopMatrix();

//    glPushMatrix();
//    glLoadIdentity();
//    glTranslatef(-1.5f, 0.0f, -6.0f);
//    glRotatef(rotTri, 0.0f, 1.0f, 0.0f);
//    glBegin(GL_TRIANGLES);
//        /* front of pyramid */
//        glColor3f(1.0f, 0.0f, 0.0f);
//        glVertex3f(0.0f, 1.0f, 0.0f);
//        glColor3f(0.0f, 0.0f, 1.0f);
//        glVertex3f(-1.0f, -1.0f, 1.0f);
//        glColor3f(0.0f, 1.0f, 0.0f);
//        glVertex3f(1.0f, -1.0f, 1.0f);
//        /* right side of pyramid */
//        glColor3f(1.0f, 0.0f, 0.0f);
//        glVertex3f(0.0f, 1.0f, 0.0f);
//        glColor3f(0.0f, 1.0f, 0.0f);
//        glVertex3f(1.0f, -1.0f, 1.0f);
//        glColor3f(0.0f, 0.0f, 1.0f);
//        glVertex3f(1.0f, -1.0f, -1.0f);
//        /* back of pyramid */
//        glColor3f(1.0f, 0.0f, 0.0f);
//        glVertex3f(0.0f, 1.0f, 0.0f);
//        glColor3f(0.0f, 0.0f, 1.0f);
//        glVertex3f(1.0f, -1.0f, -1.0f);
//        glColor3f(0.0f, 1.0f, 0.0f);
//        glVertex3f(-1.0f, -1.0f, -1.0f);
//        /* left side of pyramid */
//        glColor3f(1.0f, 0.0f, 0.0f);
//        glVertex3f(0.0f, 1.0f, 0.0f);
//        glColor3f(0.0f, 1.0f, 0.0f);
//        glVertex3f(-1.0f, -1.0f, -1.0f);
//        glColor3f(0.0f, 0.0f, 1.0f);
//        glVertex3f(-1.0f, -1.0f, 1.0f);
//    glEnd();
//    glPopMatrix();

//
//    glLoadIdentity();
//    glTranslatef(1.5f, 0.0f, -7.0f);
//    glRotatef(rotQuad, 1.0f, 0.0f, 0.0f);
//
//    glBegin(GL_QUADS);
//        /* top of cube */
//        glColor3f(0.0f, 1.0f, 0.0f);
//        glVertex3f(1.0f, 1.0f, -1.0f);
//        glVertex3f(-1.0f, 1.0f, -1.0f);
//        glVertex3f(-1.0f, 1.0f, 1.0f);
//        glVertex3f(1.0f, 1.0f, 1.0f);
//        /* bottom of cube */
//        glColor3f(1.0f, 0.5f, 0.0f);
//        glVertex3f(1.0f, -1.0f, 1.0f);
//        glVertex3f(-1.0f, -1.0f, 1.0f);
//        glVertex3f(-1.0f, -1.0f, -1.0f);
//        glVertex3f(1.0f, -1.0f, -1.0f);
//        /* front of cube */
//        glColor3f(1.0f, 0.0f, 0.0f);
//        glVertex3f(1.0f, 1.0f, 1.0f);
//        glVertex3f(-1.0f, 1.0f, 1.0f);
//        glVertex3f(-1.0f, -1.0f, 1.0f);
//        glVertex3f(1.0f, -1.0f, 1.0f);
//        /* back of cube */
//        glColor3f(1.0f, 1.0f, 0.0f);
//        glVertex3f(-1.0f, 1.0f, -1.0f);
//        glVertex3f(1.0f, 1.0f, -1.0f);
//        glVertex3f(1.0f, -1.0f, -1.0f);
//        glVertex3f(-1.0f, -1.0f, -1.0f);
//        /* right side of cube */
//        glColor3f(1.0f, 0.0f, 1.0f);
//        glVertex3f(1.0f, 1.0f, -1.0f);
//        glVertex3f(1.0f, 1.0f, 1.0f);
//        glVertex3f(1.0f, -1.0f, 1.0f);
//        glVertex3f(1.0f, -1.0f, -1.0f);
//        /* left side of cube */
//        glColor3f(0.0f, 1.0f, 1.0f);
//        glVertex3f(-1.0f, 1.0f, 1.0f);
//        glVertex3f(-1.0f, 1.0f, -1.0f);
//        glVertex3f(-1.0f, -1.0f, -1.0f);
//        glVertex3f(-1.0f, -1.0f, 1.0f);
//    glEnd();
}
