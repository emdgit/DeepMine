#include "Schema.h"

Schema::Schema(QObject *p) : QObject(p) {}

void Schema::addResource(int id, const std::string &name)
{
    res_.emplace_back(id, name);
}

void Schema::addMine(int level, int speed, int vol)
{
    mines_.emplace_back(level, speed, vol);
}

void Schema::addLocation(const Location &loc)
{
    locations_.push_back(loc);
}

Schema::Resource::Resource(int id, const std::string &name) :
    id(id), name(name) {}

Schema::Mine::Mine(int level, int speed, int vol) :
    level(level), speed(speed), volume(vol) {}

void Schema::Location::addPartition(int resId, int prob)
{
    res_partitions.emplace(resId, prob);
}

void Schema::Location::setExtra(std::list<int> &&ids,
                                const std::string &ratio)
{

}
