#include "router.h"
using namespace std;

// converts using stringstream with ',' as delimiter
GeoPoint StringToGeoPoint(const string& str) {
    istringstream stream(str);
    string lat, lon;
    getline(stream, lat, ',');
    getline(stream, lon);
    return GeoPoint(lat, lon);
}

vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const {
    // route to same point
    if(pt1.to_string() == pt2.to_string()){return {pt1};}

    queue<GeoPoint> geoQueue;
    geoQueue.push(pt1);

    HashMap<string> locationOfPreviousWayPoint;

    // queue based implementation
    while(!geoQueue.empty()){
        GeoPoint curr = geoQueue.front();
        geoQueue.pop();
        
        // destination found
        if(curr.to_string() == pt2.to_string()){
            vector<GeoPoint> path;
            string currKey = pt2.to_string();
            
            path.push_back(pt2);
            
            //copy values into path by tracing through prev points
            while(currKey != pt1.to_string()){
                currKey = *locationOfPreviousWayPoint.find(currKey); // get prev key
                if (currKey.empty()) break; // no previous connections stop loop

                GeoPoint prevPoint = StringToGeoPoint(currKey);
                path.insert(path.begin(), prevPoint); // add prevPoint to the path
            }
            return path;
        }
        for(const auto& next : geoDB.get_connected_points(curr)){
            string nextKey = next.to_string();
            if(locationOfPreviousWayPoint.find(nextKey) == nullptr){ //not previously visited
                geoQueue.push(next);
                locationOfPreviousWayPoint.insert(nextKey, curr.to_string());
            }
        }
    }
    return {};  // no path found
}
