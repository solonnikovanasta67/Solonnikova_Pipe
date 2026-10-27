#include "Pipe.h"
using namespace std;

int Pipe::maxId = 0; 
Pipe::Pipe()
{
    id = ++maxId;
    d = 0;
    length = 0.0;
    name = "Unknown";
    remont = false;
}

ostream& operator << (ostream& out, const Pipe& p)
{
    out << "Труба id=" << p.id << " | " << p.name << " | диаметр=" << p.d
        << " | длина=" << p.length << " | в ремонте=" << (p.remont ? "да" : "нет");
    return out;
}

istream& operator >> (istream& in, Pipe& p)
{
    cout << "Введите название трубы: ";
    getline(in >> ws, p.name);

    cout << "Введите диаметр трубы: ";
    in >> p.d;

    cout << "Введите длину трубы: ";
    in >> p.length;

    cout << "Труба в ремонте? (1 - да, 0 - нет): ";
    int rep;
    in >> rep;
    p.remont = (rep == 1);

    return in;
}

void Pipe::save(std::ostream& file) const
{
    file << id << '\n'
        << d << '\n'
        << length << '\n'
        << name << '\n'
        << remont << '\n';
}

void Pipe::load(std::istream& file)
{
    file >> id >> d >> length;
    getline(file >> ws, name);
    file >> remont;

    if (id >= maxId)
    {
        maxId = id + 1;
    }
}