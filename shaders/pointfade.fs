varying vec2 v_uv;

uniform float u_aspect_ratio;

void main()
{
    vec2 scaled_uv = v_uv * vec2(u_aspect_ratio, 1.0); // Scale the x-coordinate by the aspect ratio
    
    // Compute distance from the center (0.5, 0.5)
    float dist = distance(scaled_uv, vec2(0.5, 0.5));

    // Map distance to grayscale value (0 at the center, 1 at edges)
    vec3 color = vec3(dist);

    gl_FragColor = vec4(color, 1.0);
}
