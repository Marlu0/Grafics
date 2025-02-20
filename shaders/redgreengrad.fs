varying vec2 v_uv;

uniform float u_aspect_ratio;

void main()
{
    float N = 12.0;

    vec2 scaled_uv = v_uv * vec2(u_aspect_ratio, 1.0);

    vec2 uvpixel = floor(scaled_uv * N) / N;
    vec3 color = vec3(uvpixel, 0.0);
    
    gl_FragColor = vec4(color, 1.0);
}
