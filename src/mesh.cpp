#include "mesh.hpp"

Mesh::Mesh
(
    std::vector<Vertex> vertices, 
    std::vector<unsigned int> indices, 
    std::vector<Texture> textures
) : vertices{vertices}, indices{indices}, textures{textures}
{
    setup_mesh();
}