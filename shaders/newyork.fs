varying vec2 v_uv;

void main()
{
    // Number of stripes (adjust for spacing)
    float N = 10.0; 

    // Compute full cycle size (stripe + gap)
    float cycleSize = 1.0 / N;

    // Stripe width is 1/3 of the full cycle (1 part stripe, 2 parts black space)
    float stripeWidth = cycleSize / 3.0;

    // Compute vertical red stripes (1 if inside stripe region, 0 otherwise)
    float red = step(mod(v_uv.x, cycleSize), stripeWidth);

    // Compute horizontal blue stripes (1 if inside stripe region, 0 otherwise)
    float blue = step(mod(v_uv.y, cycleSize), stripeWidth);

    // Combine red and blue
    vec3 color = vec3(red, 0.0, blue);

    gl_FragColor = vec4(color, 1.0);
}
