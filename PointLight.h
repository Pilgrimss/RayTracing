//
// Created by 王颖 on 24/06/2017.
//

#ifndef RAYTRACING_POINTLIGHT_H
#define RAYTRACING_POINTLIGHT_H

#include "Light.h"
class PointLight : public Light {
    Vec3d _center;
public:
    PointLight() : Light() {}
    Vec3d _GetCenter() { return _center; }

    LightCollision* _Collide( Vec3d origin , Vec3d direction ) {
        LightCollision* lightCollision = new LightCollision;
        lightCollision->_SetLight(this);
        return lightCollision;
    }

    Color _GetIrradiance( Collision* collision , Object* head , double shade , int* hash ) {
        Object* object = collision->_GetObject();
        Vec3d vec = _center - collision->_GetHit();
        double distance = vec._Module();
        for (Object* temp = head ; temp; temp = temp->_GetNext()) {
            Collision* tempCollision = temp->_Collide(collision->_GetHit(), vec);
            if (tempCollision && tempCollision->_GetDistance() < distance ) return Color();
        }
        return _CalnIrradiance( collision , vec , hash );
    }
//
//    Photon EmitPhoton() {
//        Photon photon;
//        photon._power = _color / _color._Power();
//        photon._center = _center;
//        photon._direction._RandomVec();
//        return photon;
//    }

    // IO
    void _Input( string var , stringstream& fin ) {
        if ( var == "O=" ) fin >> _center;
        Light::_Input( var , fin );
    }
};

#endif //RAYTRACING_POINTLIGHT_H
