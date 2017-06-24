//
// Created by 王颖 on 23/06/2017.
//

#ifndef RAYTRACING_SCENE_H
#define RAYTRACING_SCENE_H

#include "Light.h"
#include "PointLight.h"
#include "AreaLight.h"
#include "Camera.h"
#include "Object.h"
#include "Sphere.h"
#include "Rectangle.h"
#include "Plane.h"
#include "Collision.h"
//#include "Triangle.h"
class Scene {
public:
    // Construction & Destruction
    Scene():_object(NULL),_light(NULL){
        _color = Color();
        _camera = new Camera;
    }
    ~Scene() {
        while (_object ) {
            Object* _next = _object->_GetNext();
            delete _object;
            _object = _next;
        }

        while (_light) {
            Light* _next = _light->_GetNext();
            delete _light;
            _light = _next;
        }

        delete _camera;
    }

    // Set & Get
    Object* _GetObject() { return _object; }
    Light* _GetLight() { return _light; }
    Camera* _GetCamera() { return _camera; }
//    Color _GetColor(Collision* collision,int *hash) {
//        double shade = _camera->_GetShade();
//        Object* object = collision->_GetObject();
//        Color color = Color();
//        for (Light * temp = _light; temp; temp = temp->_GetNext()){
//            color += _light->_GetIrradiance(collision,_object,shade,hash);
//        }
//        color *= _color * _object->_GetMaterial()->_GetDiff();
//        return color;
//    }

    void _PushObject( Object * object) {
        if (object) {
            object->_SetNext( _object );
            _object = object;
        }
    }
    void _PushLight( Light* light) {
        if(light) {
            light ->_SetNext(_light);
            _light = light;
        }
    }

    Collision* _NearestCollide( Vec3d origin , Vec3d direction ) {
        Collision* nearest = NULL;
        for ( Object* temp = _object ; temp; temp = temp->_GetNext() ) {
            Collision* collision = temp -> _Collide(origin,direction);
            if(collision) {
                if (!nearest || collision->_GetDistance() < nearest->_GetDistance())
                    nearest = collision;
            }
        }

        return nearest;
    }
    LightCollision* _NearestLight( Vec3d origin , Vec3d direction ) {
        LightCollision* nearest = NULL;
        if(_light) {
            nearest = new LightCollision;
            nearest = _light->_Collide(origin, direction);
        }
        for (Light* temp = _light->_GetNext(); temp; temp = temp->_GetNext()) {
            LightCollision* lightCollision = temp->_Collide(origin, direction);
            if (lightCollision && lightCollision->_GetDistance() < nearest->_GetDistance())
                nearest = lightCollision;
        }
        return nearest;
    }

    // IO

    void _CreateScene( string file ) {
        if ( file == "" ) return;
        srand( 2017 - 6 - 23);
        ifstream fin( file.c_str() );

        string obj;
        while ( fin >> obj ) {
            Object* object = NULL;
            Light* new_light = NULL;

            if ( obj == "primitive" ) {
                string type; fin >> type;
                if ( type == "sphere" ) object = new Sphere;
                if ( type == "plane" ) object = new Plane;
                if ( type == "rectangle" ) object = new Rectangle;
                //if ( type == "triangle" ) object = new Triangle;
                //if ( type == "polyhedron" ) object = new Polyhedron;
                _PushObject( object );
            } else
            if ( obj == "light" ) {
                string type; fin >> type;
                if ( type == "point" ) new_light = new PointLight;
                if ( type == "area" ) new_light = new AreaLight;
                _PushLight( new_light );
            } else
            if ( obj != "background" && obj != "camera" ) continue;

            fin.ignore( 1024 , '\n' );

            string order;
            while ( std::getline( fin , order , '\n' ) ) {
                stringstream fin2( order );
                string var; fin2 >> var;
                if ( var == "end" ) {
                    if (obj == "primitive" && object) object->_PreProcess();
                    break;
                }

                if ( obj == "background") fin2 >> _color;
                if ( obj == "primitive" && object) object->_Input( var , fin2 );
                if ( obj == "light" && new_light) new_light->_Input( var , fin2 );
                if ( obj == "camera" ) _camera->_Input( var , fin2 );
            }
        }
        _camera->_Init();
    }

private:
    Object* _object;
    Light* _light;
    Camera* _camera;
    Color _color;

};



#endif //RAYTRACING_SCENE_H
