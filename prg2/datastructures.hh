// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <memory>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <map>
#include <unordered_set>

// Types for IDs
using StopID = long int;
using RegionID = std::string;
using RouteID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
RouteID const NO_ROUTE = "!!NO_ROUTE!!";
StopID const NO_STOP = -1;
RegionID const NO_REGION = "!!NO_REGION!!";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};
double square_distance(Coord c1, Coord c2);

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.x*c1.x + c1.y*c1.y < c2.x*c2.x + c2.y*c2.y)
        {return true;}
    else if (c1.x*c1.x + c1.y*c1.y > c2.x*c2.x + c2.y*c2.y)
        {return false;}
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for time of day in minutes from midnight (i.e., 60*hours + minutes)
using Time = int;

// Return value for cases where color was not found
Time const NO_TIME = std::numeric_limits<Time>::min();

// Type for a duration of time (in minutes)
using Duration = int;

// Return value for cases where Duration is unknown
Duration const NO_DURATION = NO_VALUE;

// Type for a distance (in metres)
using Distance = int;
Distance const MAX_DISTANCE = std::numeric_limits<int>::max();

// Return value for cases where Duration is unknown
Distance const NO_DISTANCE = NO_VALUE;


// This is the class you are supposed to implement
class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance
    // Short rationale for estimate:
    int stop_count();

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_all();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StopID> all_stops();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_stop(StopID id, Name const& name, Coord xy);

    // Estimate of performance:
    // Short rationale for estimate:
    Name get_stop_name(StopID id);

    // Estimate of performance:
    // Short rationale for estimate:
    Coord get_stop_coord(StopID id);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StopID> stops_alphabetically();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StopID> stops_coord_order();

    // Estimate of performance:
    // Short rationale for estimate:
    StopID min_coord();

    // Estimate of performance:
    // Short rationale for estimate:
    StopID max_coord();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StopID> find_stops(Name const& name);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_stop_name(StopID id, Name const& newname);

    // Estimate of performance:
    // Short rationale for estimate:
    bool change_stop_coord(StopID id, Coord newcoord);

    // We recommend you implement the operations below only after implementing the ones above

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_region(RegionID id, Name const& name);

    // Estimate of performance:
    // Short rationale for estimate:
    Name get_region_name(RegionID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<RegionID> all_regions();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_stop_to_region(StopID id, RegionID parentid);

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<RegionID> stop_regions(StopID id);

    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    void creation_finished();

    // Estimate of performance:
    // Short rationale for estimate:
    std::pair<Coord, Coord> region_bounding_box(RegionID id);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StopID> stops_closest_to(StopID id);

    // Estimate of performance:
    // Short rationale for estimate:
    bool remove_stop(StopID id);

    // Estimate of performance:
    // Short rationale for estimate:
    RegionID stops_common_region(StopID id1, StopID id2);

    // Phase 2 operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<RouteID> all_routes();

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_route(RouteID id, std::vector<StopID> stops);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<RouteID, StopID>> routes_from(StopID stopid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<StopID> route_stops(RouteID id);

    // Estimate of performance:
    // Short rationale for estimate:
    void clear_routes();

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_any(StopID fromstop, StopID tostop);

//    // Non-compulsory operations

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_least_stops(StopID fromstop, StopID tostop);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_with_cycle(StopID fromstop);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<StopID, RouteID, Distance>> journey_shortest_distance(StopID fromstop, StopID tostop);

    // Estimate of performance:
    // Short rationale for estimate:
    bool add_trip(RouteID routeid, const std::vector<Time> &stop_times);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::pair<Time, Duration> > route_times_from(RouteID routeid, StopID stopid);

    // Estimate of performance:
    // Short rationale for estimate:
    std::vector<std::tuple<StopID, RouteID, Time>> journey_earliest_arrival(StopID fromstop, StopID tostop, Time starttime);

    // Estimate of performance:
    // Short rationale for estimate:
    void add_walking_connections(); // Note! This method is completely optional, and not part of any testing

private:
    //phase 2 : set color
    using Color = int;
    const Color WHITE = 0,
                GRAY   = 1,
                BLACK = -1;
    std::shared_ptr<Color> global_white = std::make_shared<Color>(WHITE);
    std::shared_ptr<Color> global_gray = std::make_shared<Color>(GRAY);
    std::shared_ptr<Color> global_black = std::make_shared<Color>(BLACK);
    void reset_color();

    using V_ptr = std::shared_ptr<std::pair<StopID,Coord>>;
    struct Point{
        StopID id;
        Name name;
        Coord coord;
        RegionID r_id = NO_REGION;
        // this pointer is the pointer of the pair that stores in the vector.
        V_ptr ptr_v;
        //phase2:
        using Point_ptr =  std::shared_ptr <Point>;
        std::unordered_multimap<Point_ptr,std::pair<RouteID, Distance>> next_stop;
        //a map that has keys are RouteID, point to the next stop pointer, including depart time and arrival time.
        std::unordered_multimap<Point_ptr, std::tuple<RouteID, Time, Time>> stop_time;
        Point_ptr previous_node;
        std::shared_ptr <Color> color;
        Distance d;
        Time arrive_time;
        //constructor
        Point(const StopID& id_,const Name& name_,const Coord& coord_, V_ptr ptr_v_):
            id(id_),
            name(name_),
            coord(coord_),
            ptr_v(ptr_v_)
        {

        }
    };
    using Point_ptr =  std::shared_ptr <Point>;
    //this map contains keys are name, then values are stop_id.
    std::multimap <Name, StopID > namemap;
    //this map contains keys are stop_id, then values are pointer point to Point.
    std::unordered_map < StopID,Point_ptr > mp;
    //this vector contains pair stop_id and coord.
    std::vector<V_ptr>  id_to_coordinate;
    //if vector is sorted by coord, then this value is true;
    bool vector_is_sorted = false;
    //if the namemap is inserted, then this value is true;
    bool namemap_is_added = false;
    Point_ptr coord_min = nullptr;
    Point_ptr coord_max = nullptr;

    struct Region{
        RegionID id;
        Name name;
        //this map contains subpoints of this region
        std::unordered_map <StopID,Point_ptr> subpoints;
        //this map contains subregions of this regions
        std::unordered_map <RegionID,std::shared_ptr<Region>> subregions;
        std::shared_ptr<Region> parent_region =nullptr;
        //constructor
        Region(RegionID id_, Name name_)
        {
            id = id_;
            name = name_;
        }
    };
    using Region_ptr = std::shared_ptr<Region>;
    //this map contain regions
    std::unordered_map <RegionID, Region_ptr> region_map;
    //this function is recursive for finding the bounding box of a region.
    std::pair<Coord,Coord> recursive_region_bounding_box(Region_ptr region);

    // phase 2:
    using Route = std::vector <StopID>;
    std::unordered_map<RouteID,Route> routes;
};


#endif // DATASTRUCTURES_HH
