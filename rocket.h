#ifndef ROCKET__H
#define ROCKET__H

#include "particle.h"
#include "stateVectorRocket.h"
#include <vector>
#define maxParticles  1000

class Rocket
{
public:
  static const int SIZE_PAR = maxParticles;
  Vector3d Velocity;
  Vector3d Position;
  Vector3d Force;
  Vector3d Accel;
  float Mass;
  float Lifespan;
  float Fade;
  Vector3d Color;
  bool Active;
  bool Explode;
  bool initializeExp;
  static int Count;
  int rocketIndex;
  int displayParticles;

 int countFade;
 Particle parExp[SIZE_PAR];
 StateVectorRocket SVExp;
 std::vector <StateVectorRocket> SVExpPrev;

        Rocket(Vector3d vel,Vector3d pos,Vector3d force,Vector3d accel, int mass,float life,float fade,Vector3d color,bool active,bool explode,bool initExp, int noOfPart) : SVExp(maxParticles*2),Velocity(vel), Position(pos),Force(force),Accel(accel),Mass(mass),Lifespan(life),Fade(fade),Color(color),Active(active),Explode(explode) ,initializeExp(initExp) ,displayParticles(noOfPart)           
        { 
          rocketIndex = Count++;
        }
	Rocket(){ }



};

#endif
