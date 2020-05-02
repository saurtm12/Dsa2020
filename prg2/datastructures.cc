// Datastructures.cc

#include "datastructures.hh"

#include <random>
#include <cmath>
#include <stdexcept>
#include <queue>
#include <stack>
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

Datastructures::Datastructures()
{
    // Replace this comment with your implementation
}

Datastructures::~Datastructures()
{

}

int Datastructures::stop_count()
{
    return id_to_coordinate.size();
}

void Datastructures::clear_all()
{
    mp.clear();
    region_map.clear();
    namemap.clear();
    id_to_coordinate.clear();
    vector_is_sorted = false;
    namemap_is_added = false;
    coord_max = nullptr;
    coord_min = nullptr;

    //phase 2:
    routes.clear();
}

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

bool Datastructures::add_stop(StopID id, const Name& name, Coord xy)
{
    auto iter = mp.find(id);
    if (iter != mp.end())
    {
        return false;
    }
    id_to_coordinate.push_back(std::make_shared<std::pair<StopID,Coord>>(id,xy));
    std::shared_ptr <Point> new_point = std::make_shared<Point>(id, name,xy,id_to_coordinate[id_to_coordinate.size()-1]);
    new_point->color = global_white;
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
        {
            coord_max = new_point;
            vector_is_sorted = true;
        }
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

    for (auto const& stop: mp)
    {
        namemap.insert({stop.second->name,stop.first});
    }
    namemap_is_added = true;
    for (auto const& point : namemap)
    {
        temp_vector.push_back(point.second);
    }
    return temp_vector;
}

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
    if (!namemap_is_added)
    {
        for (auto const& stop: mp)
        {
            namemap.insert({stop.second->name,stop.first});
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
            parent_iter->second->subregions.insert({id,iter->second});
            iter->second->parent_region = parent_iter->second;
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

}

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
        std::sort(id_to_coordinate.begin(),id_to_coordinate.end(),
                  [&](const V_ptr& point_a,const V_ptr& point_b)->bool
                    {
                       return square_distance(point_a->second,origin) < square_distance(point_b->second,origin);
                    });
        vector_is_sorted = false;
        for (auto const& stop: id_to_coordinate)
        {
            if (stop->first!=id)
            temp.push_back(stop->first);
        }
        return temp;
    }
    std::nth_element(id_to_coordinate.begin(),id_to_coordinate.begin()+5,id_to_coordinate.end(),
                     [&](const V_ptr& point_a,const V_ptr& point_b)->bool
                       {
                          return square_distance(point_a->second,origin) < square_distance(point_b->second,origin);
                       } );
    std::sort(id_to_coordinate.begin(),id_to_coordinate.begin()+5,
              [&](const V_ptr& point_a, const V_ptr& point_b)->bool
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

bool Datastructures::remove_stop(StopID id)
{
    auto iter = mp.find(id);
    if (iter == mp.end())
        return false;
    if (iter->second->r_id != NO_REGION)
        region_map.at(iter->second->r_id)->subpoints.erase(id);

    //delete in namemap (map : name->id)
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

    //delete in map_point mp (stopid-> Point_ptr)
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
    //if removing max
    if (coord_max->id == id)
    {

        if (vector_is_sorted)
            coord_max = mp.at(id_to_coordinate[id_to_coordinate.size()-2]->first);
        else
        {
            //swaping the last element of vector and the already known element then erase cost O(1).
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
    //if removing min
     if (coord_min->id == id)
    {
        //swaping the last element of vector and the already known element then erase cost O(1).
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
    //swaping the last element of vector and the already known element then erase cost O(1).
    mp.at(id_to_coordinate[id_to_coordinate.size()-1]->first)->ptr_v = iter->second->ptr_v;
    iter->second->ptr_v->second = id_to_coordinate[id_to_coordinate.size()-1]->second;
    iter->second->ptr_v->first=id_to_coordinate[id_to_coordinate.size()-1]->first;
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

//End of Phase 1.
//------------------------------------------------------------------------------------------------------//
//Phase 2:

std::vector<RouteID> Datastructures::all_routes()
{
    if (routes.size() == 0)
        return {NO_ROUTE};
    std::vector <RouteID> temp;
    temp.reserve(routes.size());
    for (auto const& route : routes)
    {
        temp.push_back(route.first);
    }
    return temp;
}

bool Datastructures::add_route(RouteID id, std::vector<StopID> stops)
{
    auto iter = routes.find(id);
    if (iter != routes.end())
    {
        return false;
    }
    if (stops.size() == 0)
    {
        return false;
    }


    for (auto const& stop : stops)
    {
        auto find_stop = mp.find(stop);
        if (find_stop == mp.end())
        {
            return false;
        }
    }
    //add next stop to every point in the route
    auto next_stop_iter = mp.find(stops.back());
    next_stop_iter->second->next_stop.insert({nullptr, {id, NO_DISTANCE}});
    for (auto iterator = stops.end()-2; iterator != stops.begin()-1; iterator--)
    {
        auto find_stop = mp.find(*iterator);
        find_stop->second->next_stop.insert(
        { next_stop_iter->second, {id,
          (int)(sqrt(square_distance(next_stop_iter->second->coord,find_stop->second->coord)))}});
        next_stop_iter = find_stop;
    }
    routes.insert({id,stops});
    return true;
}

std::vector<std::pair<RouteID, StopID>> Datastructures::routes_from(StopID stopid)
{
    auto iter = mp.find(stopid);
    if (iter == mp.end())
    {
        return {{NO_ROUTE, NO_STOP}};
    }

    std::vector<std::pair<RouteID, StopID>> temp_vector;
    temp_vector.reserve(iter->second->next_stop.size());
    for (auto& next : iter->second->next_stop)
    {
        if (std::get<0>(next) != nullptr )
            temp_vector.push_back({(next.second).first,next.first->id});
    }
    return temp_vector;
}

std::vector<StopID> Datastructures::route_stops(RouteID id)
{
    auto iter = routes.find(id);
    if (iter == routes.end())
        return {NO_STOP};
    return iter->second;
}

void Datastructures::clear_routes()
{
    routes.clear();
    for (auto& point: mp)
    {
        point.second->next_stop.clear();
    }
}

void Datastructures::reset_color()
{
    *global_black = WHITE;
    *global_gray = WHITE;
    global_black = std::make_shared<Color>(BLACK);
    global_gray = std::make_shared<Color>(GRAY);
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_any(StopID fromstop, StopID tostop)
{
    /* This function is implement as BFS as I consider it is a stable search for this function.
    */
    auto start = mp.find(fromstop);
    if (start  == mp.end())
    {
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }
    auto end = mp.find(tostop);
    if (end == mp.end())
    {
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }
    if (fromstop == tostop)
    {
        return {};
    }

    std::queue<Point_ptr> visit;
    end->second->previous_node = nullptr;
    visit.push(start->second);
    while (visit.size() != 0)
    {
        auto current = visit.front();
        visit.pop();
        for (auto& direct : current->next_stop)
        {
            if (std::get<0>(direct) != nullptr)
            {
                if (*((std::get<0>(direct))->color) == WHITE)
                {
                    visit.push(std::get<0>(direct));
                    std::get<0>(direct)->color = global_gray;
                    std::get<0>(direct)->previous_node = current;
                    if (std::get<0>(direct)->id == tostop)
                    {
                        goto end_loop;
                    }
                }
            }
        }
        current->color = global_black;
    }

    end_loop:
    if (end->second->previous_node == nullptr)
    {
        reset_color();
        return {};
    }

    //adding result;
    auto current = end->second;
    std::vector<Point_ptr> temp;
    while (current != start->second)
    {
        temp.push_back(current);
        current = current->previous_node;
    }
    temp.push_back(current);
    std::reverse(temp.begin(),temp.end());
    std::vector<std::tuple<StopID,RouteID, Distance>> result;
    for (auto iter = temp.begin(); iter != temp.end()-1; iter++)
    {
        auto find_route = (*iter)->next_stop.find(*(iter+1));
        result.push_back({(*iter)->id, find_route->second.first,find_route->second.second});
    }
    result.push_back({end->second->id, NO_ROUTE, 0});

    for (auto iter = result.rbegin(); iter != result.rend()-1;iter++)
    {
        std::get<2>(*iter) = std::get<2>(*(iter+1));
    }

    std::get<2>(result.front()) = 0;
    for (auto iter = result.begin()+1; iter != result.end();iter++)
    {
        std::get<2>(*iter) += std::get<2>(*(iter-1));
    }
    reset_color();
    return result;
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_least_stops(StopID fromstop, StopID tostop)
{
    // This function is copied from the journey_any function because they are BFS.
    auto start = mp.find(fromstop);
    if (start  == mp.end())
    {
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }
    auto end = mp.find(tostop);
    if (end == mp.end())
    {
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }
    if (fromstop == tostop)
    {
        return {};
    }

    std::queue<Point_ptr> visit;
    end->second->previous_node = nullptr;
    visit.push(start->second);
    while (visit.size() != 0)
    {
        auto current = visit.front();
        visit.pop();
        for (auto& direct : current->next_stop)
        {
            if (direct.first != nullptr)
            {
                if (*(direct.first)->color == WHITE)
                {
                    visit.push(direct.first);
                    (direct.first)->color = global_gray;
                    (direct.first)->previous_node = current;
                    if (std::get<0>(direct)->id == tostop)
                    {
                        goto end_loop;
                    }
                }
            }
        }
        current->color = global_black;
    }

    end_loop:
    if (end->second->previous_node == nullptr)
    {
        reset_color();
        return {};
    }

    //adding result;
    auto current = end->second;
    std::vector<Point_ptr> temp;
    while (current != start->second)
    {
        temp.push_back(current);
        current = current->previous_node;
    }
    temp.push_back(current);
    std::reverse(temp.begin(),temp.end());
    std::vector<std::tuple<StopID,RouteID, Distance>> result;
    for (auto iter = temp.begin(); iter != temp.end()-1; iter++)
    {
        auto find_route = (*iter)->next_stop.find(*(iter+1));
        result.push_back({(*iter)->id, find_route->second.first,find_route->second.second});
    }
    result.push_back({end->second->id, NO_ROUTE, 0});

    for (auto iter = result.rbegin(); iter != result.rend()-1;iter++)
    {
        std::get<2>(*iter) = std::get<2>(*(iter+1));
    }

    std::get<2>(result.front()) = 0;
    for (auto iter = result.begin()+1; iter != result.end();iter++)
    {
        std::get<2>(*iter) += std::get<2>(*(iter-1));
    }
    reset_color();
    return result;
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_with_cycle(StopID fromstop)
{
    // The GRAY node is the node which is being processed, if the next node of a path is GRAY so
    // a cycle is found
    auto start = mp.find(fromstop);
    if (start  == mp.end())
    {
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }
    std::stack<Point_ptr> visit;
    start->second->previous_node = nullptr;
    visit.push(start->second);
    bool loop_found = false;
    Point_ptr last_route,second_last_route;
    while (visit.size() != 0)
    {
        auto current = visit.top();
        if (*current->color == WHITE)
        {
            current->color = global_gray;
            for (auto& direct : current->next_stop)
                {
                    if (direct.first != nullptr)
                    {
                        if (*(direct.first)->color == GRAY)
                        {
                            loop_found = true;
                            last_route = direct.first;
                            second_last_route = current;
                            goto end_loop;
                        }
                        if (*(direct.first)->color == WHITE)
                        {
                            visit.push(direct.first);
                            direct.first->previous_node = current;
                        }
                    }
                }
        }
        else
        {
           current->color = global_black;
           visit.pop();
        }
    }

    end_loop:
    if (!loop_found)
    {
        reset_color();
        return {};
    }

    auto current = second_last_route;
    std::vector<Point_ptr> temp;
    temp.push_back(last_route);
    while (current != start->second)
    {
        temp.push_back(current);
        current = current->previous_node;
    }
    temp.push_back(current);
    std::reverse(temp.begin(),temp.end());
    std::vector<std::tuple<StopID,RouteID, Distance>> result;
    for (auto iter = temp.begin(); iter != temp.end()-1; iter++)
    {
        auto find_route = (*iter)->next_stop.find(*(iter+1));
        result.push_back({(*iter)->id, find_route->second.first,find_route->second.second});
    }
    result.push_back({last_route->id, NO_ROUTE, 0});
    for (auto iter = result.rbegin(); iter != result.rend()-1;iter++)
    {
        std::get<2>(*iter) = std::get<2>(*(iter+1));
    }

    std::get<2>(result.front()) = 0;
    for (auto iter = result.begin()+1; iter != result.end();iter++)
    {
        std::get<2>(*iter) += std::get<2>(*(iter-1));
    }
    reset_color();
    return result;
}

std::vector<std::tuple<StopID, RouteID, Distance>> Datastructures::journey_shortest_distance(StopID fromstop, StopID tostop)
{
    // This function is implemented by Dijsktra Algorithm.
    // Because I implemented reset color, I dont need to add function reset the distance as infinity at the begining of the
    // function because logically if a node is white, it is not discovered yet, so the distance to that node is infinity,
    // So I change the condition of there exists a shorter path by the node is white before.
    auto start = mp.find(fromstop);
    if (start  == mp.end())
    {
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }
    auto end = mp.find(tostop);
    if (end == mp.end())
    {
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }
    if (fromstop == tostop)
    {
        return {};
    }
    struct compare_distance
    {
        bool operator()(const Point_ptr& ptr1,const Point_ptr& ptr2)
        {
            return ptr1->d > ptr2->d;
        }
    };
    std::priority_queue<Point_ptr,std::vector<Point_ptr>,compare_distance> visit;
    end->second->previous_node = nullptr;
    start->second->color = global_gray;
    start->second->d = 0;
    visit.push(start->second);
    //last route is for tracking the last route followed
    while (visit.size() != 0)
    {
        auto current = visit.top();
        visit.pop();
        // end immediately when find optimal path.
        if (current->id == tostop)
        {
            goto end_loop;
        }
        for (auto& direct : current->next_stop)
        {
            if (direct.first != nullptr)
            {
                if (*(direct.first)->color == WHITE)
                {
                    direct.first->color = global_gray;
                    direct.first->d = current->d + direct.second.second;
                    direct.first->previous_node = current;
                    visit.push(direct.first);
                }
                if (direct.first->d > current->d + direct.second.second)
                {
                    direct.first->d = current->d + direct.second.second;
                    direct.first->previous_node = current;
                }
            }
        }
        current->color = global_black;
    }

    end_loop:
    if (end->second->previous_node == nullptr)
    {
        reset_color();
        return {};
    }
    //adding result;
    auto current = end->second;
    std::vector<Point_ptr> temp;
    while (current != start->second)
    {
        temp.push_back(current);
        current = current->previous_node;
    }
    temp.push_back(current);
    std::reverse(temp.begin(),temp.end());
    std::vector<std::tuple<StopID,RouteID, Distance>> result;
    for (auto iter = temp.begin(); iter != temp.end()-1; iter++)
    {
        auto find_stop = (*iter)->next_stop.find(*(iter+1));
        result.push_back({(*iter)->id, find_stop->second.first, find_stop->second.second});
    }
    result.push_back({end->second->id, NO_ROUTE, 0});

    for (auto iter = result.rbegin(); iter != result.rend()-1;iter++)
    {
        std::get<2>(*iter) = std::get<2>(*(iter+1));
    }

    std::get<2>(result.front()) = 0;
    for (auto iter = result.begin()+1; iter != result.end();iter++)
    {
        std::get<2>(*iter) += std::get<2>(*(iter-1));
    }
    reset_color();
    return result;
}

bool Datastructures::add_trip(RouteID routeid, std::vector<Time> const& stop_times)
{
    auto trip = routes.find(routeid);
    if (trip == routes.end())
    {
        return false;
    }
    if (trip->second.size() != stop_times.size())
    {
        return false;
    }
    auto time_iter = stop_times.end()-1;
    auto next_stop_iter = mp.find(trip->second.back());
    next_stop_iter->second->stop_time.insert({nullptr, {routeid, *time_iter, NO_TIME}});
    for (auto iterator = trip->second.rbegin()+1; iterator != trip->second.rend(); iterator++,time_iter--)
    {
        auto find_stop = mp.find(*iterator);
        find_stop->second->stop_time.insert(
        { next_stop_iter->second, {routeid ,*(time_iter-1), *time_iter}});
        next_stop_iter = find_stop;
    }
    return true;
}

std::vector<std::pair<Time, Duration>> Datastructures::route_times_from(RouteID routeid, StopID stopid)
{
    auto iter = mp.find(stopid);
    if (iter == mp.end())
    {
        return {{NO_TIME,NO_DURATION}};
    }
    auto route_iter = routes.find(routeid);
    if (route_iter == routes.end())
    {
        return {{NO_TIME,NO_DURATION}};
    }
    std::vector<std::pair<Time, Duration>> result;
    for (auto route = iter->second->stop_time.begin(); route != iter->second->stop_time.end(); route++)
    {
        if (std::get<0>(route->second) == routeid && route->first != nullptr)
        {
            result.push_back({std::get<1>(route->second),std::get<2>(route->second)-std::get<1>(route->second)});
        }
    }
    return result;
}

std::vector<std::tuple<StopID, RouteID, Time> > Datastructures::journey_earliest_arrival(StopID fromstop, StopID tostop, Time starttime)
{
    //this function is implemented followed by dijkstra algorithm
    auto start = mp.find(fromstop);
    if (start  == mp.end())
    {
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }
    auto end = mp.find(tostop);
    if (end == mp.end())
    {
        return {{NO_STOP, NO_ROUTE, NO_DISTANCE}};
    }
    if (fromstop == tostop)
    {
        return {};
    }

    struct compare_arrival_time
    {
        bool operator()(const Point_ptr& ptr1,const Point_ptr& ptr2)
        {
            return ptr1->arrive_time > ptr2->arrive_time;
        }
    };
    std::priority_queue<Point_ptr,std::vector<Point_ptr>,compare_arrival_time> visit;
    end->second->previous_node = nullptr;
    start->second->color = global_gray;
    start->second->arrive_time = starttime;
    visit.push(start->second);
    //last route is for tracking the last route followed
    while (visit.size() != 0)
    {
        auto current = visit.top();
        visit.pop();
        // stop when the optimal time_arrival is found
        if (current->id == tostop)
        {
            goto end_loop;
        }
        // Because of one route can have duplicate times of stop ( there are more than 2 trips in a day)
        // so there is problem that we are processing the route have been processed, this command is just
        // a small optimization
        if (*current->color == BLACK )
        {
            continue;
        }
        for (auto& direct : current->stop_time)
        {
            if (direct.first != nullptr && std::get<1>(direct.second) >= current->arrive_time)
            {
                if (*(direct.first)->color == WHITE)
                {
                    direct.first->color = global_gray;
                    direct.first->arrive_time = std::get<2>(direct.second);
                    direct.first->previous_node = current;
                    visit.push(direct.first);
                }
                if (direct.first->arrive_time > std::get<2>(direct.second))
                {
                    direct.first->arrive_time = std::get<2>(direct.second);
                    direct.first->previous_node = current;
                    visit.push(direct.first);
                }
            }
        }
        current->color = global_black;
    }

    end_loop:
    if (end->second->previous_node == nullptr)
    {
        reset_color();
        return {};
    }

    //adding result;
    auto current = end->second;
    std::vector<Point_ptr> temp;
    while (current != start->second)
    {
        temp.push_back(current);
        current = current->previous_node;
    }
    temp.push_back(current);
    std::reverse(temp.begin(),temp.end());
    std::vector<std::tuple<StopID,RouteID, Time>> result;
    Time  depart = starttime;
    Time last_arrival = starttime;
    // make sure that we give the right result that have the right department time
    //due to there are multiple routes have two stops consecutive are identical and
    // there are multiple trips during a day.
    for (auto iter = temp.begin(); iter != temp.end()-1; iter++)
    {
        auto pair = (*iter)->stop_time.equal_range(*(iter+1));
        if (std::distance(pair.first,pair.second) == 1)
        {
            depart = std::get<1>(pair.first->second);
            result.push_back({(*iter)->id, std::get<0>(pair.first->second),std::get<1>(pair.first->second)});
            if (iter == temp.end()-2)
                last_arrival = std::get<2>(pair.first->second);
        }
        else
        {
            auto find_route = pair.first;
            while (find_route != pair.second)
            {
                if (depart <= std::get<1>(find_route->second) )
                {
                    break;
                }
                find_route++;
            }
            for (auto route = find_route; route != pair.second; route++)
            {
                if (depart <= std::get<1>(route->second)
                        && std::get<2>(route->second) < std::get<2>(find_route->second))
                {
                    find_route = route;
                }
            }
            depart = std::get<2>(find_route->second);
            result.push_back({(*iter)->id, std::get<0>(find_route->second), std::get<1>(find_route->second)});
            if (iter == temp.end()-2)
                last_arrival = std::get<2>(find_route->second);
        }
    }
    result.push_back({end->second->id, NO_ROUTE, last_arrival});
    reset_color();
    return result;
}

void Datastructures::add_walking_connections()
{
    // Replace this comment and the line below with your implementation
}
