const float PI = 3.14159265359f;

/******************************************
ID=2
xy(-1,3)
uv(0,2)
|`.
|   `.
|______`. 
|clip  |  `.
|space |     `.
|______|________`.
ID=0           ID=1
xy(-1,-1)      xy(3,-1) 
uv(0,0)        uv(2,0)
******************************************/

vec4 fullscreenTriangle(in int vertID, out vec2 texCoord)
{
    float x = -1.f + float((vertID & 1) << 2);
    float y = -1.f + float((vertID & 2) << 1);
    texCoord.x = (x + 1.f) * 0.5f;
    texCoord.y = (y + 1.f) * 0.5f;
    return vec4(x, y, 0.f, 1.f);
}

vec3 homogenize(in vec4 v)
{
    return (1.0f / v.w) * v.xyz;
}

vec3 getPositionFromDepth(in sampler2D depth, in vec2 uv, in mat4 invProj)
{
    float z = texture(depth, uv).r;

    vec4 ndc = vec4(vec3(uv.x, uv.y, z) * 2.f - 1.f, 1.0f);
    
    vec4 vsPos = invProj * ndc;
    
    return homogenize(vsPos); 
}

vec3 getVsNormal(in sampler2D normals, in vec2 uv, in mat4 view)
{
    return mat3(view) * (texture(normals, uv).xyz * 2.f - 1.f);
}