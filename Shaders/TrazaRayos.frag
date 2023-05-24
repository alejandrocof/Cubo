varying vec3 pos_uvw;
varying vec3 direccion;

uniform sampler3D VolumeData;
uniform sampler1D ColorTransferFunction;
uniform sampler1D AlphaTransferFunction;

float near = 0.5;
float far  = 100.0;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}


void main()
{
 //gl_FragColor = vec4(pos_uvw,1.0);


 float dx=0.005;
 vec3 new_pos=pos_uvw;
 vec3 vector=normalize(direccion)*dx;
 vec4 color_punto=vec4(0.0,0.0,0.0,0.0);
 vec4 color_rayo=vec4(0.0,0.0,0.0,1.0);

 for(int i=0;new_pos.x>=0.0 && new_pos.y>=0.0 && new_pos.z>=0.0 && new_pos.x<=1.0 && new_pos.y<=1.0 && new_pos.z<=1.0 && color_rayo.a>0.0 ;++i)
  {
    color_punto=texture1D(ColorTransferFunction,texture3D(VolumeData,new_pos.xyz).a);
                color_punto.a=texture1D(AlphaTransferFunction,texture3D(VolumeData,new_pos.xyz).a).a;
                //color_punto = vec4(texture1D(ColorTransferFunction,new_pos.x).rgb,texture1D(AlphaTransferFunction,new_pos.x).a);
                color_rayo.rgb+=color_punto.rgb*color_punto.a*color_rayo.a;
                color_rayo.a*=1.0-color_punto.a;
                new_pos+=vector;
                //if(new_pos.z<LinearizeDepth(gl_FragCoord.z)/far)color_rayo.a=1.0;
  }

 gl_FragColor = color_rayo;
 //float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
 //gl_FragColor = vec4(vec3(depth), 0.0);
 //gl_FragColor = vec4(vec3(gl_FragCoord.z), 0.0);


// gl_FragColor = vec4(texture1D(ColorTransferFunction,pos_uvw.x).rgb,1.0-texture1D(AlphaTransferFunction,pos_uvw.x).a);
 //gl_FragColor = vec4(pos_uvw.xxx,0.0);
// gl_FragColor = vec4(texture3D(VolumeData,pos_uvw).aaa, 0.5 );
// gl_FragColor = vec4(pos_uvw.xxx,1.0);


}


