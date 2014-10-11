#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform highp float u_miny;
uniform highp float u_maxy;

void main()
{
    vec4 normalColor = texture2D(u_texture, v_texCoord);

    if (v_texCoord.y > u_miny && v_texCoord.y < u_maxy)
    {
        gl_FragColor = normalColor;
    }
    else
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
}
