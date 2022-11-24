#include <windows.h>
#include <gl\glut.h>

float spin1 = 60;
float spin2 = 30;
float spin3 = 30;

void light (void)
	//
{
	GLfloat light_position[]={0.0, 0.0, 6.0, 0.0};
	GLfloat light_ambient[]={0.8, 0.8, 0.8, 1.0};
	GLfloat light_diffuse[]={0.8, 0.8, 0.8, 1.0};
	GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0};
	GLfloat light_shininess[]={30.0};

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_SHININESS, light_shininess);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

void sun_material (void)
{
	GLfloat mat_diffuse[]={1.0, 0.2, 0.2, 1.0};
	GLfloat mat_ambient[]={1.0, 0.2, 0.2, 1.0};
	GLfloat mat_specular[]={0.8, 0.8, 0.8, 1.0};
	GLfloat mat_emission[]={1.0, 0.3, 0.3, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	//	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, 20.0);
}

void earth_material (void)
{
	GLfloat mat_diffuse[]={0.2, 0.2, 0.5, 1.0};
	GLfloat mat_ambient[]={0.2, 0.2, 0.5, 1.0};
	GLfloat mat_specular[]={0.7, 0.7, 0.8, 1.0};
	GLfloat mat_emission[]={0.0, 0.0, 0.0, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
}

void moon_material (void)
{
	GLfloat mat_diffuse[]={0.8, 0.8, 0.6, 1.0};
	GLfloat mat_ambient[]={0.5, 0.5, 0.3, 1.0};
	GLfloat mat_specular[]={1.0, 1.0, 0.6, 1.0};
	GLfloat mat_emission[]={0.0, 0.0, 0.0, 1.0};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
}

void init (void)
{
	GLfloat color[]={1.0, 0.0, 0.0, 0.3};

	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glDepthFunc(GL_LESS);
	glFrontFace(GL_CW);
	glShadeModel(GL_SMOOTH);

	light();
}


void display (void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //刷新背景

	sun_material();
	glutSolidSphere(2.5, 50, 50);	//sun
	//	glutWireSphere(2.5, 30, 30);	//sun

	glPushMatrix();
	glRotatef((float)(spin1), 0.0, 1.0, 0.0);
	glTranslatef(7.0, 0.0, 0.0);
	//	glRotatef((float)(spin), 1.0, -1.0, 0.0);
	//	glTranslatef(7.0, 7.0, 0.0);
	glPushMatrix();
	glRotatef((float)(spin2), 1.0, 1.0, 0.0);
	earth_material();
	glutSolidSphere(0.7, 20, 20);	//earth
	//	glutWireSphere(0.7, 15, 15);	//earth
	glPopMatrix();

	glPushMatrix();
	glRotatef((float)(spin3), 1.0, 1.0, 0.0);
	glTranslatef(-1.0, 1.0, 0.0);
	//	glRotatef((float)(spin2), -1.0, -1.0, 0.0);
	moon_material();
	glutSolidSphere(0.2, 20, 20);	//moon
	//	glutWireSphere(0.2, 10, 10);	//moon
	glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();
	glFlush();
}

void reshape (int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -25.0);
}

void idle (void)
{
	static float time = (float)0;
	static long lasttime = timeGetTime();	//此代码只执行一次
	long currtime = timeGetTime();			//得到当前时间
	float dtime = (float)(currtime - lasttime)/1000.0f;		//从上次调用此函数到这次调用此函数经过了多少时间
	time += dtime;	//程序运行了多长时间
	if (time>=.01) {
		spin1+=1;
		spin2+=4;	
		spin3+=7;
		glutPostRedisplay();	//通知系统窗口需重画
		time=(float)0;
	}

	if(spin1>=360)
		spin1=spin1-360;
	if(spin2>=360)
		spin2=spin2-360;
	if(spin3>=360)
		spin3=spin3-360;

	lasttime = currtime;
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (700, 700);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("Solar system");
	init();
	glutDisplayFunc (display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}