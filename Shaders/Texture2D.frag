//#version 150 core

varying vec4 diffuse,ambient;
varying vec3 normal,halfVector;

//out vec4 outColor;
varying vec2 pos2D;
varying vec3 k3D;
uniform int remove;
uniform int isovalue_N;
uniform float isovalue_width;
uniform int is3D;

uniform sampler2D Texture2D;
uniform sampler1D ColorTransferFunction;
uniform sampler1D AlphaTransferFunction;
uniform sampler2D TextureMask2D;

void main()
{

//    float min_s=sin(radians(36.25));
//    float max_s=sin(radians(70.75));
//    float s=(sin(radians(36.25+(70.75-36.25)*pos2D.y))-min_s)/(max_s-min_s);
    //float lambda=texture2D(Texture2D,vec2(pos2D.x,s)).a;
    float lambda=texture2D(Texture2D,pos2D).a;


    gl_FragColor = texture1D(ColorTransferFunction,lambda);



//    vec3 n,halfV;//,lightDir;
//    float NdotL,NdotHV;
//    //lightDir = vec3(gl_LightSource[0].position);
//    n = abs(normalize(normal));
//    NdotL = dot(n,gl_LightSource[0].position.xyz);

//    vec4 color = ambient;
//    color += gl_FragColor * gl_LightSource[0].diffuse * NdotL;
//    halfV = normalize(halfVector);
//    NdotHV = max(dot(n,halfV),0.0);
//    color += gl_FrontMaterial.specular *
//            gl_LightSource[0].specular *
//            pow(NdotHV, gl_FrontMaterial.shininess);
//    gl_FragColor = color;
    float alpha=texture1D(AlphaTransferFunction,lambda).a*texture2D(TextureMask2D,pos2D).a;
    gl_FragColor.a = alpha;


//    gl_FragColor.rg=InTexCoord0.xy;
//    gl_FragColor.b=0.0;

//    gl_FragColor.rg = gl_FragCoord.xy;

//    if(pos2D.x<0.5){
//            gl_FragColor=vec4(0,0,1,1);
//        }

//    if(gl_FragCoord.y<0.5){
//        gl_FragColor=vec4(0,0,1,1);
//    }
//    else{
//        gl_FragColor=vec4(1,0,0,1);
//    }

//    //float alpha=texture1D(AlphaTransferFunction,lambda).a;

////    float lambda=texture2D(Texture2D,pos2D).a;
////    float alpha=texture1D(AlphaTransferFunction,lambda).a*texture2D(TextureMask2D,pos2D).a;
//    //float alpha=texture1D(AlphaTransferFunction,lambda).a;


//    if(remove>0){
//            if(lambda>=0.00392156862745098039){
//                float beta=(lambda-0.00392156862745098039)/0.99607843137254901961;
//                gl_FragColor = texture1D(ColorTransferFunction,beta);
//            }
//            else{
//                gl_FragColor = vec4(0.5,0.5,0.5, 0.0);
//                alpha=0.0;
//            }
//        }
//    else{
//        gl_FragColor = texture1D(ColorTransferFunction,lambda);
//        //gl_FragColor = mix(vec4(0.4,0.5,0.8, 0.0),texture1D(ColorTransferFunction,lambda),texture2D(TextureMask2D,pos2D).a);
//    }

//    if(isovalue_N>0){
//        float f  = abs(fract (lambda * float(isovalue_N))-0.5);
//        float df = fwidth(lambda * float(isovalue_N) );
//        float g = smoothstep(-isovalue_width*df,isovalue_width*df , f);
//        gl_FragColor *= g*g;
//    }
///*
//    if(is3D==1){
//        vec3 n,halfV;//,lightDir;
//        float NdotL,NdotHV;
//        //lightDir = vec3(gl_LightSource[0].position);
//        n = abs(normalize(normal));
//        NdotL = dot(n,gl_LightSource[0].position.xyz);

//        vec4 color = ambient;
//        color += gl_FragColor * gl_LightSource[0].diffuse * NdotL;
//        halfV = normalize(halfVector);
//        NdotHV = max(dot(n,halfV),0.0);
//        color += gl_FrontMaterial.specular *
//                gl_LightSource[0].specular *
//                pow(NdotHV, gl_FrontMaterial.shininess);
//        gl_FragColor = color;
//    }
//*/
//    gl_FragColor.a = alpha;
}
