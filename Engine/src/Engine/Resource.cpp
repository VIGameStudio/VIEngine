#include "Engine/Resource.hpp"

#include "Engine/Log.hpp"
#include "Engine/ResourceData.hpp"

#include <fstream>
#include <sstream>
#include <unordered_map>

namespace Gaun
{
    std::string ResourceManager::GetFilePath(const std::string& filename)
    {
        return ASSETS_PATH + filename;
    }
}

namespace Gaun
{
    template<>
    bool ResourceData<ShaderData>::LoadImpl(const std::string& filename, ShaderData& data)
    {
        std::ifstream is(ResourceManager::GetFilePath(filename));
        if (is.fail())
            return false;

        std::stringstream ss;
        ss << is.rdbuf();

        data.text = ss.str();
        return true;
    }
}

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Gaun
{
    template<>
    bool ResourceData<TextureData>::LoadImpl(const std::string& filename, TextureData& data)
    {
        stbi_set_flip_vertically_on_load(true);
        auto pData = stbi_load(ResourceManager::GetFilePath(filename).c_str(), &data.width, &data.height, &data.channels, 4);
        if (pData == nullptr)
            return false;

        data.pixels.resize((i64)data.width * (i64)data.height * 4);
        memcpy(data.pixels.data(), pData, data.pixels.size());
        stbi_image_free(pData);
        
        return true;
    }
}

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Gaun
{
    static glm::mat4 aiMatrix4x4_to_glmmat4(const aiMatrix4x4& m)
    {
        glm::mat4 mm = glm::mat4(1);
        for (std::size_t i = 0; i < 4; ++i)
            for (std::size_t j = 0; j < 4; ++j)
                mm[i][j] = m[i][j];
        return mm;
    }

    static void MeshDataLoadImpl_AssimpLoadNode(const aiScene* pScene, const aiNode* pNode, ModelData& data)
    {
        for (std::size_t i = 0; i < pNode->mNumChildren; ++i)
        {
            auto pChild = pNode->mChildren[i];
            MeshDataLoadImpl_AssimpLoadNode(pScene, pChild, data);
        }

        if (pNode->mNumMeshes == 0)
            return;

        for (std::size_t m = 0; m < pNode->mNumMeshes; ++m)
        {
            auto pMesh = pScene->mMeshes[pNode->mMeshes[m]];
            ModelData::Mesh mesh;

            if (pScene->HasMaterials())
            {
                mesh.materialIdx = pMesh->mMaterialIndex;
            }

            // Extract vertex data
            mesh.vertices.resize(pMesh->mNumVertices);
            for (std::size_t v = 0; v < pMesh->mNumVertices; ++v)
            {
                auto& vert = mesh.vertices[v];
                
                const auto& p = pMesh->mVertices[v];
                vert.position = glm::vec4(p.x, p.y, p.z, 1.0f);

                if (pMesh->HasNormals())
                {
                    const auto& n = pMesh->mNormals[v];
                    vert.normal = glm::vec4(n.x, n.y, n.z, 1.0f);
                }

                if (pMesh->HasTangentsAndBitangents())
                {
                    const auto& t = pMesh->mTangents[v];
                    const auto& bt = pMesh->mBitangents[v];
                    vert.tangent = glm::vec4(t.x, t.y, t.z, 1.0f);
                    vert.bitangent = glm::vec4(bt.x, bt.y, bt.z, 0.0f);
                }

                if (pMesh->HasTextureCoords(0))
                {
                    const auto& tc = pMesh->mTextureCoords[0][v];
                    vert.texcoord0 = glm::vec2(tc.x, tc.y);
                }

                if (pMesh->HasVertexColors(0))
                {
                    const auto& c = pMesh->mColors[0][v];
                    vert.position.w = c.r;
                    vert.normal.w = c.g;
                    vert.tangent.w = c.b;
                    vert.bitangent.w = c.a;
                }
            }

            // Extract bone data
            if (pMesh->HasBones())
            {
                for (std::size_t b = 0; b < pMesh->mNumBones; ++b)
                {
                    const auto pBone = pMesh->mBones[b];

                    ModelData::Bone bone;
                    bone.idx = b;
                    bone.offset = aiMatrix4x4_to_glmmat4(pBone->mOffsetMatrix);
                    mesh.armature.bones.insert(std::make_pair(pBone->mName.C_Str(), bone));

                    for (std::size_t w = 0; w < pBone->mNumWeights; w++)
                    {
                        const auto& weight = pBone->mWeights[w];
                        auto& vert = mesh.vertices[weight.mVertexId];

                        for (std::size_t i = 0; i < 4; ++i)
                        {
                            if (vert.bones[i] != -1 && vert.weights[i] > weight.mWeight)
                                continue;

                            vert.bones[i] = (f32)b;
                            vert.weights[i] = weight.mWeight;
                            break;
                        }
                    }
                }
            }

            mesh.indices.resize(pMesh->mNumFaces * (std::size_t)3);
            for (std::size_t f = 0; f < pMesh->mNumFaces; ++f)
            {
                const auto& face = pMesh->mFaces[f];
                assert(face.mNumIndices == 3 && "Only triangles are supported!");
                
                for (std::size_t i = 0; i < face.mNumIndices; ++i)
                {
                    auto idx = face.mIndices[i];
                    mesh.indices[f * 3 + i] = idx;
                }
            }

            data.meshes.emplace_back(mesh);
        }
    }

    static void Assimp_CalcHierarchy(const aiScene* pScene, std::unordered_map<std::string, glm::mat4>& matrices, f32 animTime, aiNode* pNode, const aiMatrix4x4& parent)
    {
        /*std::string name = pNode->mName.C_Str();

        const aiAnimation* pAnimation = pScene->mAnimations[0];

        aiMatrix4x4 transform = pNode->mTransformation;

        const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, name);

        if (pNodeAnim)
        {
            // Interpolate scaling and generate scaling transformation matrix
            aiVector3D scale;
            //CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
            //Matrix4f ScalingM;
            //ScalingM.InitScaleTransform(Scaling.x, Scaling.y, Scaling.z);

            // Interpolate rotation and generate rotation transformation matrix
            aiQuaternion rotation;
            //CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
            //Matrix4f RotationM = Matrix4f(RotationQ.GetMatrix());

            // Interpolate translation and generate translation transformation matrix
            aiVector3D translation;
            //CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
            //Matrix4f TranslationM;
            //TranslationM.InitTranslationTransform(Translation.x, Translation.y, Translation.z);

            // Combine the above transformations
            //transform = TranslationM * RotationM * ScalingM;
        }

        aiMatrix4x4 global = parent * transform;

        matrices[name] = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;

        for (std::size_t i = 0; i < pNode->mNumChildren; i++) {
            Assimp_CalcHierarchy(pScene, matrices, animTime, pNode->mChildren[i], global);
        }*/
    }

    static bool MeshDataLoadImpl_Assimp(const std::string& filename, ModelData& data)
    {
        // Create an instance of the Importer class
        Assimp::Importer importer;
        importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, 65536);

        // And have it read the given file with some example postprocessing
        // Usually - if speed is not the most important aspect for you - you'll
        // probably to request more postprocessing than we do in this example.
        const aiScene* pScene = importer.ReadFile(ResourceManager::GetFilePath(filename),
            aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_JoinIdenticalVertices |
            aiProcess_GenSmoothNormals |
            aiProcess_SplitLargeMeshes |
            //aiProcess_ValidateDataStructure |
            aiProcess_ImproveCacheLocality |
            aiProcess_RemoveRedundantMaterials |
            aiProcess_OptimizeMeshes |
            //aiProcess_OptimizeGraph |
            aiProcess_SortByPType);

        // If the import failed, report it
        if (pScene == nullptr)
        {
            Log::Error(importer.GetErrorString());
            return false;
        }

        // Extract material data
        if (pScene->HasMaterials())
        {
            data.materials.resize(pScene->mNumMaterials);
            for (std::size_t i = 0; i < pScene->mNumMaterials; ++i)
            {
                const auto pMaterial = pScene->mMaterials[i];
                auto& material = data.materials[i];

                aiColor3D c(1.f, 1.f, 1.f);
                if (pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, c) == aiReturn_SUCCESS)
                    material.diffuse = glm::vec4(c.r, c.g, c.b, 1.0f);

                // Diffuse maps
                int numTextures = pMaterial->GetTextureCount(aiTextureType_DIFFUSE);
                aiString textureName;

                if (numTextures > 0)
                {
                    if (pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), textureName) == aiReturn_SUCCESS)
                        material.diffuseMap = Resource<TextureData>(textureName.data);

                    if (pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), textureName) == aiReturn_SUCCESS)
                        material.normalMap = Resource<TextureData>(textureName.data);

                    if (pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_EMISSIVE, 0), textureName) == aiReturn_SUCCESS)
                        material.emmisionMap = Resource<TextureData>(textureName.data);

                    if (pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_METALNESS, 0), textureName) == aiReturn_SUCCESS)
                        material.metallicMap = Resource<TextureData>(textureName.data);

                    if (pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE_ROUGHNESS, 0), textureName) == aiReturn_SUCCESS)
                        material.roughnessMap = Resource<TextureData>(textureName.data);

                    if (pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_AMBIENT_OCCLUSION, 0), textureName) == aiReturn_SUCCESS)
                        material.aoMap = Resource<TextureData>(textureName.data);
                }
            }
        }

        if (pScene->HasAnimations())
        {
            aiMatrix4x4 rootInvMatrix = pScene->mRootNode->mTransformation;
            rootInvMatrix = rootInvMatrix.Inverse();

            data.animations.resize(pScene->mNumAnimations);
            for (std::size_t i = 0; i < pScene->mNumAnimations; ++i)
            {
                const auto pAnim = pScene->mAnimations[i];
                auto& anim = data.animations[i];

                for (std::size_t j = 0; j < pAnim->mNumChannels; ++j)
                {
                    const auto pChannel = pAnim->mChannels[j];
                    std::vector<glm::mat4> keyframes;
                    anim.keyframes.insert(std::make_pair(pChannel->mNodeName.C_Str(), keyframes));
                }

                /*f32 time = 0.f;
                bool baking = true;
                while (baking)
                {
                    f32 tps = pAnim->mTicksPerSecond != 0 ? pAnim->mTicksPerSecond : 25.0f;
                    f32 ticks = time * tps;
                    f32 animTime = glm::mod(ticks, (f32)pAnim->mDuration);

                    std::unordered_map<std::string, glm::mat4> matrices;
                    Assimp_CalcHierarchy(pScene, matrices, animTime, pScene->mRootNode, pScene->mRootNode->mTransformation);
                    
                    for (const auto& it : matrices)
                    {
                        auto it2 = anim.keyframes.find(it.first);
                        assert(it2 != anim.keyframes.end() && "");
                        it2->second.emplace_back(it.second);
                    }
                }*/
            }
        }

        // Now we can access the file's contents.
        auto pNode = pScene->mRootNode;
        MeshDataLoadImpl_AssimpLoadNode(pScene, pNode, data);

        // We're done. Everything will be cleaned up by the importer destructor
        return true;
    }

    template<>
    bool ResourceData<ModelData>::LoadImpl(const std::string& filename, ModelData& data)
    {
        return MeshDataLoadImpl_Assimp(filename, data);
    }
}