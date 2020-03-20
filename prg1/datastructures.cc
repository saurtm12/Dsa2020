// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <cmath>
#include <stdexcept>
#include <algorithm>
std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator
template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)
const Coord COORD_MAX = {std::numeric_limits<int>::max(),
                         std::numeric_limits<int>::max()};
Datastructures::Datastructures()
{
    // Replace this comment with your implementation
}

Datastructures::~Datastructures()
{
    mp.clear();
    region_map.clear();
    coord_min = nullptr;
    coord_max = nullptr;
}

int Datastructures::stop_count()
{
    return mp.size();

}

void Datastructures::clear_all()
{
    mp.clear();
    region_map.clear();
    coord_max = nullptr;
    coord_min = nullptr;
}

std::vector<StopID> Datastructures::all_stops()
{

    std::vector <StopID> temp_vector;
    temp_vector.reserve(mp.size());
    for (auto point : mp)
    {
        temp_vector.push_back(point.second->id);
    }
    return temp_vector;
}


/*Adds a stop to the data structure with given unique id,
name, and coordinates. Initially a stop is not part of
any region. If there already is a stop with the given id,
nothing is done and false is returned, otherwise
true is returned.*/
bool Datastructures::add_stop(StopID id, const Name& name, Coord xy)
{
    auto iter = mp.find(id);
    if (iter != mp.end())
    {
        return false;
    }
    std::shared_ptr <Point> new_point = std::make_shared<Point>(id, name,xy);
    mp.insert({id,new_point});
    if (mp.size() == 1)
    {
        coord_max = new_point;
        coord_min = new_point;
        return true;
    }
    if (coord_max->coord < new_point->coord)
        {coord_max = new_point;}
    if (new_point->coord < coord_min->coord )
        coord_min = new_point;
    return true;
}

Name Datastructures::get_stop_name(StopID id)
{
    auto iter = mp.find(id);
    if (iter == mp.end())
    {
        return NO_NAME;
    }
    return iter->second->name;
}

Coord Datastructures::get_stop_coord(StopID id)
{
    auto iter = mp.find(id);
    if (iter == mp.end())
    {
        return NO_COORD;
    }
    return iter->second->coord;
}

std::vector<StopID> Datastructures::stops_alphabetically()
{
    std::vector<Point_ptr> vec;
    vec.reserve(mp.size());
    for(auto point : mp)
    {
        vec.push_back(point.second);
    }

    std::sort(vec.begin(), vec.end(),
              [](const Point_ptr& point_a,const Point_ptr& point_b) -> bool
                {
                    return point_a->name < point_b->name;
                } );
    std::vector <StopID> temp_vector;
    temp_vector.reserve(vec.size());
    for (auto point : vec)
    {
        temp_vector.push_back(point->id);
    }
    return temp_vector;

}



std::vector<StopID> Datastructures::stops_coord_order()
{
    std::vector<Point_ptr> vec;
    vec.reserve(mp.size());
    for(auto point : mp)
    {
        vec.push_back(point.second);
    }

    std::sort(vec.begin(), vec.end(),
              [](const Point_ptr& point_a,const Point_ptr& point_b)->bool
                {
                   return point_a->coord < point_b->coord;
                } );
    std::vector <StopID> temp_vector;
    for (auto point : vec)
    {
        temp_vector.push_back(point->id);
    }
    return temp_vector;
}

StopID Datastructures::min_coord()
{
    if (mp.size() != 0)
        return coord_min->id;
    return NO_STOP;
}

StopID Datastructures::max_coord()
{
    if (mp.size() != 0)
        return coord_max->id;
    return NO_STOP;
}

std::vector<StopID> Datastructures::find_stops(Name const& name)
{
    std::vector<StopID> temp_vector;
    for (auto point : mp)
    {
        if (point.second->name == name)
            temp_vector.push_back(point.second->id);
    }
    return temp_vector;
}

bool Datastructures::change_stop_name(StopID id, const Name& newname)
{
    auto point = mp.find(id);
    if (point == mp.end())
    {
        return false;
    }
    point->second->name = newname;
    return true;
}

bool Datastructures::change_stop_coord(StopID id, Coord newcoord)
{
    auto point = mp.find(id);
    if (point == mp.end())
    {
        return false;
    }
    point->second->coord = newcoord;
    return true;
}

