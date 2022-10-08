/*---------------------------------------------------------------------------------
 * This file contains a program, drawing & animating a wind-mill.
 * This program is used to teach LCS and scene graph concepts.
 *
 * by S. K. Ueng, Nov.  2006
 */
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>
/*----Define normals of faces ----*/
float  normal[][4] = {{0.0, 0.0, -1.0}, {0.0, -1.0, 0.0}, {1.0, 0.0, 0.0},
		      {0.0, 0.0, 1.0}, {0.0, 1.0, 0.0}, {-1.0, 0.0, 0.0}};

/*----Define material properties for cube -----*/
float  mat_ambient[] = {0.1, 0.3, 0.1, 1.0};
float  mat_diffuse[] = {0.95,0.95,0.95,1.0};//{0.1, 0.9, 0.1, 1.0};
float  mat_specular[] = {0.4, 0.4, 0.4, 1.0};
float  mat_shininess = 64.0;

//floor
float  floor_ambient[]  = {0.3, 0.3, 0.3, 1.0};
float  floor_diffuse[]  = {0.703800, 0.270480, 0.082800, 1.000000};
float  floor_specular[] = {0.0, 0.0, 0.0, 1.0};
float  floor_shininess  = 10.600000;

int floorchange=0;

//plastic
float  plas_ambient[]  = {0.191250, 0.073500, 0.022500, 1.000000};
float  plas_diffuse[]  = {0.010000, 0.010000, 0.010000, 1.000000};
float  plas_specular[] = {0.400000, 0.400000, 0.400000, 1.000000};
float  plas_shininess  = 10;

//copper
float  copper_ambient[]  = {0.329412, 0.223529, 0.027451, 1.000000};
float  copper_diffuse[]  = {0.780392, 0.568627, 0.113725, 1.000000};
float  copper_specular[] = {0.992157, 0.941176, 0.807843, 1.000000};
float  copper_shininess  = 27.897400;

//beryl
float  beryl_ambient[]  = {0.100000, 0.187250, 0.174500, 0.800000};
float  beryl_diffuse[]  = {0.396000, 0.741510, 0.691020, 0.800000};
float  beryl_specular[] = {0.297254, 0.308290, 0.306678, 0.800000};
float  beryl_shininess  = 12.800000;

//gold
float  gold_ambient[]  = {0.247250, 0.199500, 0.074500, 1.000000};
float  gold_diffuse[]  = {0.751640, 0.606480, 0.226480, 1.000000};
float  gold_specular[] = {0.628281, 0.555802, 0.366065, 1.000000};
float  gold_shininess  = 51.200001;

//Chrome
float ch_ambient[] ={0.25f, 0.25f, 0.25f, 1.0f  };
float ch_diffuse[] ={0.4f, 0.4f, 0.4f, 1.0f };
float ch_specular[] ={0.774597f, 0.774597f, 0.774597f, 1.0f };
float ch_shininess = 76.8f;

///*----Define light source properties -------*/
float  lit_position[] = {0.0, 0.0, 0.0, 1.0};
float  lit_direction[] = {-1.0, -1.0, 1.0, 0.0};
float  lit_diffuse[] = {0.95,0.95,0.95,1.0};//{0.8, 0.4, 0.4, 1.0};
float  lit_specular[] = {1.0,1.0,1.0, 1.0};
float  lit_cutoff=30.0;
float  lit_exponent=1.0;
int open0=1;
float  ligshin = 1.0;


float  lit1_position[] = {10.0,14.0,0.0, 0.0};
float  lit1_diffuse[] = {0.95, 0.95, 0.95, 1.0};
int open1=1;

float  lit2_position[] = {5.0,5.0,0.0, 1.0};
float  lit2_diffuse[] = {0.95, 0.95, 0.95, 1.0};
int open2=1;
/*----Define light source properties -------*/
//float  lit_position[] = {10.0, 14.0, 0.0, 1.0};
//float  lit_direction[] = {-1.0, -1.0, 1.0, 0.0};
//float  lit_diffuse[] = {0.8, 0.4, 0.4, 1.0};
//float  lit_specular[] = {0.7, 0.7, 0.7, 1.0};
//float  lit_cutoff=60.0;
//float  lit_exponent=8.0;
//
//float  lit1_position[] = {-0.5, 1.0, -2.0, 1.0};
//float  lit1_diffuse[] = {0.7, 0.7, 0.0, 1.0};

float  global_ambient[]={0.2, 0.2, 0.2, 1.0};

/*----Some global variables for transformation------*/
float  lit_angle=0.0;
float  cube_angle=0.0;
float  head=0.0, roll=0.0, Epitch=0.0, Wpitch=0.0;
float  up = 0.0, right = 0.0, zoom= 0.0;

/*-----Define a unit box--------*/
/* Vertices of the box */
float  points[][3] = {{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, 
                      {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, 
                      {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
                      {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}};
/* face of box, each face composing of 4 vertices */
int    face[][4] = {{0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5}, 
                    {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3}};
/* indices of the box faces */
int    cube[6] = {0, 1, 2, 3, 4, 5};

/*-----Define GLU quadric objects, a sphere and a cylinder----*/
GLUquadricObj  *sphere=NULL, *cylind=NULL;

/*-----Define window size----*/
int width=600, height=600;

//Global variables recording rotation angles of H-arm and blades.

float  arm_ang=45.0, bld_ang=10.0;


/*-----Translation and rotations of eye coordinate system---*/
float   eyeDx=0.0, eyeDy=0.0, eyeDz=0.0;
float   eyeAngx=0.0, eyeAngy=0.0, eyeAngz=0.0;
double  Eye[3]={0.0, 0.0, 30.0}, Focus[3]={0.0, 0.0, 0.0}, Vup[3]={0.0, 1.0, 0.0};

float   u[3][3]={{1.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}};
float   eye[3];
float   cv, sv; /* cos(5.0) and sin(5.0) */

#define X 0.525731112119
#define Z 0.850650808352
#define  PI   3.141592653
#define Step  0.5

float vdata[12][3]={
  { -1*X,0.0,Z},{X,0.0,Z},{-1*X,0.0,-1*Z},{X,0.0,-1*Z},
  { 0.0,Z,X},{0.0,Z,-1*X},{0.0,-1*Z,X},{0.0,-1*Z,-1*X},
  {Z,X,0.0},{-1*Z,X,0.0},{Z,-1*X,0.0},{-1*Z,-1*X,0.0}
};
int tindiceds[20][3]={
  {1,4,0},{4,9,0},{4,5,9},{8,5,4},{1,8,4},
  {1,10,8},{10,3,8},{8,3,5},{3,2,5},{3,7,2},
  {3,10,7},{10,6,7},{6,11,7},{6,0,11},{6,1,0},
  {10,1,6},{11,0,9},{2,11,9},{5,2,9},{11,2,7}
};
float ba = 1.0;

//double viewx=0,viewy=-9,viewz=22;
//double nowx=-28,nowy=-5,nowz=53;
//double objectx=30,objecty=-31,objectz=-5;
double viewx=7,viewy=-16,viewz=12;
double nowx=-34,nowy=-5,nowz=43;
double objectx=30,objecty=1,objectz=11;
double nowrotat=0;
int cyclecc=0;

int froonnt =0;
int baacck =0;
int leeft =0;
int riight =0;

int sel = 0;
/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void  myinit()
{
  glClearColor(0,0,0,1);      /*set the background color BLACK */
                   /*Clear the Depth & Color Buffers */
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */
  glEnable(GL_NORMALIZE);   /*Enable mornalization  */

  glEnable(GL_LIGHTING);    /*Enable lighting effects */
  glEnable(GL_LIGHT0);      /*Turn on light0 */
  glEnable(GL_LIGHT1);      /*Turn on light1 */
  glEnable(GL_LIGHT2);
  /*-----Define light0 ---------*/
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lit_cutoff);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, lit_exponent);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0025/2);

  /*-----Define light1 ---------*/
  glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, lit_specular);
  
  /*-----Define light1 ---------*/
  glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
  glLightfv(GL_LIGHT2, GL_SPECULAR, lit_specular);
 
  /*-----Define some global lighting status -----*/
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); /* local viewer */
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient); /*global ambient*/

  /*-----Enable face culling -----*/
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  if(sphere==NULL){
    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricNormals(sphere, GLU_SMOOTH);
  }
  if(cylind==NULL){
    cylind = gluNewQuadric();
    gluQuadricDrawStyle(cylind, GLU_FILL);
    gluQuadricNormals(cylind, GLU_SMOOTH);
  }


    /*---- Compute cos(5.0) and sin(5.0) ----*/
    cv = cos(5.0*PI/180.0);
    sv = sin(5.0*PI/180.0);
    /*---- Copy eye position ---*/
    eye[0] = Eye[0];
    eye[1] = Eye[1];
    eye[2] = Eye[2];
}

