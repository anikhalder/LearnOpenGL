#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;

// uniform mixValue coming from the program
uniform float mixValue;

void main()
{
    //FragColor = mix(texture2D(texture1, TexCoord), texture2D(texture2, TexCoord), 0.2);
    FragColor = mix(texture2D(texture1, TexCoord),
                    texture2D(texture2, vec2(TexCoord.s, TexCoord.t)),
                    mixValue);
}
