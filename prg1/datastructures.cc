// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <cmath>
#include <stdexcept>
#include <algorithm>

#include <unordered_set>
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
    for(auto const& region : region_map)
    {
        region.second->subpoints.clear();
        region.second->subregions.clear();
        region.second->parent_region = nullptr;
    }
    region_map.clear();
    id_to_coordinate.clear();
    namemap.clear();
    coord_min = nullptr;
    coord_max = nullptr;
}
//O(1)
int Datastructures::stop_count()
{
    return id_to_coordinate.size();
}
//O(1)
void Datastructures::clear_all()
{
    mp.clear();
    for(auto const& region : region_map)
    {
        region.second->subpoints.clear();
        region.second->subregions.clear();
        region.second->parent_region = nullptr;
    }
    region_map.clear();
    namemap.clear();
    id_to_coordinate.clear();
    vector_is_sorted =false;
    coord_max = nullptr;
    coord_min = nullptr;
}
//O(n)
std::vector<StopID> Datastructures::all_stops()
{
    std::vector <StopID> temp_vector;
    temp_vector.reserve(mp.size());
    for (auto const& point : id_to_coordinate)
    {
        temp_vector.push_back(point->first);
    }
    return temp_vector;
}


/*Adds a stop to the data structure with given unique id,
name, and coordinates. Initially a stop is not part of
any region. If there already is a stop with the given id,
nothing is done and false is returned, otherwise
true is returned.*/
//O(log n) (line 84)
bool Datastructures::add_stop(StopID id, const Name& name, Coord xy)
{
    auto iter = mp.find(id);
    if (iter != mp.end())
    {
        return false;
    }
    id_to_coordinate.push_back(std::make_shared<std::pair<StopID,Coord>>(id,xy));

    std::shared_ptr <Point> new_point = std::make_shared<Point>(id, name,xy,id_to_coordinate[id_to_coordinate.size()-1]);
    mp.insert({id,new_point});
    if (namemap_is_added)
        namemap.insert({name,id});

    if (id_to_coordinate.size() == 1)
    {
        coord_max = new_point;
        coord_min = new_point;
        new_point = nullptr;
        return true;
    }

    vector_is_sorted = false;

    if (coord_max->coord < new_point->coord)
        {coord_max = new_point;}
    if (new_point->coord < coord_min->coord )
        coord_min = new_point;
    new_point = nullptr;
    return true;
}
//average O(1)
Name Datastructures::get_stop_name(StopID id)
{
    auto iter = mp.find(id);
    if (iter == mp.end())
    {
        return NO_NAME;
    }
    return iter->second->name;
}
//Average (O(1))
Coord Datastructures::get_stop_coord(StopID id)
{
    auto iter = mp.find(id);
    if (iter == mp.end())
    {
        return NO_COORD;
    }
    return iter->second->coord;
}
//O(n) nlogn at the first time and then O(n)
std::vector<StopID> Datastructures::stops_alphabetically()
{
    std::vector <StopID> temp_vector;
    temp_vector.reserve(id_to_coordinate.size());
    if (namemap_is_added)
    {
        for (auto const& point : namemap)
        {
            temp_vector.push_back(point.second);
        }
        return temp_vector;
    }

    std::vector<Point_ptr> vec;
    vec.reserve(mp.size());
    for(auto const& point : mp)
    {
        vec.push_back(point.second);
    }

    std::sort(vec.begin(), vec.end(),
              [](const Point_ptr& point_a,const Point_ptr& point_b) -> bool
                {
                    return point_a->name < point_b->name;
                } );
    namemap_is_added = true;
    auto iter = namemap.begin();
    for (auto const& point: vec)
    {
        namemap.insert(iter,{point->name,point->id});
        iter++;
    }
    namemap_is_added = true;

    for (auto const& point : vec)
    {
        temp_vector.push_back(point->id);
    }
    return temp_vector;


}


