
/*
 *  clip.cpp
 *  This program clips the line with the given window.
 */
#include <windows.h>
#include <GL/glut.h>
#include <math.h>


void myinit(void)
{
    glShadeModel (GL_FLAT);
    glClearColor (0.0, 0.0, 0.0, 0.0);
}


void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) 
	gluOrtho2D (0.0, 1.0, 0.0, 1.0*(GLfloat)h/(GLfloat)w);
    else 
	gluOrtho2D (0.0, 1.0*(GLfloat)w/(GLfloat)h, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

//-------------------------------
// your task!!!
//-------------------------------

void myclip()
// line clipping algorithm 
{

	

}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
// The window
// ------------------------------------
//  you can change the window definition on yourself.
// ------------------------------------
    glColor4f (0.0, 1.0, 1.0, 0.75);
	glBegin(GL_POLYGON);
	    glVertex2f( 0.2f, 0.3f); // Bottom Left
		glVertex2f( 0.8f, 0.3f); // Bottom Left
		glVertex2f( 0.8f, 0.7f); // Bottom Right
		glVertex2f( 0.2f, 0.7f); // Bottom Right
	glEnd();
// ------------------------------------
//	please define your own line segment and draw 
//	it here with different color and line width
// ------------------------------------


 	//-------------------------------
	//do the clipping in myclip() funtion 
	//-------------------------------
   myclip();
// ------------------------------------
//	please draw clipped line here with another 
//  color and line width
// ------------------------------------   


    glFlush();
}


/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA);
   //define size and the relative positon of the applicaiton window on the display
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
 	//init the defined window with "argv[1]" as topic showed on the top the window
   glutCreateWindow (argv[0]);
	// opengl setup
   myinit ();

	//define callbacks
   glutDisplayFunc(display); 
   glutReshapeFunc(myReshape);
   //enter the loop for display
   glutMainLoop();

	return 1;
}