float norm2(float v[])
{
  double  sum;
  float   temp;

  sum = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
  temp = (float) sqrt(sum);
  return temp;
}


void draw_hard(){
  int  i;
  for(i=0;i<20;i++){
    glColor3f(i*1.00/20, 1.0,1.0-i*1.00/20);
    glBegin(GL_POLYGON);  // Draw the face 
      glVertex3fv(vdata[tindiceds[i][0]]);
      glVertex3fv(vdata[tindiceds[i][1]]);
      glVertex3fv(vdata[tindiceds[i][2]]);
    glEnd();
  }
  //glScalef(1/1.5, 1/1.5, 1/1.5);
}
/*------------------------------------------------------
 * Procedure to make projection matrix
 */
void make_projection(int x)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(x==4){
    gluPerspective(90.0, (double) width/ (double) height, 1.5, 50.0);
  }else{
      if(width>height)
	glOrtho(-40.0, 40.0, -40.0*(float)height/(float)width, 
		40.0*(float)height/(float)width, 
		-0.0, 100.0);
      else
	glOrtho(-40.0*(float)width/(float)height, 
		40.0*(float)width/(float)height, -40.0, 40.0, 
		-0.0, 100.0);
  }
  glMatrixMode(GL_MODELVIEW);
}

/*-------------------------------------------------------
 * Make viewing matrix
 */
