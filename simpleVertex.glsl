/* 
SimpleVertex.glsl

Vertex shader with position, color, normal and ModelViewProject
input and color output.

Mike Barnes
8/17/2013
*/

# version 330 core

//uniforms
uniform mat4 Model;
uniform mat3 Normal;
uniform mat4 ModelView;
uniform mat4 ModelViewProjection;  // = projection * view * model
uniform vec4 planetColor;  // = projection * view * model
uniform bool planet;  // = projection * view * model


//in
in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec2 vTextCoord;

//out
out vec3 position;
out vec3 normal;
out vec4 color;
out vec2 fTextCoord;
void main() {
	
	position = vec3(ModelViewProjection * vPosition);
	normal = normalize(Normal * vNormal);
	fTextCoord = vTextCoord;
	if(planet){
		color = planetColor;
	}else{
		color = vColor;
	}
	//calculate the location of this fragment (pixel) in world coordinates	
	gl_Position = ModelViewProjection * vPosition;	
  }
  
  