varying vec2 v_uv;

uniform sampler2D u_texture;  // Texture to sample the image

void main()
{
    // Define the size of the blur
    float blurSize = 0.0025;  // Adjust this value to control the amount of smoothing

    // Sample the current pixel and neighboring pixels (8 neighbors + center)
    vec3 color_center = texture2D(u_texture, v_uv).rgb;
    vec3 color_left = texture2D(u_texture, v_uv + vec2(-blurSize, 0.0)).rgb;
    vec3 color_right = texture2D(u_texture, v_uv + vec2(blurSize, 0.0)).rgb;
    vec3 color_up = texture2D(u_texture, v_uv + vec2(0.0, -blurSize)).rgb;
    vec3 color_down = texture2D(u_texture, v_uv + vec2(0.0, blurSize)).rgb;
    vec3 color_up_left = texture2D(u_texture, v_uv + vec2(-blurSize, -blurSize)).rgb;
    vec3 color_up_right = texture2D(u_texture, v_uv + vec2(blurSize, -blurSize)).rgb;
    vec3 color_down_left = texture2D(u_texture, v_uv + vec2(-blurSize, blurSize)).rgb;
    vec3 color_down_right = texture2D(u_texture, v_uv + vec2(blurSize, blurSize)).rgb;

    // Mix the current pixel with its 8 neighboring pixels to smooth the image
    vec3 mixed_color = color_center * 0.125;  // Start with the center pixel
    mixed_color += color_left * 0.125;         // Add left pixel
    mixed_color += color_right * 0.125;        // Add right pixel
    mixed_color += color_up * 0.125;           // Add up pixel
    mixed_color += color_down * 0.125;         // Add down pixel
    mixed_color += color_up_left * 0.125;      // Add upper-left diagonal pixel
    mixed_color += color_up_right * 0.125;     // Add upper-right diagonal pixel
    mixed_color += color_down_left * 0.125;    // Add lower-left diagonal pixel
    mixed_color += color_down_right * 0.125;   // Add lower-right diagonal pixel

    // Output the smoothed color
    gl_FragColor = vec4(mixed_color, 1.0);
}