void make_view(int x)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  switch(x){
  case 4:       /* Perspective */

    /* In this sample program, eye position and Xe, Ye, Ze are computed
       by ourselves. Therefore, use them directly; no transform is
       applied upon eye coordinate system
       */
    gluLookAt(eye[0], eye[1], eye[2], 
	      eye[0]-u[2][0], eye[1]-u[2][1], eye[2]-u[2][2],
	      u[1][0], u[1][1], u[1][2]);
    break;

  case 1:       /* X direction parallel viewing */
    gluLookAt(30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    break;
  case 2:       /* Y direction parallel viewing */
    gluLookAt(0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
    break;
  case 3:
    gluLookAt(0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    break;
  }
}
void draw_view()
{
  int    i;

  glMatrixMode(GL_MODELVIEW);

  
  //----Draw Eye position-----
  glPushMatrix();
  glTranslatef(eye[0], eye[1], eye[2]);
  glColor3f(0.0, 1.0, 0.0);
  glutWireSphere(1.0, 10, 10);
  glPopMatrix();
  /*
  //----Draw eye coord. axes -----
  glColor3f(1.0, 1.0, 0.0);           // Draw Xe 
  glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0]+5.0*u[0][0], eye[1]+20.0*u[0][1], eye[2]+20.0*u[0][2]);
  glEnd();
  
  glColor3f(1.0, 0.0, 1.0);          // Draw Ye 
  glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0]+20.0*u[1][0], eye[1]+20.0*u[1][1], eye[2]+20.0*u[1][2]);
  glEnd();

  glColor3f(0.0, 1.0, 1.0);          // Draw Ze 
  glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0]+20.0*u[2][0], eye[1]+20.0*u[2][1], eye[2]+20.0*u[2][2]);
  glEnd();*/
  
  glColor3f(0.0, 1.0, 1.0);          // Draw Ze 
  glBegin(GL_LINES);
    glVertex3f(eye[0], eye[1], eye[2]);
    glVertex3f(eye[0]-5.0*u[2][0], eye[1]-5.0*u[2][1], eye[2]-5.0*u[2][2]);
  glEnd();

  //----Draw eye coord. axes -----
  glColor3f(1.0, 1.0, 0.0);           // Draw Xe 
  glBegin(GL_LINES);
    glVertex3f(eye[0]-5.0*u[0][0]-5.0*u[2][0], eye[1]-5.0*u[0][1]-5.0*u[2][1], eye[2]-5.0*u[0][2]-5.0*u[2][2]);
    glVertex3f(eye[0]+5.0*u[0][0]-5.0*u[2][0], eye[1]+5.0*u[0][1]-5.0*u[2][1], eye[2]+5.0*u[0][2]-5.0*u[2][2]);
  glEnd();

  glColor3f(1.0, 0.0, 1.0);          // Draw Ye 
  glBegin(GL_LINES);
    glVertex3f(eye[0]-5.0*u[1][0]-5.0*u[2][0], eye[1]-5.0*u[1][1]-5.0*u[2][1], eye[2]-5.0*u[1][2]-5.0*u[2][2]);
    glVertex3f(eye[0]+5.0*u[1][0]-5.0*u[2][0], eye[1]+5.0*u[1][1]-5.0*u[2][1], eye[2]+5.0*u[1][2]-5.0*u[2][2]);
  glEnd();

  double ochar[3];
  double lay1[3]={eye[0]-5.0*u[0][0]-5.0*u[2][0], eye[1]-5.0*u[0][1]-5.0*u[2][1], eye[2]-5.0*u[0][2]-5.0*u[2][2]};
  double lay2[3]={eye[0]+5.0*u[0][0]-5.0*u[2][0], eye[1]+5.0*u[0][1]-5.0*u[2][1], eye[2]+5.0*u[0][2]-5.0*u[2][2]};
  double lay3[3]={eye[0]-5.0*u[1][0]-5.0*u[2][0], eye[1]-5.0*u[1][1]-5.0*u[2][1], eye[2]-5.0*u[1][2]-5.0*u[2][2]};
  double lay4[3]={eye[0]+5.0*u[1][0]-5.0*u[2][0], eye[1]+5.0*u[1][1]-5.0*u[2][1], eye[2]+5.0*u[1][2]-5.0*u[2][2]};

  glColor3f(0.0, 1.0, 0.5);           
  glBegin(GL_LINES);
    glVertex3f(lay1[0]-5.0*u[1][0], lay1[1]-5.0*u[1][1], lay1[2]-5.0*u[1][2]);
    glVertex3f(lay2[0]-5.0*u[1][0], lay2[1]-5.0*u[1][1], lay2[2]-5.0*u[1][2]);
  glEnd();

  glBegin(GL_LINES);
    glVertex3f(lay1[0]+5.0*u[1][0], lay1[1]+5.0*u[1][1], lay1[2]+5.0*u[1][2]);
    glVertex3f(lay2[0]+5.0*u[1][0], lay2[1]+5.0*u[1][1], lay2[2]+5.0*u[1][2]);
  glEnd();

  glBegin(GL_LINES);
    glVertex3f(lay3[0]-5.0*u[0][0], lay3[1]-5.0*u[0][1], lay3[2]-5.0*u[0][2]);
    glVertex3f(lay4[0]-5.0*u[0][0], lay4[1]-5.0*u[0][1], lay4[2]-5.0*u[0][2]);
  glEnd();

  glBegin(GL_LINES);
    glVertex3f(lay3[0]+5.0*u[0][0], lay3[1]+5.0*u[0][1], lay3[2]+5.0*u[0][2]);
    glVertex3f(lay4[0]+5.0*u[0][0], lay4[1]+5.0*u[0][1], lay4[2]+5.0*u[0][2]);
  glEnd();

  double newlay1[3]={lay1[0]-5.0*u[1][0], lay1[1]-5.0*u[1][1], lay1[2]-5.0*u[1][2]};
  double newlay2[3]={lay2[0]-5.0*u[1][0], lay2[1]-5.0*u[1][1], lay2[2]-5.0*u[1][2]};
  double newlay3[3]={lay1[0]+5.0*u[1][0], lay1[1]+5.0*u[1][1], lay1[2]+5.0*u[1][2]};
  double newlay4[3]={lay2[0]+5.0*u[1][0], lay2[1]+5.0*u[1][1], lay2[2]+5.0*u[1][2]};
  double newlay5[3]={lay3[0]-5.0*u[0][0], lay3[1]-5.0*u[0][1], lay3[2]-5.0*u[0][2] };
  double newlay6[3]={lay4[0]-5.0*u[0][0], lay4[1]-5.0*u[0][1], lay4[2]-5.0*u[0][2] };
  double newlay7[3]={lay3[0]+5.0*u[0][0], lay3[1]+5.0*u[0][1], lay3[2]+5.0*u[0][2] };
  double newlay8[3]={lay4[0]+5.0*u[0][0], lay4[1]+5.0*u[0][1], lay4[2]+5.0*u[0][2] };
  glColor3f(1.0, 1.0, 1.0);  
  glBegin(GL_LINES);
    glVertex3f(newlay1[0]*2-eye[0],newlay1[1]*2-eye[1],newlay1[2]*2-eye[2]);
    glVertex3f(newlay2[0]*2-eye[0],newlay2[1]*2-eye[1],newlay2[2]*2-eye[2]);
  glEnd();
  glBegin(GL_LINES);
    glVertex3f(newlay3[0]*2-eye[0],newlay3[1]*2-eye[1],newlay3[2]*2-eye[2]);
    glVertex3f(newlay4[0]*2-eye[0],newlay4[1]*2-eye[1],newlay4[2]*2-eye[2]);
  glEnd();
  glBegin(GL_LINES);
    glVertex3f(newlay5[0]*2-eye[0],newlay5[1]*2-eye[1],newlay5[2]*2-eye[2]);
    glVertex3f(newlay6[0]*2-eye[0],newlay6[1]*2-eye[1],newlay6[2]*2-eye[2]);
  glEnd();
  glBegin(GL_LINES);
    glVertex3f(newlay7[0]*2-eye[0],newlay7[1]*2-eye[1],newlay7[2]*2-eye[2]);
    glVertex3f(newlay8[0]*2-eye[0],newlay8[1]*2-eye[1],newlay8[2]*2-eye[2]);
  glEnd();

  double oldlay1[3]={newlay1[0]*2-eye[0],newlay1[1]*2-eye[1],newlay1[2]*2-eye[2]};
  double oldlay2[3]={newlay2[0]*2-eye[0],newlay2[1]*2-eye[1],newlay2[2]*2-eye[2]};
  double oldlay3[3]={newlay3[0]*2-eye[0],newlay3[1]*2-eye[1],newlay3[2]*2-eye[2]};
  double oldlay4[3]={newlay4[0]*2-eye[0],newlay4[1]*2-eye[1],newlay4[2]*2-eye[2]};
  double oldlay5[3]={newlay5[0]*2-eye[0],newlay5[1]*2-eye[1],newlay5[2]*2-eye[2]};
  double oldlay6[3]={newlay6[0]*2-eye[0],newlay6[1]*2-eye[1],newlay6[2]*2-eye[2]};
  double oldlay7[3]={newlay7[0]*2-eye[0],newlay7[1]*2-eye[1],newlay7[2]*2-eye[2]};
  double oldlay8[3]={newlay8[0]*2-eye[0],newlay8[1]*2-eye[1],newlay8[2]*2-eye[2]};

  glColor3f(0.5, 0.5, 1.0);  
  glBegin(GL_LINES);
    glVertex3f(newlay1[0],newlay1[1],newlay1[2]);
    glVertex3f(oldlay1[0],oldlay1[1],oldlay1[2]);
  glEnd();
  glBegin(GL_LINES);
    glVertex3f(newlay2[0],newlay2[1],newlay2[2]);
    glVertex3f(oldlay2[0],oldlay2[1],oldlay2[2]);
  glEnd();
  glBegin(GL_LINES);
    glVertex3f(newlay3[0],newlay3[1],newlay3[2]);
    glVertex3f(oldlay3[0],oldlay3[1],oldlay3[2]);
  glEnd();
  glBegin(GL_LINES);
    glVertex3f(newlay4[0],newlay4[1],newlay4[2]);
    glVertex3f(oldlay4[0],oldlay4[1],oldlay4[2]);
  glEnd();
  
}


/*-------------------------------------------------------
 * Procedure to draw a polygon as a blade of the windmill
 */
void draw_blade()
{
  glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.5, 4.0, 0.0);
    glVertex3f(0.5, 8.0, 0.0);
    glVertex3f(-0.5, 8.0, 0.0);
    glVertex3f(-0.5, 4.0, 0.0);
    glVertex3f(-0.5, 4.0, 0.1);
    glVertex3f(-0.5, 8.0, 0.1);
    glVertex3f(0.5, 8.0, 0.1);
    glVertex3f(0.5, 4.0, 0.1);
    glVertex3f(0.0, 0.0, 0.1);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.5, 4.0, 0.0);
    glVertex3f(0.5, 8.0, 0.0);
    glVertex3f(-0.5, 8.0, 0.0);
    glVertex3f(-0.5, 4.0, 0.0);
  glEnd();
}

/*--------------------------------------------------------
 * Procedure to draw a cube. The geometrical data of the cube
 * are defined above.
 */
void draw_cube()
{
  int    i;

  //glColor3f(0.20, 0.75, 0.0);   Set the current color 
  for(i=0;i<6;i++){
    glBegin(GL_POLYGON);  // Draw the face 
      glVertex3fv(points[face[i][0]]);
      glVertex3fv(points[face[i][1]]);
      glVertex3fv(points[face[i][2]]);
      glVertex3fv(points[face[i][3]]);
    glEnd();
  }
}

