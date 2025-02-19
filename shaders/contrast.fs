// Receive the uvs interpolated from the vertex
varying vec2 v_uv; 

// Receive the texture as a sampler2D from our application
uniform sampler2D u_texture; 

void main()
{
	// Fetch sampler
	vec4 texture_color = texture2D( u_texture, v_uv );

	float contrast_factor = 6.0;

    float grayscale = dot(texture_color.rgb, vec3(0.299, 0.587, 0.114));
    
    float contrast_value = (grayscale - 0.5) * contrast_factor + 0.5;

    contrast_value = clamp(contrast_value, 0.0, 1.0);
	

    // Assign the color to the pixel
	gl_FragColor = vec4(contrast_value, contrast_value, contrast_value, texture_color.a);
}