varying vec2 v_uv;

uniform float u_aspect_ratio;

void main()
{
    float N = 10.0; 
    float cycleSize = 1.0 / N;
    float stripeWidth = cycleSize / 5.0;

    vec2 scaled_uv = v_uv * vec2(u_aspect_ratio, 1.0); // Scale the x-coordinate by the aspect ratio

    float diffusionStrength = 0.1;
    float edgeSoftness = diffusionStrength * cycleSize; // Keeps blur proportional

    // Compute vertical red stripes (centered blend)
    float xEdge = mod(scaled_uv.x, cycleSize);
    float red = clamp((stripeWidth - min(xEdge, cycleSize - xEdge)) / edgeSoftness, 0.0, 1.0);

    // Compute horizontal blue stripes (centered blend)
    float yEdge = mod(scaled_uv.y, cycleSize);
    float blue = clamp((stripeWidth - min(yEdge, cycleSize - yEdge)) / edgeSoftness, 0.0, 1.0);

    // Apply linear interpolation using mix()
    float redLerp = mix(0.0, 1.0, red);
    float blueLerp = mix(0.0, 1.0, blue);

    vec3 color = vec3(redLerp, 0.0, blueLerp);

    gl_FragColor = vec4(color, 1.0);
}
