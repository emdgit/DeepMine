#pragma once

#include <QObject>

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

    explicit Schema(QObject *p = nullptr);

    void addResource(int id, const std::string &name);
    void addMine(int level, int speed, int vol);


private:

    std::vector<Resource> res_;
    std::vector<Mine> mines_;

};
