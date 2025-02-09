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

#define BMP_Header_Length 54  //图像数据在内存块中的偏移量

static int spin = 0;
static int obj = TORUS;
static int begin;

float translateX = 0.0;//水平方向平移量
float translateY = 0.0;//垂直方向平移量
float angle = 0.0;//旋转角度
float scaleValue = 0.5;//缩放系数

int textangle = 0;//纹理旋转角度

//自定义的灯光的颜色
GLfloat Lightcolor[] = {0.0, 1.0, 1.0};

//自定义物体的rgb分量：
float R=0, G=0, B=0;
// 将立方体的八个顶点保存到一个数组里面
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
//绘制次序
static const GLint index_list[][4] = {
	0, 2, 3, 1,
	0, 4, 6, 2,
	0, 1, 5, 4,
	4, 5, 7, 6,
	1, 3, 7, 5,
	2, 6, 7, 3,
};
//纹理绘制次序
static const GLfloat coord_list[][2] = {
	0,0,
	0,1,
	1,1,
	1,0,
};


//计时器，循环控制贴图立方体旋转
void Timer(int id)
{
	//改变角度
	textangle += 1;
	textangle %= 360;
	glutPostRedisplay();
	glutTimerFunc(16, Timer, 1);//函数中需要调用一次来保证循环
}


