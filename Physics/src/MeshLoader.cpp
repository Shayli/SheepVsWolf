
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "MeshLoader.hpp"
#include "Model3D.hpp"
#include "Model3DBuffer.hpp"

namespace MeshLoader {

bool load(Model3D* ptr, const std::string& file)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file.c_str(), aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals);
    if(!scene) {
        std::cout << importer.GetErrorString() << std::endl;
        return false;
    }

    
    
    for(size_t c = 0; c < scene->mNumMeshes; ++c) {
        Model3DBufferPtr buffer(new Model3DBuffer);
        VertexBuffer vbuffer;
        aiMesh* m = scene->mMeshes[c];
        for(uint32_t i=0;i<m->mNumFaces;i++)
        {
            const aiFace& face = m->mFaces[i];
            for(int j=0;j<3;j++)
            {
                glm::vec2 uv(0,0);
                if(m->mTextureCoords[0] != nullptr) {
                    aiVector3D uvtmp = m->mTextureCoords[0][face.mIndices[j]];
                    uv = glm::vec2(uvtmp.x, 1-uvtmp.y);
                }
                aiVector3D normaltmp = m->mNormals[face.mIndices[j]];
                glm::vec3 normal(normaltmp.x, normaltmp.y, normaltmp.z);
                aiVector3D postmp = m->mVertices[face.mIndices[j]];
                glm::vec3 pos(postmp.x, postmp.y, postmp.z);
                vbuffer.addVertex(pos, normal, uv, {1,1,1,1});
            }
            vbuffer.addTriangle(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
        }

        buffer->loadFromMemory(vbuffer);
        ptr->addModelBuffer(buffer);
    }
    
    return true;
}
}