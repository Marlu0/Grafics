varying vec2 v_uv;
uniform int u_is_first_entity_rendered;
uniform vec3 u_material_Ka;
uniform vec3 u_material_Kd;
uniform vec3 u_material_Ks;
uniform vec3 u_light_position;
uniform vec3 u_light_intensity;
uniform vec3 u_ambient_intensity;
uniform float u_shininess;


uniform sampler2D u_texture;

uniform vec3 u_camera_position;

varying vec3 v_world_position;
varying vec3 v_world_normal;




void main()
{
    vec3 ambient;
    vec3 intesity_at_point;
    vec3 L;
    vec3 N;
    vec3 diffuse;
    vec3 R;
    vec3 V;
    vec3 specular;
    vec4 total;
	// Render WITH ambient light
    if(u_is_first_entity_rendered == 0){
         ambient = u_material_Ka*u_ambient_intensity;
         intesity_at_point = u_light_intensity/pow(distance(u_light_position, v_world_position), 2.0);
        
         L = normalize(u_light_position - v_world_position);
         N = normalize(v_world_normal);
         diffuse = intesity_at_point*u_material_Kd*(clamp(dot(L,N), 0.0, 1.0));
        
        R = normalize(reflect(-L, N));
        V = normalize(u_camera_position - v_world_position);
        specular = intesity_at_point * u_material_Ks * pow(clamp(dot(R, V), 0.0, 1.0), u_shininess);
        
         total = vec4(ambient + diffuse + specular, 1);
 
    }
    //Without ambient light
    else if ( u_is_first_entity_rendered == 1){
         L = normalize(u_light_position - v_world_position);
         N = normalize(v_world_normal);
         diffuse = intesity_at_point*u_material_Kd*(clamp(dot(L,N), 0.0, 1.0));
        
        R = normalize(reflect(-L, N));
        V = normalize(u_camera_position - v_world_position);
        specular = intesity_at_point * u_material_Ks * pow(clamp(dot(R, V), 0.0, 1.0), u_shininess);
        
         total = vec4(diffuse + specular, 1.0);
    }
    
    vec4 color = texture2D(u_texture, v_uv);

    gl_FragColor = color*total;
    
}
