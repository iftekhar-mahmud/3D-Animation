#include<windows.h>
#include <GL/glut.h>
#include <math.h>

#define pi (2*acos(0.0))

struct Point3D
{
    double x,y,z;
};
struct Point13D
{
    double x,y,z;
};

Point3D eye = {100,100,100};
Point3D pos = {0,0,0};


Point13D eye1 = {100,100,100};
Point13D pos1 = {5,5,16};


double time = 0.0;  //t
double u = 30;       //Initial Velocity
double A = 60*pi/180;      //Initial Angle
double g = 9.8;     //Gravitational Acceleration
double B = 20*pi/180;      //Angle between projectile plane and x axis
double T = 2*u*sin(A*pi/180)/g; //Travel time
double R = u*u * sin(2*A)/g;


void drawAxes()
{

    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    {
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f( 1000,0,0);
        glVertex3f(-1000,0,0);

        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f(0,-1000,0);
        glVertex3f(0, 1000,0);

        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(0,0, 1000);
        glVertex3f(0,0,-1000);
    }
    glEnd();

}

void drawCircleWithPolygon(float cx, float cy, float radius, int slices)
{
    int i;
    float angle, x, y;
    glBegin(GL_POLYGON);
    {
        for(i=0; i<slices; i++)
        {
            angle = ((2 * pi)/slices) * i;
            x = cx + radius * cos(angle);
            y = cy + radius * sin(angle);
            glVertex2f(x, y);
        }
    }
    glEnd();
}

void drawGrid()
{
    int i;

    glColor3f(0.6, 0.6, 0.6);	//grey
    glBegin(GL_LINES);
    {
        for(i=-8; i<=8; i++)
        {

            if(i==0)
                continue;	//SKIP the MAIN axes

            //lines parallel to Y-axis
            glVertex3f(i*10, -90, 0);
            glVertex3f(i*10,  90, 0);

            //lines parallel to X-axis
            glVertex3f(-90, i*10, 0);
            glVertex3f( 90, i*10, 0);
        }
    }
    glEnd();

}

void drawCuboid(double length, double width, double height)
{
    glBegin(GL_QUADS);
    {
        //Bottom
        glColor3f(1,1,0);
        glVertex3d(0,0,0);
        glVertex3d(length,0,0);
        glVertex3d(length,width,0);
        glVertex3d(0,width,0);

        //Top
        glVertex3d(0,0,height);
        glVertex3d(length,0,height);
        glVertex3d(length,width,height);
        glVertex3d(0,width,height);

        //Right
        glColor3f(1,1,0);
        glVertex3d(0,0,0);
        glVertex3d(0,width,0);
        glVertex3d(0,width,height);
        glVertex3d(0,0,height);

        //Left
        glVertex3d(length,0,0);
        glVertex3d(length,width,0);
        glVertex3d(length,width,height);
        glVertex3d(length,0,height);

        //Back
        glColor3f(1,1,0);
        glVertex3d(0,0,0);
        glVertex3d(length,0,0);
        glVertex3d(length,0,height);
        glVertex3d(0,0,height);

        //Front
        glVertex3d(0,width,0);
        glVertex3d(length,width,0);
        glVertex3d(length,width,height);
        glVertex3d(0,width,height);
    }
    glEnd();
}


void drawCylinder(double radius, double height, int slices)
{
    int i;
    double angle,angle2;
    //Bottom Circle
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
    {
        for(i=0; i<slices; i++)
        {
            angle = (2*pi)*((double)i/slices);
            glVertex3d(radius*cos(angle), radius*sin(angle),0);
        }
    }
    glEnd();


    //Top Circle
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
    {
        for(i=0; i<slices; i++)
        {
            angle = (2*pi)*((double)i/slices);
            glVertex3d(radius*cos(angle), radius*sin(angle),height);
        }
    }
    glEnd();

    //Surrounding Surface
    //glColor3f(1,0,1);
    glBegin(GL_QUADS);
    {
        for(i=0; i<slices; i++)
        {
            if(i%2==0)
            {
                glColor3f(0,1,1);
            }
            else
            {
                glColor3f(0,1,1);
            }
            angle = (2*pi)/((double)i/slices);
            angle2 = (2*pi)/((double)(i+1)/slices);

            glVertex3d(radius*cos(angle), radius*sin(angle), 0);
            glVertex3d(radius*cos(angle2), radius*sin(angle2), 0);
            glVertex3d(radius*cos(angle2), radius*sin(angle2), height);
            glVertex3d(radius*cos(angle), radius*sin(angle), height);
        }
    }
    glEnd();
}

