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

Schema::Resource::Resource(int id, const std::string &name) :
    id(id), name(name) {}

Schema::Mine::Mine(int level, int speed, int vol) :
    level(level), speed(speed), volume(vol) {}
