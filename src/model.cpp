#include "model.hpp"

// constructor
Model::Model(char *path)
{
    load_model(path);
}