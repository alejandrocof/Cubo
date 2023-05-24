#version 330

in vec2 texcoords2D;
uniform vec3 textColor;
uniform sampler2D text;
out vec4 fragColour;


void main(void)
{
    //fragColour = vec4(textColor,texture(text, posTexture2D));

    //fragColour = vec4(textColor,texture(text, texcoords2D).r);
    fragColour = vec4(texcoords2D,0.0,1.0);
    //fragColour = vec4(textColor,1.0);

    //fragColour = vec4(vec3(1.0,0.0,0.0),texture(text, posTexture2D).r);

    //fragColour = vec4(0.0,0.0,1.0,texture(text, vec2(posTexture2D.x,1.0-posTexture2D.y)).r);
    //fragColour = vec4(posTexture2D,0.0,1.0);


//    gl_FragColor=vec4(0.0, 0.0, 0.0,1.0);
}
