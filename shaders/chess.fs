varying vec2 v_uv;

uniform float u_aspect_ratio;

void main()
{
    // Number of squares along axis
    float N = 20.0;

    vec2 scaled_uv = v_uv * vec2(u_aspect_ratio, 1.0); // Scale the x-coordinate by the aspect ratio

    // Compute the UV coordinates scaled by the number of squares
    float x = floor(scaled_uv.x * N);
    float y = floor(scaled_uv.y * N);

    // Alternate colors using mod()
    float checker = mod(x + y, 2.0);

    // Set color based on checker value (black or white)
    vec3 color = vec3(checker);

    gl_FragColor = vec4(color, 1.0);
}
