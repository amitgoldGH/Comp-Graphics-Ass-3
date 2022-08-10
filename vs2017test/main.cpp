#include "glut.h"
#include <math.h>
//#include <iostream>
//#include <stdlib.h>

#define SPACEBAR 32
#define R_KEY 114

//const int W = 600;
//const int H = 600;

const int W = 1280;
const int H = 768;

const double PI = 3.14;
const double gravity = -9.8;
const double sine_multiplier = 0.1;

// TANK
double tank_scale_factor = 0.3;
double angle = 0;
double beta = 0;
double delta = 1;
int direction = 1;
bool moving = false;
double dx = 0, dy = 0;
// SHELL

double shell_scale_factor = 0.2;
double t = 0;
double time_delta = 0.0005;
double shell_angle = 0;
double shooting_angle = 0;
int shooting_direction = -1;
double shell_dx = 0, shell_dy = 0;
double initial_force = 4;

double x_init = 0, y_init = 0;
bool fired = false;
bool exploded = false;


const double bearing_height = 0.12;
const double wheel_distance = 0.05;
const double base_bottom = -0.05;
const double base_top = 0.15;
const double layer_height_delta = 0.05;
const double cannon_length = 0.3, cannon_thickness = 0.05, cannon_gap = 0.07;
//const double cannon_elevation_angle = 36.87;
//const double cannon_elevation_angle = 18.43;
const double cannon_elevation_angle = 13.134;


void init()
{
	glClearColor(0, 0, 0.3, 0);// color of window background
	glOrtho(-1, 1, -1, 1, -1, 1); // defines the main axis and their range
}

