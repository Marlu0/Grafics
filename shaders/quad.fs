varying vec2 v_uv;

void main()
{

    vec3 color = mix(vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), v_uv.y);

    gl_FragColor = vec4(color, 1.0);
}