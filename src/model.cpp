#include "model.hpp"

// constructor
Model::Model(char *path)
{
    load_model(path);
}

void Model::draw(Shader &shader)
{
    for(unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].draw(shader);
    }
}