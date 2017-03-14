#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <freeglut/glut.h>
#include "textfile.h"
#include "glm.h"

#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "freeglut.lib")

#ifndef GLUT_WHEEL_UP
# define GLUT_WHEEL_UP   0x0003
# define GLUT_WHEEL_DOWN 0x0004
#endif

#ifndef GLUT_KEY_ESC
# define GLUT_KEY_ESC 0x001B
#endif

#ifndef max
# define max(a,b) (((a)>(b))?(a):(b))
# define min(a,b) (((a)<(b))?(a):(b))
#endif

// Shader attributes
GLint iLocPosition;
GLint iLocColor;
GLint Channel;

char *filename = "ColorModels/blitzcrank_incognito.obj";

GLMmodel* OBJ;
GLfloat* vertices;
GLfloat* colors;

int Model = 0;
int PolygonMode = 0;
int channel = 0;

void traverseColorModel()
{
	int i,j,k;

	GLfloat maxVal[3];
	GLfloat minVal[3];

	// TODO:
	//// You should traverse the vertices and the colors of each triangle, and 
	//// then normalize the model to unit size. i.e. Each vertex should be 
	//// bounded in [-1, 1], which will fit the camera clipping window.

	// number of triangles
	
	OBJ->numtriangles;

	// number of vertices
	OBJ->numvertices;

	// The center position of the model 
	OBJ->position[0] = 0;
	OBJ->position[1] = 0;
	OBJ->position[2] = 0;

	vertices = new GLfloat[OBJ->numtriangles*9];
	colors   = new GLfloat[OBJ->numtriangles*9];

	// Initialized 
	minVal[0] = minVal[1] = minVal[2] = 100.0f;
	maxVal[0] = maxVal[1] = maxVal[2] = -100.0f;

	k = j = 0;

	for(i=0; i<(int)OBJ->numtriangles; i++)
	{
		// the index of each vertex
		int indv1 = OBJ->triangles[i].vindices[0];
		int indv2 = OBJ->triangles[i].vindices[1];
		int indv3 = OBJ->triangles[i].vindices[2];

		// the index of each color
		int indc1 = indv1;
		int indc2 = indv2;
		int indc3 = indv3;

		GLfloat vx, vy, vz;

		// First vertex of a triangle
		vx = OBJ->vertices[indv1*3+0];
		vy = OBJ->vertices[indv1*3+1];
		vz = OBJ->vertices[indv1*3+2];

		vertices[j++] = vx;				vertices[j++] = vy;				vertices[j++] = vz; 
		maxVal[0] = max(vx,maxVal[0]); 	maxVal[1] = max(vy,maxVal[1]); 	maxVal[2] = max(vz,maxVal[2]);
		minVal[0] = min(vx,minVal[0]); 	minVal[1] = min(vy,minVal[1]); 	minVal[2] = min(vz,minVal[2]);

		// Second vertex of a triangle
		vx = OBJ->vertices[indv2*3+0];
		vy = OBJ->vertices[indv2*3+1];
		vz = OBJ->vertices[indv2*3+2];

		vertices[j++] = vx;			   vertices[j++] = vy;			    vertices[j++] = vz; 
		maxVal[0] = max(vx,maxVal[0]); maxVal[1] = max(vy,maxVal[1]); 	maxVal[2] = max(vz,maxVal[2]);
		minVal[0] = min(vx,minVal[0]); minVal[1] = min(vy,minVal[1]); 	minVal[2] = min(vz,minVal[2]);

		// Last Vertex off a triangle
		vx = OBJ->vertices[indv3*3+0];
		vy = OBJ->vertices[indv3*3+1];
		vz = OBJ->vertices[indv3*3+2];

		vertices[j++] = vx;				vertices[j++] = vy;				vertices[j++] = vz; 
	    maxVal[0] = max(vx,maxVal[0]); 	maxVal[1] = max(vy,maxVal[1]); 	maxVal[2] = max(vz,maxVal[2]);
		minVal[0] = min(vx,minVal[0]); 	minVal[1] = min(vy,minVal[1]); 	minVal[2] = min(vz,minVal[2]);

		// colors
		GLfloat c1, c2, c3;
		c1 = OBJ->colors[indv1*3+0];	c2 = OBJ->colors[indv1*3+1];    c3 = OBJ->colors[indv1*3+2];
		colors[k++] = c1;				colors[k++] = c2;				colors[k++] = c3;

		c1 = OBJ->colors[indv2*3+0];	c2 = OBJ->colors[indv2*3+1];	c3 = OBJ->colors[indv2*3+2];
		colors[k++] = c1;				colors[k++] = c2;				colors[k++] = c3;

		c1 = OBJ->colors[indv3*3+0];	c2 = OBJ->colors[indv3*3+1];	c3 = OBJ->colors[indv3*3+2];
		colors[k++] = c1;				colors[k++] = c2;				colors[k++] = c3;
	}

	// Normalization
	GLfloat mean_x, mean_y, mean_z;
	GLfloat mid_x, mid_y, mid_z;

	//  Transform to the center.
	mean_x = (maxVal[0] + minVal[0]) / 2 ; 
	mean_y = (maxVal[1] + minVal[1]) / 2 ;
	mean_z = (maxVal[2] + minVal[2]) / 2 ;

	for (int k = 0; k < (OBJ->numtriangles) * 3; k++)
	{
		vertices[k*3 + 0]  -= mean_x;
		vertices[k*3 + 1]  -= mean_y;
		vertices[k*3 + 2]  -= mean_z;
	}

	// Scale to proper size
	mid_x = (maxVal[0] - minVal[0]) / 2;
	mid_y = (maxVal[1] - minVal[1]) / 2;
	mid_z = (maxVal[2] - minVal[2]) / 2;
	
	for(i=0; i<(int)OBJ->numtriangles*9; i++){

		vertices[i] /=  max(max(mid_x, mid_y),mid_z);
	}
	

}