/*-------------------------------------------------------
 * Procedure to draw three axes and the orign
 */
void draw_axes()
{

  /*----Draw a white sphere to represent the original-----*/
  glColor3f(0.9, 0.9, 0.9);

  gluSphere(sphere, 2.0,   /* radius=2.0 */
	    12,            /* composing of 12 slices*/
	    12);           /* composing of 8 stacks */

  /*----Draw three axes in colors, yellow, meginta, and cyan--*/
  /* Draw Z axis  */
  glColor3f(0.0, 0.95, 0.95);
  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
	      10.0,              /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);               /* Divide it into 3 sections */
  
  /* Draw Y axis */
  glPushMatrix();
  glRotatef(-90.0, 1.0, 0.0, 0.0);  /*Rotate about x by -90', z becomes y */
  glColor3f(0.95, 0.0, 0.95);
  gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
	      10.0,             /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);               /* Divide it into 3 sections */
  glPopMatrix();

  /* Draw X axis */
  glColor3f(0.95, 0.95, 0.0);
  glPushMatrix();
  glRotatef(90.0, 0.0, 1.0, 0.0);  /*Rotate about y  by 90', x becomes z */
  gluCylinder(cylind, 0.5, 0.5,   /* radius of top and bottom circle */
	      10.0,             /* height of the cylinder */
              12,               /* use 12-side polygon approximating circle*/
	      3);               /* Divide it into 3 sections */
  glPopMatrix();
  /*-- Restore the original modelview matrix --*/
  glPopMatrix();
}


