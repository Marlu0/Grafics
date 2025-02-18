varying vec2 v_uv;

void main()
{
    // Number of squares along axis
    float N = 20.0;

    // Compute the UV coordinates scaled by the number of squares
    float x = floor(v_uv.x * N);
    float y = floor(v_uv.y * N);

    // Alternate colors using mod()
    float checker = mod(x + y, 2.0);

    // Set color based on checker value (black or white)
    vec3 color = vec3(checker);

    gl_FragColor = vec4(color, 1.0);
}
