#version 450

#include "common.glsl"
#line 5

layout (std140) uniform Camera 
{
    mat4 proj;
    mat4 invProj;
    mat4 view;
    mat4 invView;
    mat4 viewProj;
    mat4 invViewProj;
    vec4 wsPosition;
} camera;

in vsData {
    vec3 wsPosition;
    vec3 wsNormal;
    vec3 wsTangent;
    vec3 wsBitanget;
    vec2 texCoord;
} vsData;


layout (std140) uniform Material 
{
    vec4 color;
    float metalness;
    float roughness;
};

struct light_t
{
    vec4 wsPosition;
    vec4 color;
    float intensity;
} light;


in vec3 outNormal;

out vec4 outColor;

void main()
{
    vec3 n = normalize(vsData.wsNormal);                       // Normal vector
    vec3 v = normalize(camera.wsPosition.xyz - vsData.wsPosition); // View vector

    // Use metallic to find diffuse and specular color.
    vec3 diffuseColor = (1 - metalness) * color.xyz;

    // Default F0 for dielectric is set to 0.04
    vec3 F0 = mix(vec3(0.04f), color.xyz, metalness);
    
    // Precompute constants
    vec3 diffuseBRDF = diffuseColor * (1.0f / PI) ;

    float a = roughness * roughness;
    float aa = a * a;
    float k = pow(roughness + 1.0f, 2);
    float one_k = 1.0f - k;
    vec3 one_F0 = 1.0f - F0;

    light_t light;
    light.wsPosition = vec4(1, 10, 1, 1);
    light.color = vec4(1, 1, 1, 1);
    light.intensity = 100.f;

    vec3 l = normalize(light.wsPosition.xyz - vsData.wsPosition); 
    vec3 h = normalize(v + l); // Half vector

    // Dot products
    float lon = dot(l, n);
    float von = dot(v, n);
    float hon = dot(h, n);
    float voh = dot(h, v);
        
    // Early exit if the fragment is not facing the light
    if(lon < 0.0f)
        return;

    // ///////////// Incomming light /////////////

    float distanceToLight = distance(vsData.wsPosition, light.wsPosition.xyz);
    float falloff = 1 / pow(distanceToLight, 2);
    
    float attenuation = falloff;


    vec3 Li = light.color.xyz * light.intensity * attenuation;
    if (length(Li) < 1e-5)
        return;

    ///////////// BRDF ///////////// from: Real Shading in Unreal Engine 4
    // Specular D
    float specularD = aa / (PI * pow(hon * hon * (aa - 1) + 1, 2));
    
    // Specular G
    float Gv = von / (von * (one_k) + k);
    float Gl = lon / (lon * (one_k) + k);
    float specularG = Gv * Gl;

    // Specular F
    float p = pow(2.0f, (-5.55473f * voh - 6.98316f) * voh);
    vec3 specularF = one_F0 * p + F0;

    // BRDF for specular 
    vec3 specularBRDF = specularD * specularG * specularF / (4.0f * lon * von);
    
    // Rendering equation, drastically simplified
    vec3 finalColor =  Li * (diffuseBRDF * (1.0f - specularBRDF) + specularBRDF) * lon;

    outColor += vec4(finalColor, 1.0f);
}