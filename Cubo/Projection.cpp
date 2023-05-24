#include "Projection.hpp"

Projection::Projection(){
    double a = 6378137, f = 1/298.257223563; /* WGS84 */
    geod_init(&geodesical, a, f);
}

void Projection::loadXML(pugi::xml_node_iterator node_it){
    std::cout<<"name:"<<node_it->name()<<std::endl;

    for (pugi::xml_node_iterator it = node_it->begin(); it != node_it->end(); ++it){
        std::cout<<"    <"<<it->name();

        std::string nodeName( it->name() );

        if( nodeName.compare("Proj")==0 ){
            for (pugi::xml_attribute_iterator ait = it->attributes_begin(); ait != it->attributes_end(); ++ait){
                ifTheAttributeIsFoundAssignString(ait, "in", sProjIn);
                ifTheAttributeIsFoundAssignString(ait, "out", sProjOut);
            }
            bProj=true;
        }

        std::cout <<"/>"<< std::endl;
    }

    if( !(projIn = pj_init_plus(sProjIn.c_str())) ){
        cout<<"Input projection parameters error: "<<sProjIn<<endl;
        bProj=false;
    }

    if( !(projOut = pj_init_plus(sProjOut.c_str())) ){
        cout<<"Output projection parameters error: "<<sProjOut<<endl;
        bProj=false;
    }

//    struct geod_geodesic g;
//    struct geod_geodesicline l;
//    double lat[101],lon[101];
//    int i;
//    geod_init(&g, 6378137, 1/298.257223563);
//    geod_inverseline(&l, &g, 40.64, -73.78, 1.36, 103.99, 0);
//    for (i = 0; i <= 100; ++i) {
//      geod_position(&l, i * l.s13 * 0.01, lat + i, lon + i, 0);
//      printf("%.5f %.5f\n", lat[i], lon[i]);
//    }



//    double lat1, lon1, azi1, lat2, lon2, azi2, s12;

//    lat1=11.5;
//    lon1=-91.0;
//    azi1=90.0;
//    s12=4000.0;

//    //geod_direct(&g, lat1, lon1, azi1, s12, &lat2, &lon2, &azi2);
//    cout<<"lat="<<lat2<<" lon="<<lon2<<" azi="<<azi2<<endl;

}

void Projection::geodDirect(double lat1, double lon1, double azi1, double s12, double &lat2, double &lon2, double &azi2){
    geod_direct(&geodesical, lat1, lon1, azi1, s12, &lat2, &lon2, &azi2);
}

void Projection::geodInverse(double lat1, double lon1, double lat2, double lon2, double &s12, double &azi1, double &azi2){
    geod_inverse(&geodesical, lat1, lon1, lat2, lon2, &s12, &azi1, &azi2);
}


int Projection::transform3d(std::vector<double> &xyz){
    if(bProj){

        std::size_t found = sProjIn.find("longlat");
        if (found!=std::string::npos){
            for(unsigned int i=0;i<xyz.size();i+=3){
                xyz[i]*=DEG_TO_RAD;
                xyz[i+1]*=DEG_TO_RAD;
            }

        }
        int q= pj_transform(projIn, projOut, xyz.size()/3, 3, &xyz[0], &xyz[1], NULL);
        if(q){
            cout<<"Error code: "<<q<<endl;
            cout<<"Error message: "<<pj_strerrno(q)<<endl;
        }
        return q;
    }
    return 0;
}

int Projection::transform(std::vector<double> &xy){
    if(bProj){
        std::size_t found = sProjIn.find("longlat");
        if (found!=std::string::npos){
            for(unsigned int i=0;i<xy.size();i+=2){
                xy[i]*=DEG_TO_RAD;
                xy[i+1]*=DEG_TO_RAD;
            }
        }
        int q= pj_transform(projIn, projOut, xy.size()/2, 2, &xy[0], &xy[1], NULL);
        if(q){
            cout<<"Error code: "<<q<<endl;
            cout<<"Error message: "<<pj_strerrno(q)<<endl;
        }
        return q;
    }
    return 0;
}

int Projection::transform(double &x, double &y){
    if(bProj){
        double xy[2];

        std::size_t found = sProjIn.find("longlat");
        if (found!=std::string::npos){
            xy[0]=x*DEG_TO_RAD;
            xy[1]=y*DEG_TO_RAD;
        }
        else{
            xy[0]=x;
            xy[1]=y;
        }

        int q= pj_transform(projIn, projOut, 1,1, &xy[0], &xy[1], NULL);
        x=xy[0];
        y=xy[1];
        if(q){
            cout<<"Error code: "<<q<<endl;
            cout<<"Error message: "<<pj_strerrno(q)<<endl;
        }
        return q;
    }
    return 0;
}

