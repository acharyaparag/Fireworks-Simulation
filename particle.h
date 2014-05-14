#ifndef PARTICLE__H
#define PARTICLE__H

#include "Vector.h"

class Particle
{
public:

  Vector3d Velocity;
  Vector3d Position;
  Vector3d Force;
  Vector3d Accel;
  float Mass;
  float Lifespan;
  float Fade;
  Vector3d Color;
  bool Active;
  int toggleColor;

        Particle(Vector3d vel,Vector3d pos,Vector3d force,Vector3d accel, int mass,float life,float fade,Vector3d color,bool active,int togcol) : Velocity(vel), Position(pos),Force(force),Accel(accel),Mass(mass),Lifespan(life),Fade(fade),Color(color),Active(active),toggleColor(togcol)              
        { }

	Particle(){ }

        void Reset(Vector3d vel,Vector3d pos,Vector3d force,Vector3d accel, int mass,float life,float fade,Vector3d color,bool active);
};

#endif
