varying vec2 v_uv;

uniform float u_aspect_ratio;

void main()
{
    vec2 scaled_uv = v_uv * vec2(u_aspect_ratio, 1.0); // Scale the x-coordinate by the aspect ratio

    vec3 color = mix(vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), scaled_uv.x);

    gl_FragColor = vec4(color, 1.0);
}