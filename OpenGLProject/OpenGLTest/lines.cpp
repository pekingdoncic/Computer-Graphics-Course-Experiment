
/*  
 *  lines.c  
 *  This program demonstrates different line stipples and widths.
 */
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define	drawOneLine(x1,y1,x2,y2) glBegin(GL_LINES); \
	glVertex2f ((x1),(y1)); glVertex2f ((x2),(y2)); glEnd();

void myinit (void) {
    /*  background to be cleared to black	*/
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_FLAT);
}

void display(void)
{
    int i;

    glClear (GL_COLOR_BUFFER_BIT);
/*  draw all lines in white	*/
    glColor3f (1.0, 1.0, 1.0);

/*  in 1st row, 3 lines drawn, each with a different stipple	*/
    glEnable (GL_LINE_STIPPLE);
    glLineStipple (1, 0x0101);	/*  dotted	*/
    drawOneLine (50.0, 125.0, 150.0, 125.0);
    glLineStipple (1, 0x00FF);	/*  dashed	*/
    drawOneLine (150.0, 125.0, 250.0, 125.0);
    glLineStipple (1, 0x1C47);	/*  dash/dot/dash	*/
    drawOneLine (250.0, 125.0, 350.0, 125.0);

/*  in 2nd row, 3 wide lines drawn, each with different stipple	*/
    glLineWidth (5.0);
    glLineStipple (1, 0x0101);
    drawOneLine (50.0, 100.0, 150.0, 100.0);
    glLineStipple (1, 0x00FF);
    drawOneLine (150.0, 100.0, 250.0, 100.0);
    glLineStipple (1, 0x1C47);
    drawOneLine (250.0, 100.0, 350.0, 100.0);
    glLineWidth (1.0);

/*  in 3rd row, 6 lines drawn, with dash/dot/dash stipple,	*/
/*  as part of a single connect line strip			*/
    glLineStipple (1, 0x1C47);
    glBegin (GL_LINE_STRIP);
    for (i = 0; i < 7; i++)
	glVertex2f (50.0 + ((GLfloat) i * 50.0), 75.0);
    glEnd ();

/*  in 4th row, 6 independent lines drawn,	*/
/*  with dash/dot/dash stipple			*/
    for (i = 0; i < 6; i++) {
	drawOneLine (50.0 + ((GLfloat) i * 50.0), 
	    50.0, 50.0 + ((GLfloat)(i+1) * 50.0), 50.0);
    }

/*  in 5th row, 1 line drawn, with dash/dot/dash stipple	*/
/*  and repeat factor of 5			*/
    glLineStipple (5, 0x1C47);
    drawOneLine (50.0, 25.0, 350.0, 25.0);
    glFlush ();
}

/*  Called when the window is first opened and whenever 
 *  the window is reconfigured (moved or resized).
 */
void  myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluOrtho2D (0.0, 400.0, 0.0, 150.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();	
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
	   glutInitWindowSize (400, 150); 
	   glutInitWindowPosition (0, 0);
	   //init the defined window with "argv[1]" as topic showed on the top the window
	   glutCreateWindow (argv[0]);
	   // opengl setup
	   myinit ();
	   //define callbacks
	   glutReshapeFunc(myReshape);		  
	   glutDisplayFunc(display); 
	   //enter the loop for display
	   glutMainLoop();
	   return 0;	   
}
