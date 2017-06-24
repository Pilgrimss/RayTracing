//
// Created by 王颖 on 23/06/2017.
//

#ifndef RAYTRACING_LIGHT_H
#define RAYTRACING_LIGHT_H


#include "Vec3.h"
#include "Color.h"
#include "Object.h"
#include "Collision.h"
//#include "Photon.h"
#define HASH_MOD  10000007

class Light;

class LightCollision {

public:
    //Construction & Destruction
    LightCollision():_light(NULL) {}
    ~LightCollision(){}

    Light* _light;//光源
    double _distance;//和第一个碰到的物体之间的距离
};

class Light{
public:
    //Construction
    Light():_sample(rand()){}
    ~Light(){}
    // Get & Set
    int _GetSample() { return _sample; }
    int _SetSample(int sample) {_sample = sample; return _sample;}
    Color _GetColor() { return _color; }
    Color _SetColor(Color color) {_color = color; return _color;}
    Light* _GetNext() { return _next; }
    Light* _SetNext( Light* next ) { _next = next; return _next;}

    // Virtual API
    virtual Vec3d _GetCenter() = 0;//光源位置
    virtual LightCollision* _Collide( Vec3d origin , Vec3d direction ) = 0;//
    virtual Color _GetIrradiance( Collision* collision , Object* head , double shade, int* hash ) = 0;//计算该光源对于某碰撞点辐出度的贡献
    //virtual Photon _EmitPhoton() = 0;

    // IO
    virtual void _Input( string var, stringstream& fin) {
        if ( var == "color=" )
            fin >> _color;
    }
protected:
    int _sample;
    Color _color;
    Light* _next;
    // 利用BRDF公式计算辅出度
    Color _CalnIrradiance( Collision* collision , Vec3d ray , int* hash ) {
        Object* object =  collision->_GetObject();
        Color color = _color * object->_GetMaterial()->_BRDF(ray, collision->_GetNormal(),-collision->_GetDirection());
        color /= ray._ModuleSquare();
        return color;
    }
};






#endif //RAYTRACING_LIGHT_H
