//
// Created by 王颖 on 23/06/2017.
//

#ifndef RAYTRACING_COLOR_H
#define RAYTRACING_COLOR_H


#include "Vec3.h"

class Color {
public:
    //Construction & Destruction
    Color():_r(0),_g(0),_b(0){}
    Color(ifstream& fin){
        fin >> _r >> _g >> _b;
    }
    Color(double r, double g, double b): _r(r), _g(g), _b(b){}
    ~Color(){};

    // Operators
    const double& operator [] (uint8_t i) const {
        return (&_r)[i];
    }

    double& operator [] (uint8_t i) { return (&_r)[i];
    }

    Color operator + ( const Color& v) const {
        return Color( _r + v._r , _g + v._g , _b + v._b );
    }
    Color operator - ( const Color& v) const {
        return Color( _r - v._r , _g - v._g , _b - v._b );
    }
    Color operator * ( const Color& v) const {
        return Color( _r * v._r , _g * v._g , _b * v._b );
    }
    Color operator * ( const double& k) const {
        return Color( _r * k , _g * k , _b * k );
    }
    Color operator / ( const double& k) const {
        return Color( _r / k , _g / k , _b / k );
    }
    Color& operator += (const Color& v) {
        *this = *this + v;
        return *this;
    }
    Color& operator -= (const Color& v) {
        *this = *this - v;
        return *this;
    }
    Color& operator *= (const Color& v) {
        *this = *this * v;
        return *this;
    }
    Color& operator *= (const double& k) {
        *this = *this * k;
        return *this;
    }
    Color& operator /= (const double& k) {
        *this = *this / k;
        return *this;
    }

    // Parameters

    Color _Confine() {
        _r = min( _r , 1.0 ), _g =  min( _g , 1.0 ) , _b = min( _b , 1.0 ) ;
        return *this;
    }

    Color _Exp() const {
        return Color(exp(_r),exp(_g),exp(_b));
    }

    double _Power() const {
        return (_r + _g + _b) /3;
    }

    double _RgbMax() const{
        return max(_r,max(_g,_b));
    }

    double _IsBlack() const{
        return fabs( _r ) < MYEPS && fabs( _g ) < MYEPS && fabs( _b ) < MYEPS;
    }

    // IO stream
    friend istream & operator >> (istream &is,  Color &v)
    {

        is >> v._r >> v._g >> v._b;
        return is;
    }

    friend ostream & operator << (ostream &os, const Color &v)
    {
        os << "[" << v._r << " " << v._g << " " << v._b << "]";
        return os;
    }

private:
    double _r,_g,_b;
};



#endif //RAYTRACING_COLOR_H
