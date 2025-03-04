// Global variables from the CPU
uniform mat4 u_model;
uniform mat4 u_viewprojection;

// Variables to pass to the fragment shader
varying vec2 v_uv;
varying vec3 v_final_color;

// Uniforms for lighting
uniform int u_is_first_entity_rendered;
uniform vec3 u_material_Ka;
uniform vec3 u_material_Kd;
uniform vec3 u_material_Ks;
uniform vec3 u_light_position;
uniform vec3 u_light_intensity;
uniform vec3 u_ambient_intensity;
uniform float u_shininess;
uniform vec3 u_camera_position;

void main()
{
    v_uv = gl_MultiTexCoord0.xy;


    vec3 world_position = (u_model * vec4(gl_Vertex.xyz, 1.0)).xyz;


    vec3 world_normal = normalize((u_model * vec4(gl_Normal.xyz, 0.0)).xyz);


    vec3 ambient = u_material_Ka * u_ambient_intensity;
    vec3 intensity_at_point = u_light_intensity / pow(distance(u_light_position, world_position), 2.0);

    vec3 L = normalize(u_light_position - world_position);
    vec3 N = normalize(world_normal);
    vec3 diffuse = intensity_at_point * u_material_Kd * max(dot(L, N), 0.0);

    vec3 R = normalize(reflect(-L, N));
    vec3 V = normalize(u_camera_position - world_position);
    vec3 specular = intensity_at_point * u_material_Ks * pow(max(dot(R, V), 0.0), u_shininess);

    if (u_is_first_entity_rendered == 0) {
        v_final_color = ambient + diffuse + specular;
    } else {
        v_final_color = diffuse + specular;
    }

    gl_Position = u_viewprojection * vec4(world_position, 1.0);
}
