//
// Created by 王颖 on 23/06/2017.
//

#ifndef RAYTRACING_COLLISION_H
#define RAYTRACING_COLLISION_H

#include "Object.h"
class Collision {
public:
    // Construction & Destruction
    Collision():_object(NULL),_conNormal(false),_distance(INF){}
    virtual ~Collision() {
        if(_object) delete _object;
    }
    // Get & Set
    Object* _GetObject(){return _object;}
    Object* _SetObject(Object* object){_object = object; return _object;}
    double _GetDistance(){return _distance;}
    double _SetDistance(double distance){_distance = distance; return _distance;}
    bool _GetConNormal(){return _conNormal;}
    bool _SetConNormal(bool conNormal){_conNormal = conNormal; return _conNormal;}
    Vec3d _GetHit(){return _hit;}
    Vec3d _SetHit(Vec3d hit){_hit = hit;return _hit;}
    Vec3d _GetNormal(){return _normal;}
    Vec3d _SetNormal(Vec3d normal){_normal = normal;return _normal;}
    Vec3d _GetDirection(){return _direction;}
    Vec3d _SetDirection(Vec3d direction){_direction = direction;return _direction;}

private:
    Object * _object;
    double _distance;
    bool _conNormal;//conNormal: consistent with the direction of normal
    Vec3d _hit, _normal, _direction;
};



#endif //RAYTRACING_COLLISION_H
