//
// Created by 王颖 on 24/06/2017.
//

#ifndef RAYTRACING_AREALIGHT_H
#define RAYTRACING_AREALIGHT_H

#include"Light.h"
class AreaLight : public Light {

public:
    AreaLight() : Light() {}
    ~AreaLight() {}
    // virtual API

    Vec3d _GetCenter() { return _center; }

    LightCollision* _Collide( Vec3d origin , Vec3d direction ) {
        LightCollision* lightCollision = new LightCollision;
        lightCollision->_SetLight(this);

        direction._Normalize();
        Vec3d normal = (_xAxis._Cross(_yAxis));
        normal._Normalize();
        double cos = normal._Dot( direction);
        if ( fabs( cos ) < MYEPS ) return lightCollision;
        double distance = ( normal * _center._Dot( normal ) - origin )._Dot( normal ) / cos;
        if ( distance < MYEPS ) return lightCollision;

        Vec3d vec = ( origin + direction * distance ) - _center;
        if ( fabs( _xAxis._Dot( vec ) ) > _xAxis._ModuleSquare() ) return lightCollision;
        if ( fabs( _yAxis._Dot( vec ) ) > _yAxis._ModuleSquare() ) return lightCollision;


        lightCollision->_SetDistance(distance);
        return lightCollision;
    }

    Color _GetIrradiance( Collision* collision , Object* head , double shade, int* hash ) {
        Object* object = collision->_GetObject();
        Color color;

        for ( int i = -2 ; i < 2 ; i++ )
            for ( int j = -2 ; j < 2 ; j++ )
                for ( int k = 0 ; k < shade ; k++ ) {
                    Vec3d vec = (_center + _xAxis * ( ( ran() + i ) / 2 ) + _yAxis * ( ( ran() + j ) / 2 )) - collision->_GetHit();
                    double distance = vec._ModuleSquare();
                    bool isShade = false;
                    for ( Object* temp = head ; temp; temp = temp->_GetNext() ) {
                        Collision* tempCollision = temp->_Collide(collision->_GetHit(), vec);
                        if ( tempCollision&& tempCollision->_GetDistance() < distance ) {
                            isShade = true;
                            break;
                        }
                    }
                    if (!isShade) color += _CalnIrradiance(collision , vec , NULL );
                }

        color /= (16.0 * shade);
        return color;
    }

//    Photon _EmitPhoton() {
//        Photon photon;
//        photon._power = _color / _color._Power();
//        photon._center = _center + _xAxis * ( ran() * 2 - 1 ) + _yAxis * ( ran() * 2 - 1 );
//        photon._direction._RandomVec();
//        return  photon;
//    }

    // IO
    void _Input( string var , stringstream& fin ) {
        if ( var == "O=" ) fin >> _center;
        if ( var == "Dx=" ) fin >> _xAxis;
        if ( var == "Dy=" ) fin >> _yAxis;
        Light::_Input( var , fin );
    }
private:
    Vec3d _center , _xAxis , _yAxis;
};


#endif //RAYTRACING_AREALIGHT_H
