// https://open.kattis.com/problems/roadtrip2

#include <iostream>
#include <list>
#include <map>
#include <unordered_map>

class RoadTrip {

private:

  struct City; // forward declaration due to interdependence between Road and City

  struct Road {
    std::list<City>::iterator destination;
    unsigned travel_duration;
  };

  struct City {
    std::multimap<unsigned,Road> roads;
    std::string name;
    unsigned visit_duration;
    unsigned last_visited;
    bool has_been_visited;
  };

  const unsigned city_timeout; // the amount of time before a city can be revisited
  const unsigned max_trip_duration;

  std::list<City>::iterator current_city;
  unsigned current_time;

  std::list<City> cities;
  std::unordered_map<unsigned,std::list<City>::iterator> city_id_map;

public:

  RoadTrip(unsigned city_timeout, unsigned max_trip_duration) : 
    city_timeout(city_timeout),
    max_trip_duration(max_trip_duration) { }

  void addCity(unsigned id, const std::string& name, unsigned visit_duration) {
    cities.emplace_front();
    auto city = cities.begin();
    city->name = name;
    city->visit_duration = visit_duration;
    city->has_been_visited = false;
    city_id_map[id] = city;
  }

  void addRoad(unsigned city1_id, unsigned city2_id, unsigned travel_duration) {
    // roads are bi-directional so they must be registered for both cities
    auto city1 = city_id_map[city1_id];
    auto city2 = city_id_map[city2_id];
    city1->roads.insert({travel_duration,{city2,travel_duration}});
    city2->roads.insert({travel_duration,{city1,travel_duration}});
  }

  const City* beginTrip(unsigned id) {
    current_city = city_id_map[id];
    current_time = current_city->visit_duration;
    current_city->has_been_visited = true;
    current_city->last_visited = 0;
    return &*current_city;
  }

  const City* nextCity() {
    for (auto pair : current_city->roads) {
      auto road = pair.second;
      auto destination = road.destination;

      // decide a city is worth visiiting based on rule #2 of the problem
      bool worth_visit = (
        !destination->has_been_visited ||
        current_time + road.travel_duration - destination->last_visited >= city_timeout
      );

      // decide if there time remaining to make the trip
      bool enough_time = current_time + road.travel_duration + destination->visit_duration <= max_trip_duration;

      // if both coniditions satisfied travel to the city
      if (worth_visit && enough_time) {
        current_time += road.travel_duration + destination->visit_duration;
        destination->has_been_visited = true;
        destination->last_visited = current_time;
        current_city = destination;
        return &*current_city;
      }
    }
    return NULL;
  }

  unsigned currentTime() {
    return current_time;
  }

};

int main() {
  unsigned city_count, road_count, city_timeout, max_trip_duration, starting_city_id;
  std::cin >> city_count >> road_count >> city_timeout >> max_trip_duration >> starting_city_id;

  RoadTrip roadTrip(city_timeout, max_trip_duration);

  // load cities
  for (unsigned i = 0; i < city_count; i++) {
    unsigned city_id, city_visit_duration;
    std::string city_name;
    std::cin >> city_id >> city_name >> city_visit_duration;
    roadTrip.addCity(city_id, city_name, city_visit_duration);
  }

  // load roads
  for (unsigned i = 0; i < road_count; i++) {
    unsigned city1_id, city2_id, road_travel_duration;
    std::cin >> city1_id >> city2_id >> road_travel_duration;
    roadTrip.addRoad(city1_id, city2_id, road_travel_duration);
  }

  // perform road trip
  auto city = roadTrip.beginTrip(starting_city_id);
  do {
    std::cout << city->name << " ";
    city = roadTrip.nextCity();
  } while (city);
  std::cout << std::endl << roadTrip.currentTime() << std::endl;

  return 0;
}
