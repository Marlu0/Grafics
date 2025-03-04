varying vec2 v_uv;
uniform int u_is_first_entity_rendered;
uniform int u_is_colortext_activated;
uniform int u_is_specular_activated;
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

    vec4 total;
	// Render WITH ambient light
    vec3 ambient = u_material_Ka * u_ambient_intensity;
    vec3 intensity_at_point = u_light_intensity / pow(distance(u_light_position, v_world_position), 2.0);

    vec3 L = normalize(u_light_position - v_world_position);
    vec3 N = normalize(v_world_normal);
    vec3 diffuse = intensity_at_point * u_material_Kd * clamp(dot(L, N), 0.0, 1.0);

    vec3 R = normalize(reflect(-L, N));
    vec3 V = normalize(u_camera_position - v_world_position);
    vec3 specular = intensity_at_point * u_material_Ks * pow(clamp(dot(R, V), 0.0, 1.0), u_shininess);

    if (u_is_first_entity_rendered == 0) {
        if (u_is_colortext_activated == 0 && u_is_specular_activated == 0){
            total = vec4(ambient, 1.0);
        } else if (u_is_colortext_activated == 0){
            total = vec4(ambient + specular, 1.0);
        } else if (u_is_specular_activated == 0){
            total = vec4(ambient + diffuse, 1.0);
        } else {
            total = vec4(ambient + diffuse + specular, 1.0);
        }
        
    } else {
        total = vec4(diffuse + specular, 1.0);
    }
    
    vec4 color = texture2D(u_texture, v_uv);

    gl_FragColor = color*total;
    
}
