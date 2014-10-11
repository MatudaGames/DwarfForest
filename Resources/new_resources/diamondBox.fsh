#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform sampler2D u_colorRampTexture;

void main()
{
    vec4 normalColor = texture2D(u_texture, v_texCoord);
    
    vec3 normalColor2 = texture2D(u_colorRampTexture, v_texCoord).rgb;

    gl_FragColor = vec4(normalColor.r * normalColor2.r,
                        normalColor.g * normalColor2.r,
                        normalColor.b * normalColor2.r,
                        normalColor.a * normalColor2.r);
}