void draw_scene()
{
  //easy
  glPushMatrix();
  glLightfv(GL_LIGHT2, GL_POSITION, lit2_position);  /*fixed position in eye space---*/
  glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glTranslatef(lit2_position[0], lit2_position[1], lit2_position[2]);
  glColor3f(lit2_diffuse[0],lit2_diffuse[1],lit2_diffuse[2]);
  glScalef(5.0, 5.0, 5.0);
  if(open2==1)glutSolidSphere(0.3, 12, 12);
  glEnable(GL_LIGHTING);
  glEnable(GL_CULL_FACE);
  glPopMatrix();

  int i,j; 

  glPushMatrix();
  glScalef(ba, ba, ba);
  glPushMatrix();
  glMaterialfv(GL_FRONT, GL_AMBIENT, beryl_ambient);
  glMaterialfv(GL_FRONT, GL_SPECULAR, (float[]){0.95,0.95,0.95,1.0});
  glMaterialfv(GL_FRONT, GL_DIFFUSE, beryl_diffuse);
  glMaterialf(GL_FRONT, GL_SHININESS, beryl_shininess);
  draw_axes();
  glPopMatrix();
  glScalef(ba, ba, ba);
  glNormal3f(0.0, 1.0, 0.0);  
  if(floorchange==0){
    glMaterialfv(GL_FRONT, GL_AMBIENT, floor_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, floor_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, floor_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, floor_shininess);
  }
  else{
    glMaterialfv(GL_FRONT, GL_AMBIENT, ch_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ch_specular);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ch_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, ch_shininess);
  }
  
  for(i=0;i<10;i++){
    for(j=0;j<10;j++){
      if((i+j)%2==0) glColor3f(0.3, 0.8, 0.8);
      else glColor3f(0.1, 0.1, 0.2);
      glBegin(GL_POLYGON);
        glVertex3f((i-5.0)*10.0, -2.0, (j-5.0)*10.0);
        glVertex3f((i-5.0)*10.0, -2.0, (j-4.0)*10.0);
        glVertex3f((i-4.0)*10.0, -2.0, (j-4.0)*10.0);
        glVertex3f((i-4.0)*10.0, -2.0, (j-5.0)*10.0);
      glEnd();
    }
  }

  
  glMaterialfv(GL_FRONT, GL_AMBIENT, plas_ambient);
  glMaterialfv(GL_FRONT, GL_SPECULAR, plas_specular);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, plas_diffuse);
  glMaterialf(GL_FRONT, GL_SHININESS, plas_shininess);
  glPushMatrix();
  //glScalef(0.5, 0.5, 0.5);//small
  //glColor3f(1.0, 1.0, 1.0);
  //glTranslatef(34, -5, 43);
  //glTranslatef(15.0, 5.0, 10.0);
  //glTranslatef(13.0, -2.0, 13.0);
  //glTranslatef(-33.0, 0, 0);
  glTranslatef(5, -1, 5);
  //glScalef(5, 5, 5);
  draw_cube();

  glScalef(1/5.0, 1/5.0, 1/5.0);
  glTranslatef(10.0, 0.0, 0.0);
  glScalef(5.0, 5.0, 5.0);
  glColor3f(1.0, 0.5, 0.5);
  draw_cube();

  glScalef(1/5.0, 1/5.0, 1/5.0);
  glTranslatef(10.0, 0.0, 0.0);
  glScalef(5.0, 5.0, 5.0);
  glColor3f(0.5, 0.5, 0.5);
  draw_cube();

  glScalef(1/5.0, 1/5.0, 1/5.0);
  glTranslatef(10.0, 0.0, 0.0);
  glScalef(5.0, 5.0, 5.0);
  glColor3f(0.5, 1.0, 0.5);
  draw_cube();

  glScalef(1/5.0, 1/5.0, 1/5.0);
  glTranslatef(10.0, 0.0, 0.0);
  glScalef(5.0, 5.0, 5.0);
  glColor3f(0.5, 0.5, 1.0);
  draw_cube();

  //12small big size
  glScalef(1/5.0, 1/5.0, 1/5.0);
  glTranslatef(0.0, 0.0, 10.0);
  glScalef(3.0, 3.0, 3.0);
  draw_hard();

  glScalef(1/3.0, 1/3.0, 1/3.0);
  //12big size
  glPushMatrix();
  glTranslatef(0.0, 1.0, 10.0);
  glScalef(5.0, 5.0, 5.0);
  draw_hard();
  glPopMatrix();
    //cyrcle1
  glTranslatef(-10.0, 0.0, 0.0);
  glScalef(3.0, 3.0, 3.0);
  glColor3f(0.95, 0.5, 0.2);
  gluSphere(sphere, 1.0,   // radius=2.0 
      12,            // composing of 12 slices
      12);           // composing of 12 stacks 
  glScalef(1/3.0, 1/3.0, 1/3.0);
  //cyrcle2
  glTranslatef(-8.0, 0.0, 0.0);
  glScalef(3.0, 3.0, 3.0);
  glColor3f(0.95, 0.8, 0.2);
  gluSphere(sphere, 1.0,   // radius=2.0 
      12,            // composing of 12 slices
      12);           // composing of 12 stacks
  glScalef(1/3.0, 1/3.0, 1/3.0);
    //cyrcle3
  glTranslatef(-8.0, 0.0, 0.0);
  glScalef(3.0, 3.0, 3.0);
  glColor3f(0.95, 0.5, 0.9);
  gluSphere(sphere, 1.0,   // radius=2.0 
      12,            // composing of 12 slices
      12);           // composing of 12 stacks
  glScalef(1/3.0, 1/3.0, 1/3.0);
    //cyrcle4
  glTranslatef(-8.0, 0.0, 0.0);
  glScalef(3.0, 3.0, 3.0);
  glColor3f(0.0, 0.5, 0.2);
  gluSphere(sphere, 1.0,   // radius=2.0 
      12,            // composing of 12 slices
      12);           // composing of 12 stacks
  glScalef(1/3.0, 1/3.0, 1/3.0);
  //cyrcle5
  glTranslatef(-8.0, 0.0, 0.0);
  glScalef(3.0, 3.0, 3.0);
  glColor3f(0.5, 0.1, 0.8);
  gluSphere(sphere, 1.0,   // radius=2.0 
      12,            // composing of 12 slices
      12);           // composing of 12 stacks
  glScalef(1/3.0, 1/3.0, 1/3.0);

  glPopMatrix();
  glPushMatrix();
  
  glMaterialfv(GL_FRONT, GL_AMBIENT, gold_ambient);
  glMaterialfv(GL_FRONT, GL_SPECULAR, gold_specular);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, gold_diffuse);
  glMaterialf(GL_FRONT, GL_SHININESS, gold_shininess);
  //------------------------------------
  glScalef(0.5, 0.5, 0.5);//small
  
  
  glTranslatef(nowx*-1, nowy, nowz);
  //glRotatef(nowrotat, 0.0, 1.0, 0.0);
  
  //glTranslatef(10.0, 3.0, 6.0);
  glTranslatef(15.0, 5.0, 10.0);
  

  if(cyclecc!=0){
    glTranslatef( 0.0,0.0, -10.0);
    glRotatef(-1*cyclecc, 0.0, 1.0, 0.0);
    glRotatef(10, -1.0, 0.0, 1.0);
    glTranslatef(0.0, 0.0, 10.0);
  }

  if(froonnt == 1){
    glRotatef(10, 0, 0.0, 1.0);
  }
  if(baacck == 1){
    glRotatef(10, 0, 0.0, -1.0);
  }
  if(leeft == 1){
    glRotatef(10, 1, 0.0, 0.0);
  }
  if(riight == 1){
    glRotatef(10, -1, 0.0, 0.0);
  }


  //----------------draw_hard
  //glPushMatrix();
  //glScalef(5, 5, 5);//small
  //glTranslatef( 0.0,5.0, 0.0);
  //draw_hard();
  //glPopMatrix();


  glRotatef(nowrotat, 0.0, 1.0, 0.0);

  glPushMatrix();
  glPushMatrix();
  glPushMatrix();              // Save M1 coord. sys 
  //glRotatef(50.0, 0.0, 1.0, 0.0);  // rotate about y-axis by 30' 
  glScalef(6.0, 6.0, 6.0);     // Scale up the axes by 6 times 
  glColor3f(0.20, 0.75, 0.0);
  draw_cube();                 // Draw a unit cube in the new coord. sys. 
  glPopMatrix();               // Get M1 baacck 



  glTranslatef(0.0, 3.0, 0.0); // Go up by 2 units, move to M2 coord. sys 
  //-------Draw the cylinder, arm of the windmill------
  glColor3f(0.68, 0.68, 0.68);    // Gray colored 
  
  glPushMatrix();              // Save M2 system 
  glScalef(0.5, 0.5, 0.5);
  glRotatef(-90.0, 1.0, 0.0, 0.0);  // Roate about x axis, z become y,and y become -z 
  //--- Draw a cylinder ---
  gluCylinder(cylind, 1.0, 1.0, // radius of top and bottom circle 
	      6.0,              // height of the cylinder 
          12,               // use 12-side polygon approximating circle
	      3);               // Divide it into 3 sections 
  glPopMatrix();         // Get M2 baacck 



  
  glTranslatef(0.0, 3.0, 0.0); // Go up 8 units, become M3 coord. sys
  //----- Draw a unit sphere ---
  //cyrcle
  
  glColor3f(0.95, 0.2, 0.2);

  gluSphere(sphere, 1.0,   // radius=2.0 
	    12,            // composing of 12 slices
	    12);           // composing of 12 stacks 
  glPushMatrix();
  glTranslatef(0.0, 1.8, 0.0);
  draw_hard();
  glPopMatrix();
  


  //---- Draw forearm of the windmill ---
  //cyrcle line
  //glRotatef(arm_ang, 1.0, 0.0, 0.0);
  glRotatef(-90, 1.0, 0.0, 0.0);

  //glColor3f(0.68, 0.68, 0.68);
  //gluCylinder(cylind, 1.0, 1.0, 4.0, 12, 3);

  
  
  //------Draw three blades ---
  glColor3f(1.0, 1.0, 1.0);
  glScalef(2.0, 2.0, 2.0);
  glRotatef(bld_ang, 0.0, 0.0, 1.0);  // Rotate about x axis, M5 coord. sys. 
  draw_blade();// draw the first blade 
 
  glRotatef(90.0, 0.0, 0.0, 1.0);    // rotate by 120 degree, M6 coord. sys 
  draw_blade();// draw 2nd blade 
  
  glRotatef(90.0, 0.0, 0.0, 1.0);    // rotate by 120 degree,M7 coord. sys 
  draw_blade();// draw 3rd blade 

  glRotatef(90.0, 0.0, 0.0, 1.0);    // rotate by 120 degree,M8 coord. sys 
  draw_blade();// draw 3rd blade 


  glPopMatrix();
  glPushMatrix();
  glTranslatef(-3.0, 0.0, 0.0);
  glColor3f(0.20, 0.75, 0.0);
  //froonnt
  glBegin(GL_POLYGON);
    glVertex3f(0.0, 3.0, 3.0);
    glVertex3f(0.0, 3.0, -3.0);
    glVertex3f(-5.0, 0.0, 0.0);
  glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 3.0, -3.0);
    glVertex3f(0.0, -3.0, -3.0);
    glVertex3f(-5.0, 0.0, 0.0);
  glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(0.0, -3.0, -3.0);
    glVertex3f(0.0, -3.0, 3.0);
    glVertex3f(-5.0, 0.0, 0.0);
  glEnd();
  glBegin(GL_POLYGON);
    glVertex3f(0.0, -3.0, 3.0);
    glVertex3f(0.0, 3.0, 3.0);
    glVertex3f(-5.0, 0.0, 0.0);
  glEnd();

  glPushMatrix();
  glTranslatef(-5.0, 0.0, 0.0);
  //glScalef(10, 10, 10);
  //draw_hard();
  if(open0==1){
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
  }
  glColor3f(lit_diffuse[0], lit_diffuse[1], lit_diffuse[2]);
  glutWireSphere(0.5, 8, 8);
  //glPopMatrix();
  glLightfv(GL_LIGHT0, GL_POSITION, lit_position);
  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, lit_exponent);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lit_cutoff);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lit_specular);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lit_diffuse);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lit_direction);
  if(open0==1){
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
  }

  glPopMatrix();
  //windows
  glColor3f(0.0, 0.0, 1.0);
  glBegin(GL_POLYGON);
    glVertex3f(0.0, 3.0, -3.0);
    glVertex3f(0.0, 0.0, -3.0);
    glVertex3f(-5.1, 0.0, 0.0);
  glEnd();
  glBegin(GL_POLYGON);
    glVertex3f(0.0, 3.0, 3.0);
    glVertex3f(0.0, 0.0, 3.0);
    glVertex3f(-5.1, 0.0, 0.0);
  glEnd();
  glBegin(GL_POLYGON);
    glVertex3f(0.0, 3.01, -3.0);
    glVertex3f(0.0, 3.01, 3.0);
    glVertex3f(-5.1, 0.0, 0.0);
  glEnd();
  //baacck
  glPopMatrix();
  glColor3f(0.20, 0.75, 0.0);
  glTranslatef(3.0, 0.0, 0.0);
  glPushMatrix();
  glBegin(GL_POLYGON);
    glVertex3f(0.0,3.0, 3.0);
    glVertex3f(0.0,3.0, -3.0);
    glVertex3f(15.0,0.0, 0.0);
  glEnd();
  glBegin(GL_POLYGON);
    glVertex3f(0.0,3.0, -3.0);
    glVertex3f(0.0,-3.0, -3.0);
    glVertex3f(15.0,0.0, 0.0);
  glEnd();
  glBegin(GL_POLYGON);
    glVertex3f(0.0,-3.0, -3.0);
    glVertex3f(0.0,-3.0, 3.0);
    glVertex3f(15.0,0.0, 0.0);
  glEnd();
  glBegin(GL_POLYGON);
    glVertex3f(0.0,-3.0, 3.0);
    glVertex3f(0.0,3.0, 3.0);
    glVertex3f(15.0,0.0, 0.0);
  glEnd();
  glPopMatrix();
  glTranslatef(15.0, 0.0, 0.0);
  glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(3.0, 0.0, 0.1);
    glVertex3f(3.0, 3.0, 0.0);
  glEnd();
  glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(3.0, 0.0, -0.1);
    glVertex3f(3.0, 3.0, 0.0);
  glEnd();
  glBegin(GL_POLYGON);
    glVertex3f(3.0, 0.0, -0.1);
    glVertex3f(3.0, 0.0, 0.1);
    glVertex3f(3.0, 3.0, 0.0);
  glEnd();

  glPushMatrix();              // Save M2 system 
  glScalef(0.3, 0.3, 1);
  glColor3f(0.95, 0.2, 0.2);
  glRotatef(-90.0, 0.0, 0.0, 1.0);  // Roate about x axis, z become y,and y become -z 
  //--- Draw a cylinder ---
  gluCylinder(cylind, 1.0, 1.0, // radius of top and bottom circle 
      1.0,              // height of the cylinder 
      12,               // use 12-side polygon approximating circle
      3);               // Divide it into 3 sections 
  glPopMatrix();         // Get M2 baacck 




  glTranslatef(0.0, 0.0, 1.0);
  glColor3f(1.0, 1.0, 1.0);
  glScalef(0.5, 0.5, 1.0);
  glRotatef(arm_ang, 0.0, 0.0, 1.0);  // Rotate about x axis, M5 coord. sys. 
  draw_blade();// draw the first blade 

  glRotatef(120.0, 0.0, 0.0, 1.0);    // rotate by 120 degree, M6 coord. sys 
  draw_blade();// draw 2nd blade 

  glRotatef(120.0, 0.0, 0.0, 1.0);    // rotate by 120 degree,M7 coord. sys 
  draw_blade();// draw 3rd blade 
  
  glPopMatrix();
  //------------------------------------------------------
  glPushMatrix();
  glTranslatef(2, -3, 2);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  gluCylinder(cylind, 0.25, 0.25, // radius of top and bottom circle 
      1,              // height of the cylinder 
      12,               // use 12-side polygon approximating circle
      3);               // Divide it into 3 sections 
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-2, -3, 2);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  gluCylinder(cylind, 0.25, 0.25, // radius of top and bottom circle 
      1,              // height of the cylinder 
      12,               // use 12-side polygon approximating circle
      3);               // Divide it into 3 sections 
  glPopMatrix();

  glPushMatrix();
  glTranslatef(2, -3, -2);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  gluCylinder(cylind, 0.25, 0.25, // radius of top and bottom circle 
      1,              // height of the cylinder 
      12,               // use 12-side polygon approximating circle
      3);               // Divide it into 3 sections 
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-2, -3, -2);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  gluCylinder(cylind, 0.25, 0.25, // radius of top and bottom circle 
      1,              // height of the cylinder 
      12,               // use 12-side polygon approximating circle
      3);               // Divide it into 3 sections 
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-3, -4, -2);
  glRotatef(90.0, 0.0, 1.0, 0.0);
  gluCylinder(cylind, 0.5, 0.25, // radius of top and bottom circle 
      6,              // height of the cylinder 
      12,               // use 12-side polygon approximating circle
      3);               // Divide it into 3 sections 
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-3, -4, 2);
  glRotatef(90.0, 0.0, 1.0, 0.0);
  gluCylinder(cylind, 0.5, 0.25, // radius of top and bottom circle 
      6,              // height of the cylinder 
      12,               // use 12-side polygon approximating circle
      3);               // Divide it into 3 sections 
  glPopMatrix();

  ///************************************

  glPopMatrix();
  glPopMatrix();
}

