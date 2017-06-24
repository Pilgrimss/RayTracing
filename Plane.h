//
// Created by 王颖 on 23/06/2017.
//

#ifndef RAYTRACING_PLANE_H
#define RAYTRACING_PLANE_H

#include "Object.h"
#include "Collision.h"
class Plane: public Object {
public:
    // Construction & Destruction
    Plane():Object(){}
    // Virtual API
    Collision* _Collide( Vec3d origin , Vec3d direction ) {
        Collision* collision = NULL;
        direction._Normalize();
        _normal._Normalize();
        double cos = direction._Dot(_normal);
        if(fabs(cos) < MYEPS) return collision;
        double distance = (_normal * _r - origin)._Dot(_normal)/cos;
        if(distance < MYEPS) return collision;
        collision = new Collision;
        collision->_SetDirection(direction);
        collision->_SetObject(this);
        collision->_SetDistance(distance);
        collision->_SetConNormal((cos < 0));
        collision->_SetHit(origin + direction * distance);
        collision->_SetNormal((cos < 0)? _normal : - _normal);
        return collision;
    }
    Color _GetTexture(Vec3d point){
        double u = point._Dot(_xAxis) / _xAxis._ModuleSquare() + 0.5;
        double v = point._Dot(_yAxis) /_yAxis._ModuleSquare() + 0.5;
        return _GetMaterial()->_GetSmoothColor(u,v);
    }
    //IO
    void _Input( std::string var, std::stringstream& fin) {
        if ( var == "N=" ) fin >> _normal;
        if ( var == "R=" ) fin >> _r;
        if ( var == "Dx=" ) fin >> _xAxis;
        if ( var == "Dy=" ) fin >> _yAxis;
        Object::_Input( var , fin );
    }
private:
    Vec3d _normal, _xAxis, _yAxis;
    double _r;
};



#endif //RAYTRACING_PLANE_H
