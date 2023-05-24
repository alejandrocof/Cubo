#ifndef SHAPES_3D_HPP
#define SHAPES_3D_HPP
#include<iostream>
#include<functional>
//#include <vector>
#include <list>
#include <limits>

class Shapes{
  public:
    static void draw2D(){
        for ( std::list< std::function<void()> >::iterator it = s_2D.begin() ; it != s_2D.end(); ++it)
      //  for ( std::vector< std::function<void()> >::iterator it = s_2D.begin() ; it != s_2D.end(); ++it)
        (*it)();
    }
    static void draw3D(){
        for ( std::list< std::function<void()> >::iterator it = s_3D.begin() ; it != s_3D.end(); ++it)
            (*it)();
//        int i=0;
//        for ( std::vector< std::function<void()> >::iterator it = s_3D.begin() ; it != s_3D.end(); ++it){
//            std::cout<<"it["<<i<<"]="<<std::endl;
//            (*it)();
//            i++;
//        }

    }

    template <class Ret, class Fn>
    static void push_back_2D(Ret &&A,Fn &&B){
      s_2D.push_back( std::bind( A , B ) );
    };
    
    template <class Ret, class Fn>
    static void push_back_3D(Ret &&A,Fn &&B){
      s_3D.push_back( std::bind( A , B ) );
    };

    template <class Ret, class Fn>
    static void push_front_3D(Ret &&A,Fn &&B){
      s_3D.push_front( std::bind( A , B ) );
    };
    
    //shapes.push_back( std::bind( &VolumeRendering::draw , vr ) );
    static float xmin;
    static float xmax;
    static float ymin;
    static float ymax;

    static double xtmin;
    static double xtmax;
    static double ytmin;
    static double ytmax;

  private:
    static std::list< std::function<void()> > s_2D;
    static std::list< std::function<void()> > s_3D;
//    static std::vector< std::function<void()> > s_2D;
//    static std::vector< std::function<void()> > s_3D;

};


class Updates{
  public:
    static void update(){
        for ( std::list< std::function<void()> >::iterator it = updateObject.begin() ; it != updateObject.end(); ++it)
    //    for ( std::vector< std::function<void()> >::iterator it = updateObject.begin() ; it != updateObject.end(); ++it)
        (*it)();
    }
    
    template <class Ret, class Fn>
    static void push_back_Updates(Ret &&A,Fn &&B){
      updateObject.push_back( std::bind( A , B ) );
    };
    
    //shapes.push_back( std::bind( &VolumeRendering::draw , vr ) );
    
  private:
    static std::list< std::function<void()> > updateObject;
    //static std::vector< std::function<void()> > updateObject;
};
#endif


