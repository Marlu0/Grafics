varying vec2 v_uv;

void main()
{
    // Compute distance from the center (0.5, 0.5)
    float dist = distance(v_uv, vec2(0.5, 0.5));

    // Map distance to grayscale value (0 at the center, 1 at edges)
    vec3 color = vec3(dist);

    gl_FragColor = vec4(color, 1.0);
}
