#include "Schema.h"

#include <QString>
#include <QRegExp>

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

void Schema::Location::setExtra(std::vector<int> &&ids,
                                const std::string &ratio)
{
    if (res_partitions.find(0) == res_partitions.cend()) {
        throw std::runtime_error("Couldn't add extra. Resource "
                                 "map doesn't have 0 id res.");
    }

    const size_t s = ids.size();
    const QString ratio_str = QString::fromStdString(ratio);
    const QRegExp set_rx(QString("\\((\\d{1,2}-){%1}\\d{1,2}\\)").arg(s - 1));
    const QRegExp id_rx("\\d{1,2}");
    int pos(0);

    std::vector<int> probs(s, 0);

    while ((pos = set_rx.indexIn(ratio_str, pos)) != -1) {
        const QString ratio_set = set_rx.cap();

        int pos_(0), i(0);
        std::vector<int> extra_probs(s, 0);

        while ((pos_ = id_rx.indexIn(ratio_set, pos_)) != -1) {
            if (i >= s) {
                throw std::runtime_error("Extra string contains more "
                                         "elements than needed.");
            }

            extra_probs[i] = id_rx.cap().toInt();

            ++i;
            pos_ += id_rx.matchedLength();
        }

        if (i != s) {
            throw std::runtime_error("Extra string contains less "
                                     "elements than needed.");
        }

        for (size_t j(0); j < s; ++j) {
            probs[j] += extra_probs[j];
        }

        pos += set_rx.matchedLength();
    }

    auto all = std::accumulate(probs.cbegin(), probs.cend(), 0);

    for (auto &p : probs) {
        p = static_cast<float>(p) * 100.0f / all;
    }

    for (size_t i(0); i < s; ++i) {
        extra_partitions.emplace(ids[i], probs[i]);
    }
}