void loadOBJModel()
{
	// read an obj model here
	if(OBJ != NULL){
		free(OBJ);
	}
	OBJ = glmReadOBJ(filename);
	printf("%s\n", filename);

	// traverse the color model
	traverseColorModel();
}

void onIdle()
{
	glutPostRedisplay();
}

void onDisplay(void)
{
	// clear canvas
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(iLocPosition);
	glEnableVertexAttribArray(iLocColor);

	// organize the arrays
	static GLfloat triangle_color[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	static GLfloat triangle_vertex[] = {
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f
	};

	// bind array pointers to shader
	glVertexAttribPointer(iLocPosition, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(   iLocColor, 3, GL_FLOAT, GL_FALSE, 0, colors);
	
	// draw the array we just bound
	glDrawArrays(GL_TRIANGLES, 0, OBJ->numtriangles*3);

	glutSwapBuffers();
}

void showShaderCompileStatus(GLuint shader, GLint *shaderCompiled)
{
	glGetShaderiv(shader, GL_COMPILE_STATUS, shaderCompiled);
	if(GL_FALSE == (*shaderCompiled))
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character.
		GLchar *errorLog = (GLchar*) malloc(sizeof(GLchar) * maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
		fprintf(stderr, "%s", errorLog);

		glDeleteShader(shader);
		free(errorLog);
	}
}

void setShaders()
{
	GLuint v, f, p;
	char *vs = NULL;
	char *fs = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	vs = textFileRead("shader.vert");
	fs = textFileRead("shader.frag");

	glShaderSource(v, 1, (const GLchar**)&vs, NULL);
	glShaderSource(f, 1, (const GLchar**)&fs, NULL);

	free(vs);
	free(fs);

	// compile vertex shader
	glCompileShader(v);
	GLint vShaderCompiled;
	showShaderCompileStatus(v, &vShaderCompiled);
	if(!vShaderCompiled) system("pause"), exit(123);

	// compile fragment shader
	glCompileShader(f);
	GLint fShaderCompiled;
	showShaderCompileStatus(f, &fShaderCompiled);
	if(!fShaderCompiled) system("pause"), exit(456);

	p = glCreateProgram();

	// bind shader
	glAttachShader(p, f);
	glAttachShader(p, v);

	// link program
	glLinkProgram(p);

	iLocPosition = glGetAttribLocation (p, "av4position");
	iLocColor    = glGetAttribLocation (p, "av3color");
	// create a uniform variable called "channel" to control the RGB channel
	Channel = glGetUniformLocation(p, "channel");

	glUseProgram(p);
}


void onMouse(int who, int state, int x, int y)
{
	printf("%18s(): (%d, %d) ", __FUNCTION__, x, y);

	switch(who)
	{
		case GLUT_LEFT_BUTTON:   printf("left button   "); break;
		case GLUT_MIDDLE_BUTTON: printf("middle button "); break;
		case GLUT_RIGHT_BUTTON:  printf("right button  "); break; 
		case GLUT_WHEEL_UP:      printf("wheel up      "); break;
		case GLUT_WHEEL_DOWN:    printf("wheel down    "); break;
		default:                 printf("0x%02X          ", who); break;
	}

	switch(state)
	{
		case GLUT_DOWN: printf("start "); break;
		case GLUT_UP:   printf("end   "); break;
	}

	printf("\n");
}

void onMouseMotion(int x, int y)
{
	printf("%18s(): (%d, %d) mouse move\n", __FUNCTION__, x, y);
}

void onKeyboard(unsigned char key, int x, int y) 
{
	printf("%18s(): (%d, %d) key: %c(0x%02X) ", __FUNCTION__, x, y, key, key);
	switch(key) 
	{
		case GLUT_KEY_ESC: 
		/* the Esc key */ 

			exit(0); 
			break;
	
		case 'Z':                
		case 'z':                
		/* change to previous model */ 
				
				Model--;	
				if(Model < 0) Model+=4;

				if(Model % 4 ==0)		 filename = "ColorModels/bunny5KC.obj";
				else if(Model %4 ==1)    filename = "ColorModels/dragon10KC.obj";
				else if(Model %4 ==2)    filename = "ColorModels/ziggs.obj";
				else if(Model %4 ==3)    filename = "ColorModels/blitzcrank_incognito.obj";
			break;
				
		case 'X':		
		case 'x':			
		/* change to next model */
				
				Model++;
				if(Model > 3) Model-=4;
				
				if(Model % 4 ==0)		 filename = "ColorModels/bunny5KC.obj";
				else if(Model %4 ==1)    filename = "ColorModels/dragon10KC.obj", printf("WTF\n");
				else if(Model %4 ==2)    filename = "ColorModels/ziggs.obj";
				else if(Model %4 ==3)    filename = "ColorModels/blitzcrank_incognito.obj";
			break;

		case 'H':				 
		case 'h':                
		/* open the help menu */
			printf("\n----------HELP MENU----------\n\n");
			printf("z/x: switch different models \n");
			printf("h: to show help menu\n");
			printf("w: switch solid/wireframe mode\n");
			printf("s: show author information\n");
			printf("c: switch between RGB colors and the normal version\n");
			printf("\n----------HELP MENU----------\n\n");
			break;

		case 'W':				 
		case 'w':                
		/* switch solid/wireframe mode */

			if(PolygonMode==1)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE), PolygonMode = 0;
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL), PolygonMode = 1;
			break;

		case 'C':				  
		case 'c':		 
		/* color filter function */

			channel++;
			if(channel > 3) channel-=4;

			// 1 for float, int, unsigned, bool, etc.
			// 2 for vec2, ivec2, etc
			// i for int, ivec2, ivec3, etc.
			// Others like ui, f , etc.
			glUniform1i( Channel , channel );

			break;
		
		case 'S':
	    case 's':
			printf("103062129 Wayne §õ°ê½n ---> CG_HW1\n");
			break;
	}

	// Have to reload the model to see the changes.
	loadOBJModel();
	
	printf("\n");
}

