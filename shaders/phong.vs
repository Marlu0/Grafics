// Global variables from the CPU
uniform mat4 u_model;
uniform mat4 u_viewprojection;

uniform int u_is_normal_activated;
uniform sampler2D u_normals;
// Variables to pass to the fragment shader
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;


//here create uniforms for all the data we need here

void main()
{	
	v_uv = gl_MultiTexCoord0.xy;

	// Convert local position to world space
	vec3 world_position = (u_model * vec4( gl_Vertex.xyz, 1.0)).xyz;

	// Convert local normal to world space
	vec3 world_normal = (u_model * vec4( gl_Normal.xyz, 0.0)).xyz;

	// Pass them to the fragment shader interpolated
    if (u_is_normal_activated == 1){
        
        vec4 sampledNormal = texture2D(u_normals, v_uv);
    
        float mix_factor = 0.5;
        
        vec3 final_normal = mix(sampledNormal.rgb, world_normal, mix_factor);
        
        v_world_normal = final_normal;

    }else if (u_is_normal_activated == 0 ){
        v_world_normal = world_normal;
    }
	
    v_world_position = world_position;

	// Project the vertex using the model view projection matrix
	gl_Position = u_viewprojection * vec4(world_position, 1.0); //output of the vertex shader
}
