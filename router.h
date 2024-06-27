#ifndef router_h
#define router_h

#include "hashmap.h"
#include "base_classes.h"
#include <sstream>
#include <queue>

class Router : public RouterBase {
  public:
    Router(const GeoDatabaseBase& geo_db) : geoDB(geo_db){};
    virtual ~Router(){};
    virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const;
  private:
    const GeoDatabaseBase& geoDB;
};

#endif

