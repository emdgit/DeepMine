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

    explicit Schema(QObject *p = nullptr);

    void addResource(int id, const std::string &name);


private:

    std::vector<Resource> res_;

};
