#include <windows.h>   // Standard Header For Most Programs
#include <gl/glut.h>   // The GL Utility Toolkit (Glut) Header
#include "mathlib.h"

GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat blue[] = { 0.2, 0.2, 1.0, 1.0 };
GLfloat cyan[] = { 0.0, 1.0, 1.0, 1.0 };
GLfloat red[] = { 0.9, 0.2, 0.2, 1.0 };
GLfloat green[] = { 0.2, 1.0, 0.2, 1.0 };
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat grey[] = { 0.4, 0.4, 0.4, 1.0 };
GLfloat lpos1[] = { 1.0, 1.0, 1.0, 0.0 };
GLfloat lpos2[] = { -1.0, 1.0, 1.0, 0.0 };

Vector3 pos(1.0, 1.0, 0.0);
Vector3 vx(-0.79984003, 0.59988004, -0.019996010);
Vector3 vy(0.18814418, 0.28221628, 0.94072086);
Vector3 vz(0.56996286, 0.74866408, -0.33859181);

void init ( GLvoid )     // Create Some Everyday Functions
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glEnable ( GL_COLOR_MATERIAL );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

//	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cyan);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 120);
	glLightfv(GL_LIGHT0, GL_AMBIENT, black);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightf(GL_LIGHT0, GL_SHININESS, 120);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos1);

	glLightfv(GL_LIGHT1, GL_AMBIENT, black);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white);
	glLightf(GL_LIGHT1, GL_SHININESS, 120);
	glLightfv(GL_LIGHT1, GL_POSITION, lpos2);

	glEnable(GL_LIGHTING);                // so the renderer considers light
	glEnable(GL_LIGHT0);                  // turn LIGHT0 on
	glEnable(GL_LIGHT1);                  // turn LIGHT1 on

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void drawPlane(void)
{
	GLUquadric *gluQ = gluNewQuadric();

	glPushMatrix();
	gluCylinder(gluQ, 0.1, 0.1, 1.0, 20, 20);
	glTranslatef(0.0, 0.0, 1.0);
	gluCylinder(gluQ, 0.1, 0.0, 0.3, 20, 20);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.6);
	glScalef(2.0, 0.1, 0.2);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.1);
	glScalef(1.0, 0.1, 0.1);
	glutSolidCube(1.0);
	glPopMatrix();

	gluDeleteQuadric(gluQ);
}

void drawVector(Vector3 p, Vector3 d)
{
	GLUquadric *gluQ = gluNewQuadric();
	Vector3 v = ML_AZ.cross(d);
	glPushMatrix();
	glTranslatef(p.x, p.y, p.z);
	glRotatef(Acos(d.dot(ML_AZ)), v.x, v.y, v.z);
	gluCylinder(gluQ, 0.04, 0.04, 1.0, 20, 20);
	glTranslatef(0.0, 0.0, 1.0);
	gluCylinder(gluQ, 0.08, 0.0, 0.08, 20, 20);
	glPopMatrix();
	gluDeleteQuadric(gluQ);
}

void display ( void )   // Create The Display Function
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Translate and rotate the scene so we can better observe it
	glTranslatef(0.0, 0.0, -5.0);
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glRotatef(-40.0, 0.0, 1.0, 0.0);
	
	// Draw the principle frame
	glColor3fv(red);
	drawVector(ML_O, ML_AX);
	glColor3fv(green);
	drawVector(ML_O, ML_AY);
	glColor3fv(blue);
	drawVector(ML_O, ML_AZ);

	// Draw the target frame
	glColor3fv(red);
	drawVector(pos, vx);
	glColor3fv(green);
	drawVector(pos, vy);
	glColor3fv(blue);
	drawVector(pos, vz);
	
	glColor3fv(grey);

	drawPlane();

	Matrix4 Tran;
	Tran.m[3][0] = pos.x;
	Tran.m[3][1] = pos.y;
	Tran.m[3][2] = pos.z;
	Tran.m[3][3] = 1.0f;
	Tran.m[0][0] = vx.x;
	Tran.m[0][1] = vx.y;
	Tran.m[0][2] = vx.z;
	Tran.m[1][0] = vy.x;
	Tran.m[1][1] = vy.y;
	Tran.m[1][2] = vy.z;
	Tran.m[2][0] = vz.x;
	Tran.m[2][1] = vz.y;
	Tran.m[2][2] = vz.z;
	glPushMatrix();
	glMultMatrixf(&Tran.M[0]);
	drawPlane();
	glPopMatrix();

	glutSwapBuffers();								// Swap The Buffers To Not Be Left With A Clear Screen
}

void reshape ( int w, int h )   // Create The Reshape Function (the viewport)
{
	glViewport     ( 0, 0, w, h );
	glMatrixMode   ( GL_PROJECTION );  // Select The Projection Matrix
	glLoadIdentity ( );                // Reset The Projection Matrix
	if ( h==0 )  // Calculate The Aspect Ratio Of The Window
		gluPerspective ( 80, ( float ) w, 1.0, 5000.0 );
	else
		gluPerspective ( 80, ( float ) w / ( float ) h, 1.0, 5000.0 );
	glMatrixMode   ( GL_MODELVIEW );  // Select The Model View Matrix
	glLoadIdentity ( );    // Reset The Model View Matrix
}

void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{
	switch ( key ) {
    case 27:        // When Escape Is Pressed...
		exit ( 0 );   // Exit The Program
		break;        // Ready For Next Case
	default:        // Now Wrap It Up
		break;
	}
}

void arrow_keys ( int a_keys, int x, int y )  // Create Special Function (required for arrow keys)
{
	switch ( a_keys ) {
    case GLUT_KEY_UP:     // When Up Arrow Is Pressed...
		glutFullScreen ( ); // Go Into Full Screen Mode
		break;
	case GLUT_KEY_DOWN:               // When Down Arrow Is Pressed...
		glutReshapeWindow ( 500, 500 ); // Go Into A 500 By 500 Window
		break;
	default:
		break;
	}
}

int main ( int argc, char** argv )   // Create Main Function For Bringing It All Together
{
	glutInit            ( &argc, argv ); // Erm Just Write It =)
	glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE ); // Display Mode
	glutInitWindowSize  ( 800, 800 ); // If glutFullScreen wasn't called this is the window size
	glutCreateWindow    ( "Lecture demo" ); // Window Title (argv[0] for current directory as title)
//	glutFullScreen      ( );          // Put Into Full Screen
	glutDisplayFunc     ( display );  // Matching Earlier Functions To Their Counterparts
	glutReshapeFunc     ( reshape );
	glutKeyboardFunc    ( keyboard );
	glutSpecialFunc     ( arrow_keys );
	init ();
	glutMainLoop        ( );          // Initialize The Main Loop
}

