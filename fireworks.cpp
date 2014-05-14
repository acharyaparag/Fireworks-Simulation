
#include "Camera.h"
#include "particle.h"
#include <vector>
#include "rocket.h"
#include "stateVectorRocket.h"
#include <vector>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif


#define RADIUS		0.04
#define radiusOrigin    0.1
#define P_I             3.14
#define RANDOM_RANGE(lo, hi) ((lo) + (hi - lo) * drand48())
#define WINDOW_WIDTH	1000	/* window dimensions */
#define WINDOW_HEIGHT	740
#define width           14
#define height          10 
#define noOfRocket     100


Camera *camera;
Vector3d Wind(0.03,0.0,0.01);
Vector3d centerOrigin(0.0,-2.0,0.0);

static double WinWidth = WINDOW_WIDTH;
static double WinHeight = WINDOW_HEIGHT;
static double TimeStep;
static double Time = 0;
static int TimerDelay;
static int TimeStepsPerDisplay;
static double DispTime;
static char *ParamFilename = NULL;
static double CoeffofRestitution;
static int NTimeSteps = -1;
int persp_win;




std::vector<Rocket*> rockets;
StateVectorRocket SV(noOfRocket*2);
std::vector <Vector3d> colors;

int framesLaunch = ((int)rand() % 300);
int Count = 1;
int colorIndex = 0;

void TimerCallback(int value);

using namespace std;



void initializeExplosion(int i)
{

  Vector3d centerExp = rockets[i]->Position;
  Vector3d radiusExp = 0.01;
  rockets[i]->countFade = rockets[i]->displayParticles;

  for ( int k = 0; k < rockets[i]->displayParticles ; k++)
      {
             rockets[i]->initializeExp = true;
             float theta = (RANDOM_RANGE(1,360))*P_I/180.0;
             float phi = (RANDOM_RANGE(1,360))*P_I/180.0;
             float speed = RANDOM_RANGE(0.01,0.1);	
             float X = sin(theta)*cos(phi);
             float Y = sin(theta)*sin(phi);
             float Z = cos(theta);
             Vector3d unitVec(X,Y,Z);
             unitVec = unitVec.normalize();
      	     Vector3d pos = centerExp + unitVec*radiusExp;
             Vector3d Velocity = speed*unitVec;
             Vector3d force(0,0,0);
             Vector3d accel(0,0,0);
             float Lifespan = 1.0f;
             float Fade = RANDOM_RANGE(0.001,0.01) ;
             float Mass = 1;
             Vector3d Color(0,0,0); 
             Color = colors[colorIndex];
             bool Active = true;
             int togglecolor = 1;
             rockets[i]->parExp[k] = Particle(Velocity,pos,force,accel,Mass,Lifespan,Fade,Color,Active,togglecolor);
             rockets[i]->SVExp.Data[k] = pos; 
             rockets[i]->SVExp.Data[rockets[i]->displayParticles + k] = Velocity;
         
      }
             colorIndex++;
             if ( colorIndex >= colors.size())
                  colorIndex = 0;
}


void drawExpParticles(int i)
{


 for ( int k = 0; k < rockets[i]->displayParticles ; k++)
      {
        if ( rockets[i]->parExp[k].Active == true)
          {
          Vector3d Color;
          int index = 0;            
          Color = rockets[i]->parExp[k].Color;
          glColor4f(Color.x,Color.y,Color.z,rockets[i]->parExp[k].Lifespan);
          if (rockets[i]->rocketIndex%3 == 0)
             index = rockets[i]->SVExpPrev.size() - 1;
          else
             index = rockets[i]->SVExpPrev.size() - 10; 

          if ( rockets[i]->rocketIndex%7 == 0 )
             {            
               glBegin(GL_POINTS);
               glVertex3f(rockets[i]->SVExp.Data[k].x,rockets[i]->SVExp.Data[k].y,rockets[i]->SVExp.Data[k].z);
               glEnd();
             }
          else
             {
               glBegin(GL_LINES);
               if(index <0)
                  glVertex3f(rockets[i]->SVExp.Data[k].x,rockets[i]->SVExp.Data[k].y,rockets[i]->SVExp.Data[k].z);
               else 
                 glVertex3f(rockets[i]->SVExpPrev[index].Data[k].x,rockets[i]->SVExpPrev[index].Data[k].y,rockets[i]->SVExpPrev[index].Data[k].z);
     
              glVertex3f(rockets[i]->SVExp.Data[k].x,rockets[i]->SVExp.Data[k].y,rockets[i]->SVExp.Data[k].z);
              glEnd();  
             } 
 	  rockets[i]->parExp[k].Lifespan -= rockets[i]->parExp[k].Fade;
    	  if(rockets[i]->parExp[k].Lifespan<0.0f)
            {
             rockets[i]->parExp[k].Active = false;
             rockets[i]->countFade--;
            } 
          } 
      }
}


