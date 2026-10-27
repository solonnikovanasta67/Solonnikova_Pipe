#pragma once
#include <iostream>
#include <string>

class Pipe
{
    static int maxId; 
    int id;
    int d;
    double length;
    std::string name;
    bool remont;

public:
    Pipe(); 

    static void reset_id() { maxId = 0; } 
    
    int get_id() const { return id; }
    int get_d() const { return d; }
    double get_length() const { return length; }
    const std::string& get_name() const { return name; }
    bool get_remont() const { return remont; }

    void set_name(const std::string& val) { name = val; }
    void set_d(int val) { d = val; }
    void set_length(double val) { length = val; }
    void set_remont(bool value) { remont = value; }

    
    void save(std::ostream& file) const;
    void load(std::istream& file);

    
    friend std::ostream& operator << (std::ostream& out, const Pipe& p);
    friend std::istream& operator >> (std::istream& in, Pipe& p);
};
