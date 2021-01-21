#include <QCoreApplication>
#include <QDebug>

#include "nlohmann/json.hpp"

using nlohmann::json;


#define MY_JSON_TO(v1) nlohmann_json_j[#v1] = nlohmann_json_t._##v1;
#define MY_JSON_FROM(v1) nlohmann_json_j.at(#v1).get_to(nlohmann_json_t._##v1);

#define MY_DEFINE_TYPE_INTRUSIVE(Type, ...)  \
    friend void to_json(nlohmann::json& nlohmann_json_j, const Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(MY_JSON_TO, __VA_ARGS__)) } \
    friend void from_json(const nlohmann::json& nlohmann_json_j, Type& nlohmann_json_t) { NLOHMANN_JSON_EXPAND(NLOHMANN_JSON_PASTE(MY_JSON_FROM, __VA_ARGS__)) }

class Car {

public:
    std::string _carname;
    int _maxSpeed;
    bool _old;


    MY_DEFINE_TYPE_INTRUSIVE (Car, carname, maxSpeed, old)
};


class SportCar : public Car{
public:
    int _number;
    std::vector<int> _brakes;
    MY_DEFINE_TYPE_INTRUSIVE (SportCar, number, brakes, carname, maxSpeed, old)

    virtual void from_json (const json& j){
        *this = j;
    }
    virtual void to_json (json& j){
        j = *this;
    }
};


class Garage {
public:
    SportCar _sportcar;
    Car _car;

    MY_DEFINE_TYPE_INTRUSIVE (Garage, sportcar, car)
};


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    json j, j1, j2;

    Car car;
    car._carname = "test_1";
    car._old = false;
    car._maxSpeed = 100;
    j1 = car;

    SportCar sportcar;
    sportcar._carname = "test_2";
    sportcar._old = true;
    sportcar._maxSpeed = 250;
    sportcar._number = 777;
    for (int i = 0; i < 10; ++i){
        sportcar._brakes.push_back(i);
    }
    j2 = sportcar;

    Garage garage;
    garage._car = car;
    garage._sportcar = sportcar;

    j = garage;

    std::string s1 = j1.dump();
    std::string s2 = j2.dump();
    std::string s = j.dump();

    qDebug() << s1.c_str();
    qDebug() << s2.c_str();
    qDebug() << s.c_str();

    return a.exec();
}
