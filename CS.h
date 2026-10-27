#pragma once
#include <iostream>
#include <string>

class CS
{
    static int maxId; 
    int id;
    std::string name;
    int workshop;
    int activworkshop;   
    int class_station;   

public:
    CS();

    static void reset_id() { maxId = 0; } 

  
    int get_id() const { return id; }
    const std::string& get_name() const { return name; }
    int get_workshop() const { return workshop; }
    int get_activworkshop() const { return activworkshop; }
    int get_class_station() const { return class_station; }


    void set_name(const std::string& val) { name = val; }
    void set_workshop(int val) { workshop = val; }
    void set_activworkshop(int val) { activworkshop = val; }
    void set_class_station(int val) { class_station = val; }

    void start_workshop() { if (activworkshop < workshop) activworkshop++; }
    void stop_workshop() { if (activworkshop > 0) activworkshop--; }

    void save(std::ostream& file) const;
    void load(std::istream& file);

    friend std::ostream& operator << (std::ostream& out, const CS& s);
    friend std::istream& operator >> (std::istream& in, CS& s);
};
