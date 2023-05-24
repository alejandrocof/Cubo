//Para mejorar el código revisar
//http://www.lighthouse3d.com/tutorials/glsl-tutorial/directional-lights-per-pixel/

varying vec4 diffuse,ambient;
varying vec3 normal,halfVector;

varying vec2 pos2D;
varying vec3 k3D;
uniform float West;
uniform float East;
uniform float South;
uniform float North;
attribute vec4 InVertex;
attribute vec3 InNormal;
attribute vec2 InTexCoord0;
//layout (location = 0) in vec3 position;
//layout (location = 1) in vec2 texCoord;

void main()
{
//    Texcoord = texcoord;
    //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    //TexCoord = gl_MultiTexCoord0;


    //pos2D = (InVertex.xy-vec2(West,South))/vec2(East-West,North-South);
    pos2D = InTexCoord0;

    //k3D = gl_Vertex.zzz;

    //pos2D = (gl_Vertex.xy-vec2(-99.252,19.223))/vec2(0.315789473,0.344827586);


    /* first transform the normal into eye space and
        normalize the result */
        //normal = normalize(gl_NormalMatrix * gl_Normal);
        normal = normalize(gl_NormalMatrix * InNormal);

        /* pass the halfVector to the fragment shader */
        halfVector = gl_LightSource[0].halfVector.xyz;

        /* Compute the diffuse, ambient and globalAmbient terms */
        diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
        ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
        ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;


        //gl_Position = gl_ModelViewProjectionMatrix * InVertex*vec4(0.75,1.5,1,1);
        gl_Position = gl_ModelViewProjectionMatrix * InVertex;

    //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

//void main()
//{
//    vec4 Vertex=gl_Vertex;
//    gl_Position = gl_ModelViewProjectionMatrix * Vertex;
//}