void init() {
  // set up camera
  // parameters are eye point, aim point, up vector

camera = new Camera(Vector3d(1.8,0.8,2.9), Vector3d(0,0,0), 
		      Vector3d(0, 1, 0));

glClearColor(0.0, 0.0, 0.0, 0.0);	
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_COLOR);

}

void PerspDisplay() {

  glDisable(GL_LIGHTING);
  int i;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glAccum(GL_RETURN,0.95f);

  glClear(GL_ACCUM_BUFFER_BIT);

  // draw the camera created in perspective
  camera->PerspectiveDisplay(WINDOW_WIDTH,WINDOW_HEIGHT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


glPointSize(4.f);
glBegin(GL_POINTS);


for(int i=0;i<rockets.size();i++)
   {  
    if (rockets[i]->Active == true)
       {
 
         glColor4f(rockets[i]->Color.x,rockets[i]->Color.y,rockets[i]->Color.z,rockets[i]->Lifespan); 
         glVertex3f(SV.Data[i].x,SV.Data[i].y,SV.Data[i].z);
         rockets[i]->Lifespan -= rockets[i]->Fade;
         if(rockets[i]->Lifespan<0.2f)
             {
               rockets[i]->Active = false;
               rockets[i]->Explode = true;
             }  
       }
   }

glEnd();

glPointSize(2.f);
for(int i=0;i<rockets.size();i++)
   {

        if ( rockets[i]->rocketIndex % 4 == 0)      
             glEnable(GL_LIGHTING);
        else 
             glDisable(GL_LIGHTING);
 

    if ( rockets[i]->Explode == true and rockets[i]->initializeExp == false)
       {
        initializeExplosion(i);
       }

   if (rockets[i]->Explode == true and rockets[i]->initializeExp == true)
       {
        drawExpParticles(i);
       }

   }

glutSwapBuffers();

glAccum(GL_ACCUM, 0.8f);

}

Vector3d externalForce(int i){

  Vector3d G(0.0,-0.01,0.0),acceleration;
  acceleration = G  ;
  return acceleration;
}

Vector3d externalForceRocket(int i){


  Vector3d G;
  float x = 0.0;
  float y = 0.0001;
  float z = -0.03;
  if (rockets[i]->rocketIndex%3 == 0)
     {
       z = 0.03;
     }
  G.set(x,y,z);
  return G;
}

StateVectorRocket calcForce(const StateVectorRocket &inSV,double t,int rocketNo)
{
 StateVectorRocket SVDot(inSV.Data.size());

 for ( int i = 0 ; i<rockets[rocketNo]->displayParticles; i++)
      {
        rockets[rocketNo]->parExp[i].Force.set(0,0,0);
        rockets[rocketNo]->parExp[i].Force = rockets[rocketNo]->parExp[i].Force + externalForce(i);
        rockets[rocketNo]->parExp[i].Accel =   rockets[rocketNo]->parExp[i].Force/  rockets[rocketNo]->parExp[i].Mass;
      }

  for ( int i = 0 ; i<rockets[rocketNo]->displayParticles; i++)
     {
        SVDot.Data[i] = inSV.Data[rockets[rocketNo]->displayParticles + i];
        SVDot.Data[rockets[rocketNo]->displayParticles + i]  =   rockets[rocketNo]->parExp[i].Accel;
     }

  return SVDot;
}


StateVectorRocket calcForceRocket(const StateVectorRocket &inSV,double t)
{
 StateVectorRocket SVDot(inSV.Data.size());

 for ( int i = 0 ; i<rockets.size(); i++)
      {
        rockets[i]->Force.set(0,0,0);
        rockets[i]->Force = rockets[i]->Force + externalForceRocket(i);
        rockets[i]->Accel =   rockets[i]->Force/rockets[i]->Mass;
      }

  for ( int i = 0 ; i<rockets.size(); i++)
     {
         if(rockets[i]->Active == true)
             {
               SVDot.Data[i] = inSV.Data[rockets.size() + i];
               SVDot.Data[rockets.size() + i]  = rockets[i]->Accel;
             }
     }

  return SVDot;
}


void Simulate(){


  StateVectorRocket K1 = calcForceRocket(SV,TimeStep)*TimeStep;
  StateVectorRocket K2 = calcForceRocket(SV + K1*0.5,TimeStep)*TimeStep;
  StateVectorRocket K3 = calcForceRocket(SV + K2*0.5,TimeStep)*TimeStep;
  StateVectorRocket K4 = calcForceRocket(SV + K3,TimeStep)*TimeStep;
  SV = SV + (K1+K2*2+K3*2+K4)*0.1666;


 for ( int i = 0 ; i<rockets.size(); i++)
      {
         if(rockets[i]->Explode ==  true)
           {     
             rockets[i]->SVExpPrev.push_back(rockets[i]->SVExp); 
             StateVectorRocket K1 = calcForce(rockets[i]->SVExp,TimeStep,i)*TimeStep;
             StateVectorRocket K2 = calcForce(rockets[i]->SVExp + K1*0.5,TimeStep,i)*TimeStep;
             StateVectorRocket K3 = calcForce(rockets[i]->SVExp + K2*0.5,TimeStep,i)*TimeStep;
             StateVectorRocket K4 = calcForce(rockets[i]->SVExp + K3,TimeStep,i)*TimeStep;
             rockets[i]->SVExp = rockets[i]->SVExp + (K1+K2*2+K3*2+K4)*0.1666;
           }  
      } 

   for ( int i = 0 ; i<rockets.size(); i++)
      {
           if(rockets[i]->Active == true)
             { 
              rockets[i]->Velocity =  SV.Data[rockets.size()+i];
              rockets[i]->Position = SV.Data[i];
             }
           if(rockets[i]->Explode == true)
             { 
              for ( int k = 0 ; k<rockets[i]->displayParticles; k++)
                  {
     		    rockets[i]->parExp[k].Velocity =  rockets[i]->SVExp.Data[rockets[i]->displayParticles+k];
     		    rockets[i]->parExp[k].Position =  rockets[i]->SVExp.Data[k];
                  }
             }
      }

// advance the timestep and set the velocity and position to their new values
  Time += TimeStep;
  NTimeSteps++;
  framesLaunch--;
 
 if(framesLaunch<=0 and Count <= rockets.size())
   {
     rockets[Count++]->Active = true; 
     framesLaunch = ((int)rand() % 400);
   }

  if(NTimeSteps % TimeStepsPerDisplay == 0)
   PerspDisplay();

 for (int i = 0 ; i< rockets.size() ; i++)
     {  
   
       if(rockets[i]->countFade<=0 and rockets[i]->initializeExp == true)
         { 
          SV.Data.erase(SV.Data.begin() + rockets.size() + i);  
          SV.Data.erase(SV.Data.begin() + i);
          SV.SIZE = SV.Data.size();
          rockets.erase(rockets.begin() + i); 
         }
     }

   glutTimerFunc(TimerDelay, TimerCallback, 0);
}

/*
  Run a single time step in the simulation
*/
void TimerCallback(int){

  Simulate();
}

void LoadParameters(char *filename){
  
  FILE *paramfile;

  if((paramfile = fopen(filename, "r")) == NULL){
    fprintf(stderr, "error opening parameter file %s\n", filename);
    exit(1);
  }

  ParamFilename = filename;

  if(fscanf(paramfile, "%lf %lf %lf",
	    &CoeffofRestitution,
	    &TimeStep, &DispTime) != 3){ 
    fprintf(stderr, "error reading parameter file %s\n", filename);
    fclose(paramfile);
    exit(1);
  }

  TimeStepsPerDisplay = Max(1, int(DispTime / TimeStep + 0.5));

  TimerDelay = int(0.5 * TimeStep * 1000);
}

void InitParticles(int argc, char* argv[]){

  if(argc <= 1){
    fprintf(stderr, "Parameter file not specified\n");
    exit(1);
  }
  LoadParameters(argv[1]);
  Time = 0;
  NTimeSteps = -1;

  rockets.reserve(noOfRocket);
 
	for(int i=0; i<noOfRocket; i++)
	   {
             float theta = (RANDOM_RANGE(1,360))*P_I/180.0;
             float phi = (RANDOM_RANGE(1,360))*P_I/180.0;
             float speed = RANDOM_RANGE(0.3,0.4);
             float X = sin(theta)*cos(phi);
             float Y = WINDOW_HEIGHT/2;
             float Z = cos(theta);
             Vector3d unitVec(X,Y,Z);
             unitVec = unitVec.normalize();
      	     Vector3d pos = centerOrigin + unitVec*radiusOrigin;
             Vector3d Velocity = speed*unitVec;
             Vector3d force(0,0,0);
             Vector3d accel(0,0,0);
             float Lifespan = 1.0f;
             float Fade = (RANDOM_RANGE(0.004,0.01)) ;
             float Mass = 1;
             Vector3d Color(1.0,1.0,1.0);       
             bool Active = false;
             bool Explode = false;
             bool initExplode = false;
             int noOfParticles;
             if ( i%2 == 0)
                noOfParticles = 100;
             else 
                noOfParticles = 1000;
             rockets.push_back(new Rocket(Velocity,pos,force,accel,Mass,Lifespan,Fade,Color,Active,Explode,initExplode,noOfParticles));
             SV.Data[i] = pos; 
             SV.Data[noOfRocket + i] = Velocity;    
           }
             rockets[0]->Active = true;

 colors.push_back(Vector3d(1.0,0.84,0.0));     
 colors.push_back(Vector3d(0.54,0.168,0.88));
 colors.push_back(Vector3d(1.0,0.13,0.0)); 
 colors.push_back(Vector3d(0.09,0.09,0.43));
 colors.push_back(Vector3d(0.75,0.75,0.75)); 
 colors.push_back(Vector3d(1.0,0.84,0.0)); 
 colors.push_back(Vector3d(0.48,1.0,0.0)); 


  

  glutIdleFunc(Simulate);   
}

void handleKey(unsigned char key, int x, int y){
  
  switch(key){
    case 'q':		// 'quit the program'
    case 'Q':
    case 27:
        {
          exit(0);
        }
      
    default:		// not a valid key -- just ignore it
      return;
  }
}

void mouseEventHandler(int button, int state, int x, int y) {
  // let the camera handle some specific mouse events (similar to maya)
  camera->HandleMouseEvent(button, state, x, y);
  glutPostRedisplay();
}

void motionEventHandler(int x, int y) {
  // let the camera handle some mouse motions if the camera is to be moved
  camera->HandleMouseMotion(x, y);
  glutPostRedisplay();
}

void doReshape(int w, int h){
  int vpw, vph;
  
  float aspect = float(WINDOW_WIDTH) / float(WINDOW_HEIGHT);
  if(float(w) / float(h) > aspect){
    vph = h;
    vpw = int(aspect * h + 0.5);
  }
  else{
    vpw = w;
    vph = int(w / aspect + 0.5);
  }
  
  glViewport(0, 0, vpw, vph);
  WinWidth = w;
  WinHeight = h;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);

  glMatrixMode(GL_MODELVIEW);
}


