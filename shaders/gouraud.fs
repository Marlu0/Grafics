varying vec2 v_uv;
varying vec3 v_final_color;

uniform sampler2D u_texture;

void main()
{
    vec4 color = texture2D(u_texture, v_uv);
    gl_FragColor = color * vec4(v_final_color, 1.0);
}
