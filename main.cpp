#include <iostream>

#include "Raytracing.h"
using namespace std;
class Complex
{
public:
    friend ostream& operator << (ostream& output ,Complex&c) //声明重载运算符“<<”
    {
        output<<"("<<c.real<<"+"<<c.imag<<"i)";
        return output;
    }

    friend istream& operator >> (istream&input,Complex&c) //声明重载运算符“>>”
    {
        cout<<"input real part and imaginary part of complex number:";
        input>>c.real>>c.imag;
        return input;
    }
private:
    double real;
    double imag;
};

void test(){
    string file = "./test.txt";
    ifstream fin( file.c_str() );
    string obj;
    Complex c;
    while(fin >> obj){
        fin>>c;
        cout<<"c="<<c<<endl;
    }

}

void run(){
    Raytracing* raytracing = new Raytracing;
    raytracing->_SetInput("./scene.txt");
    raytracing->_SetOutput("./picture.jpg");
    raytracing->_Rendering();
}
int main() {
    srand48(13);
    //test();
    run();
    return 0;
}




