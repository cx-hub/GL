#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

static float angle = 0.0f, ratio;
static float x = 0.0f, y = 1.75f, z = 5.0f;
static float lx = 0.0f, ly = 0.0f, lz = -1.0f;
static GLint snowman_list;

//����۲췽ʽ
void changeSize(int w, int h)
{
    //����0�����
    if (h == 0)
        h = 1;
    ratio = 1.0f * w / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //�����ӿ�Ϊ�������ڴ�С
    glViewport(0, 0, w, h);

    //���ÿ��ӿռ�
    gluPerspective(45, ratio, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}

//������ʾ�б�����ѩ�ˣ���ʼ����������Ⱦ����
void drawSnowMan(int eyes)
{
    glColor3f(1.0f, 1.0f, 1.0f);

    //������
    glTranslatef(0.0f, 0.75f, 0.0f);
    glutSolidSphere(0.75f, 20, 20);

    //��ͷ
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.25f, 20, 20);

    //���۾�
    if (eyes)
    {
        glPushMatrix();
        glColor3f(0.0f, 0.0f, 0.0f);
        glTranslatef(0.05f, 0.10f, 0.18f);
        glutSolidSphere(0.05f, 10, 10);
        glTranslatef(-0.1f, 0.0f, 0.0f);
        glutSolidSphere(0.05f, 10, 10);
        glPopMatrix();
    }

    //������
    glColor3f(1.0f, 0.5f, 0.5f);
    glRotatef(0.0f, 1.0f, 0.0f, 0.0f);
    glutSolidCone(0.08f, 0.5f, 10, 2);
}

GLuint createDL()
{
    GLuint snowManDL;

    //����һ����ʾ�б��
    snowManDL = glGenLists(1);

    //��ʼ��ʾ�б�
    glNewList(snowManDL, GL_COMPILE);

    drawSnowMan(1);

    glEndList();

    return (snowManDL);
}

void initScenne()
{
    glEnable(GL_DEPTH_TEST);
    snowman_list = createDL();
}

void renderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //������
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();
    //��9��ѩ��
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            glPushMatrix();
            glTranslatef(i * 3.0, 0, j * 3.0);
            glCallList(snowman_list);
            glPopMatrix();
        }
    glutSwapBuffers();
}

//��ת���
void orientMe(float ang)
{
    lx = sin(ang);
    lz = -cos(ang);
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}
//�ƶ����
void moveMeFlat(int direction)
{
    x = x + direction * (lx) * 0.1;
    z = z + direction * (lz) * 0.1;
    glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);
}
//������Ӧ
//���Ҽ���ת��������¼��ƶ����
void inputKey(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        angle -= 0.01f;
        orientMe(angle);
        break;
    case GLUT_KEY_RIGHT:
        angle += 0.01f;
        orientMe(angle);
        break;
    case GLUT_KEY_UP:
        moveMeFlat(1);
        break;
    case GLUT_KEY_DOWN:
        moveMeFlat(-1);
        break;
    default:
        break;
    }
}

int main(int argc, char** argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 600);
    glutCreateWindow("snowman test");

    initScenne();
    glutSpecialFunc(inputKey);
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutMainLoop();
    return 0;
}