#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <GL/glut.h>
#include<iostream>
//using namespace std;
#define TORUS 0
#define TEAPOT 1
#define DOD 2
#define TET 3
#define ISO 4
#define MutiObject 5
#define textture 6
#define QUIT 7

#define BMP_Header_Length 54  //ͼ���������ڴ���е�ƫ����

static int spin = 0;
static int obj = TORUS;
static int begin;

float translateX = 0.0;//ˮƽ����ƽ����
float translateY = 0.0;//��ֱ����ƽ����
float angle = 0.0;//��ת�Ƕ�
float scaleValue = 0.5;//����ϵ��

int textangle = 0;//������ת�Ƕ�

//�Զ���ĵƹ����ɫ
GLfloat Lightcolor[] = {0.0, 1.0, 1.0};

//�Զ��������rgb������
float R=0, G=0, B=0;
// ��������İ˸����㱣�浽һ����������
static const GLfloat vertex_list[][3] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
};
//���ƴ���
static const GLint index_list[][4] = {
	0, 2, 3, 1,
	0, 4, 6, 2,
	0, 1, 5, 4,
	4, 5, 7, 6,
	1, 3, 7, 5,
	2, 6, 7, 3,
};
//������ƴ���
static const GLfloat coord_list[][2] = {
	0,0,
	0,1,
	1,1,
	1,0,
};


//��ʱ����ѭ��������ͼ��������ת
void Timer(int id)
{
	//�ı�Ƕ�
	textangle += 1;
	textangle %= 360;
	glutPostRedisplay();
	glutTimerFunc(16, Timer, 1);//��������Ҫ����һ������֤ѭ��
}


// ����power_of_two�����ж�һ�������ǲ���2����������
int power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

/* ����load_texture
* ��ȡһ��BMP�ļ���Ϊ����
* ���ʧ�ܣ�����0������ɹ�������������
*/
GLuint load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID = 0, texture_ID = 0;

	// ���ļ������ʧ�ܣ�����
	FILE* pFile = fopen(file_name, "rb");
	if (pFile == 0)
		return 0;

	// ��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶�
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	// ����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ���
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	// �����������ֽ��������ڴ�
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	// ��ȡ��������
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// �Ծ;ɰ汾�ļ��ݣ����ͼ��Ŀ�Ⱥ͸߶Ȳ��ǵ������η�������Ҫ��������
	// ��ͼ���߳�����OpenGL�涨�����ֵ��Ҳ����
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!power_of_two(width)
			|| !power_of_two(height)
			|| width > max
			|| height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // �涨���ź��µĴ�СΪ�߳���������
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			// ����ÿ����Ҫ���ֽ��������ֽ���
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			// �����ڴ�
			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			// ������������
			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			// �ͷ�ԭ�����������ݣ���pixelsָ���µ��������ݣ�����������width��height
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	// ����һ���µ�������
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// ���µ������������������������
// �ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
	GLint lastTextureID = last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);//����һ����ά������
	glBindTexture(GL_TEXTURE_2D, lastTextureID);  //�ָ�֮ǰ�������,width��height�Ƕ�ά�������صĿ�Ⱥ͸߶�,����ʹ�ô�СΪ2�������η�������
	free(pixels);
	return texture_ID;
}

void menu_select(int item)
{
  if (item == QUIT)
    exit(0);
  obj = item;
  glutPostRedisplay();
}



/* ARGSUSED2 */
void movelight(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    begin = x;
  }
}

/* ARGSUSED1 */
void motion(int x, int y)
{
  spin = (spin + (x - begin)) % 360;
  begin = x;
  glutPostRedisplay();
}

void myinit(void)
{
	  glEnable(GL_LIGHTING);
	  glEnable(GL_LIGHT0);
	  glDepthFunc(GL_LESS);// ������Ȳ��Ժ���ΪGL_LESS�����ڴ�����Ȼ���
	  glEnable(GL_TEXTURE_2D);//��������
	 glEnable(GL_DEPTH_TEST);// z - buffer����	
}

/*  Here is where the light position is reset after the modeling
 *  transformation (glRotated) is called.  This places the 
 *  light at a new position in world coordinates.  The cube
 *  represents the position of the light.
 */
