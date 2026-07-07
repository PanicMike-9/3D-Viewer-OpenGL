#include "model.hpp"

// constructor
Model::Model(char *path)
{
    load_model(path);
}

// Draw meshes
void Model::draw(Shader &shader)
{
    for(unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].draw(shader); // calls draw() from Mesh class
    }
}

// load models and handle model loading error
void Model::load_model(std::string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
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
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(process_mesh(mesh, scene));
    }

    // process all child nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        process_node(node->mChildren[i], scene);
    }
}

Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
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
        if(mesh->mTextureCoords)
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
    }
}