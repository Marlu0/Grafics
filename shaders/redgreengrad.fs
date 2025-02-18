varying vec2 v_uv;

void main()
{
    // Define colors at each corner
    vec3 color_top_left = vec3(0.0, 1.0, 0.0);  // Green
    vec3 color_top_right = vec3(1.0, 1.0, 0.0); // Yellow
    vec3 color_bottom_left = vec3(0.0, 0.0, 0.0); // Black
    vec3 color_bottom_right = vec3(1.0, 0.0, 0.0); // Red

    // Interpolate between bottom colors (black & red)
    vec3 bottom_mix = mix(color_bottom_left, color_bottom_right, v_uv.x);
    
    // Interpolate between top colors (green & yellow)
    vec3 top_mix = mix(color_top_left, color_top_right, v_uv.x);

    // Final interpolated gradient
    vec3 final_color = mix(bottom_mix, top_mix, v_uv.y);

    // --- SUBTLE GRID BUMP EFFECT ---

    // Grid resolution (higher values = denser bumps)
    float grid_size = 20.0;

    // Use sine waves to create soft bumps along rows & columns
    float column_bump = 0.45 + 0.5 * sin(v_uv.x * grid_size * 3.14159);
    float row_bump = 0.45 + 0.5 * sin(v_uv.y * grid_size * 3.14159);

    // Blend the row and column bumps smoothly
    float bump = (column_bump * row_bump) * 0.15; // Small bump effect

    // Darken the color slightly to create the bump illusion
    final_color *= (1.0 - bump); 

    gl_FragColor = vec4(final_color, 1.0);
}
