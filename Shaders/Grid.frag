//#version 310 core
//Jaguar Supported versions are: 1.10, 1.20, 1.30, 1.00 ES, 3.00 ES, and 3.10 ES
//uniform float uLineWidth;
//uniform vec4 uColor;
//uniform float uBlendFactor; //1.5..2.5
varying vec2 vLineCenter;

uniform vec4 gridColor;

//https://developer.nvidia.com/gpugems/GPUGems2/gpugems2_chapter22.html
//https://vitaliburkov.wordpress.com/2016/09/17/simple-and-fast-high-quality-antialiased-lines-with-opengl/

void main()
{
    float uLineWidth=8.0;
    float uBlendFactor=1.5;

    vec4 col = gridColor;
    //col.w=1.0;
    //float d = sqrt(vLineCenter.x*vLineCenter.x+vLineCenter.y*vLineCenter.y);

//    float d = length(vLineCenter-gl_FragCoord.xy);
//    float w = uLineWidth;
//    float r;
//    if (d>w)
//        col.w = 0.0;
//    else
//        col.w *= pow(float((w-d)/w), uBlendFactor);


    //col.w *= pow(float((w-d)/w), uBlendFactor);

    //float dd = sin( length(gl_FragCoord.xy) );
    //float dd = sin(length(vLineCenter-gl_FragCoord.xy));
    //col=vec4(dd,dd,dd,1.0);
    //gl_FragColor = col;
//    col.r=length(vLineCenter);
//    col.g=length(vLineCenter);
//    col.b=length(vLineCenter);
//    col.r=vLineCenter.x;

//    gl_FragColor=col;
    gl_FragColor=vec4(0.0, 0.0, 0.0,1.0);

    //vec4 col = r;
//    double d = length(vLineCenter-gl_FragCoord.xy);
//    double w = uLineWidth;
//    if (d>w)
//        col.a = 0.0;
//    else
//        col.a *= pow(float((w-d)/w), uBlendFactor);
//    gl_FragColor = col;

    //gl_FragColor = vec4(r, 0.0, 0.0,1.0);
    //gl_FragColor = gridColor;
    //gl_FragColor = vec4(col.www,1.0);

//    gl_FragColor = vec4(vLineCenter.xy,0,1.0);
//    if(vLineCenter.x<0.0 || vLineCenter.y<0.0)
//        gl_FragColor = vec4(1.0, 1.0, 1.0,1.0);
//    if(vLineCenter.x>1.0 || vLineCenter.y>1.0)
//        gl_FragColor = vec4(0.0, 0.0, 1.0,1.0);

}

//uniform float uLineWidth;
//uniform vec4 uColor;
//uniform float uBlendFactor; //1.5..2.5
//varying vec2 vLineCenter;
//void main(void)
//{
//      vec4 col = uColor;
//      double d = length(vLineCenter-gl_FragCoord.xy);
//      double w = uLineWidth;
//      if (d>w)
//        col.w = 0;
//      else
//        col.w *= pow(float((w-d)/w), uBlendFactor);
//      gl_FragColor = col;
//};
