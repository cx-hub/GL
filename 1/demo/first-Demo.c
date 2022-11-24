/*demo 旋转，平移和缩放 a,d垂直屏幕旋转 w，s沿着红点法向量平移 左键，右键放大缩小*/
/**/
#include <GL/glut.h>
#include<stdio.h>


void myDisplay(void)//画一个大矩形和红色矩形
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
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) //左键放大
	{
		glScaled(1.1, 1.1, 0);
		glutPostRedisplay();
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) //右键缩小
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
		glRotatef(angle, 0, 0, 1); //垂直屏幕旋转
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
		glMatrixMode(GL_MODELVIEW); //沿着红点方向前进
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
	glutInit(&argc, argv);   //初始化GLUT
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(500, 200);
	glutInitWindowSize(400, 400);
	glutCreateWindow("OpenGL");
	glutMouseFunc(&OnMouse);  //注册鼠标事件
	glutKeyboardFunc(&KeyBoards);  //注册键盘事件
	glutDisplayFunc(&myDisplay);   //回调函数
	glutMainLoop();    //持续显示，当窗口改变会重新绘制图形
	return 0;
}
