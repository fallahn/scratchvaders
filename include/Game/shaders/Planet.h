///spherically distorts and crops a tiled map to resemble a planet
#ifndef PLANET_MAP_H_
#define PLANET_MAP_H_

const char planetMap[] = 
	"#version 120\n"

	"uniform sampler2D baseTexture;"
	"uniform sampler2D normalTexture;"
	"uniform sampler2D globeNormalTexture;"
	"uniform float radius = 0.5;" //normalised text coord - so 0.5 would be half texture width
	"uniform vec2 position = vec2(0.0);" //in texture coords

	"const vec2 centre = vec2(0.5);"
	"uniform vec3 lightPos = vec3(0.5, 0.2, 0.3);"
	"const vec3 lightSpecColour = vec3(0.2, 0.19, 0.18);"
	"const float lightSpecPower = 0.08;" //smaller is more glossy

	"void main(void)"
	"{"		
		"float alpha;" //calc alpha to make texture appear circular
		"float distance = distance(centre, gl_TexCoord[0].xy);"
		"if(distance > radius) alpha = 0.0;"
		"else alpha = 1.0;"

		//globe normal
		"vec3 globeNormalColour = texture2D(globeNormalTexture, gl_TexCoord[0].xy).rgb;"
		"globeNormalColour.g = 1.0 - globeNormalColour.g;"

		//offset coord for distortion
		"vec2 offset = position;"
		"offset.y = 1.0 - position.y;"
		"vec2 coord = gl_TexCoord[0].xy + offset;"

		"coord += globeNormalColour.rg;"
		"coord += (centre + offset);"
		
		//get pixels at coord
		"vec3 colour = texture2D(baseTexture, coord).rgb;"
		"vec3 normalColour = texture2D(normalTexture, coord).rgb;"

		//calc normal / spec / shading
		//"normalColour.g = 1.0 - normalColour.g;" //invert G
		"vec3 normal = normalize(normalColour * 2.0 - 1.0);"

		//calculate the colour intensity based on normal and add specular to pixels facing light
		"vec3 lightDir = normalize(vec3(position, 0.3));"
		"float colourIntensity = max(dot(normal, lightDir), 0.0);"
		"vec3 specular = vec3(0.0);"
		"vec3 diffuse = vec3(0.0);"
		"if(colourIntensity > 0.0)"
		"{"
			//vector half way between light and view direction
			"vec3 halfVec = normalize(lightPos + vec3(0.5, 0.5, 0.5));" //fixed 2D view, so view is centred
			"float specModifier = max(dot(normal, halfVec), 0.0);"
			"specular = pow(specModifier, lightSpecPower) * lightSpecColour;"
			"diffuse = lightSpecColour * colour * colourIntensity;"
		"}"
		
		//globe shadow
		"vec3 globeNormal = normalize(globeNormalColour * 2.0 - 1.0);"
		"colourIntensity = max(dot(globeNormal, lightDir), 0.0);"
		"colour += (vec3(0.4, 0.4, 0.38) * colourIntensity);"
		"colour *= (colourIntensity);"
		
		//output
		"gl_FragColor = vec4(clamp(specular + diffuse + colour, 0.0, 1.0), alpha);"

	"}";


#endif //PLANET_MAP_H_