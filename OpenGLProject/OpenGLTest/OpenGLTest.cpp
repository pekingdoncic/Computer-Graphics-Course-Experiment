// OpenGLTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <GL/glut.h>

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);//清空色彩缓存区
	glRectf(-0.5f, -0.5f, 0.5f, 0.5f);//绘制
	glFlush();

}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);//初始化glut  （可不写默认）
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);//显示模式初始化 RGB显示模式，单缓冲模式
	glutInitWindowPosition(100, 100);//窗口窗口位置  （可不写默认）
	glutInitWindowSize(400, 400);//窗口大小  （可不写默认）
	glutCreateWindow("第一个OpenGl程序");//窗口名称
	glutDisplayFunc(&myDisplay);//自定义函数绘图
	glutMainLoop();//消息循环相应
	return 0;
}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
