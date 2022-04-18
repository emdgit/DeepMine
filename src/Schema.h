#pragma once

#include <QObject>

#include <unordered_map>

class Schema : public QObject {

    Q_OBJECT

public:

    struct Resource {
        Resource(int id, const std::string &name);
        int id;
        std::string name;
    };

    struct Mine {
        Mine(int level, int speed, int vol);
        int level;
        int speed;
        int volume;
    };

    struct Location {

        Location() = default;

        void addPartition(int resId, int prob);
        void setExtra(std::list<int> &&ids,
                      const std::string &ratio);

        int number;
        bool hasOil = false;

        /// Res Id / Probability
        std::unordered_map<int, int> res_partitions;

        /// Res Id / Probability
        std::unordered_map<int, int> extra_partitions;
    };

    explicit Schema(QObject *p = nullptr);

    void addResource(int id, const std::string &name);
    void addMine(int level, int speed, int vol);
    void addLocation(const Location &loc);


private:

    std::vector<Resource> res_;
    std::vector<Mine> mines_;
    std::vector<Location> locations_;

};
