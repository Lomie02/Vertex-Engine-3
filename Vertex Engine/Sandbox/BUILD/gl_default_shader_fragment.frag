#version 330 core
out vec4 FragColor;
uniform vec4 Colour;

uniform sampler2D AlbedoMap;
uniform bool HasAlbedo;

in vec2 TexCoord;

void main()
{
	vec4 finalColour = Colour;

	if(HasAlbedo){
		finalColour = texture(AlbedoMap, TexCoord);
	}
	else{
	
		finalColour = Colour;
	}

    FragColor = finalColour;
}