void display(void)
{
	  GLfloat position[] =
	  {0.0, 0.0, 1.5, 1.0};
	  
	  //�Զ���һЩ���գ�
	  GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	  GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };//�������λ��
	 GLfloat mat_ambient_color[] = { 1.0, 0.2, 0.2, 1.0 };//���������ɫ
	  GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };//��������λ��
	  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };//���淴����λ��
	  GLfloat no_shininess[] = { 0.0 };//�޸߹�
	  GLfloat low_shininess[] = { 5.0 };//��ǿ�ȸ߹�
	  GLfloat mat_emission[] = { 0.3, 0.2, 0.2, 0.0 }; //���ò��ʵ��Է�����ɫ

	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glTranslatef(0.0, 0.0, -5.0);

	  glPushMatrix();// ���浱ǰģ����ͼ�������ں����Ĺ�Դ����任
		  glRotated((GLdouble) spin, 0.0, 1.0, 0.0);// ��Y����ת��Դ����
		  // ���ù�Դ����
		  glLightfv(GL_LIGHT0, GL_POSITION, position);// ���ù�Դλ��
		  glLightfv(GL_LIGHT0, GL_SMOOTH, Lightcolor);// ���ù�Դ����ģʽΪƽ��
		  glLightfv(GL_LIGHT0, GL_DIFFUSE, Lightcolor); // �����������ɷ�
		  glLightfv(GL_LIGHT0, GL_AMBIENT, Lightcolor);// ���û�������ɫ
		  
		  // ����Դ������λ��
		  glTranslated(0.0, 0.0, 1.5); // ƽ�Ƶ�ָ��λ��
		  glDisable(GL_LIGHTING);// �رչ����Ա����������
		  glColor3f(0.0,1.0,0.0);// ������������ɫ
		  glutWireCube(0.1);// ���������α߿��ϸ
		  glEnable(GL_LIGHTING); // �������ù���
	  glPopMatrix();// �ָ�֮ǰ�����ģ����ͼ����

	  //��������Ʒ����ƽ��������ת���޸������
	  glTranslatef(translateX, translateY, 0.0);
	  glRotatef(angle, 0.0, 0.0, 1.0);
	  glScalef(scaleValue, scaleValue, 1.0);
	  //����������ͼ��
	  GLuint texture;//�����ȡ������ͼ�ı���
	  
	  switch (obj) {
	  case TORUS:
		  //�������������޻�����;����
		  glEnable(GL_COLOR_MATERIAL);
		  glPushMatrix();
		  // ���ò�������
		  glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);// ���û����ⷴ�������ɫ
		  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);// ���������������ɫ
		  glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);// ���þ��淴�������ɫ
		  glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);// ���þ���߹�ָ��
		  glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);// �����Է��������ɫ
		  glColor3f(0.5 + R, 0.2 + G, 0.2 + B);
		  glRotatef(120, 1, 0, 0);
		  glutSolidTorus(0.275, 0.85, 20, 20);// ��ʵ��Բ��
		  // dInnerRadius  Բ������뾶  
		  //dOuterRadius  Բ���뾶
		  //nSides  ������������ı߳���
		  //nRings  ������ĸ���
		 // glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		  glPopMatrix();
		  glDisable(GL_COLOR_MATERIAL);
		break;
	  
	  case TEAPOT://���������;���⣬���е͸߹⣬���޻�����
		 glEnable(GL_COLOR_MATERIAL);
		 glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
		 glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		 glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		 glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
		 glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
		glColor3f(0.0 + R, 0.2 + G, 0.4 + B);
		glutSolidTeapot(1.0);
		glDisable(GL_COLOR_MATERIAL);
		break;
	  
	  case DOD://��������������ɫ�Ļ������Լ�����⣬���޾����
		  glEnable(GL_COLOR_MATERIAL);
		  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
		  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		  glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		  glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
		  glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
		glColor3f(0.0 + R, 0 + G, 0.5 + B);
		glutSolidDodecahedron();
		glDisable(GL_COLOR_MATERIAL);
		break;
	 
	  case TET:
		  glColor3f(0.0 + R, 0.2 + G, 0.4 + B);
		glutSolidTetrahedron();
		break;
	  
	  case ISO:
		  glColor3f(0.0 + R, 1 + G, 0.5 + B);
		glutSolidIcosahedron();
		break;

	  case MutiObject://���ƶ�����壬�Ա�ʵ���������������Ӿ�Ч���Ĳ��
		  glEnable(GL_COLOR_MATERIAL);
		  glColor3f(0 + R, 0.5 + G, 0.6 + B);
		  glEnable(GL_CULL_FACE);
		  glCullFace(GL_FRONT_AND_BACK);
		  glDisable(GL_CULL_FACE);
		  glutSolidTeapot(0.8);//��ʵ�Ĳ��
		  glPushMatrix();
		  glTranslated(0.3, 0.0, 1.2);
		  glRotated(120, 90, 0, 0);
		  glScaled(0.3, 0.3, 0.3);
		  glColor3f(0.1 + R, 0.3 + G, 0.2 + B);
		  glutSolidTorus(0.275, 0.85, 20, 20);//���ڶ���������
		  glPopMatrix();
		  glPushMatrix();
		  glTranslated(-0.3, 0.0, 0.7);
		  glRotated(110, 70, 0, 0);
		  glScaled(0.5, 0.5, 0.5);
		  glColor3f(0.5 + R, 0.1 + G, 0.2 + B);
		  glutSolidTorus(0.275, 0.85, 20, 20);//��������������
		  glPopMatrix();
		  glDisable(GL_COLOR_MATERIAL);	  
		  break;
	 
	  case textture://�������������������
		  glRotatef(30, 1.0f, 0.0f, 0.0f); //��ת
		  glRotatef(textangle, 0.0f, 1.0f, 0.0f); //��ת
		  // ��������
		  texture = load_texture("1.bmp");  //��������
		  glBindTexture(GL_TEXTURE_2D, texture);// ʹ�� glBegin ��ʼ����ͼԪ��ͼԪ����Ϊ�ı���
		  glBegin(GL_QUADS);
		  for (int i = 0; i < 6; ++i) {
			  // �������棬ѭ������
			  for (int j = 0; j < 4; ++j) {
				  // ÿ�������ĸ����㣬ѭ���Ĵ�
				  //����
				  // ������������
				  glTexCoord2f(coord_list[j][0], coord_list[j][1]);
				  //������
				   // ���ö������겢����
				  glVertex3fv(vertex_list[index_list[i][j]]);
			  }
		  }
		  glEnd();// glEnd ��ʾͼԪ���ƽ���
		  break;
	  }

  glPopMatrix();
  glutSwapBuffers();
}