//O(nlogn)
std::vector<StopID> Datastructures::stops_coord_order()
{
    if (!vector_is_sorted)
    {
        std::sort(id_to_coordinate.begin(), id_to_coordinate.end(),
              [](const V_ptr& point_a,const V_ptr& point_b)->bool
                {
                   return point_a->second < point_b->second;
                } );
        vector_is_sorted = true;
    }

    std::vector <StopID> temp_vector;
    temp_vector.reserve(id_to_coordinate.size());
    for (auto point : id_to_coordinate)
    {
        temp_vector.push_back(point->first);
    }

    return temp_vector;
}
//O(1)
StopID Datastructures::min_coord()
{

    if (mp.size() != 0)
        return coord_min->id;
    return NO_STOP;
}
//O(1)
StopID Datastructures::max_coord()
{
    if (mp.size() != 0)
        return coord_max->id;
    return NO_STOP;
}
//O(logn)
std::vector<StopID> Datastructures::find_stops(Name const& name)
{
    if (!namemap_is_added)
    {
        std::vector<Point_ptr> vec;
        vec.reserve(mp.size());
        for(auto const& point : mp)
        {
            vec.push_back(point.second);
        }

        std::sort(vec.begin(), vec.end(),
                  [](const Point_ptr& point_a,const Point_ptr& point_b) -> bool
                    {
                        return point_a->name < point_b->name;
                    } );

        auto iter = namemap.begin();
        for (auto const& point: vec)
        {
            namemap.insert(iter,{point->name,point->id});
            iter++;
        }
        namemap_is_added = true;
    }


    auto pair = namemap.equal_range(name);
    if (pair.first == namemap.end())
        return {};
    std::vector<StopID> temp_vector;
    for (auto iter = pair.first;iter != pair.second;iter++)
    {

            temp_vector.push_back(iter->second);

    }
    return temp_vector;
}
//O(logn)
bool Datastructures::change_stop_name(StopID id, const Name& newname)
{
    auto point = mp.find(id);
    if (point == mp.end())
    {
        return false;
    }
    if (namemap_is_added)
    {
        auto pair = namemap.equal_range(point->second->name);

        for (auto iter = pair.first;iter != pair.second;iter++)
        {
            if (iter->second == id)
            {
                namemap.insert(iter,{newname,id});
                namemap.erase(iter);
                point->second->name = newname;
                return true;
            }
        }
    }

    point->second->name = newname;
    return true;

}
//O(1) if remove max_coord/min_coord, O(n)
bool Datastructures::change_stop_coord(StopID id, Coord newcoord)
{
    auto point = mp.find(id);
    if (point == mp.end())
    {
        return false;
    }


    point->second->ptr_v->second = newcoord;
    vector_is_sorted = false;

    if (coord_max->id == id)
    {
       if (newcoord < coord_max->coord)
       {
           if (vector_is_sorted)
                coord_max = mp.at(id_to_coordinate[id_to_coordinate.size()-2]->first);
            else
            {

                auto max = std::max_element(id_to_coordinate.begin(),id_to_coordinate.end(),[&](auto ptr1, auto ptr2)->bool
            {
                return ptr1->second<ptr2->second;
            });
            coord_max =  mp.at((*max)->first);
            }
        }
    }

    if (coord_min->id == id)
    {
       if (coord_min->coord <  newcoord)
       {
           if (vector_is_sorted)
                coord_min = mp.at(id_to_coordinate[1]->first);
            else
            {

                auto min = std::min_element(id_to_coordinate.begin(),id_to_coordinate.end(),[&](auto ptr1, auto ptr2)->bool
            {
                return ptr1->second<ptr2->second;
            });
            coord_min =  mp.at((*min)->first);
            }
        }
    }
    point->second->coord = newcoord;
        return true;
}
//O(1)
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
//O(1)
Name Datastructures::get_region_name(RegionID id)
{
    auto iter = region_map.find(id);
    if (iter != region_map.end())
    {
        return iter->second->name;
    }
    return NO_NAME;
}
//O(n)
std::vector<RegionID> Datastructures::all_regions()
{
    if  (region_map.size() == 0)
        return {};

    std::vector<RegionID> regions;
    regions.reserve(region_map.size());
    for (auto const& region : region_map)
    {
        regions.push_back(region.first);
    }
    return regions;
}
//O(1)
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
//O(1)
bool Datastructures::add_subregion_to_region(RegionID id, RegionID parentid)
{
    auto iter = region_map.find(id);
    if (iter != region_map.end())
    {
        auto parent_iter = region_map.find(parentid);
        if (parent_iter != region_map.end())
        {
            parent_iter->second->subregions.insert({id,iter->second});
            iter->second->parent_region = parent_iter->second;
            return true;
        }
    }
    return false;
}
//Average O(1)
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
//O(n) n is the number of the points belonging to that region
std::pair<Coord,Coord> Datastructures::region_bounding_box(RegionID id)
{

    Region_ptr region = region_map.at(id);
    auto result = this->recursive_region_bounding_box(region);
    if (result.second == NO_COORD)
        return {NO_COORD,NO_COORD};
    return result;
}
std::pair<Coord,Coord> Datastructures::recursive_region_bounding_box(Region_ptr region)
{
    if (region->subpoints.size() == 0 && region->subregions.size() == 0)
        return  { {std::numeric_limits<int>::max(),std::numeric_limits<int>::max()} ,NO_COORD };
    else
    {
    Coord min = {std::numeric_limits<int>::max(),std::numeric_limits<int>::max()};
    Coord max = NO_COORD;
    for (auto const& point : region->subpoints)
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

        for (auto const& subregion : region->subregions)
        {
            std::pair<Coord,Coord> sub_box = recursive_region_bounding_box(subregion.second);
            if (sub_box.second != NO_COORD)
            {
                if (max.x < sub_box.second.x)
                    {
                    max.x = sub_box.second.x;
                    }
                if (max.y < sub_box.second.y)
                    {
                    max.y = sub_box.second.y;
                    }
                if (min.x > sub_box.first.x)
                    {
                    min.x = sub_box.first.x;
                    }
                if (min.y > sub_box.first.y)
                    {
                    min.y = sub_box.first.y;
                    }
            }

        }
    return {min,max};

  }
}



