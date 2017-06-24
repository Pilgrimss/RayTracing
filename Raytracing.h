//
// Created by 王颖 on 23/06/2017.
//

#ifndef RAYTRACING_RAYTRACING_H
#define RAYTRACING_RAYTRACING_H
#define MAX_DREFL_DEP  2
#define MAX_RAYTRACING_DEP  20
#define HASH_FAC  7
#include "Scene.h"
class Raytracing {
public:
    Raytracing() {
        _scene = new Scene;
    }
    ~Raytracing() {}

    void _SetInput( string file ) { _input = file; }
    void _SetOutput( string file ) { _output = file; }
    void _Rendering() {
        _scene->_CreateScene(_input);
        if(!_scene) {
            printf("no scene\n");
            return;
        }
        Camera* camera = _scene->_GetCamera();
        Vec3d origin = camera->_GetCenter();
        Vec3d imageDirection = Vec3d(0,0,1);
        Vec3d cameraDirection = camera->_GetNormal()._Normalize();
        Vec3d realDirection = imageDirection._Cross(cameraDirection);
        int h = camera->_GetImageH(), w = camera->_GetImageW();
        for (int i = 0; i < h; i++){
            for (int j = 0; j < w; j++){
                Vec3d direction = Vec3d(-0.5 + i / ((double)h - 1), -0.5 + j / ((double)w - 1), 1);
                direction = direction._Rotate(realDirection, acos((imageDirection._Dot(cameraDirection))));
                Color temp = _Trace(origin, direction, 0, NULL);
                camera->_SetColor(i,j,temp);
            }
        }
        camera->_Output();
    }
private:
    string _input, _output;
    Scene*_scene;
    //PhotonMap* _photonMap;
    //HitMap* _hitMap;
    //int** _sample;
    Color _Diffuse(Collision* collision , int depth,int* hash) {
        Object* object = collision->_GetObject();

        //Color color =  _scene->_GetColor(collision,hash) * ;


    }
    Color _Reflect(Collision* collision , Vec3d direction , int depth, int* hash){
        //if(refl <= MYEPS)  return Color();
        Object* object = collision->_GetObject();
        Vec3d hit = collision->_GetHit();
        direction = direction.Reflect(collision->_GetNormal());
        Color alpha = object->_GetTexture(hit) * object->_GetMaterial()->_GetRefl();
        Color reflect = _Trace(hit,direction,depth + 1,hash);
        return alpha * reflect;
    }
    Color _Refract(Collision* collision, Vec3d direction, int depth, int*hash) {
        Object* object = collision->_GetObject();
        double ior = object->_GetMaterial()->_GetIor();
        Color absor = object->_GetMaterial()->_GetAbsorb();
        if(!collision->_GetConNormal()) ior = 1/ior;
        direction = direction.Refract(collision->_GetNormal(),ior);
        Vec3d normal = collision->_GetNormal();
        Vec3d hit = collision->_GetHit();
        Color refraction = _Trace(hit,direction,depth + 1,hash);
        return refraction;
    }
    Color _Trace(Vec3d origin, Vec3d direction, int depth,int* hash) {
        if(depth > MAX_RAYTRACING_DEP) return Color();
        Collision* collision = _scene->_NearestCollide(origin,direction);
        if(!collision) return Color();
        LightCollision* lightCollision = _scene->_NearestLight(origin,direction);
        Color color = Color();
        color += _Diffuse(collision,depth,hash);
        color += _Reflect(collision,direction,depth,hash);
        color += _Refract(collision,direction,depth,hash);
        return color;
    }
};


#endif //RAYTRACING_RAYTRACING_H