void myReshape(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, (GLfloat) w / (GLfloat) h, 1.0, 20.0);
  glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	//ƽ��
	case 'w':
		translateY += 0.1;
		break;
	case 's':
		translateY -= 0.1;
		break;
	case 'a':
		translateX -= 0.1;
		break;
	case 'd':
		translateX += 0.1;
		break;
	//��ת
	case 'q':
		angle += 1.0;
		break;
	case 'e':
		angle -= 1.0;
		break;
	//����
	case 'i':
		scaleValue -= 0.1;
		break;
	case 'o':
		scaleValue += 0.1;
		break;
	case'1':
		Lightcolor[0] = 0.0;
		Lightcolor[1] = 1.0;
		Lightcolor[2] = 1.0;
		break;
	case'2':
		Lightcolor[0] = 0.0;
		Lightcolor[1] = 1.0;
		Lightcolor[2] = 0.0;
		break;
	case'r'://�Լ��ζ�����ϵ��޸�
		R -= 0.05;
		break;
	case'R':
		R += 0.05;
		break;
	case'g'://�Լ��ζ�����ϵ��޸�
		G -= 0.05;
		break;
	case'G':
		G += 0.05;
		break;
	case'b'://�Լ��ζ�����ϵ��޸�
		B -= 0.05;
		break;
	case'B':
		B += 0.05;
		break;
	}
	glutPostRedisplay();
}




/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char **argv)
{
	  glutInit(&argc, argv);
	  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	  glutInitWindowSize(500, 500);
	  glutCreateWindow(argv[0]);
	  myinit();
	  glutMouseFunc(movelight);
	  glutMotionFunc(motion);
	  glutReshapeFunc(myReshape);
	  glutDisplayFunc(display);
	  glutTimerFunc(16, Timer, 1);//��ʱ����ʱ����
	  glutKeyboardFunc(keyboard);

	  glutCreateMenu(menu_select);
	  glutAddMenuEntry("Torus", TORUS);
	  glutAddMenuEntry("Teapot", TEAPOT);
	  glutAddMenuEntry("Dodecahedron", DOD);
	  glutAddMenuEntry("Tetrahedron", TET);
	  glutAddMenuEntry("Icosahedron", ISO);
	  glutAddMenuEntry("MutiObject", MutiObject);
	  glutAddMenuEntry("TextTure", textture);
	  glutAddMenuEntry("Quit", QUIT);
	  glutAttachMenu(GLUT_RIGHT_BUTTON);
	  glutMainLoop();
	  return 0;             /* ANSI C requires main to return int. */
}
