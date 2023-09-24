#version 450

uniform float iTime;
uniform vec3 iResolution;    

uniform vec3 skyTop; //vec3(0.0,0.5,0.188)
uniform vec3 skyBot; //vec3(0.22,0.5,1.0)
uniform vec3 sunColor; //vec3(1.0,0.0,0.7)
uniform vec3 hillColor; //vec3(0.0,0.0,0.0)
uniform float sunRadius; //2
uniform float sunSpeed; //1.0

float circleSDF(vec2 p, float r){
    return length(p)-r;
}

out vec4 fragColor;
vec2 fragCoord = gl_FragCoord.xy;

void main()
{
    vec2 uv = fragCoord/iResolution.xy;
    float aspect = iResolution.x / iResolution.y;
    
    float sky = sin(uv.y*0.25+iTime*sunSpeed)*1.0 + 0.5;
    vec3 col = mix(skyBot, skyTop, uv.y+sky);
    
    uv = (2.0*fragCoord-iResolution.xy)/iResolution.y;
    
    float blur = 15.0*aspect/iResolution.x;
    
    vec2 sunPos = uv + vec2(-0.02, 0.6+sin(iTime*sunSpeed)*1.0);
    col = mix(col, sunColor, smoothstep(blur,-blur,circleSDF(sunPos,sunRadius)));
    
    float hills = 0.6 + sin(uv.x*5.0)*0.3;
    hills = step(hills,-uv.y);
    
    col = mix(col, hillColor, hills);
    
    // Output to screen
    fragColor = vec4(col,1.0);
}