#include <GL/glut.h>

float translateX = 0.0;
float translateY = 0.0;
float angle = 0.0;
float scaleValue = 0.5;

// ��ʼ��
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-100.0, 100.0, -100.0, 100.0);
}

// ������Ʒ
void drawCustomObject() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // ���ñ�����ɫΪ��ɫ
    glClear(GL_COLOR_BUFFER_BIT); // �����ɫ������

    glColor3f(1.0, 1.0, 1.0); // ������ɫΪ��ɫ
    glutSolidTeapot(1.0); // ����һ����ɫ�Ĳ��

    glFlush(); // ˢ��OpenGL����
}

// ����ͼ��
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
   
    glTranslatef(translateX, translateY, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0);
    glScalef(scaleValue, scaleValue, 1.0);
    drawCustomObject();
    glFlush();
}

// �����¼�����
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

// ����¼�����
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