//O(n)
std::vector<StopID> Datastructures::stops_closest_to(StopID id)
{
    auto stop = mp.find(id);
    if (stop == mp.end())
    {
        return {NO_STOP};
    }
    Coord origin = stop->second->coord;
    if (mp.size()<=6)
    {
        std::vector<StopID> temp;
        temp.reserve(5);
        for (auto stop: mp)
        {
            if (stop.second->id!=id)
            temp.push_back(stop.second->id);
        }
        return temp;
    }

    std::nth_element(id_to_coordinate.begin(),id_to_coordinate.begin()+5,id_to_coordinate.end(),
                     [&](const V_ptr& point_a,const V_ptr& point_b)->bool
                       {
                          return square_distance(point_a->second,origin) < square_distance(point_b->second,origin);
                       } );
    std::sort(id_to_coordinate.begin(),id_to_coordinate.begin()+5,
              [&](const V_ptr& point_a,const V_ptr& point_b)->bool
                {
                   return square_distance(point_a->second,origin) < square_distance(point_b->second,origin);
                });
    vector_is_sorted =false;
    std::vector<StopID> temp;
    for (int i =0; i<=5;i++)
    {
        if (id_to_coordinate[i]->first != id)
            temp.push_back(id_to_coordinate[i]->first);
    }
    return temp;
}

