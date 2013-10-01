/*
 *  Coordinates
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//  Globals
int th=0;       // Azimuth of view angle
int ph=0;       // Elevation of view angle
// ^ For rotation 
int initials=1; //Used to swap between inital value set 1-4(or user input (5))
double w=1;     // W variable
double dim=160;   // Dimension of orthogonal box
char* text[] = {"","r=28,s=10,b=2.666","r=13,s=10,b=2.66",
						"r=14,s=30,b=4.666","r=42,s=15,b=4.666",
							"User Input"}; //Initials
//Variables for user input
char usr_in[10000];
double usr_r = 0;
double usr_s = 0;
double usr_b = 0; 

char trans_mode = 'x'; //Determines axis of translation.
double x_trans = 0;
double y_trans = 0;
double z_trans = 0;
//Global so that they can be modified with + and - commands 
/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  // Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

void glLorenz(int r, int s, int b, int w){
   
   int i;
   /*  Coordinates  */
   double x = 1;
   double y = 1;
   double z = 1;
   /*  Time step  */
   double dt = 0.001;

   glVertex4d(x+x_trans,y+y_trans,z+z_trans,w);
   /*
    *  Integrate 50,000 steps (50 time units with dt = 0.001)
    *  Explicit Euler integration
    */
   for (i=0;i<50000;i++)
   {
      glColor3f(0,(1-i/50000.0),i/50000.0);
      double dx = s*(y-x);
      double dy = x*(r-z)-y;
      double dz = x*y - b*z;
      x += dt*dx;
      y += dt*dy;
      z += dt*dz;
      glVertex4d(x+x_trans,y+y_trans,z+z_trans,w);
   }
}
/*
 *  Display the scene
 */
void display()
{
   
   //  Clear the image
   glClear(GL_COLOR_BUFFER_BIT);
   //  Reset previous transforms
   glLoadIdentity();
   //  Set view angle
   glRotated(ph,-1,0,0);
   glRotated(th,0,1,0);
   //  Color the attractor
   glColor3f(0,1,0.1);
   glPointSize(10);

   //  Draw the Lorenz Attractor
   glBegin(GL_LINE_STRIP);
   switch(initials)
   {
		case 1:
			glLorenz(28,10,2.666,w);
		break;
		
		case 2:
			glLorenz(13,10,2.666,w);
			break;
		
		case 3:
			glLorenz(14,30,4.666,w);
			break;
		
		case 4:
			glLorenz(42,15,4.666, w);
			break;
		
		case 5:
			if(usr_r != 0 && usr_s != 0 && usr_b != 0){
				glLorenz(usr_r, usr_s, usr_b, w);
			}else{
				printf("Please enter r(between 0-60 for optimal viewing): ");
				scanf("%s", usr_in);
				usr_r = atof(usr_in);
				printf("Please enter s: ");
				scanf("%s", usr_in);
				usr_s = atof(usr_in);
				printf("Please enter b(between 2-8 for optimal viewing): ");
				scanf("%s", usr_in);
				usr_b = atof(usr_in);
				glLorenz(usr_r, usr_s, usr_b, w);
			}
			break;
		
   }
   glEnd();
   //  Draw axes in white
   glColor3f(1,1,1);
   glBegin(GL_LINES);
   glVertex3d(100+x_trans,0+y_trans,0+z_trans);
   glVertex3d(-100+x_trans,0+y_trans,0+z_trans);
   glVertex3d(0+x_trans,100+y_trans,0+z_trans);
   glVertex3d(0+x_trans,-100+y_trans,0+z_trans);
   glVertex3d(0+x_trans,0+y_trans,100+z_trans);
   glVertex3d(0+x_trans,0+y_trans,-100+z_trans);
   glEnd();
   //  Label axes
   glRasterPos3d(100+x_trans,0+y_trans,0+z_trans);
   Print("X");
   glRasterPos3d(0+x_trans,100+y_trans,0+z_trans);
   Print("Y");
   glRasterPos3d(0+x_trans,0+y_trans,100+z_trans);
   Print("Z");
   //  Display parameters
   glWindowPos2i(5,5);
   Print("View Angle=%d,%d  %s",-th,-ph,text[initials]);
   Print("  w=%.1f",w);
   //  Flush and swap
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle and trace ball
   else if (ch == '0')
      th = ph = x_trans = y_trans = z_trans = 0;

   else if (ch == 'x' || ch == 'y' || ch == 'z' || ch == 'w')
   {
		if(ch == 'x'){
			trans_mode = ch;
		}else if(ch == 'y'){
			trans_mode = ch;
		}else if(ch == 'z'){
			trans_mode = ch;
		}else if(ch == 'w'){
			trans_mode = ch;
		}
   }
   //  Increase x, z, or y by 1
   else if (ch == '+')
   {
       switch(trans_mode){
			case 'x':
				x_trans +=1;
				break;
			case 'y':
				y_trans +=1;
				break;
			case 'z':
				z_trans +=1;
				break;
			case 'w':
				w += 0.1;
				break;
		}
   }
   //  Decrease x, z, or y by 1
   else if (ch == '-')
   {
        switch(trans_mode){
			case 'x':
				x_trans -=1;
				break;
			case 'y':
				y_trans -=1;
				break;
			case 'z':
				z_trans -=1;
				break;
			case 'w':
				w -= 0.1;
				break;
		}
   }
   //   Determine initial values
   else if ('1' <= ch && ch <= '5')
   {
		initials = ch-'0';
		if( ch == '5'){
			usr_s = 0;
			usr_b = 0;
			usr_r = 0;
		}
   } 
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease azimuth by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   
   //  Ratio of the width to the height of the window
   double w2h = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection box adjusted for the
   //  aspect ratio of the window
   glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
  //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window 
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   //  Request 500 x 500 pixel window
   glutInitWindowSize(500,500);
   //  Create the window
   glutCreateWindow("Lorenz Attractor");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   //  Return code
   return 0;
}
