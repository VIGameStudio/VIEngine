#ifdef VERT_SHADER
layout (location = 0) in mat4 iVertex;
layout (location = 4) in mat4 iMatrix;
layout (location = 8) in mat4 iMaterial;
layout (location = 12) in uint iDrawId;

layout (location = 0) out vec3 ioPosition;
layout (location = 1) out vec3 ioNormal;
layout (location = 2) out vec3 ioTangent;
layout (location = 3) out vec3 ioColor;
layout (location = 4) out vec2 ioTexCoord;
layout (location = 5) out vec4 ioDiffuse;
layout (location = 6) flat out uint ioDrawId;

layout (location = 0) uniform mat4 uView;
layout (location = 1) uniform mat4 uProjection;

void main()
{
    ioPosition = iVertex[0].xyz;
    //ioBones = iVertex[3].zw;
    gl_Position = uProjection * uView * iMatrix * vec4(ioPosition, 1.0);

    ioNormal = iVertex[1].xyz;
    ioTangent = iVertex[2].xyz;
    ioColor = vec3(iVertex[0].w, iVertex[1].w, iVertex[2].w);
    ioTexCoord = iVertex[3].xy;
    ioDiffuse = iMaterial[0];
    ioDrawId = iDrawId;
}
#endif

#ifdef FRAG_SHADER
precision mediump float;

layout (location = 0) in vec3 ioPosition;
layout (location = 1) in vec3 ioNormal;
layout (location = 2) in vec3 ioTangent;
layout (location = 3) in vec3 ioColor;
layout (location = 4) in vec2 ioTexCoord;
layout (location = 5) in vec4 ioDiffuse;
layout (location = 6) flat in uint ioDrawId;

layout (location = 0) out vec4 oFragColor;

layout (binding = 0) uniform mediump sampler2DArray uTextureArray;

void main()
{
    vec4 diffuseTex = (ioDiffuse.a < 0.0) ? vec4(1) : texture(uTextureArray, vec3(ioTexCoord.x, ioTexCoord.y, ioDiffuse.a));
    vec4 diffuse = vec4(ioColor, 1.0) * vec4(ioDiffuse.rgb, 1.0) * diffuseTex;
    vec3 I = pow(diffuse.rgb, vec3(1.0 / 2.2));
    oFragColor = vec4(I, diffuse.a);
}
#endif