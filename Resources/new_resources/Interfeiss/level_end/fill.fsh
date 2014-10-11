#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;
uniform sampler2D u_texture;
uniform highp float u_posx;
uniform highp float u_brightness;
uniform highp float u_contrast;

void main()
{
    vec4 normalColor = texture2D(u_texture, v_texCoord);

    if (v_texCoord.x < u_posx)
    {
        if (normalColor.a > 0.2)
        {
        	normalColor.rgb = ((normalColor.rgb - 0.5) * max(u_contrast, 0.0)) + 0.5;
	        normalColor.rgb += u_brightness;
	    }
        
        gl_FragColor = normalColor;
    }
    else
    {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
}
