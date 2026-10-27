#include "CS.h"
using namespace std;

int CS::maxId = 0;

CS::CS()
{
    id = ++maxId;
    name = "Unknown";
    workshop = 0;
    activworkshop = 0;
    class_station = 0;
}

ostream& operator << (ostream& out, const CS& s)
{
    out << "КС id=" << s.id << " | " << s.name << " | цехов=" << s.workshop
        << " | в работе=" << s.activworkshop << " | класс=" << s.class_station;
    return out;
}

istream& operator >> (istream& in, CS& s)
{
    cout << "Введите название КС: ";
    getline(in >> ws, s.name);

    cout << "Введите количество цехов: ";
    in >> s.workshop;

    cout << "Введите количество цехов в работе: ";
    in >> s.activworkshop;

    cout << "Введите класс станции (6 или 7): ";
    in >> s.class_station;

    return in;
}

void CS::save(std::ostream& file) const
{
    file << id << '\n'
        << name << '\n'
        << workshop << '\n'
        << activworkshop << '\n'
        << class_station << '\n';
}

void CS::load(std::istream& file)
{
    file >> id;
    getline(file >> ws, name);
    file >> workshop >> activworkshop >> class_station;

    if (id >= maxId)
    {
        maxId = id + 1;
    }
}