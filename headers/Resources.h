#pragma once
#include "Enums.h"
#include "Production.h"

#include <string>
#include <memory>
#include <optional>
#include <chrono>
#include <array>
#include <limits>

class Resources
{
public:
    Resources();

    ~Resources();
    
    void read_from_config(const std::string& section, const char* name = nullptr, const char* limitName = nullptr);

    //set and get individual resource
    void set_resource(RESOURCETYPES type, double res);
    double get_resource(RESOURCETYPES type) const;

    void set_limit(RESOURCETYPES type, double limit);
    double get_limit(RESOURCETYPES type) const;

    //returns a Resources object that is ticking behind the actual resources
    //to make it nice to display
    int get_display(RESOURCETYPES type) const;

    //sets all resources to 0
    void set_empty();
    //checks if all resources are 0
    bool empty();

    //subtracts cost resources if possible, returns true on success
    //cost is unchanged
    //can't be const cause for some reason operator[] of vector is not const
    bool sub(const Resources& cost);
    //if it is possible to subtract the resources return true, otherwise false
    bool can_sub(const Resources& cost);

    //adds resources, checking for limits
    void add(const Resources& income);

    Resources operator/(const double &d) const;

    //transfers all the resources it can into this object taking into account the limit
    //the source is emptied during this
    //returns true if the source is empty afterwards
    //false otherwise
    void transfer(Resources& source);

    //transfers all resources according to the production given
    void transfer(Resources& source, const Production& inout);

    //transfers one resource type
    //returns true if the source is empty afterwards
    //false otherwise
    void transfer(RESOURCETYPES type, Resources &r);

    //returns a string literal containing the name of the resource as written in config etc
    static std::string to_string(RESOURCETYPES type);

    //returns a string literal containing the name of the resource for display to the player
    static std::string get_name(RESOURCETYPES type);

    class Res {
    public:
        Res();

        // sets the resource to the given value
        void set(double res);
        double get() const;

        void set_limit(double res);
        double get_limit() const;

        int get_display() const;

        bool empty() const;

        void add(const Res& r);
        bool can_sub(const Res& r) const;
        void sub(const Res& r);
        
        void transfer(Res& source);
        void transfer(Res& source, PRODUCTIONSTATE inout);

    private:

        // currently stored resources
        double mRes;

        // the limit to the resources
        double mLimit;

        // the previous resources
        double mPrev_res;

        //the last time point this was changed at
        std::chrono::time_point<std::chrono::system_clock> mLast_change;
    };

private:

    // array of the different resource types
    std::array<Res, RESOURCES_TOTAL> mResources;
};
