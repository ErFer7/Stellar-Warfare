uniform sampler2D texture;
uniform float threshold;
uniform float intensity;

void main() {
    vec4 color = texture2D(texture, gl_TexCoord[0].xy);
    if (color.a < threshold) {
        gl_FragColor = color;
    } else {
        vec4 bloom = vec4(0.0);
        for (int i = -4; i <= 4; i++) {
            bloom += texture2D(texture, gl_TexCoord[0].xy + vec2(i, 0.0)) * intensity;
            bloom += texture2D(texture, gl_TexCoord[0].xy + vec2(0.0, i)) * intensity;
        }
        gl_FragColor = color + bloom;
    }
}
