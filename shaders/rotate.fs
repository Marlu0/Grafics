// Receive the uvs interpolated from the vertex
varying vec2 v_uv; 
uniform int trans_direction;
// Receive the texture as a sampler2D from our application
uniform sampler2D u_texture; 

void main()
{
	// Fetch sampler
    vec4 texture_color;
    
    if(trans_direction == 1){
        texture_color = texture2D( u_texture, vec2(v_uv.y, v_uv.x) );
    }else if(trans_direction == 2){
        texture_color = texture2D( u_texture, vec2(v_uv.x,1.0 - v_uv.y) );
    }else if(trans_direction == 3){
        
        texture_color = texture2D( u_texture, vec2(v_uv.y, 1.0 - v_uv.x) );
    }else if(trans_direction == 4){
        texture_color = texture2D( u_texture, vec2(v_uv.x, v_uv.y) );
    }

    // Assign the color to the pixel
	gl_FragColor = texture_color;
}