void do_lights()
{
 float light0_ambient[] = {0.5,0.5,0.5,1.0};
 float light0_diffuse[] = {0.7,0.7,0.7,1.0};
 float light0_position[] = {1.5,2.0,2.0,1.0};
 float light0_direction[] = {-1.5,-2.0,-2.0,1.0};

 glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient);
 glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);
 glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
 glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
 glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);

}

void do_material()
{
 float mat_ambient[] = { 0.5,0.5,0.5,1.0};
 float mat_diffuse[] = { 0.9,0.9,0.1,1.0};
 float mat_specular[] = { 1.0,1.0,1.0,1.0};
 float mat_shininess[] = {5.0};
 glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
 glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
 glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
 glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}


int main(int argc, char *argv[]) {

  // set up opengl window
  glutInit(&argc, argv);
  InitParticles(argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_ACCUM);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutInitWindowPosition(50, 50);
  persp_win = glutCreateWindow("Fireworks");
  // initialize the camera and such
  init();
  // set up opengl callback functions
  do_lights();
  do_material();
  glutDisplayFunc(PerspDisplay);
  glutMouseFunc(mouseEventHandler);
  glutMotionFunc(motionEventHandler);
  glutKeyboardFunc(handleKey);

  glutMainLoop();
  return(0);
}

