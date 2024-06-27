#include "geodb.h"
using namespace std;

string convertDoubleToString(double value) {
    ostringstream oss;
    oss << fixed << setprecision(7) << value; // Set precision as needed
    return oss.str();
}

bool GeoDatabase::load(const std::string& map_data_file) {
    ifstream file(map_data_file);
    if(!file) return false; // Ensure the file is open

    string line;
    
    while (getline(file, line)) { // Street name
        string streetName = line;

        if (!getline(file, line)) break; // GeoPoints line
        istringstream iss(line);
        double lat1, lon1, lat2, lon2;
        iss >> lat1 >> lon1 >> lat2 >> lon2;
        
        if(!getline(file, line)) break;  // POI line
        istringstream issPoi(line);
        int poiCount;
        issPoi >> poiCount;
        
        GeoPoint start{convertDoubleToString(lat1), convertDoubleToString(lon1)};
        GeoPoint end{convertDoubleToString(lat2), convertDoubleToString(lon2)};
        
        string startKey = start.to_string();
        string endKey = end.to_string();
        
        //connect start to end and add street names
        connections[startKey].push_back(end);
        connections[endKey].push_back(start);
        streetNames[startKey + endKey] = streetName;
        streetNames[endKey + startKey] = streetName;

        GeoPoint mid = midpoint(start, end);
        string midpointKey = mid.to_string();
        
        // update connections and street names with midpoint
        connections[startKey].push_back(mid);
        connections[midpointKey].push_back(start);
        connections[endKey].push_back(mid);
        connections[midpointKey].push_back(end);
        // to_string serializes keys with a comma sign already
        streetNames[startKey + midpointKey] = streetName;
        streetNames[midpointKey + startKey] = streetName;
        streetNames[endKey + midpointKey] = streetName;
        streetNames[midpointKey + endKey] = streetName;
        
        for(int i = 0; i < poiCount; ++i) {
            // filter poi information using stringstream
            getline(file, line);
            size_t delim = line.find('|');
            string poiName = line.substr(0, delim);
            string poiCoords = line.substr(delim + 1);
            istringstream poiIss(poiCoords);
            double poiLat, poiLon;
            poiIss >> poiLat >> poiLon;
            GeoPoint poi(convertDoubleToString(poiLat), convertDoubleToString(poiLon));
            poiLocations[poiName] = poi;
            
            // update connections for poi to midpoint and vice versa
            string poiKey = poi.to_string();
            connections[poiKey].push_back(mid);
            connections[midpointKey].push_back(poi);
            streetNames[poiKey + midpointKey] = "a path";
            streetNames[midpointKey + poiKey] = "a path";
        }
    }
    return true;
}

bool GeoDatabase::get_poi_location(const string& poi, GeoPoint& point) const{
    auto poiLocation = poiLocations.find(poi);
    if(poiLocation){
        point = *poiLocation;
        return true;
    }
    return false;
}

vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const{
    string key = pt.to_string();
    const vector<GeoPoint>* connect = connections.find(key);
    if(connect){return *connect;}
    //return empty vector if key not found
    return {};
}

string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const{
    string key = pt1.to_string() + pt2.to_string(); // hashed st name
    const string* streetName = streetNames.find(key);
    if(streetName){return *streetName;}
    return ""; //empty street name if not found
}
