#include <string>
#include <sstream>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

using namespace std;

enum class QueryType {
  NewBus,
  BusesForStop,
  StopsForBus,
  AllBuses
};

struct Query {
  QueryType type;
  string bus;
  string stop;
  vector<string> stops;
};

istream& operator >> (istream& is, Query& q) {
  string operation_code;
  is >> operation_code;
  if (operation_code == "NEW_BUS") {
	  q.type = QueryType::NewBus;
      is >> q.bus;
      size_t stop_count;
      is >> stop_count;
      q.stops.resize(stop_count);
      for (string& stop : q.stops) {
        is >> stop;
      }

  } else if (operation_code == "BUSES_FOR_STOP") {
	  q.type = QueryType::BusesForStop;
      is >> q.stop;

  } else if (operation_code == "STOPS_FOR_BUS") {
	  q.type = QueryType::StopsForBus;
	  is >> q.bus;

  } else if (operation_code == "ALL_BUSES") {
	  q.type = QueryType::AllBuses;

  }
  return is;
}

struct BusesForStopResponse {
  vector<string> buses;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
	if (r.buses.empty()) {
	  os << "No stop";
	} else {
	  for (const string& bus : r.buses) {
		os << bus << " ";
	  }
	}
	return os;
}

struct StopsForBusResponse {
	vector<string> stops;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r) {
  for (const string& stop : r.stops) {
    os << stop << endl;
  }
	return os;
}

struct AllBusesResponse {
	map<string, vector<string>> buses;
};

ostream& operator << (ostream& os, const AllBusesResponse& r) {
	if (r.buses.empty()) {
		os << "No buses";
	} else {
		for (const auto& bus_item : r.buses) {
			os << "Bus " << bus_item.first << ": ";
			for (const string& stop : bus_item.second) {
				os << stop << " ";
			}
			os << '\n';
		}
	}
	return os;
}

class BusManager {
public:
  void AddBus(const string& bus, const vector<string>& stops) {
    for (const string& stop : stops) {
      stops_to_buses[stop].push_back(bus);
    }
    buses_to_stops[bus] = stops;
  }

  BusesForStopResponse GetBusesForStop(const string& stop) const {
	  auto entry = stops_to_buses.find(stop);
	  if(entry == stops_to_buses.end()) {
		  return {};
	  }
    return {entry->second};
  }

  StopsForBusResponse GetStopsForBus(const string& bus) const {
    StopsForBusResponse response;
	  auto entry = buses_to_stops.find(bus);
	  if(entry == buses_to_stops.end()) {
	    response.stops.push_back("No bus");
	  } else {
      for (const string& stop : entry->second) {
        std::ostringstream oss;
        oss << "Stop " << stop << ": ";
        auto buses = stops_to_buses.find(stop);
        if (buses->second.size() == 1) {
          oss << "no interchange";
        } else {
          for (const string& other_bus : buses->second) {
            if (bus != other_bus) {
              oss << other_bus << " ";
            }
          }
        }
        response.stops.push_back(oss.str());
      }
	  }
    return response;
  }

  AllBusesResponse GetAllBuses() const {
    return { buses_to_stops };
  }

private:
  map<string, vector<string>> buses_to_stops, stops_to_buses;
};

int main() {
  int query_count;
  Query q;

  cin >> query_count;

  BusManager bm;
  for (int i = 0; i < query_count; ++i) {
    cin >> q;
    switch (q.type) {
    case QueryType::NewBus:
      bm.AddBus(q.bus, q.stops);
      break;
    case QueryType::BusesForStop:
      cout << bm.GetBusesForStop(q.stop) << endl;
      break;
    case QueryType::StopsForBus:
      cout << bm.GetStopsForBus(q.bus);
      break;
    case QueryType::AllBuses:
      cout << bm.GetAllBuses();
      break;
    }
  }

  return 0;
}
