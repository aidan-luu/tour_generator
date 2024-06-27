#ifndef tour_generator_h
#define tour_generator_h

#include "base_classes.h"
#include "geotools.h"

class TourGenerator : public TourGeneratorBase {
  public:
    TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router) : geoDB(geodb), router(router){};
    virtual ~TourGenerator(){};
    virtual std::vector<TourCommand> generate_tour(const Stops &stops) const;
  private:
    const GeoDatabaseBase& geoDB;
    const RouterBase& router;
};

#endif
