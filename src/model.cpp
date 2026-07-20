#define STB_IMAGE_IMPLEMENTATION
#include "vendor/stb_image.h"

#include "model.hpp"
// constructor
Model::Model(char *path)
{
    load_model(path);
}

// Draw meshes
void Model::draw(Shader &shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].draw(shader); // calls draw() from Mesh class
    }
}

// load models and handle model loading error
void Model::load_model(std::string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << import.GetErrorString() << '\n';
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    process_node(scene->mRootNode, scene);
}

// process to retrive mesh indices and for each children node
void Model::process_node(aiNode *node, const aiScene *scene)
{
    // process all nodes meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene));
    }

    // process all child nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        process_node(node->mChildren[i], scene);
    }
}

// process mesh data and create a Mesh object
Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        glm::vec3 vector; // temp vector transfer data to Assimp

        // process vertices
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;

        vertex.position = vector;

        // process normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;

        vertex.normal = vector;

        // process texture coordinate
        if (mesh->mTextureCoords)
        {
            glm::vec2 tex_vec;
            tex_vec.x = mesh->mTextureCoords[0][i].x;
            tex_vec.y = mesh->mTextureCoords[0][i].y;

            vertex.tex_coords = tex_vec;
        }
        else 
        {
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);

        // process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        // process material
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            // create diffuse map, load the material texture and insert into textures vector
            std::vector<Texture> diffuse_map = load_material_textures(material, 
                                                                      aiTextureType_DIFFUSE, 
                                                                      "texture_diffuse");
            textures.insert(textures.end(), diffuse_map.begin(), diffuse_map.end());

            // create specular map, load the material texture and insert into textures vector
            std::vector<Texture> specular_map = load_material_textures(material, 
                                                                       aiTextureType_SPECULAR, 
                                                                       "texture_specular");
            textures.insert(textures.end(), specular_map.begin(), specular_map.end());
        }
    }

    return Mesh(vertices, indices, textures);
}

// read the texture data from the file path
unsigned int texture_from_file(const char *path, const std::string &directory)
{
    std::string file_name = std::string(path);
    file_name = directory + '/' + file_name;

    unsigned int texture_id;
    glGenTextures(1, &texture_id);

    int width;
    int height;
    int nr_components;

    unsigned char *data = stbi_load(file_name.c_str(), &width, &height, &nr_components, 0);

    if (data)
    {
        GLenum format;
        if (nr_components == 1)
        {
            format = GL_RED;
        }
        else if (nr_components == 3)
        {
            format = GL_RGB;
        }
        else if (nr_components == 4)
        {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << '\n';
        stbi_image_free(data);
    }

    return texture_id;
}

// load and generate texture objects from a material
std::vector<Texture> Model::load_material_textures(aiMaterial *mat, aiTextureType type, std::string type_name)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString assimp_str;
        mat->GetTexture(type, i, &assimp_str);
        bool skip = false;

        // load and check texture data
        for (unsigned int j = 0; j < textures_loaded.size(); ++j)
        {
            if (std::strcmp(textures_loaded[j].path.data(), assimp_str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        // skip if texture data is already loaded
        if (!skip)
        {
            Texture texture;
            texture.id = texture_from_file(assimp_str.C_Str(), directory);
            texture.type = type_name;
            texture.path = assimp_str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }

    return textures;
}