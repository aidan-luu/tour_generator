#include "tour_generator.h"
using namespace std;

//helper function that returns direction based on angle
string directionFromAngle(double angle) {
    if (angle >= 337.5 || angle < 22.5) return "east";
    if (angle >= 22.5 && angle < 67.5) return "northeast";
    if (angle >= 67.5 && angle < 112.5) return "north";
    if (angle >= 112.5 && angle < 157.5) return "northwest";
    if (angle >= 157.5 && angle < 202.5) return "west";
    if (angle >= 202.5 && angle < 247.5) return "southwest";
    if (angle >= 247.5 && angle < 292.5) return "south";
    if (angle >= 292.5 && angle < 337.5) return "southeast";
    return "unknown";
}

vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const {
    vector<TourCommand> tourCommands;
    
    // empty vector if no stops detected
    if(stops.size() == 0){return tourCommands;}
    
    TourCommand commentary;
    string poi, talkingPoints;
    
    for(int i = 0; i < stops.size()-1; ++i){
        
        if(!stops.get_poi_data(i, poi, talkingPoints)) continue;
        
        GeoPoint previousPOI; // load info of poi
        if(!geoDB.get_poi_location(poi, previousPOI)) continue;
        
        // Generate commentary for the current POI
        commentary.init_commentary(poi, talkingPoints);
        tourCommands.push_back(commentary);
        
        string poi2, talkingPoints2;
        if(!stops.get_poi_data(i+1, poi2, talkingPoints2)) continue;
        
        GeoPoint currentPOI;
        if(!geoDB.get_poi_location(poi2, currentPOI)) continue;
    
        // generate route from previous poi to current poi
        vector<GeoPoint> route = router.route(previousPOI, currentPOI);
        if (route.empty()) continue; // skip if no route found
        
        // Generate proceed and turn commands based on the route
        for(int j = 0; j < route.size() - 1; ++j){
            const GeoPoint& start = route[j];
            const GeoPoint& end = route[j + 1];
            
            string streetName = geoDB.get_street_name(start, end);
            double distance = distance_earth_miles(start, end);
            
            double directionAngle = angle_of_line(start, end);
            string direction = directionFromAngle(directionAngle);

            // proceed command
            TourCommand proceed;
            proceed.init_proceed(direction, streetName, distance, start, end);
            tourCommands.push_back(proceed);

            // check for a turn command if not last segment
            if (j + 2 < route.size()) {
                const GeoPoint& nextEnd = route[j + 2];
                double turnAngle = angle_of_turn(route[j], route[j + 1], nextEnd);
                string turnDirection;
                
                if(geoDB.get_street_name(end, nextEnd) != geoDB.get_street_name(start, end) && turnAngle != 0){
                    
                    if(turnAngle >= 1 && turnAngle < 180){turnDirection = "left";}
                    else if(turnAngle >= 180 && turnAngle < 360){turnDirection = "right";}
                    else{continue;} // straight
                    
                    TourCommand turn;
                    turn.init_turn(turnDirection, geoDB.get_street_name(end, nextEnd));
                    tourCommands.push_back(turn);
                }
            }
        }
    }
    
    // provide commentary on last POI
    stops.get_poi_data(stops.size()-1, poi, talkingPoints);
    commentary.init_commentary(poi, talkingPoints);
    tourCommands.push_back(commentary);
    
    return tourCommands;
}