bool Datastructures::add_region(RegionID id, const Name &name)
{
    auto iter = region_map.find(id);
    if (iter == region_map.end())
    {
        std::shared_ptr <Region> new_region = std::make_shared<Region>(id, name);
        region_map.insert({id,new_region});
        return true;
    }

    return false;
}

Name Datastructures::get_region_name(RegionID id)
{
    auto iter = region_map.find(id);
    if (iter != region_map.end())
    {
        return iter->second->name;
    }
    return NO_NAME;
}

std::vector<RegionID> Datastructures::all_regions()
{
    if  (region_map.size()== 0)
        return {NO_REGION};

    std::vector<RegionID> regions;
    for (auto region : region_map)
    {
        regions.push_back(region.first);
    }
    return regions;
}

bool Datastructures::add_stop_to_region(StopID id, RegionID parentid)
{
    auto region_iter = region_map.find(parentid);
        if (region_iter != region_map.end())
        {
            auto stop_iter = mp.find(id);
            if (stop_iter != mp.end())
            {
                stop_iter->second->r_id = parentid;
                region_iter->second->subpoints.insert({id,stop_iter->second});
                return true;
            }
        }
        return false;

}

bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    auto iter = region_map.find(id);
    if (iter != region_map.end())
    {
        auto parent_iter = region_map.find(parentid);
        if (parent_iter != region_map.end())
        {
            iter->second->subregions.insert({id,region_map.at(id)});
            region_map.at(id)->parent_region = region_map.at(parentid);
            return true;
        }
    }
    return false;
}

std::vector<RegionID> Datastructures::stop_regions(StopID id)
{
    auto iter = mp.find(id);
    if (iter != mp.end())
    {
        std::vector<RegionID> temp;
        Region_ptr region = region_map.at(iter->second->r_id);
        while (region != nullptr)
        {
            temp.push_back(region->id);
            region= region->parent_region;
        }
        if (temp.size() != 0 )
        {
            return temp;
        }
    }
    return {NO_REGION};
}

void Datastructures::creation_finished()
{
    // Replace this comment with your implementation
    // You don't have to use this method for anything, if you don't need it
}

std::pair<Coord,Coord> Datastructures::region_bounding_box(RegionID id)
{
    Region_ptr region = region_map.at(id);
    return this->recursive_region_bounding_box(region);
}

std::pair<Coord,Coord> Datastructures::recursive_region_bounding_box(Region_ptr region)
{
    if (region->subpoints.size() == 0 && region->subregions.size() == 0)
        return {NO_COORD, {std::numeric_limits<int>::max(),std::numeric_limits<int>::max()} };
    else
    {
        Coord min = {std::numeric_limits<int>::max(),std::numeric_limits<int>::max()};
        Coord max = NO_COORD;
        for (auto point : region->subpoints)
        {
            if (max.x < point.second->coord.x)
                {
                max.x = point.second->coord.x;
                }
            if (max.y < point.second->coord.y)
                {
                max.y = point.second->coord.y;
                }
            if (min.x > point.second->coord.x)
                {
                min.x = point.second->coord.x;
                }
            if (min.y > point.second->coord.y)
                {
                min.y = point.second->coord.y;
                }
        }

        if (region->subregions.size() == 0)
            return {min,max};
        for (auto subregion : region->subregions)
        {
            std::pair<Coord,Coord> sub_box = this->recursive_region_bounding_box(subregion.second);
            if (sub_box.second != NO_COORD)
            {
                if (max.x < sub_box.second.x)
                    {
                    max.x = sub_box.second.y;
                    }
                if (max.y < sub_box.second.y)
                    {
                    max.y = sub_box.second.y;
                    }
                if (min.x > sub_box.first.x)
                    {
                    min.x = sub_box.first.x;
                    }
                if (min.y < sub_box.first.y)
                    {
                    min.y = sub_box.first.y;
                    }
            }
            return {min,max};

        }
    }


}



std::vector<StopID> Datastructures::stops_closest_to(StopID id)
{
    // Replace this comment and the line below with your implementation
    return {NO_STOP};
}

bool Datastructures::remove_stop(StopID id)
{

    return false;
}

RegionID Datastructures::stops_common_region(StopID id1, StopID id2)
{

    return NO_REGION;
}
