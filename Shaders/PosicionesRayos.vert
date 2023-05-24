varying vec3 pos_uvw;
varying vec3 direccion;
uniform vec4 sizeVR;
//uniform mat4 matrixVR;


void main()
{	
 pos_uvw = gl_Vertex.xyz+0.5;
 //vec4 Vertex=gl_Vertex;
 vec4 Vertex=gl_Vertex*sizeVR;
//vec4 Vertex=gl_Vertex*vec4(0.666666666,0.633333333,0.233334111,1.0);
//direccion = (gl_ModelViewMatrix*Vertex*gl_ModelViewMatrix).xyz;
 direccion = (gl_ModelViewMatrix*Vertex*gl_ModelViewMatrix/sizeVR).xyz;
 //direccion = gl_ModelViewMatrix*Vertex*gl_ModelViewMatrix/sizeVR;
//  direccion = (gl_ModelViewMatrix*Vertex*gl_ModelViewMatrix/vec4(0.666666666,0.633333333,0.233334111,1.0)).xyz;
 gl_Position = gl_ModelViewProjectionMatrix * Vertex;
}