void onKeyboardSpecial(int key, int x, int y){
	printf("%18s(): (%d, %d) ", __FUNCTION__, x, y);
	switch(key)
	{
		case GLUT_KEY_LEFT:
			printf("key: LEFT ARROW");
			break;
			
		case GLUT_KEY_RIGHT:
			printf("key: RIGHT ARROW");
			break;

		default:
			printf("key: 0x%02X      ", key);
			break;
	}
	printf("\n");
}


void onWindowReshape(int width, int height)
{
	printf("%18s(): %dx%d\n", __FUNCTION__, width, height);
}

int main(int argc, char **argv) 
{
	// glut init
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// create window
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("10420 CS550000 CG HW1 TA");

	glewInit();
	if(glewIsSupported("GL_VERSION_2_0")){
		printf("Ready for OpenGL 2.0\n");
	}else{
		printf("OpenGL 2.0 not supported\n");
		system("pause");
		exit(1);
	}

	// load obj models through glm
	loadOBJModel();

	// register glut callback functions
	glutDisplayFunc (onDisplay);
	glutIdleFunc    (onIdle);
	glutKeyboardFunc(onKeyboard);
	glutSpecialFunc (onKeyboardSpecial);
	glutMouseFunc   (onMouse);
	glutMotionFunc  (onMouseMotion);
	glutReshapeFunc (onWindowReshape);

	// set up shaders here
	setShaders();
	
	glEnable(GL_DEPTH_TEST);

	// main loop
	glutMainLoop();

	// free
	glmDelete(OBJ);

	return 0;
}