/*-------------------------------------------------------
 * Display and Idle callbaacck func. This func. draws three
 * cubes at proper places to simulate a solar system.
 */
void display()
{

  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

  /*----Define the current eye position and the eye-coordinate system---*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, lit_exponent);
  glRotatef(head, 0.0, 1.0, 0.0);    /*--Transformation in Eye coord. sys--*/
  glRotatef(roll, 0.0, .0, 1.0);
  glRotatef(Epitch, 1.0, 0.0, 0.0);
  glTranslatef(0.0, up, 0.0);
  glTranslatef(-right, 0.0, 0.0);
  glTranslatef(0.0, 0.0, -zoom);

  glPushMatrix();
  glLoadIdentity();
  
  

  glPopMatrix();
  glEnable(GL_CULL_FACE);
  /*----Define Viewing Matrix-----*/
  gluLookAt(8.0, 8.0, 20.0, 8.0, 4.0, 4.0, 0.0, 1.0, 0.0);
  
  /*-------Rotate light position----*/
  glPushMatrix();
  glTranslatef(8.0, 0.0, 8.0);          /* Move to [8, 0, 8] */
  glRotatef(lit_angle, 0.0, 0.0, 1.0);  /* Rotate about y-axis */
  /*-------Draw light source as a red cube----*/
  glPushMatrix();
  /*----position light1 (fixed) in eye coord sys ---*/
  glLightfv(GL_LIGHT1, GL_POSITION, lit1_position);  /*fixed position in eye space---*/
  glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
  /*---draw the light source ---*/
  glTranslatef(lit1_position[0], lit1_position[1], lit1_position[2]);
  glDisable(GL_LIGHTING);
  glDisable(GL_CULL_FACE);
  glColor3f(lit1_diffuse[0],lit1_diffuse[1],lit1_diffuse[2]);
  glScalef(5.0, 5.0, 5.0);
  if(open1==1)glutSolidSphere(0.3, 12, 12);
  glScalef(0.2,0.2, 0.2);
  glEnable(GL_LIGHTING);      /* Turn on lighting */
  /*----Redefine position and direction of light0-----*/
  //glLightfv(GL_LIGHT0, GL_POSITION, lit_position);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lit_direction);
  glLightfv(GL_LIGHT0, GL_SHININESS, &ligshin);
  glPopMatrix();

  /*-------Draw the floor ----*/
  glMaterialfv(GL_FRONT, GL_AMBIENT, floor_ambient);
  glMaterialfv(GL_FRONT, GL_SPECULAR, floor_specular);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, floor_diffuse);
  glMaterialf(GL_FRONT, GL_SHININESS, floor_shininess);


  if(sel==0){
    make_view(4);
    make_projection(4);
    glViewport(width/2, 0, width/2, height/2);
    draw_scene();

    make_view(1);
    make_projection(1);
    glViewport(0, height/2, width/2, height/2);
    draw_scene();
    make_view(1);
    //draw_view();
    
    make_view(2);
    glViewport(width/2, height/2, width/2, height/2);
    draw_scene();
    make_view(2);
    //draw_view();

    make_view(3);
    glViewport(0, 0, width/2, height/2);
    draw_scene();
    make_view(3);
    //draw_view();
  }
  else if(sel==1){
    make_view(1);
    make_projection(1);
    glViewport(0, 0, width, height);
    draw_scene();
    make_view(1);
    //draw_view();
  }
  else if(sel==2){
    glViewport(0, 0, width, height);
    draw_scene();
    make_view(2);
    //draw_view();
  }
  else if(sel==3){
    make_view(3);
    glViewport(0, 0, width, height);
    draw_scene();
    make_view(3);
    //draw_view();
  }
  else if(sel==4){
    make_view(4);
    make_projection(4);
    glViewport(0, 0, width, height);
    draw_scene();
  }

  glutSwapBuffers();
}


