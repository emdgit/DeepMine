#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "yaml-cpp/yaml.h"

#include "Schema.h"

void read_schema(Schema &schema);
void read_resources(Schema &schema, const YAML::Node &node);
void read_mines(Schema &schema, const YAML::Node &node);

using node_pair = std::pair<YAML::Node, YAML::Node>;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    Schema schema;

    read_schema(schema);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}

void read_schema(Schema &schema)
{
    QFile schema_file(":/schema.yaml");
    std::string data;

    if (schema_file.open(QFile::ReadOnly)) {
        data = schema_file.readAll().data();
        schema_file.close();
    } else {
        throw std::runtime_error("Cannot open schema file");
    }

    YAML::Node root = YAML::Load(data);

    if (root.Type() != YAML::NodeType::Map) {
        throw std::runtime_error("Invalid schema format. Document should be a Map.");
    }

    for (const auto &it : root) {
        const node_pair &p = it;
        if (p.first.IsScalar()) {
            if (p.first.Scalar() == "Resources") {
                if (!p.second.IsSequence()) {
                    throw std::runtime_error("Invalid schema format. "
                                             "Resource section should be a sequence.");
                }

                read_resources(schema, p.second);
            } else if (p.first.Scalar() == "Mines") {
                if (!p.second.IsSequence()) {
                    throw std::runtime_error("Invalid schema format. "
                                             "Mines section should be a sequence.");
                }

                read_mines(schema, p.second);
            } else if (p.first.Scalar() == "Locations") {

            } else {
                throw std::runtime_error("Invalid schema format. Unexpected section " +
                                         p.first.Scalar());
            }
        }
    }
}

void read_resources(Schema &schema, const YAML::Node &node)
{
    // node is a sequence

    for (const auto &it : node) {
        const YAML::Node &p = it;
        if (!p.IsMap() || p.size() != 2) {
            throw std::runtime_error("Invalid schema format. "
                                     "Each element of a resource section "
                                     "should be a Map with size = 2.");
        }

        int res_id;
        std::string res_name;

        for (const auto &res : p) {
            const node_pair &res_pair = res;
            if (!res_pair.first.IsScalar() ||
                    !res_pair.second.IsScalar()) {
                throw std::runtime_error("Invalid schema format. "
                                         "Each element of a resource map "
                                         "should be a pair of sclar / scalar");
            }

            if (res_pair.first.Scalar() == "id") {
                res_id = atoi(res_pair.second.Scalar().data());
            } else if (res_pair.first.Scalar() == "name") {
                res_name = res_pair.second.Scalar();
            } else {
                throw std::runtime_error("Invalid schema format. "
                                         "Unexpected resource key");
            }
        }

        schema.addResource(res_id, res_name);
    }
}

void read_mines(Schema &schema, const YAML::Node &node)
{
    // node is a sequence

    for (const auto &it : node) {
        const YAML::Node &p = it;
        if (!p.IsMap() || p.size() != 3) {
            throw std::runtime_error("Invalid schema format. "
                                     "Each element of a mines section "
                                     "should be a Map with size = 3.");
        }

        int mine_level, mine_speed, mine_volume;

        for (const auto &res : p) {
            const node_pair &res_pair = res;
            if (!res_pair.first.IsScalar() ||
                    !res_pair.second.IsScalar()) {
                throw std::runtime_error("Invalid schema format. "
                                         "Each element of a resource map "
                                         "should be a pair of sclar / scalar");
            }

            if (res_pair.first.Scalar() == "level") {
                mine_level = atoi(res_pair.second.Scalar().data());
            } else if (res_pair.first.Scalar() == "speed") {
                mine_speed = atoi(res_pair.second.Scalar().data());
            } else if (res_pair.first.Scalar() == "volume") {
                mine_volume = atoi(res_pair.second.Scalar().data());
            } else {
                throw std::runtime_error("Invalid schema format. "
                                         "Unexpected mine's key");
            }
        }

        schema.addMine(mine_level, mine_speed, mine_volume);
    }
}
