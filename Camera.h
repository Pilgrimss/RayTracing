//
// Created by 王颖 on 23/06/2017.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H


//
// Created by 王颖 on 21/06/2017.
//

#ifndef PPM_CAMERA_H
#define PPM_CAMERA_H

#include "Vec3.h"
#include "Color.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#define STD_DOF_SAMPLE  64 // sample in  depth of field
#define STD_APERTURE  0 // render dof when (aperture > 0)
#define STD_FOCAL_LEN  1
#define STD_LENS_WIDTH  0.88
#define STD_LENS_HEIGHT  0.88
#define STD_IMAGE_WIDTH  420
#define STD_IMAGE_HEIGHT  420
#define STD_SHADE_QUALITY  4 // calculate shade in Mento Carlo
#define STD_MONTEANGLE  20 // calculate diffused reflection in Mento Carlo
#define STD_MAX_HITPOINTS  4000000 //
#define STD_ITERATIONS  5000 // PPM
#define STD_DAMP  0.7 // damp
#define STD_MAX_PHOTONS  500000 //
#define STD_EMIT_PHOTONS  100000 //
#define STD_SAMPLE_PHOTONS  10 //
#define STD_SAMPLE_R  0.1 // radius when sample

class Camera {
public:
    Camera() {
        _center = Vec3d( 0 , 0 , 0 );
        _normal = Vec3d( 0 , 1 , 0 );
        _dofSample = STD_DOF_SAMPLE;
        _aperture = STD_APERTURE;
        _focalLength = STD_FOCAL_LEN;
        _lenW = STD_LENS_WIDTH;
        _lenH = STD_LENS_HEIGHT;
        _imageW = STD_IMAGE_WIDTH;
        _imageH = STD_IMAGE_HEIGHT;
        _shade= STD_SHADE_QUALITY;
        _monteAngle = STD_MONTEANGLE;
        _maxHits = STD_MAX_HITPOINTS;
        _iterations = STD_ITERATIONS;
        _damp = STD_DAMP;
        _maxPhotons = STD_MAX_PHOTONS;
        _emitPhotons = STD_EMIT_PHOTONS;
        _samplePhotons = STD_SAMPLE_PHOTONS;
        _sampleR = STD_SAMPLE_R;
        _color = NULL;
    }
    ~Camera() {
        if (_color) {
            for ( int i = 0 ; i < _imageH ; i++ )
                delete[] _color[i];
            delete[] _color;
        }
    }

    // Get & Set
    Vec3d _GetCenter() { return _center; }
    Vec3d _GetNormal() { return _normal; }
    double _GetDofSample() { return _dofSample; }
    double _GetAperture() { return _aperture; }
    double _GetFocalLen() { return _focalLength; }
    int _GetImageW() { return _imageW; }
    int _GetImageH() { return _imageH; }
    Color _GetColor(int i, int j) { return _color[i][j]; }
    void _SetColor( int i , int j , Color color ) { _color[i][j] = color; }
    double _GetShade() { return _shade; }
    double _GetMonteAngle() { return _monteAngle; }
    int _GetMaxHits() { return _maxHits; }
    int _GetIterations() { return _iterations; }
    double _GetDamp() { return _damp; }
    int _GetMaxPhotons() { return _maxPhotons; }
    int _GetEmitPhotons() { return _emitPhotons; }
    int _GetSamplePhotons() { return _samplePhotons; }
    double _GetSampleR() { return _sampleR; }

    Vec3d _Emit( double i , double j ) {
        return _normal + _yAxis * _lenH * (i / (_imageH - 1) - 0.5) + _xAxis * _lenW * (j / (_imageW - 1) - 0.5);
    }
    void _DofEmit(double i, double j, Vec3d* dofOrigin, Vec3d* dofDirection) {
        Vec3d focalPoint = _center + _Emit(i, j) * _focalLength;
        double x, y;
        do {
            x = ran() * 2 - 1;
            y = ran() * 2 - 1;
        } while (x * x + y * y > 1);
        *dofOrigin = _center + _xAxis * _aperture * x + _yAxis * _aperture * y;
        *dofDirection = focalPoint - *dofOrigin;
        dofDirection->_Normalize();
    }

    void _Init() {
        _normal._Normalize();
        _xAxis = _normal._VerticalVec();
        _yAxis = _normal._Cross(_xAxis);

        _color = new Color*[_imageH];
        for ( int i = 0 ; i < _imageH ; i++ )
            _color[i] = new Color[_imageW];
    }


    // IO
    void _Input( string var , stringstream& fin ) {
        if ( var == "O=" ) fin >> _center;
        if ( var == "N=" ) fin >> _normal;
        if ( var == "dofSample=" ) fin >> _dofSample;
        if ( var == "aperture=" ) fin >> _aperture;
        if ( var == "focalLen=" ) fin >> _focalLength;
        if ( var == "lens_W=" ) fin >> _lenW;
        if ( var == "lens_H=" ) fin >> _lenH;
        if ( var == "image_W=" ) fin >> _imageW;
        if ( var == "image_H=" ) fin >> _imageH;
        if ( var == "shade_quality=" ) fin >> _shade;
        if ( var == "drefl_quality=" ) fin >> _monteAngle;
        if ( var == "max_hitpoints=" ) fin >> _maxHits;
        if ( var == "iterations=" ) fin >> _iterations;
        if ( var == "reduction=" ) fin >> _damp;
        if ( var == "max_photons=" ) fin >> _maxPhotons;
        if ( var == "emit_photons=" ) fin >> _emitPhotons;
        if ( var == "sample_photons=" ) fin >> _samplePhotons;
        if ( var == "sample_dist=" ) fin >> _sampleR;
    }

    cv::Mat _Output() {
        cv::Mat image(_imageH,_imageW, CV_8UC3, cvScalar(0,0,255));
        for ( int i = 0 ; i < _imageH ; i++ )
            for ( int j = 0 ; j < _imageW ; j++ ) {
                Color color =  _color[i][j] *= 255;
                cout << color << endl;
                image.at<cv::Vec3b>(i,j) = cv::Vec3b((uchar)color[0],(uchar)color[1],(uchar)color[2]);
            }
        imshow("test", image);
        cv::waitKey();
    }

private:
    Vec3d _center , _normal , _xAxis , _yAxis;
    double _dofSample;
    double _aperture;
    double _focalLength;
    double _lenW , _lenH;
    int _imageW , _imageH;
    Color** _color;
    double _shade;
    double _monteAngle;
    int _maxHits;
    int _iterations;
    double _damp;
    int _maxPhotons;
    int _emitPhotons;
    int _samplePhotons;
    double _sampleR;
};



#endif //PPM_CAMERA_H



#endif //RAYTRACING_CAMERA_H
