#include <GL/glut.h>

float translateX = 0.0;
float translateY = 0.0;
float angle = 0.0;
float scaleValue = 0.5;

// 初始化
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-100.0, 100.0, -100.0, 100.0);
}

// 绘制物品
void drawCustomObject() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // 设置背景颜色为黑色
    glClear(GL_COLOR_BUFFER_BIT); // 清空颜色缓冲区

    glColor3f(1.0, 1.0, 1.0); // 设置颜色为白色
    glutSolidTeapot(1.0); // 绘制一个白色的茶壶

    glFlush(); // 刷新OpenGL管线
}

// 绘制图形
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
   
    glTranslatef(translateX, translateY, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glScalef(scaleValue, scaleValue, 1.0);
    drawCustomObject();
    glFlush();
}

// 键盘事件处理
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
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
    case 'q':
        angle += 1.0;
        break;
    case 'e':
        angle -= 1.0;
        break;
    }
    glutPostRedisplay();
}

// 鼠标事件处理
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        scaleValue += 0.1;
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        scaleValue -= 0.1;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Transformations in OpenGL");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
