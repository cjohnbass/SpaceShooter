/* 
SimpleFragment.glsl

Fragment shader with color input and output.

Mike Barnes
8/16/2014
*/

# version 330 core
// Represents all parameters for a single light source
struct Light {
  vec3 position;
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float attenuation;
  bool on;
};

in vec3 position;
in vec3 normal;
in vec4 color;
in vec2 fTextCoord;


// Texture information
uniform bool IsTexture;
uniform sampler2D Texture;
uniform bool ruber;
uniform Light RuberLight;
uniform Light AmbientLight;
uniform Light u_headLight;



// The position of the viewer in world space
uniform vec3 u_viewPosition;

out vec4 fragColor;

vec4 light(Light light)
{
	if (!light.on){
    return vec4(0, 0, 0, 1);
	}
	
	vec3 dirOfLight;
	dirOfLight = light.position - position;
	
	float distOfLight = length(dirOfLight);
	dirOfLight = normalize(dirOfLight);	
	
	vec3 ambient = light.ambient;
	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);
	
	if(ruber){		
		diffuse = light.diffuse * max(0, dot(-normal, dirOfLight));
	}else{			
		diffuse = light.diffuse * max(0, dot(normal, dirOfLight));		
	} 	

	return vec4(ambient + diffuse, 1) * color;
}

void main() {
	// Check if the fragment has texture
	if (IsTexture) {
	// If so, apply the texture
	fragColor = texture(Texture, fTextCoord); 
	}
	else{
	vec4 tempColor = vec4(0, 0, 0, 0);
	tempColor += light(RuberLight); // Light from Ruber
	tempColor += light(AmbientLight); // Light from Ruber

	fragColor = tempColor;
	}
  }
  
  