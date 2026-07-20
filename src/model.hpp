#pragma once

// basic C++ libs
#include <vector>
#include <string>

#include "shader.hpp"
#include "mesh.hpp"

// assimp headers
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

unsigned int texture_from_file(const char *path, const std::string &directory);

class Model
{
    public:
        Model(char *path);
        void draw(Shader &shader);
    
    private:
        std::vector<Mesh> meshes;
        std::vector<Texture> textures_loaded;
        std::string directory;

        void load_model(std::string path);
        void process_node(aiNode *node, const aiScene *scene);
        Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name);
};