// 函数power_of_two用于判断一个整数是不是2的整数次幂
int power_of_two(int n)
{
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

/* 函数load_texture
* 读取一个BMP文件作为纹理
* 如果失败，返回0，如果成功，返回纹理编号
*/
GLuint load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID = 0, texture_ID = 0;

	// 打开文件，如果失败，返回
	FILE* pFile = fopen(file_name, "rb");
	if (pFile == 0)
		return 0;

	// 读取文件中图象的宽度和高度
	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, 4, 1, pFile);
	fread(&height, 4, 1, pFile);
	fseek(pFile, BMP_Header_Length, SEEK_SET);

	// 计算每行像素所占字节数，并根据此数据计算总像素字节数
	{
		GLint line_bytes = width * 3;
		while (line_bytes % 4 != 0)
			++line_bytes;
		total_bytes = line_bytes * height;
	}

	// 根据总像素字节数分配内存
	pixels = (GLubyte*)malloc(total_bytes);
	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	// 读取像素数据
	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 对就旧版本的兼容，如果图象的宽度和高度不是的整数次方，则需要进行缩放
	// 若图像宽高超过了OpenGL规定的最大值，也缩放
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
		if (!power_of_two(width)
			|| !power_of_two(height)
			|| width > max
			|| height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
			GLint new_line_bytes, new_total_bytes;
			GLubyte* new_pixels = 0;

			// 计算每行需要的字节数和总字节数
			new_line_bytes = new_width * 3;
			while (new_line_bytes % 4 != 0)
				++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			// 分配内存
			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			// 进行像素缩放
			gluScaleImage(GL_RGB,
				width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			// 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
			free(pixels);
			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	// 分配一个新的纹理编号
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 绑定新的纹理，载入纹理并设置纹理参数
// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
	GLint lastTextureID = last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);//载入一个二维的纹理
	glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定,width和height是二维纹理像素的宽度和高度,尽量使用大小为2的整数次方的纹理，
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
	  glDepthFunc(GL_LESS);// 设置深度测试函数为GL_LESS，用于处理深度缓冲
	  glEnable(GL_TEXTURE_2D);//开启纹理
	 glEnable(GL_DEPTH_TEST);// z - buffer消隐	
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
	  
	  //自定义一些光照：
	  GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
	  GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };//环境光的位置
	 GLfloat mat_ambient_color[] = { 1.0, 0.2, 0.2, 1.0 };//环境光的颜色
	  GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };//漫发射光的位置
	  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };//镜面反射光的位置
	  GLfloat no_shininess[] = { 0.0 };//无高光
	  GLfloat low_shininess[] = { 5.0 };//低强度高光
	  GLfloat mat_emission[] = { 0.3, 0.2, 0.2, 0.0 }; //设置材质的自发光颜色

	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  glTranslatef(0.0, 0.0, -5.0);

	  glPushMatrix();// 保存当前模型视图矩阵，用于后续的光源方向变换
		  glRotated((GLdouble) spin, 0.0, 1.0, 0.0);// 绕Y轴旋转光源方向
		  // 设置光源属性
		  glLightfv(GL_LIGHT0, GL_POSITION, position);// 设置光源位置
		  glLightfv(GL_LIGHT0, GL_SMOOTH, Lightcolor);// 设置光源光照模式为平滑
		  glLightfv(GL_LIGHT0, GL_DIFFUSE, Lightcolor); // 设置漫反射光成分
		  glLightfv(GL_LIGHT0, GL_AMBIENT, Lightcolor);// 设置环境光颜色
		  
		  // 画光源正方形位置
		  glTranslated(0.0, 0.0, 1.5); // 平移到指定位置
		  glDisable(GL_LIGHTING);// 关闭光照以便绘制正方形
		  glColor3f(0.0,1.0,0.0);// 设置正方形颜色
		  glutWireCube(0.1);// 绘制正方形边框粗细
		  glEnable(GL_LIGHTING); // 重新启用光照
	  glPopMatrix();// 恢复之前保存的模型视图矩阵

	  //对所有物品进行平移缩放旋转，修改其参数
	  glTranslatef(translateX, translateY, 0.0);
	  glRotatef(angle, 0.0, 0.0, 1.0);
	  glScalef(scaleValue, scaleValue, 1.0);
	  //定义纹理贴图：
	  GLuint texture;//定义读取纹理贴图的变量
	  
	  switch (obj) {
	  case TORUS:
		  //仅有漫反射光而无环境光和镜面光
		  glEnable(GL_COLOR_MATERIAL);
		  glPushMatrix();
		  // 设置材质属性
		  glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);// 设置环境光反射材质颜色
		  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);// 设置漫反射材质颜色
		  glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);// 设置镜面反射材质颜色
		  glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);// 设置镜面高光指数
		  glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);// 设置自发光材质颜色
		  glColor3f(0.5 + R, 0.2 + G, 0.2 + B);
		  glRotatef(120, 1, 0, 0);
		  glutSolidTorus(0.275, 0.85, 20, 20);// 画实心圆环
		  // dInnerRadius  圆环柱体半径  
		  //dOuterRadius  圆环半径
		  //nSides  各个径向剖面的边长数
		  //nRings  径向部面的个数
		 // glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		  glPopMatrix();
		  glDisable(GL_COLOR_MATERIAL);
		break;
	  
	  case TEAPOT://有漫反射光和镜面光，并有低高光，而无环境光
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
	  
	  case DOD://有漫反射光和有颜色的环境光以及辐射光，而无镜面光
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

	  case MutiObject://绘制多个物体，对比实验有无消隐对于视觉效果的差别
		  glEnable(GL_COLOR_MATERIAL);
		  glColor3f(0 + R, 0.5 + G, 0.6 + B);
		  glEnable(GL_CULL_FACE);
		  glCullFace(GL_FRONT_AND_BACK);
		  glDisable(GL_CULL_FACE);
		  glutSolidTeapot(0.8);//画实心茶壶
		  glPushMatrix();
		  glTranslated(0.3, 0.0, 1.2);
		  glRotated(120, 90, 0, 0);
		  glScaled(0.3, 0.3, 0.3);
		  glColor3f(0.1 + R, 0.3 + G, 0.2 + B);
		  glutSolidTorus(0.275, 0.85, 20, 20);//画第二个多面体
		  glPopMatrix();
		  glPushMatrix();
		  glTranslated(-0.3, 0.0, 0.7);
		  glRotated(110, 70, 0, 0);
		  glScaled(0.5, 0.5, 0.5);
		  glColor3f(0.5 + R, 0.1 + G, 0.2 + B);
		  glutSolidTorus(0.275, 0.85, 20, 20);//画第三个多面体
		  glPopMatrix();
		  glDisable(GL_COLOR_MATERIAL);	  
		  break;
	 
	  case textture://绘制贴有纹理的正方体
		  glRotatef(30, 1.0f, 0.0f, 0.0f); //旋转
		  glRotatef(textangle, 0.0f, 1.0f, 0.0f); //旋转
		  // 加载纹理
		  texture = load_texture("1.bmp");  //加载纹理
		  glBindTexture(GL_TEXTURE_2D, texture);// 使用 glBegin 开始绘制图元，图元类型为四边形
		  glBegin(GL_QUADS);
		  for (int i = 0; i < 6; ++i) {
			  // 有六个面，循环六次
			  for (int j = 0; j < 4; ++j) {
				  // 每个面有四个顶点，循环四次
				  //纹理
				  // 设置纹理坐标
				  glTexCoord2f(coord_list[j][0], coord_list[j][1]);
				  //正方体
				   // 设置顶点坐标并绘制
				  glVertex3fv(vertex_list[index_list[i][j]]);
			  }
		  }
		  glEnd();// glEnd 表示图元绘制结束
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
	//平移
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
	//旋转
	case 'q':
		angle += 1.0;
		break;
	case 'e':
		angle -= 1.0;
		break;
	//缩放
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
	case'r'://对几何对象材料的修改
		R -= 0.05;
		break;
	case'R':
		R += 0.05;
		break;
	case'g'://对几何对象材料的修改
		G -= 0.05;
		break;
	case'G':
		G += 0.05;
		break;
	case'b'://对几何对象材料的修改
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
	  glutTimerFunc(16, Timer, 1);//计时器定时运行
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
