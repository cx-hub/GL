/*demo ��ת��ƽ�ƺ����� a,d��ֱ��Ļ��ת w��s���ź�㷨����ƽ�� ������Ҽ��Ŵ���С*/
/**/
#include <GL/glut.h>
#include<stdio.h>


void myDisplay(void)//��һ������κͺ�ɫ����
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1, 1, 1);
	glRectf(-0.5f, -0.5f, 0.5f, 0.5f);

	glColor3f(1, 0, 0);
	glRectf(-0.02f, 0.48f, 0.02f, 0.52f);

	glFlush();
}

void OnMouse(int button, int state, int x, int y)
{
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) //����Ŵ�
	{
		glScaled(1.1, 1.1, 0);
		glutPostRedisplay();
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) //�Ҽ���С
	{
		glScaled(0.91, 0.91, 0);
		glutPostRedisplay();
	}
}

void KeyBoards(unsigned char key, int x, int y)
{
	GLfloat angle = 10.0f;
	switch (key)
	{
	
	case 'a':
		glMatrixMode(GL_MODELVIEW);
		glRotatef(angle, 0, 0, 1); //��ֱ��Ļ��ת
		glutPostRedisplay();
		break;
	
	case 'd':
		glMatrixMode(GL_MODELVIEW);
		glRotatef(angle, 0, 0, -1);
		glutPostRedisplay();
		break;

	case 's':
		glMatrixMode(GL_MODELVIEW); 
		glTranslatef(0.0, -0.1, 0.0);
		glutPostRedisplay();
		break;
	
	case 'w':
		glMatrixMode(GL_MODELVIEW); //���ź�㷽��ǰ��
		glTranslatef(0.0, 0.1, 0.0);
		glutPostRedisplay();
		break;

	
	case 27:
		exit(0);
		break;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);   //��ʼ��GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(500, 200);
	glutInitWindowSize(400, 400);
	glutCreateWindow("OpenGL");
	glutMouseFunc(&OnMouse);  //ע������¼�
	glutKeyboardFunc(&KeyBoards);  //ע������¼�
	glutDisplayFunc(&myDisplay);   //�ص�����
	glutMainLoop();    //������ʾ�������ڸı�����»���ͼ��
	return 0;
}
