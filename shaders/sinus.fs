varying vec2 v_uv;

uniform float u_aspect_ratio;

void main()
{
    vec2 scaled_uv = v_uv * vec2(u_aspect_ratio, 1.0); // Scale the x-coordinate by the aspect ratio

    // Define wave parameters
    float wave_amplitude = 0.25;  // Adjust for wave height
    float wave_y = 0.5 + wave_amplitude * sin(2.0 * 3.14159 * scaled_uv.x); // Sine wave function

    // Compute gradient factor based on distance from the wave crest
    float gradient_factor = abs(scaled_uv.y - wave_y) / (1.0 - wave_amplitude);

    // Define colors
    vec3 green = vec3(0.0, 1.0, 0.0);
    vec3 black = vec3(0.0, 0.0, 0.0);

    // Interpolate between green (at the crest) and black (at the top/bottom)
    vec3 color = mix(green, black, gradient_factor);

    gl_FragColor = vec4(color, 1.0);
}
