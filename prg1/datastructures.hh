// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
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
using Name = std::string;

// Return values for cases where required thing was not found
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
inline bool operator!=(Coord c1, Coord c2) { return !(c1 == c2); } // Not strictly necessary

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



class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: Theta(1)
    // Short rationale for estimate: return size vector costs constant time
    int stop_count();

    // Estimate of performance: O(m) (m is the number of regions)
    // Short rationale for estimate: clear the subpoints and the sub regions of
    // every regions.
    void clear_all();

    // Estimate of performance: Theta(n)
    // Short rationale for estimate: Go through elements of a vector costs linear
    // amount of time with size of vector;
    std::vector<StopID> all_stops();

    // Estimate of performance: O(1) before any funtions that related to stops name,
    // After that O(logn)
    // Short rationale for estimate: adding element to unordered_map average O(1),
    // then if the mapname_is_added, then adding costs is log(n) because it is
    // ordered_map
    bool add_stop(StopID id, Name const& name, Coord xy);

    // Estimate of performance: average O(1)
    // Short rationale for estimate: searching key in unordered_map
    Name get_stop_name(StopID id);

    // Estimate of performance: average O(1)
    // Short rationale for estimate: searching key in unordered_map
    Coord get_stop_coord(StopID id);

    // Estimate of performance: O(nlogn) first time, then O(n)
    // Short rationale for estimate: when the namemap hasnot been added,
    // add elements to ordered map overall
    // cost nlogn first time, then using ordered map, listing all id cost linear time
    // for the next time function is called.
    std::vector<StopID> stops_alphabetically();

    // Estimate of performance: O(nlogn) sometimes O(n)
    // Short rationale for estimate: sorting the vector costs nlogn.
    std::vector<StopID> stops_coord_order();

    // Estimate of performance: theta(1)
    // Short rationale for estimate: access pointer point to min_coord costs constant time
    StopID min_coord();

    // Estimate of performance: theta(1)
    // Short rationale for estimate: access pointer point to max_coord costs constant time
    StopID max_coord();

    // Estimate of performance: O(logn), if all the stops has the same name,
    // the listing element costs linear time (O(n)).
    // Short rationale for estimate: searching in ordered map by function equal_range
    // costs logn, listing elements constant time if not all stops have the same name.
    std::vector<StopID> find_stops(Name const& name);

    // Estimate of performance: average O(1) before the namemap has not been added, then O(logn)
    // Short rationale for estimate: changing in unordered map costs average O(1),
    // then searching and changing in ordered map cost logn, if the namemap has been added.
    bool change_stop_name(StopID id, Name const& newname);

    // Estimate of performance: most case average O(1), if change in coord_max/min, O(n)
    // Short rationale for estimate: if the max/min doesnot change, then changing in unordered map
    // and in vector that already have pointer costs constant time, if change max/min,
    // we have to find max/min again, cost O(n).
    bool change_stop_coord(StopID id, Coord newcoord);

    // Estimate of performance: average O(1)
    // Short rationale for estimate: insert to unordered map.
    bool add_region(RegionID id, Name const& name);

    // Estimate of performance: average O(1)
    // Short rationale for estimate: searching key in unordered_map
    Name get_region_name(RegionID id);

    // Estimate of performance: theta(m)
    // Short rationale for estimate: go through elements in a map takes linear time.
    std::vector<RegionID> all_regions();

    // Estimate of performance: average O(1)
    // Short rationale for estimate: searching key in the unoredered maps.
    bool add_stop_to_region(StopID id, RegionID parentid);

    // Estimate of performance: average O(1)
    // Short rationale for estimate: searching keys in the unordered_map.
    bool add_subregion_to_region(RegionID id, RegionID parentid);

    // Estimate of performance: average O(m) : m is the regions that the stop belongs to
    // Short rationale for estimate: searching stop in ordered map to know what is region.
    // then find the pointer of that region, then by that pointer findout what is its parents
    // until it is the most unbounded region. So it is linear with the number of region.
    std::vector<RegionID> stop_regions(StopID id);


    // Estimate of performance: theta(1)
    // Short rationale for estimate: Do nothing.
    void creation_finished();

    // Estimate of performance: theta(n) : n is the number of points belong to that region
    // Short rationale for estimate: recursive all the points in that region and subregions.
    std::pair<Coord, Coord> region_bounding_box(RegionID id);

    // Estimate of performance: theta(n)
    // Short rationale for estimate: function n_th element cost nlog(6).
    std::vector<StopID> stops_closest_to(StopID id);

    // Estimate of performance: most case O(1) before namemap is added, then most case O(logn)
    // remove max/min always costs linear amount of time.
    // Short rationale for estimate: remove in unordered map cost constant time, then searching and
    // remove in the namemap (ordered_map) costs logn
    // if remove min/max, then we have to find min/max again, cost linear amount of time
    bool remove_stop(StopID id);

    // Estimate of performance: average O(min(h1,h2))
    // Short rationale for estimate: find the regions that contain that two stop.
    RegionID stops_common_region(StopID id1, StopID id2);

private:
    using V_ptr = std::shared_ptr<std::pair<StopID,Coord>>;
    struct Point{
        StopID id;
        Name name;
        Coord coord;
        RegionID r_id = NO_REGION;
        // this pointer is the pointer of the pair that stores in the vector.
        V_ptr ptr_v;
        //constructor
        Point(const StopID& id_,const Name& name_,const Coord& coord_,
        const V_ptr ptr_v_)
        {
            id = id_;
            name = name_;
            coord = coord_;
            ptr_v = ptr_v_;
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
};

#endif // DATASTRUCTURES_HH
