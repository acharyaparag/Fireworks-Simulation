#include "particle.h"

void Particle::Reset(Vector3d vel,Vector3d pos,Vector3d force,Vector3d accel, int mass,float life,float fade,Vector3d color,bool active)
{
 Velocity = vel; 
 Position = pos;
 Force =force;
 Accel = accel;
 Mass = mass;
 Lifespan = life;
 Fade = fade;
 Color = color;
 Active = active;     
}

