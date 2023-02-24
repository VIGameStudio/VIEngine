#ifdef VERT_SHADER
layout (location = 0) in mat4 iVertex;
layout (location = 4) in mat4 iVertexExtra;
layout (location = 8) in mat4 iData;
layout (location = 12) in mat4 iDataExtra;

layout (location = 0) out vec3 ioPosition;
layout (location = 1) out vec3 ioNormal;
layout (location = 2) out vec3 ioTangent;
layout (location = 3) out vec3 ioBitangent;
layout (location = 4) out vec4 ioColor;
layout (location = 5) out vec2 ioTexCoord0;
layout (location = 6) out vec4 ioDiffuse;
layout (location = 7) out vec3 ioDiffuseMaps;
layout (location = 8) out vec3 ioPbr;
layout (location = 9) out vec3 ioPbrMaps;

layout (location = 0) uniform mat4 uView;
layout (location = 1) uniform mat4 uProjection;

void main()
{
    vec4 position = iData * vec4(iVertex[0].xyz, 1.0);

    ioPosition = position.xyz;
    ioNormal = inverse(transpose(mat3(iData))) * iVertex[1].xyz;
    ioTangent = iVertex[2].xyz;
    ioBitangent = iVertex[3].xyz;
    ioColor = vec4(iVertex[0].w, iVertex[1].w, iVertex[2].w, iVertex[3].w);

    ioTexCoord0 = iVertexExtra[0].xy;

    ioDiffuse = iDataExtra[0];
    ioDiffuseMaps = iDataExtra[1].xyz;
    ioPbr = iDataExtra[2].xyz;
    ioPbrMaps = iDataExtra[3].xyz;

    gl_Position = uProjection * uView * position;
}
#endif

#ifdef FRAG_SHADER
precision mediump float;

layout (location = 0) in vec3 ioPosition;
layout (location = 1) in vec3 ioNormal;
layout (location = 2) in vec3 ioTangent;
layout (location = 3) in vec3 ioBitangent;
layout (location = 4) in vec4 ioColor;
layout (location = 5) in vec2 ioTexCoord0;
layout (location = 6) in vec4 ioDiffuse;
layout (location = 7) in vec3 ioDiffuseMaps;
layout (location = 8) in vec3 ioPbr;
layout (location = 9) in vec3 ioPbrMaps;

layout (location = 0) out vec4 oFragColor;

layout (location = 2) uniform vec3 uCameraPosition;
layout (location = 3) uniform vec3 uDirLightDirection;
layout (location = 4) uniform vec3 uDirLightColor;
layout (location = 5) uniform float uDirLightIntensity;

layout (binding = 1) uniform mediump sampler2DArray uTextures;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);
vec3 Irradiance(vec3 albedo, float metallic, float roughness, float ao, vec3 F0, vec3 N, vec3 V, vec3 L, vec3 R);

void main()
{
    vec4 diffuseTex = (ioDiffuseMaps.x < 0.0) ? vec4(1) : texture(uTextures, vec3(ioTexCoord0.x, ioTexCoord0.y, ioDiffuseMaps.x));
    vec4 diffuse = ioColor * ioDiffuse * diffuseTex;

    vec3 albedo = diffuse.rgb;
    float metallic = ioPbr.x;
    float roughness = ioPbr.y;
    float ao = ioPbr.z;

    // Calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)  
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    vec3 N = normalize(ioNormal);
    vec3 V = normalize(uCameraPosition - ioPosition);

    // Reflectance equation
    vec3 Lo = vec3(0.0);

    // Directional light irradiance
    vec3 Ld = -uDirLightDirection;
    vec3 Rd = uDirLightColor * uDirLightIntensity;
    Lo += Irradiance(albedo, metallic, roughness, ao, F0, N, V, Ld, Rd);

    // Ambient lighting
    vec3 ambient = vec3(0.03) * albedo * ao;

    // Final irradiance
    vec3 I = ambient + Lo;

    // HDR tonemapping
    I = I / (I + vec3(1.0));

    // Gamma correct
    I = pow(I, vec3(1.0 / 2.2));

    // Final color
    oFragColor = vec4(I, diffuse.a);
}

// Based on: // https://learnopengl.com/code_viewer_gh.php?code=src/6.pbr/1.2.lighting_textured/1.2.pbr.fs
const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 Irradiance(vec3 albedo, float metallic, float roughness, float ao, vec3 F0, vec3 N, vec3 V, vec3 L, vec3 R)
{
    // Calculate light radiance
    vec3 H = normalize(V + L);

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
    vec3 specular = numerator / denominator;

    // kS is equal to Fresnel
    vec3 kS = F;
    
    // For energy conservation, the diffuse and specular light can't
    // be above 1.0 (unless the surface emits light); to preserve this
    // relationship the diffuse component (kD) should equal 1.0 - kS.
    vec3 kD = vec3(1.0) - kS;
    
    // Multiply kD by the inverse metalness such that only non-metals 
    // have diffuse lighting, or a linear blend if partly metal (pure metals
    // have no diffuse light).
    kD *= 1.0 - metallic;

    // Scale light by NdotL
    float NdotL = max(dot(N, L), 0.0);

    // Outgoing radiance (irradiance)
    vec3 I = (kD * albedo / PI + specular) * R * NdotL;  // Note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    
    return I;
}
#endif