#pragma once
#include <vector>
#include <string>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "stb_image.h"
#include "Shader.h"
#include "Mesh.h"

class Model {
public:
    Model(const char* path);
    void Draw(Shader& shader);

private:
    /* 模型数据 */
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;//已加载
    /* 函数 */
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
                                         std::string typeName);
    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma);
};

