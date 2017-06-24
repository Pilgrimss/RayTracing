//
// Created by 王颖 on 23/06/2017.
//

#ifndef RAYTRACING_MATERIAL_H
#define RAYTRACING_MATERIAL_H


#include "Color.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
class Material{
public:
    Material():_color(),_absorb(), _refl(0),_refr(0),_diff(0),_spec(0),_ior(0),_derefl(0),_texture(NULL),_bumpMap(NULL){};
    ~Material() {
        if(_texture) delete _texture;
        if(_bumpMap) delete _bumpMap;
    }

    //Set & Get
    IplImage* _GetTexture(){return _texture;}
    IplImage* _SetTexture(IplImage* texture){_texture = texture; return _texture;}
    IplImage* _GetBumpMap(){return _bumpMap;}
    IplImage* _SetBumpMap(IplImage* bumpMap){_bumpMap = bumpMap; return _bumpMap;}
    Color _GetColor(){return _color;}
    Color _SetColor(Color color){_color = color;return _color;}
    Color _GetAbsorb(){return _absorb;}
    Color _SetAbsorb(Color absorb){_absorb = absorb; return _absorb;}
    double _SetDiff(double diff){_diff = diff; return _diff;}
    double _GetDiff() {return _diff;}
    double _SetRefl(double refl){_refl = refl; return _refl;}
    double _GetRefl(){return _refl;}
    double _SetRefr(double refr){_refr = refr; return _refr;}
    double _GetRefr(){return _refr;}
    double _SetSpec(double spec){_spec = spec; return _spec;}
    double _GetSpec(){return _spec;}
    double _SetIor(double ior){_ior = ior; return _ior;}
    double _GetIor(){return _ior;}
    double _SetDerefl(double derefl){_derefl = derefl;return _derefl;}
    double _GetDerefl(){return _derefl;}


    double _BRDF(Vec3d ray, Vec3d normal, Vec3d view) {
        double brdf = 0;
        ray._Normalize();
        view._Normalize();

        if (_diff > MYEPS && ray._Dot(normal) > MYEPS)
            brdf += _diff * ray._Dot(normal);
        if (_spec > MYEPS && ray._Dot(-view.Reflect(normal)) > MYEPS)
            brdf += _spec * pow(ray._Dot(-view.Reflect(normal)), 50);
        return brdf;
    }

    Color _GetSmoothColor(double x, double y) {
        if (!_texture) return _color;
        int index_x = max(0, min((int)(x * _texture->height), _texture->height - 1));
        int index_y = max(0, min((int)(y * _texture->width), _texture->width - 1));
        return Color(cvGet2D(_texture, index_x, index_y).val[0] / 255.0, cvGet2D(_texture, index_x, index_y).val[1] / 255.0, cvGet2D(_texture, index_x, index_y).val[2] / 255.0);
    }
    //IO stream
    void _Input( string var , stringstream& fin ) {
        if ( var == "color=" ) fin >> _color;
        if ( var == "absor=" ) fin >> _absorb;
        if ( var == "refl=" ) fin >> _refl;
        if ( var == "refr=" ) fin >> _refr;
        if ( var == "diff=" ) fin >> _diff;
        if ( var == "spec=" ) fin >> _spec;
        if ( var == "drefl=" ) fin >> _derefl;
        if ( var == "rindex=" ) fin >> _ior;
        if ( var == "texture=" ) {
            std::string file;
            fin >> file;
            _texture = new IplImage;
            _texture = cvLoadImage(file.c_str(),1);
        }
        if (var == "bump=") {
            std::string file;
            fin >> file;
            _bumpMap = new IplImage;
            _bumpMap = cvLoadImage(file.c_str(),1);
        }
    }

private:
    Color _color, _absorb;
    double _refl, _refr, _diff, _spec;// radio of reflection, refraction, diffuse, specular
    double _ior;// index of refraction
    double _derefl;
    IplImage* _texture;
    IplImage* _bumpMap;
};


#endif //RAYTRACING_MATERIAL_H
