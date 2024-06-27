#ifndef geodb_h
#define geodb_h

#include "hashmap.h"
#include "geotools.h"
#include "base_classes.h"
#include <sstream>

class GeoDatabase : public GeoDatabaseBase{
  public:
    GeoDatabase(){};
    virtual ~GeoDatabase(){};
    
    virtual bool load(const std::string& map_data_file);
    virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const;
    virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
    virtual std::string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const;
  private:
    HashMap<GeoPoint> poiLocations;
    HashMap<std::vector<GeoPoint>> connections;
    HashMap<std::string> streetNames;
};

#endif
