#include "Schema.h"

Schema::Schema(QObject *p) : QObject(p) {}

void Schema::addResource(int id, const std::string &name)
{
    res_.emplace_back(id, name);
}

Schema::Resource::Resource(int id, const std::string &name) :
    id(id), name(name) {}
