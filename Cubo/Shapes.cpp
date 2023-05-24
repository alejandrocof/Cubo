#include "Shapes.hpp"

std::list< std::function<void()> > Shapes::s_2D;
std::list< std::function<void()> > Shapes::s_3D;
std::list< std::function<void()> > Updates::updateObject;
//std::vector< std::function<void()> > Shapes::s_2D;
//std::vector< std::function<void()> > Shapes::s_3D;
//std::vector< std::function<void()> > Updates::updateObject;

float Shapes::xmin=std::numeric_limits<float>::max();
float Shapes::xmax=-std::numeric_limits<float>::max();
float Shapes::ymin=std::numeric_limits<float>::max();
float Shapes::ymax=-std::numeric_limits<float>::max();

double Shapes::xtmin=std::numeric_limits<double>::max();
double Shapes::xtmax=-std::numeric_limits<double>::max();
double Shapes::ytmin=std::numeric_limits<double>::max();
double Shapes::ytmax=-std::numeric_limits<double>::max();

/*
void Shapes3D::draw(){
  for ( std::vector< std::function<void()> >::iterator it = s.begin() ; it != s.end(); ++it)
    (*it)();
  
}*/


