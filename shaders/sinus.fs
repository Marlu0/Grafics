varying vec2 v_uv;

uniform float u_aspect_ratio;

void main()
{
    float wave_pattern = step(0.5 + 0.25 * sin(v_uv.x * 2*3.1415), v_uv.y);

    vec3 gradient_color = mix(vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), distance(wave_pattern, v_uv.y));

    gl_FragColor = vec4(gradient_color, 1.0);
}