void DrawWheel()
{
	double alpha, teta = PI / 6;
	double x, y;

	/*
	for (alpha = 0; alpha <= 2 * PI; alpha += 2*teta)
	{
		glColor3d(1, 1, 1);
		glBegin(GL_POLYGON);
		x = cos(alpha);
		y = sin(alpha);
		glVertex2d(x, y);
		glVertex2d(0, 0);
		x = cos(alpha+teta);
		y = sin(alpha+teta);
		glVertex2d(x, y);
		glEnd();
	}
	*/
	teta = PI / 36;
	glLineWidth(2);
	glColor3d(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	for (alpha = 0; alpha <= 2 * PI; alpha += teta)
	{
		x = cos(alpha);
		y = sin(alpha);
		glVertex2d(x, y);
	}
	glEnd();
	glLineWidth(1);
	teta = PI / 6;
	glBegin(GL_LINES);
	for (alpha = 0; alpha <= 2 * PI; alpha += teta)
	{
		x = cos(alpha);
		y = sin(alpha);
		glVertex2d(x, y);
		glVertex2d(0, 0);
	}
	glEnd();

}

void DrawWindMill()
{
	// base
	glColor3d(1, 1, 1);

	glBegin(GL_LINE_LOOP);
	glVertex2d(-0.5, 0.5);
	glVertex2d(-0.5, -0.5);
	glVertex2d(0.5, -0.5);
	glVertex2d(0.5, 0.5);
	glVertex2d(0, 0.75);
	glEnd();

	// rotating wings
	glPushMatrix();
	glTranslated(0, 0.75, 0);// 3.
	glRotated(angle, 0, 0, 1); // 2.
	glTranslated(0, -0.75, 0);// 1.
	// top wing
	glColor3d(0.5, 1, 1);
	glBegin(GL_POLYGON);
		glVertex2d(0, 0.75);
		glVertex2d(-0.25, 1.5);
		glVertex2d(0.25, 1.5);
	glEnd();
	// left wing
	glPushMatrix();
	glTranslated(0, 0.75, 0);// 3.
	glRotated(90, 0, 0, 1); // 2.
	glTranslated(0, -0.75, 0);// 1.
	glBegin(GL_POLYGON);
	glVertex2d(0, 0.75);
	glVertex2d(-0.25, 1.5);
	glVertex2d(0.25, 1.5);
	glEnd();
	glPopMatrix();
	// right wing
	glPushMatrix();
	glTranslated(0, 0.75, 0);// 3.
	glRotated(-90, 0, 0, 1); // 2.
	glTranslated(0, -0.75, 0);// 1.
	glBegin(GL_POLYGON);
	glVertex2d(0, 0.75);
	glVertex2d(-0.25, 1.5);
	glVertex2d(0.25, 1.5);
	glEnd();
	glPopMatrix();
	// right wing
	glPushMatrix();
	glTranslated(0, 0.75, 0);// 3.
	glRotated(180, 0, 0, 1); // 2.
	glTranslated(0, -0.75, 0);// 1.
	glBegin(GL_POLYGON);
	glVertex2d(0, 0.75);
	glVertex2d(-0.25, 1.5);
	glVertex2d(0.25, 1.5);
	glEnd();
	glPopMatrix();


	glPopMatrix();

}

void DrawTank()
{

	/*const double bearing_height = 0.12;
	const double wheel_distance = 0.05;
	const double base_bottom = -0.05;
	const double base_top = 0.15;
	const double layer_height_delta = 0.05;
	const double cannon_length = 0.3, cannon_thickness = 0.03, cannon_gap = 0.07;*/
	//const double angle = 55;
	//const double cannon_y = sin(angle) * cannon_length;
	//const double cannon_x = cos(angle) * cannon_length;

	// front wheel
	glPushMatrix();
	glTranslated(-3 * wheel_distance, 0, 0);
	glScaled(0.05, 0.05, 1);
	glRotated(angle, 0, 0, 1);
	DrawWheel();
	glPopMatrix();
	// mid-front wheel
	glPushMatrix();
	glTranslated(-wheel_distance, 0, 0);
	glScaled(0.05, 0.05, 1);
	glRotated(angle, 0, 0, 1);
	DrawWheel();
	glPopMatrix();
	// mid-back wheel
	glPushMatrix();
	glTranslated(wheel_distance, 0, 0);
	glScaled(0.05, 0.05, 1);
	glRotated(angle, 0, 0, 1);
	DrawWheel();
	glPopMatrix();
	// rear wheel
	glPushMatrix();
	glTranslated(3* wheel_distance, 0, 0);
	glScaled(0.05, 0.05, 1);
	glRotated(angle, 0, 0, 1);
	DrawWheel();
	glPopMatrix();

	// front bearing
	glPushMatrix();
	glTranslated(-0.24, bearing_height, 0);
	glScaled(0.03, 0.03, 1);
	glRotated(angle, 0, 0, 1);
	DrawWheel();
	glPopMatrix();

	// mid-front bearing
	glPushMatrix();
	glTranslated(-0.1, bearing_height, 0);
	glScaled(0.02, 0.02, 1);
	glRotated(angle, 0, 0, 1);
	DrawWheel();
	glPopMatrix();

	// Mid bearing
	glPushMatrix();
	glTranslated(0, bearing_height, 0);
	glScaled(0.02, 0.02, 1);
	glRotated(angle, 0, 0, 1);
	DrawWheel();
	glPopMatrix();

	// mid-back bearing
	glPushMatrix();
	glTranslated(0.1, bearing_height, 0);
	glScaled(0.02, 0.02, 1);
	glRotated(angle, 0, 0, 1);
	DrawWheel();
	glPopMatrix();

	// back bearing
	glPushMatrix();
	glTranslated(0.24, bearing_height, 0);
	glScaled(0.03, 0.03, 1);
	glRotated(angle, 0, 0, 1);
	DrawWheel();
	glPopMatrix();

	//Bottom layer armor
	//base
	glBegin(GL_LINES);
	glVertex2d(-0.18, base_bottom);
	glVertex2d(0.18, base_bottom);

	// Right diagonal
	glVertex2d(0.205, 0);
	glVertex2d(0.3, base_top);

	//top
	glVertex2d(-0.3, base_top);
	glVertex2d(0.3, base_top);

	//left diagonal
	glVertex2d(-0.205, 0);
	glVertex2d(-0.3, base_top);

	glEnd();

	// First layer
	glBegin(GL_LINES);
	glVertex2d(-0.2, base_top);
	glVertex2d(-0.15, base_top + layer_height_delta);

	glVertex2d(0.2, base_top);
	glVertex2d(0.15, base_top + layer_height_delta);

	glVertex2d(-0.15, base_top + layer_height_delta);
	glVertex2d(0.15, base_top + layer_height_delta);

	glEnd();

	// Second layer
	glBegin(GL_LINES);
	glVertex2d(-0.1, base_top + layer_height_delta);
	glVertex2d(-0.05, base_top + 2 * layer_height_delta);

	glVertex2d(0.1, base_top + layer_height_delta);
	glVertex2d(0.05, base_top + 2 * layer_height_delta);

	glVertex2d(-0.05, base_top + 2 * layer_height_delta);
	glVertex2d(0.05, base_top + 2 * layer_height_delta);

	glEnd();

	// Cannon
	glBegin(GL_LINES);
	glVertex2d(-0.1, base_top + layer_height_delta);
	glVertex2d(-0.1 - cannon_length, base_top + layer_height_delta + cannon_gap);

	glVertex2d(-0.07, base_top + layer_height_delta + cannon_thickness);
	glVertex2d(-0.1 - cannon_length, base_top + layer_height_delta + cannon_thickness + cannon_gap);

	//Cannon tip
	glVertex2d(-0.1 - cannon_length, base_top + layer_height_delta + cannon_gap);
	glVertex2d(-0.1 - cannon_length, base_top + layer_height_delta + cannon_thickness + cannon_gap);

	glEnd();
	
}

void DrawExplosionShape() {
	glBegin(GL_POLYGON);
	// Top
	glVertex2d(-0.5, 0.5);
	glVertex2d(-0.4, 0.7);
	glVertex2d(-0.3, 0.4);
	glVertex2d(-0.2, 0.5);
	glVertex2d(-0.1, 0.4);
	glVertex2d(0.0, 0.6);
	glVertex2d(0.1, 0.5);
	glVertex2d(0.2, 0.6);
	glVertex2d(0.3, 0.4);
	glVertex2d(0.4, 0.6);
	glVertex2d(0.5, 0.5);
	//Right side
	glVertex2d(0.4, 0.4);
	glVertex2d(0.6, 0.3);
	glVertex2d(0.3, 0.1);
	glVertex2d(0.5, 0.0);
	glVertex2d(0.3, -0.1);
	glVertex2d(0.2, -0.2);
	glVertex2d(0.4, -0.4);
	glVertex2d(0.3, -0.5);
	// Bottom
	glVertex2d(0.3, -0.6);
	glVertex2d(0.2, -0.5);
	glVertex2d(0.1, -0.6);
	glVertex2d(-0.1, -0.4);
	glVertex2d(-0.2, -0.7);
	glVertex2d(-0.5, -0.5);
	// Left side
	glVertex2d(-0.6, -0.4);
	glVertex2d(-0.5, -0.3);
	glVertex2d(-0.7, -0.2);
	glVertex2d(-0.5, -0.1);
	glVertex2d(-0.4, 0.0);
	glVertex2d(-0.6, 0.1);
	glVertex2d(-0.5, 0.2);
	glVertex2d(-0.4, 0.4);
	glVertex2d(-0.5, 0.5);
	glEnd();

}
void DrawExplosion()
{

	glPushMatrix();
	glTranslated(0, 0, 0);

	glColor3d(255, 0 , 0); // red outer
	DrawExplosionShape();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.05, 0, 0);
	glScaled(0.6, 0.6, 1);
	glColor3d(255, 255, 0); // yellow inner
	DrawExplosionShape();
	glPopMatrix();


}