void drawCone(double radius, double height, int slices)
{
    int i;
    double angle, angle2;
    glColor3f(1,0,0);
    //Bottom Circle
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
    {
        for(i=0; i<slices; i++)
        {
            angle = (2*pi)*((double)i/slices);
            glVertex3d(radius*cos(angle), radius*sin(angle),0);
        }
    }
    glEnd();

    //Surrounding Surface
    glColor3f(1,0,1);
    glBegin(GL_TRIANGLES);
    {
        for(i=0; i<slices; i++)
        {
            if(i%2==0)
            {
                glColor3f(0,1,0);
            }
            else
            {
                glColor3f(0,1,0);
            }
            angle = (2*pi)/((double)i/slices);
            angle2 = (2*pi)/((double)(i+1)/slices);

            glVertex3d(radius*cos(angle), radius*sin(angle), 0);
            glVertex3d(radius*cos(angle2), radius*sin(angle2), 0);
            glVertex3d(0,0,height);
        }
    }
    glEnd();
}

void drawSphere(double radius, int stacks, int slices)
{
    int i, j;
    double angle1, angle2;

    Point13D points[stacks+1][slices+1];

    for(i=0; i<=stacks; i++)
    {
        angle1 = (pi/2)*((double)i/stacks);
        for(j=0; j<=slices; j++)
        {
            angle2 = (2*pi)*((double)j/slices);
            points[i][j].x = radius*cos(angle1)*cos(angle2);
            points[i][j].y = radius*cos(angle1)*sin(angle2);
            points[i][j].z = radius*sin(angle1);
        }
    }


    glBegin(GL_QUADS);
    {
        for(i=0; i<stacks; i++)
        {
            /*if(i%2==0)
            {
                glColor3f(0,0,1);
            }
            else{
                glColor3f(0,1,0);
            }
            */
            for(j=0; j<slices; j++)
            {
                if(j%2==0)
                {
                    glColor3f(0,0,1);
                }
                else
                {
                    glColor3f(0,0,1);
                }
                //Top Hemisphere
                glVertex3d(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3d(points[i][j+1].x, points[i][j+1].y, points[i][j+1].z);
                glVertex3d(points[i+1][j+1].x, points[i+1][j+1].y, points[i+1][j+1].z);
                glVertex3d(points[i+1][j].x, points[i+1][j].y, points[i+1][j].z);

                //Bottom Hemisphere
                glVertex3d(points[i][j].x, points[i][j].y, -points[i][j].z);
                glVertex3d(points[i][j+1].x, points[i][j+1].y, -points[i][j+1].z);
                glVertex3d(points[i+1][j+1].x, points[i+1][j+1].y, -points[i+1][j+1].z);
                glVertex3d(points[i+1][j].x, points[i+1][j].y, -points[i+1][j].z);
            }
        }
    }
    glEnd();
}
void drawSphere2(double radius, int stacks, int slices)
{
    int i, j;
    double angle1, angle2;

    Point3D points[stacks+1][slices+1];

    for(i=0; i<=stacks; i++)
    {
        angle1 = (pi/2)*((double)i/stacks);
        for(j=0; j<=slices; j++)
        {
            angle2 = (2*pi)*((double)j/slices);
            points[i][j].x = radius*cos(angle1)*cos(angle2);
            points[i][j].y = radius*cos(angle1)*sin(angle2);
            points[i][j].z = radius*sin(angle1);
        }
    }


    glBegin(GL_QUADS);
    {
        for(i=0; i<stacks; i++)
        {
            /*if(i%2==0)
            {
                glColor3f(0,0,1);
            }
            else{
                glColor3f(0,1,0);
            }
            */
            for(j=0; j<slices; j++)
            {
                if(j%2==0)
                {
                    glColor3f(0,0,1);
                }
                else
                {
                    glColor3f(0,0,1);
                }
                //Top Hemisphere
                glVertex3d(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3d(points[i][j+1].x, points[i][j+1].y, points[i][j+1].z);
                glVertex3d(points[i+1][j+1].x, points[i+1][j+1].y, points[i+1][j+1].z);
                glVertex3d(points[i+1][j].x, points[i+1][j].y, points[i+1][j].z);

                //Bottom Hemisphere
                glVertex3d(points[i][j].x, points[i][j].y, -points[i][j].z);
                glVertex3d(points[i][j+1].x, points[i][j+1].y, -points[i][j+1].z);
                glVertex3d(points[i+1][j+1].x, points[i+1][j+1].y, -points[i+1][j+1].z);
                glVertex3d(points[i+1][j].x, points[i+1][j].y, -points[i+1][j].z);
            }
        }
    }
    glEnd();
}



void drawPlayer()
{
    glPushMatrix();
    {
        glTranslated(-2,-2,0);
        drawCylinder(2,10,60);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(2,2,0);
        drawCylinder(2,10,60);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(0,0,10);
        drawCone(3,10,60);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(0,0,20);
        drawSphere2(2,10,60);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(-1.5,-1.5,16);
        glRotated(145,0,1,0);
        drawCylinder(2,10,60);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(1.5,1.5,16);
        glRotated(115,0,1,0);
        drawCylinder(2,10,60);
    }
    glPopMatrix();


}
void drawBall()
{
    int i;
    glPushMatrix();
    {
        glTranslated(pos1.x,pos1.y,pos1.z);
        drawSphere(2,50,50);
    }
    glPopMatrix();

}

void display()
{

    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/

    glMatrixMode(GL_MODELVIEW);


    glLoadIdentity();



    gluLookAt(eye.x,eye.y,eye.z,	0,0,0,	0,0,1);



    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/


    //drawAxes();
    //drawGrid();


    glColor3f(1, 1, 1);
    drawCircleWithPolygon(0, 0, 90, 60);
    drawBall();
    drawPlayer();


    glutSwapBuffers();
}


void animate()
{

    time += 0.005;

    double t = time;
    double x = u*cos(A)*t;
    double y = u*sin(A)*t - 0.5*g*t*t;
    if(x>R)
    {
        time = 0;
    }
    pos1.x = x*cos(B)+7;
    pos1.y = x*sin(B)+5;
    pos1.z = y+16;
    glutPostRedisplay();
}

void init()
{



    glClearColor(0,0,0,0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(80,	1,	1,	5000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view -
    //in the X direction (horizontally) = width/height
    //near distance
    //far distance
}

void keyboard_action(unsigned char key, int x, int y)
{
    if(key == '1')
    {
        eye.x += 0.5;
    }
    else if(key == '2')
    {
        eye.x -= 0.5;
    }
    else if(key == '3')
    {
        eye.y += 0.5;
    }
    else if(key == '4')
    {
        eye.y -= 0.5;
    }
    else if(key == '5')
    {
        eye.z += 0.5;
    }
    else if(key == '6')
    {
        eye.z -= 0.5;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{

    glutInit(&argc,argv);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    /*
    GLUT_DOUBLE - allows for display on the double buffer window

    GLUT_RGB - shows color (Red, green, blue)

    GLUT_DEPTH - allows for depth buffer
    */

    glutCreateWindow("My OpenGL Program");

    init();

    glEnable(GL_DEPTH_TEST);
    //When depth testing is enabled, OpenGL tests the depth value
    //of a fragment against the content of the depth buffer

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)
    glutKeyboardFunc(keyboard_action);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}












