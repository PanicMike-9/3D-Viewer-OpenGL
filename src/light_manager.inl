#pragma once

inline int LightManager::add_point_light(const PointLight& light)
{
    if (point_lights.size() >= MAX_POINT_LIGHTS)
        return -1;

    int id = next_point_light_id++;
    point_lights.emplace_back(ManagedPointLight{id, light});
    return id;
}

// during point light removal order in the array is maintained
// using point_lights.erase(), but can be expensive for larger sizes
// as .erase() is O(n)
inline bool LightManager::remove_point_light_ordered(int id)
{
    auto find_id = std::find_if(point_lights.begin(), point_lights.end(), 
    [id](const ManagedPointLight& managed) { return managed.id == id; });

    if (find_id == point_lights.end())
        return false;

    point_lights.erase(find_id);
    return true;
}

// unordered removals are better supported for larger sizes
// as pop_back() is O(1)
// but because of std::move and .back(), the order is lost
inline bool LightManager::remove_point_light_unordered(int id)
{
    if (id < 0 || id >= MAX_POINT_LIGHTS)
        return false;

    auto find_id = std::find_if(point_lights.begin(), point_lights.end(), 
    [id](const ManagedPointLight& managed) { return managed.id == id; });

    if (find_id == point_lights.end())
        return false;
    
    *find_id = std::move(point_lights.back());
    point_lights.pop_back();
    return true;
}

// ---- spot light ----
inline int LightManager::add_spot_light(const SpotLight& light)
{
    if (spot_lights.size() >= MAX_SPOT_LIGHTS)
        return -1;

    int id = next_spot_light_id++;
    spot_lights.emplace_back(ManagedSpotLight{id, light});
    return id;       
}

// during spot light removal order in the array is maintained
// using spot_lights.erase(), but can be expensive for larger sizes
// as .erase() is O(n)
inline bool LightManager::remove_spot_light_ordered(int id)
{
    if (id < 0 || id >= MAX_POINT_LIGHTS)
        return false;

    auto find_id = std::find_if(spot_lights.begin(), spot_lights.end(),
    [id](const ManagedSpotLight& managed) { return managed.id == id; } );

    if (find_id == spot_lights.end())
        return false;
    
    spot_lights.erase(find_id);
    return true;
}

// unordered removals are better supported for larger sizes
// as pop_back() is O(1)
// but because of std::move and .back(), the order is lost
inline bool LightManager::remove_spot_light_unordered(int id)
{
    if (id < 0 || id >= MAX_POINT_LIGHTS)
        return false;

    auto find_id = std::find_if(spot_lights.begin(), spot_lights.end(), 
    [id](const ManagedSpotLight& managed) { return managed.id == id; });

    if (find_id == spot_lights.end())
        return false;
    
    *find_id = std::move(spot_lights.back());
    spot_lights.pop_back();
    return true;
}