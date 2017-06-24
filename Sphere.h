//
// Created by 王颖 on 23/06/2017.
//

#ifndef RAYTRACING_SPHERE_H
#define RAYTRACING_SPHERE_H

#include "Object.h"
#include "Collision.h"
class Sphere : public Object {
public:
    Sphere():Object(){
        _xAxis = Vec3d( 0 , 0 , 1 );
        _yAxis = Vec3d( 0 , 1 , 0 );
    }

    Collision* _Collide( Vec3d origin , Vec3d direction ) {
        Collision* collision = NULL;
        direction._Normalize();
        Vec3d vec = origin - _center;
        double b = - vec._Dot( direction );
        double det = _r * _r - (vec._ModuleSquare() - b * b);

        if ( det <= MYEPS ) return collision;
        det = sqrt( det );
        double x1 = b - det  , x2 = b + det;
        if ( x2 <= MYEPS ) return collision;
        collision = new Collision;
        bool conNormal = (x1 > MYEPS);
        double distance = (x1 > MYEPS)? x1 : x2;
        Vec3d hit = origin + direction * distance;
        Vec3d normal = ( hit - origin);
        normal._Normalize();
        collision->_SetConNormal(conNormal);
        collision->_SetDistance( distance);
        collision->_SetHit(hit);
        collision->_SetNormal((x1 > MYEPS) ? normal : -normal);
        collision->_SetDirection(direction);
        collision->_SetObject(this);
        return collision;
    }
    Color _GetTexture(Vec3d point) {
        Vec3d vec = point - _center;
        vec._Normalize();
        double a = acos( - vec._Dot( _xAxis ) );
        double b = acos( min( max( vec._Dot( _yAxis ) / sin( a ) , -1.0 ) , 1.0 ) );
        double u = a / PI , v = b / 2 / PI;
        if ( vec._Dot(_yAxis._Cross(_xAxis)) < 0 ) v = 1 - v;
        return _GetMaterial()->_GetSmoothColor(u,v);
    }

    // IO
    void _Input( std::string var , std::stringstream& fin ) {
        if ( var == "O=" ) fin >> _center;
        if ( var == "R=" ) fin >> _r;
        if ( var == "De=" ) fin >> _xAxis;
        if ( var == "DC=" ) fin >> _yAxis;
        Object::_Input( var , fin );
    }
private:
    Vec3d _center , _xAxis , _yAxis;
    double _r;
};



#endif //RAYTRACING_SPHERE_H