void DrawShell() 
{
	glBegin(GL_LINE_LOOP);
	glVertex2d(0, 0);
	glVertex2d(-0.15, 0);
	glVertex2d(-0.2, 0.025);
	glVertex2d(-0.15, 0.05);
	glVertex2d(0, 0.05);

	glEnd();
}

void DrawGround()
{

	//glBegin(GL_LINE_STRIP);
	//glVertex2d(-1, -0.1);
	//glVertex2d(1, -0.1);
	//glEnd();

	// draw sin
	double x, y;
	glColor3d(255, 255, 255);
	glBegin(GL_LINE_STRIP);
	
	for (x = -1; x <= 1; x += 0.01)
	{
		y = sine_multiplier*sin(x*10);
		glVertex2d(x, y);
	}
	glEnd();
}

void display()
{
	double vx, vy, vxt, vyt;

	//TEMP
	double vxcos, vysin;

	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	DrawGround();


	// TANK
	glPushMatrix();

	// rotation of 2*PI equals 2*PI*Radius of Wheel
	// in our case the wheel is rotated each time by angle (in degrees)

	dx = direction * 2 * PI * 0.03 * (angle / 360);


	dy = sine_multiplier *sin(dx * 10);
	beta = atan(sine_multiplier * 10 * cos(dx * 10)); // derrivative 

	beta *= 180 / PI; // transforms beta to degrees

	
	glTranslated(dx, dy+ 0.02, 0);
	if (direction == 1)
	{
		glRotated(180, 0, 1, 0);
		beta = -beta;
	}
	glScaled(tank_scale_factor, tank_scale_factor, 1);
	
	glRotated(beta, 0, 0, 1);
	DrawTank();

	glPopMatrix();


	// SHELL
	if (fired == true && exploded == false)
	{
		glPushMatrix();
		// initial speed 
		
		vxcos = cos(shooting_angle  * PI / 180);
		vysin = sin(shooting_angle * PI / 180);

		vx = shooting_direction * initial_force * vxcos;
		vy = initial_force * vysin;

		// speed as affected by time & acceleration
		vxt = vx * t;
		vyt = (vy * t) + (gravity * t * t);

		// location at this moment
		shell_dx = x_init + vxt;
		shell_dy = y_init + vyt;

		// current shell angle
		shell_angle = atan(vyt / vxt);
		shell_angle *= 180 / PI;

		if (shell_dy <= (sine_multiplier * sin(shell_dx * 10)))
		{
			exploded = true;
			fired = false;
		}
		
		

		glTranslated(shell_dx, shell_dy , 0);

		glScaled(shell_scale_factor, shell_scale_factor, 1);

		if (shooting_direction == 1)
		{
			glRotated(180, 0, 1, 0);
			shell_angle = -shell_angle;
		}

		glRotated(shell_angle, 0, 0, 1);

		DrawShell();

		glPopMatrix();
	}

	
	if (exploded)
	{
		glPushMatrix();

		glTranslated(shell_dx, shell_dy, 0);
		glScaled(shell_scale_factor, shell_scale_factor, 1);

		DrawExplosion();

		glPopMatrix();
	}

	glutSwapBuffers(); // show all
}

void idle()
{
	if (fired && exploded == false)
	{
		t += time_delta;
	}
	else {

	}

	if (moving)
	{
		angle += delta;
	}
	glutPostRedisplay(); // sends a message to the window to run display function
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
//		delta = -delta;
		angle = -angle;
		direction = -direction;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		moving = !moving;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == SPACEBAR)
	{
		fired = !fired;
		if (fired == true)
		{
			t = 0;
			shooting_direction = direction;
			shooting_angle = (beta * -1) + cannon_elevation_angle;
			x_init = dx;
			y_init = dy + 0.095;
			exploded = false;

		}  
		

	}
	else if (key == R_KEY)
	{

		fired = false;
		moving = false;
		exploded = false;

		angle = 0;
		direction = 1;
		t = 0;
		shell_angle = 0;
		x_init = 0;
		y_init = 0;
		system("CLS");


	}
}


void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(500, 100);
	glutCreateWindow("Assignment 3 Amit Gold");

	glutDisplayFunc(display); // display is a name of refresh function
	glutIdleFunc(idle);// idle is a function that runs in background

	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	init();

	glutMainLoop();
}