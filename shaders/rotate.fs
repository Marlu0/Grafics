// Receive the uvs interpolated from the vertex
varying vec2 v_uv; 

// Receive the texture as a sampler2D from our application
uniform sampler2D u_texture; 
uniform float u_time;

void main()
{
	// Center coordinates on the center (0.5, 0.5)
    vec2 corrected_uv = v_uv - vec2(0.5);
    
    float cosinus = cos(30*u_time*0.0174532925);
    float sinus = sin(30*u_time*0.0174532925);

    mat2 rotation = mat2(cosinus, -sinus, sinus, cosinus);

    vec2 rot_uv = rotation * corrected_uv;

    rot_uv += vec2(0.5);

    vec4 texture_color = texture2D(u_texture, rot_uv);

    // Assign the color to the pixel
	gl_FragColor = texture_color;
}
