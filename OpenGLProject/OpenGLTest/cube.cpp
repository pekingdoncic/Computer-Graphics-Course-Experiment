#include <windows.h>
#include <GL/glut.h>

void init(void) 
{
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glShadeModel (GL_FLAT);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *   
 Anything you want to display on the screen, draw it here. * *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Clear the screen.  Set the current color to white.
 *  Draw the wire frame cube. 
 */

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);
    glLoadIdentity ();	/*  clear the matrix	*/
//--------------- you can define your own transformation-----------
    glTranslatef (0.0, 0.0, -5.0);	 
    glScalef (1.0, 2.0, 1.0);	 

//---------------- you can draw something else here-----------
    glutWireCube (1.0);  /*  draw the cube	*/
    glFlush();
}


/*  Called when the window is first opened and whenever 
 *  the window is reconfigured (moved or resized).
 */

void reshape (int w, int h)
{
	/*  define the projection  */
    glMatrixMode (GL_PROJECTION); 
    glLoadIdentity ();
    glFrustum (-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);	 
    glMatrixMode (GL_MODELVIEW);	/*  back to modelview matrix	*/
    glViewport (0, 0, w, h);	/*  define the viewport	*/
}


/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   //define size and the relative positon of the applicaiton window on the display
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
 	//init the defined window with "argv[1]" as topic showed on the top the window
   glutCreateWindow (argv[0]);
	// opengl setup
   init ();
	//define callbacks
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   //enter the loop for display
   glutMainLoop();
   return 0;
}