/*--------------------------------------------------
 * Reshape callbaacck function which defines the size
 * of the main window when a reshape event occurrs.
 */
void my_reshape(int w, int h)
{
  /*
  width = w;
  height = h;

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(w>h)
    glOrtho(-15.0, 15.0, -15.0*(float)h/(float)w, 15.0*(float)h/(float)w, 
            -10.5, 30.0);
  else
    glOrtho(-15.0*(float)w/(float)h, 15.0*(float)w/(float)h, -15.0, 15.0, 
            -10.5, 30.0);
            */
  
  width = w;
  height = h;

  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0, (double) w/ (double) h, 1.5, 50.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}


void cyclego(){
  for(int i=0;i<360;i++){
    cyclecc=i;
    for(int j=0;j<10;j++){
      bld_ang += 0.5;
      if(bld_ang>360.0){
        bld_ang -= 360.0;
      }
      arm_ang -= 0.5;
      if (arm_ang < 0) arm_ang += 360.0;
      display();
    }
  }
  cyclecc=0;
}

/*--------------------------------------------------
 * Keyboard callbaacck func. When a 'q' key is pressed,
 * the program is aborted.
 */
int t=0;
char last=' ';
double aa =0.5;
void my_keyboard(unsigned char key, int x, int y)
{
  float  xx[3], yy[3], zz[3];
  printf("i printf:%c\n",key);

  if(last==key){
    t++;
  }
  else{
    t=0;
    last = key;
  }

  froonnt = baacck = leeft = riight =0;
  if (key == 'Q' || key == 'q') exit(0);
  else if(key == 'W' || key == 'w'){
    nowx += (aa*t)/2;
    froonnt =1;
    //if(nowx>10) nowx=10;
  }
  else if(key == 'S' || key == 's') {
    nowx -= (aa*t)/2;
    baacck =1;
    //if(nowx<0) nowx =0;
  }
  else if(key == 'A' || key == 'a'){
    nowz += (aa*t)/2;
    leeft = 1;
    //if(nowz>10) nowz=10;
  }
  else if(key == 'D' || key == 'd') {
    nowz -= (aa*t)/2;
    riight=1;
    //if(nowz<0) nowz =0;
  }
  else if(key == 't' || key == 'T'){
    nowy += (aa*t)/2;
    
  }
  else if(key == 'G' || key == 'g') {
    nowy -= (aa*t)/2;
    if(nowy<-5) nowy=-5;
    //if(nowy<0) nowy =0;
  }
  //-view---------------------------------
  else if(key == '8'){
    eyeDy += 0.5;       // move up 
    for(int i=0;i<3;i++) eye[i] -= 0.5*u[1][i];
  }
  else if(key == '2') {
    eyeDy += -0.5;       // move down 
    for(int i=0;i<3;i++) eye[i] += 0.5*u[1][i];
  }
  else if(key == '4'){
    eyeDx += -0.5;       // move leeft 
    for(int i=0;i<3;i++) eye[i] += 0.5*u[0][i];
  }
  else if(key == '6') {
    eyeDx += 0.5;        // move riight 
    for(int i=0;i<3;i++) eye[i] -= 0.5*u[0][i];
  }
  else if(key == '9'){
    eyeDz += 0.5;        // zoom in 
    for(int i=0;i<3;i++) eye[i] -= 0.5*u[2][i];
  }
  else if(key == '3') {
    eyeDz += -0.5;       // zoom out 
    for(int i=0;i<3;i++) eye[i] += 0.5*u[2][i];
  }
  ////object
  //else if(key == 'U' || key == 'u'){
  //  objectx += 1;
  //  //if(objectx>10) objectx=10;
  //}
  //else if(key == 'J' || key == 'j') {
  //  objectx -= 1;
  //  //if(objectx<0) objectx =0;
  //}
  //else if(key == 'H' || key == 'h'){
  //  objectz += 1;
  //  //if(objectz>10) objectz=10;
  //}
  //else if(key == 'k' || key == 'K') {
  //  objectz -= 1;
  //  //if(objectz<0) objectz =0;
  //}
  //else if(key == 'o' || key == 'O'){
  //  objecty += 1;
  //  //if(objecty>10) objecty=10;
  //}
  //else if(key == 'L' || key == 'l') {
  //  objecty -= 1;
  //  //if(objecty<0) objecty =0;
  //}  
  ////rotat
  else if(key == 'E' || key == 'e') {
    nowrotat +=5;
    if(nowrotat>360.0){
      nowrotat-=360;
    }
  }
  //else if(key == 'Q' || key == 'q') {
  //  nowrotat -=5;
  //  if(nowrotat<0){
  //    nowrotat+=360;
  //  }
  //}
  //cycle_go
  //else if(key == 't' || key == 'T'){
  //  cyclego();
  //}
  
  else if(key=='p'){             // pitching 
    eyeAngx += 5.0;
    if(eyeAngx > 360.0) {
      eyeAngx = eyeAngx-360.0;
    }
    yy[0] = u[1][0]*cv - u[2][0]*sv;
    yy[1] = u[1][1]*cv - u[2][1]*sv;
    yy[2] = u[1][2]*cv - u[2][2]*sv;

    zz[0] = u[2][0]*cv + u[1][0]*sv;
    zz[1] = u[2][1]*cv + u[1][1]*sv;
    zz[2] = u[2][2]*cv + u[1][2]*sv;

    for(int i=0;i<3;i++){
      u[1][i] = yy[i];
      u[2][i] = zz[i];
    }
  }
  else if(key=='P'){          
    eyeAngx += -5.0;
    if(eyeAngx<0.0) eyeAngx += 360.0;
    yy[0] = u[1][0]*cv + u[2][0]*sv;
    yy[1] = u[1][1]*cv + u[2][1]*sv;
    yy[2] = u[1][2]*cv + u[2][2]*sv;

    zz[0] = u[2][0]*cv - u[1][0]*sv;
    zz[1] = u[2][1]*cv - u[1][1]*sv;
    zz[2] = u[2][2]*cv - u[1][2]*sv;

    for(int i=0;i<3;i++){
      u[1][i] = yy[i];
      u[2][i] = zz[i];
    }
  }
  else if(key=='h'){            // heading 
    eyeAngy += 5.0;
    if(eyeAngy>360.0) eyeAngy -= 360.0;
    for(int i=0;i<3;i++){
      xx[i] = cv*u[0][i] - sv*u[2][i];
      zz[i] = sv*u[0][i] + cv*u[2][i];
    }
    for(int i=0;i<3;i++){
      u[0][i] = xx[i];
      u[2][i] = zz[i];
    }
  }
  else if(key=='H'){
    eyeAngy += -5.0;
    if(eyeAngy<0.0) eyeAngy += 360.0;
    for(int i=0;i<3;i++){
      xx[i] = cv*u[0][i] + sv*u[2][i];
      zz[i] = -sv*u[0][i] + cv*u[2][i];
    }
    for(int i=0;i<3;i++){
      u[0][i] = xx[i];
      u[2][i] = zz[i];
    }
  }
  else if(key=='r'){            // rolling 
    eyeAngz += 5.0;
    if(eyeAngz>360.0) eyeAngz -= 360.0;
    for(int i=0;i<3;i++){
      xx[i] = cv*u[0][i] - sv*u[1][i];
      yy[i] = sv*u[0][i] + cv*u[1][i];
    }
    for(int i=0;i<3;i++){
      u[0][i] = xx[i];
      u[1][i] = yy[i];
    }
  }
  else if(key=='R'){
    eyeAngz += -5.0;
    if(eyeAngz<0.0) eyeAngz += 360.0;
    for(int i=0;i<3;i++){
      xx[i] = cv*u[0][i] + sv*u[1][i];
      yy[i] = -sv*u[0][i] + cv*u[1][i];
    }
    for(int i=0;i<3;i++){
      u[0][i] = xx[i];
      u[1][i] = yy[i];
    }
  }
  else if(key=='f'||key=='F'){
    sel= (sel>3)?0:sel+1;
  }
  else if(key == 'Y'){
    ba=ba+0.25;
    if(ba>5.0)ba=5.0;
  }
  else if(key == 'y'){
    ba=ba-0.25;
    if(ba<=0)ba=0.25;
  }
  if(key=='z'){
	  glEnable(GL_LIGHTING);
	  glShadeModel(GL_SMOOTH);
  } 
  else if(key=='x'){
	  glEnable(GL_LIGHTING);
	  glShadeModel(GL_FLAT);
  }
  else if(key==' ') lit_angle += 5.0;
  else if(key=='V'){
    glEnable(GL_LIGHT0);
    open0=1;
  } 
  else if(key=='v'){
    glDisable(GL_LIGHT0);
    open0=0;
  } 
  else if(key=='B') {
    glEnable(GL_LIGHT1);
    open1=1;
  }
  else if(key=='b') {
    glDisable(GL_LIGHT1);
    open1=0;
  }
  else if(key == '<')lit_cutoff++;
  else if(key == '>')lit_cutoff--;
  else if(key=='M'){
    if(lit_diffuse[0]-0.1>=0){
      lit_diffuse[0]-=0.1;
      lit_diffuse[1]-=0.1;
      lit_diffuse[2]-=0.1;
    }
  }
  else if(key=='m'){
    if(lit_diffuse[0]+0.1<=1.0){
      lit_diffuse[0]+=0.1;
      lit_diffuse[1]+=0.1;
      lit_diffuse[2]+=0.1;
    }
  }
  else if(key=='N'){
    if(lit_direction[0]-0.1>=0){
      lit_direction[0]-=0.1;
    }
  }
  else if(key=='n'){
    if(lit_direction[0]+0.1<=1.0){
      lit_direction[0]+=0.1;
    }
  }
  else if(key=='l'){
    lit1_position[3]=lit1_position[3]==1.0?0.0:1.0;
  }
  else if(key=='o'){
    lit1_diffuse[0]=lit1_diffuse[0]+0.1<=1?lit1_diffuse[0]+0.1:1.0;
  }
  else if(key=='O'){
    lit1_diffuse[0]=lit1_diffuse[0]-0.1>=0?lit1_diffuse[0]-0.1:0.0;
  }
  else if(key=='i'){
    lit1_diffuse[0]=lit1_diffuse[0]+0.1<=1?lit1_diffuse[0]+0.1:1.0;
    lit1_diffuse[1]=lit1_diffuse[1]+0.1<=1?lit1_diffuse[1]+0.1:1.0;
    lit1_diffuse[2]=lit1_diffuse[2]+0.1<=1?lit1_diffuse[2]+0.1:1.0;
  }
  else if(key=='I'){
    lit1_diffuse[0]=lit1_diffuse[0]-0.1>=0?lit1_diffuse[0]-0.1:0.0;
    lit1_diffuse[1]=lit1_diffuse[1]-0.1>=0?lit1_diffuse[1]-0.1:0.0;
    lit1_diffuse[2]=lit1_diffuse[2]-0.1>=0?lit1_diffuse[2]-0.1:0.0;
  }
  else if(key == 'u'){
    floorchange^=1;
  }
  else if(key == 'c'){
    open2=0;
    glDisable(GL_LIGHT2);
  }
  else if(key == 'C'){
    open2=1;
    glEnable(GL_LIGHT2);
  }
  else if(key=='k'){
    //lit_exponent=lit_exponent+0.1<=1?lit_exponent+0.1:1.0;
    lit_exponent++;
  }
  else if(key=='K'){
    //lit_exponent=lit_exponent-0.1>=0?lit_exponent-0.1:0.0;
    lit_exponent--;
  }
  else if(key=='j'){
    lit_diffuse[1]=lit_diffuse[1]+0.1<=1.0?lit_diffuse[1]+0.1:0.0;
  }
  printf("lit_exponent:%f\n",lit_exponent);
  display();
}


