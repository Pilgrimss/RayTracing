//
// Created by 王颖 on 23/06/2017.
//

#ifndef RAYTRACING_RECTANGLE_H
#define RAYTRACING_RECTANGLE_H

#include "Object.h"
#include "Collision.h"
class Rectangle : public Object {
public:

    Rectangle():Object(){}
    Collision* _Collide( Vec3d origin , Vec3d direction ) {
        Collision* collision = NULL;
        direction._Normalize();
        _normal._Normalize();
        double cos = direction._Dot(_normal);
        if(fabs(cos) < MYEPS) return collision;
        double distance = (_normal * _r - origin)._Dot(_normal)/cos;
        if(distance < MYEPS) return collision;
        Vec3d hit = origin + direction * distance;
        double u = hit._Dot( _xAxis ) / _xAxis._ModuleSquare();
        double v = hit._Dot( _yAxis ) / _yAxis._ModuleSquare();
        if(!(0 <= u && u <= 1 && 0 <= v && v <= 1)) return collision;
        collision = new Collision;
        collision->_SetDirection(direction);
        collision->_SetObject(this);
        collision->_SetDistance(distance);
        collision->_SetConNormal((cos < 0));
        collision->_SetHit(hit);
        collision->_SetNormal((cos < 0)? _normal : -_normal);
        return collision;
    }

    Color _GetTexture(Vec3d point){
        double u = point._Dot(_xAxis) / _xAxis._ModuleSquare() ;
        double v = point._Dot(_yAxis) /_yAxis._ModuleSquare();
        return _GetMaterial()->_GetSmoothColor(u,v);
    }

    // IO
    void _Input( std::string var , std::stringstream& fin ) {
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



#endif //RAYTRACING_RECTANGLE_H
