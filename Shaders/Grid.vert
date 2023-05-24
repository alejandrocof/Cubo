//uniform vec2 uViewPort; //Width and Height of the viewport
varying vec2 vLineCenter;
attribute vec4 InVertex;  //w will be set to 1.0 automatically
attribute vec2 InTexCoord0;
attribute vec3 InNormal;


void main()
{
    vec2 uViewPort=vec2(1000.0,800.0); //Width and Height of the viewport
//    xmin=-99.3709;
//    xmax=-98.9325;
//    ymin= 19.04268;
//    ymax= 19.59405;
    //vec4 pp = gl_ModelViewProjectionMatrix * gl_Vertex;
    //gl_Position = gl_ModelViewProjectionMatrix * InVertex;

    vec4 pp = gl_ModelViewProjectionMatrix * InVertex;
    gl_Position = pp;
    vec2 vp = uViewPort;
    //vLineCenter = 0.5*(pp.xy + vec2(1.0, 1.0))*vp;
    vLineCenter = 0.5*(pp.xy+vec2(1.0, 1.0))*vp;
    //float f=0.359556;
    //vLineCenter = (0.5/f)*(pp.xy+vec2(f))*vp;
    //vLineCenter = InTexCoord0;
    //vLineCenter = pp.xy/vp;
//    vLineCenter.x = pp.x*vp.x;
//    vLineCenter.y = pp.y*vp.y;
}


//uniform vec2 uViewPort; //Width and Height of the viewport
//varying vec2 vLineCenter;
//void main(void)
//{
//  vec4 pp = gl_ModelViewProjectionMatrix * gl_Vertex;
//  gl_Position = pp;
//  vec2 vp = uViewPort;
//  vLineCenter = 0.5*(pp.xy + vec2(1, 1))*vp;
//};