/*---------------------------------------------
 * Idle event callbaacck function, increase blade and arm
 * rotation angles.
 */
void idle_func()
{
  if(nowy>-5){
    bld_ang += 0.5;
    if(bld_ang>360.0){
      bld_ang -= 360.0;
    }
    arm_ang -= 0.5;
    if (arm_ang < 0) arm_ang += 360.0;
  }
  lit2_diffuse[1]=lit2_diffuse[1]+0.001<=1?lit2_diffuse[1]+0.001:0.001;
  //printf("%f\n",lit2_diffuse);
  display(); /* show the scene again */
}


/*---------------------------------------------------
 * Main procedure which defines the graphics environment,
 * the operation style, and the callbaacck func's.
 */
void main(int argc, char **argv)
{
  /*-----Initialize the GLUT environment-------*/
  glutInit(&argc, argv);

  /*-----Depth buffer is used, be careful !!!----*/
  glEnable(GLUT_MULTISAMPLE);
glutSetOption(GLUT_MULTISAMPLE, 8);
  glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB| GLUT_DEPTH|GLUT_MULTISAMPLE);
  glutInitWindowSize(width, height);
  glutCreateWindow("windmill");

  myinit();      /*---Initialize other state varibales----*/
  
  /*----Associate callbaacck func's whith events------*/
  glutDisplayFunc(display);
  glutIdleFunc(idle_func);    /* Add idle event */
  glutReshapeFunc(my_reshape);
  glutKeyboardFunc(my_keyboard);

  glutMainLoop();
}
//  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, lit_exponent);
