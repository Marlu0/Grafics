varying vec2 v_uv;
uniform float u_aspect_ratio;

uniform sampler2D u_texture; 

void main()
{
    float u_pixel_size = 0.05;
    // Calculate the size of each "pixel" in UV space
    vec2 pixel_size = vec2(u_pixel_size, u_pixel_size * u_aspect_ratio);

    // Get the center of the nearest "pixel"
    vec2 uv = floor(v_uv / pixel_size) * pixel_size + pixel_size * 0.5;

    // Sample the texture at the center of the pixel
    vec4 color = texture2D(u_texture, uv);

    gl_FragColor = color;
}
