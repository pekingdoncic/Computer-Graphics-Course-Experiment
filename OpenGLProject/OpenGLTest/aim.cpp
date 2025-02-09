/*
 *  aim.c
 *  This program calculates the fovy (field of view angle
 *  in the y direction), by using trigonometry, given the
 *  size of an object and its size.
 */
#include <windows.h>
#include <stdio.h>   
#include <math.h>
#include <GL/glut.h>

void myinit (void) {
    glShadeModel (GL_FLAT);
}

/*  Clear the screen.  Set the current color to white.
 *  Draw the wire frame cube and sphere.
 */
void   display (void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);
	//about front and back facing facets and facet cullig
// 	glCullFace(GL_BACK);
// 	glCullFace(GL_FRONT);
  	glEnable(GL_CULL_FACE);
    glLoadIdentity ();
/*	glTranslatef() as viewing transformation	*/
    glTranslatef(-1.5f,0.0f,-15.0f);
	glBegin(GL_TRIANGLES); // Drawing Using Triangles
	glVertex3f( 0.0f, 1.0f, 0.0f); // Top
	glVertex3f(-1.0f,-1.0f, 0.0f); // Bottom Left
	glVertex3f( 1.0f,-1.0f, 0.0f); // Bottom Right
	glEnd();
	//definition of front facing polygon
 	glFrontFace(GL_CW);
    glTranslatef(3.0f,0.0f,0.0f);
	glBegin(GL_QUADS); // Draw A Quad
	glVertex3f(-1.0f, 1.0f, 0.0f); // Top Left
	glVertex3f( 1.0f, 1.0f, 0.0f); // Top Right
	glVertex3f( 1.0f,-1.0f, 0.0f); // Bottom Right
	glVertex3f(-1.0f,-1.0f, 0.0f); // Bottom Left
	glEnd(); // Done Drawing The Quad

    glFlush();
}



#define PI  3.1415926535

/*  atan2 () is a system math routine which calculates
 *  the arctangent of an angle, given length of the 
 *  opposite and adjacent sides of a right triangle.
 *  atan2 () is not an OpenGL routine.
 */
GLdouble calculateAngle (double size, double distance)
{
    GLdouble radtheta, degtheta;
    
    radtheta = 2.0 * atan2 (size/2.0, distance);
    degtheta = (180.0 * radtheta) / PI;
    printf ("degtheta is %lf\n", degtheta);
    return ((GLdouble) degtheta);
}

/*  Called when the window is first opened and whenever 
 *  the window is reconfigured (moved or resized).
 */
void  myReshape(int w, int h)
{
    GLdouble theta;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    theta = calculateAngle (2.0, 5.0);
    gluPerspective(theta, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*  Main Loop		
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
void main(int argc, char** argv)
{

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   //define size and the relative positon of the applicaiton window on the display
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
 	//init the defined window with "argv[1]" as topic showed on the top the window
   glutCreateWindow (argv[0]);
	// opengl setup
   myinit ();
	//define callbacks
   glutReshapeFunc(myReshape);
   glutDisplayFunc(display); 
  //enter the loop for display
   glutMainLoop();


}
