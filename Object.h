//
// Created by 王颖 on 23/06/2017.
//

#ifndef RAYTRACING_OBJECT_H
#define RAYTRACING_OBJECT_H

#include "Material.h"
class Collision;
class Object {
public:

    //Construction & Destruction
    Object():_sample(rand()),_next(NULL){
        _material = new Material;
    }
    Object(const Object& object){
        *this = object;
        _material = object._material;
    }
    ~Object(){delete _material;}

    // Get & Set
    void _SetSample(int sample) { _sample = sample; }
    int _GetSample() { return _sample; }
    Material* _GetMaterial() { return _material; }
    Material* _SetMaterial(Material* material) { _material = material; return _material;}
    Object* _GetNext() { return _next; }
    Object* _SetNext( Object* next ) { _next = next; return _next;}

    // Virtual API
    virtual void _PreProcess() {}
    virtual Collision* _Collide( Vec3d ray_0 , Vec3d ray_V )= 0;
    virtual Color _GetTexture(Vec3d C) = 0;

    virtual void _Input( string var, stringstream& fin){
        _material->_Input( var , fin );
    }



protected:
    int _sample;
    Material* _material;
    Object* _next;
};



#endif //RAYTRACING_OBJECT_H