//O(n), most case  average logn
bool Datastructures::remove_stop(StopID id)
{
    auto iter = mp.find(id);
    if (iter == mp.end())
        return false;
    if (iter->second->r_id != NO_REGION)
        region_map.at(iter->second->r_id)->subpoints.erase(id);
    if (namemap_is_added)
    {
        auto pair = namemap.equal_range(iter->second->name);
        auto name_iter = pair.first;
        for (auto iter = pair.first;iter != pair.second;iter++)
            {
                if (iter->second == id)
                {
                    name_iter = iter;
                    break;
                }
            }
        namemap.erase(name_iter);
    }


    if (id_to_coordinate.size() == 1)
    {
        coord_max = nullptr;
        coord_min = nullptr;
        mp.clear();
        namemap.clear();
        id_to_coordinate.clear();
        vector_is_sorted = false;
        return true;
    }

    if (coord_max->id == id)
    {

        if (vector_is_sorted)
            coord_max = mp.at(id_to_coordinate[id_to_coordinate.size()-2]->first);
        else
        {
            mp.at(id_to_coordinate[id_to_coordinate.size()-1]->first)->ptr_v = iter->second->ptr_v;
            iter->second->ptr_v->first = id_to_coordinate[id_to_coordinate.size()-1]->first;
            iter->second->ptr_v->second = id_to_coordinate[id_to_coordinate.size()-1]->second;

            auto max = std::max_element(id_to_coordinate.begin(),id_to_coordinate.end()-1,[&](auto const& ptr1, auto const& ptr2)->bool
            {
                return ptr1->second<ptr2->second;
            });
            coord_max =  mp.at((*max)->first);
        }
        mp.erase(iter);

        id_to_coordinate.erase(id_to_coordinate.end()-1);
        return true;
    }

     if (coord_min->id == id)
    {
        mp.at(id_to_coordinate[id_to_coordinate.size()-1]->first)->ptr_v = iter->second->ptr_v;
        iter->second->ptr_v->first = id_to_coordinate[id_to_coordinate.size()-1]->first;
        iter->second->ptr_v->second = id_to_coordinate[id_to_coordinate.size()-1]->second;
        if (vector_is_sorted)
            coord_min = mp.at(id_to_coordinate[1]->first);
        else
        {
            auto min = std::min_element(id_to_coordinate.begin(),id_to_coordinate.end()-1,[&](auto ptr1,auto ptr2) ->bool
            {
                return ptr1->second< ptr2->second;
            });
            coord_min =  mp.at((*min)->first);

        }
        mp.erase(iter);
        id_to_coordinate.erase(id_to_coordinate.end()-1);
        vector_is_sorted =false;
        return true;
    }

    mp.at(id_to_coordinate[id_to_coordinate.size()-1]->first)->ptr_v = iter->second->ptr_v;
    std::swap(iter->second->ptr_v->second,id_to_coordinate[id_to_coordinate.size()-1]->second);
    std::swap(iter->second->ptr_v->first,id_to_coordinate[id_to_coordinate.size()-1]->first);
    iter->second->ptr_v = id_to_coordinate[id_to_coordinate.size()-1];
    mp.erase(iter);
    id_to_coordinate.erase(id_to_coordinate.end()-1);
    vector_is_sorted = false;
    return true;

}

RegionID Datastructures::stops_common_region(StopID id1, StopID id2)
{
    auto iter1 = mp.find(id1);
    auto iter2 = mp.find(id2);
    if (iter1 == mp.end() || iter2 == mp.end())
        return NO_REGION;
    if (iter1->second->r_id == NO_REGION ||  iter2->second->r_id == NO_REGION)
        return NO_REGION;
    Region_ptr parent1 = region_map.at(iter1->second->r_id),parent2 = region_map.at(iter2->second->r_id);
    std::unordered_set<RegionID> regions1,regions2;
    while (parent1 != nullptr || parent2 != nullptr)
    {   if (parent1 != nullptr)
            regions1.insert(parent1->id);
        if (parent2 != nullptr)
            regions2.insert(parent2->id);
        if (parent1!= nullptr && regions2.find(parent1->id) != regions2.end())
        {
            return parent1->id;
        }
        if (parent2 != nullptr && regions1.find(parent2->id) != regions1.end())
        {
            return parent2->id;
        }
        if (parent1 != nullptr)
            parent1 = parent1->parent_region;
        if (parent2 != nullptr)
            parent2 = parent2->parent_region;
    }
    return NO_REGION;

}

double square_distance(Coord c1, Coord c2)
{
    return (c1.x-c2.x)*(c1.x-c2.x)+(c1.y-c2.y)*(c1.y-c2.y);
}
