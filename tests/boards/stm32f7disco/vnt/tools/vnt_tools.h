
/*
 * Copyright (C) 2020 iCub Tech - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * NOTE:    this code was originally developed for
 *          the ventilator project
*/

// - include guard ----------------------------------------------------------------------------------------------------


#ifndef _VNT_TOOLS_H_
#define _VNT_TOOLS_H_

#include <cstdint>
#include <vector>
#include "vnt_core.h"

namespace vnt { namespace tools {
    
    class Histogram
    {
    public:
        
        struct Config
        {   // there are nsteps() intervals each containing .step values which fill the range [.min, ... , .max)
            std::uint64_t min {0};        // the start value of first interval.
            std::uint64_t max {0};        // the upper limit of all possible values (which is actually max-1).
            std::uint32_t step {0};       // the width of the interval 
            Config() = default;
            constexpr Config(std::uint64_t mi, std::uint64_t ma, std::uint32_t st) : min(mi), max(ma), step(st) {}
            std::uint64_t range() const { return max - min; }
            std::uint32_t nsteps() const { return ( (range() + step - 1) / step); }
            bool isvalid() const { return ((0 == step) || (min >= max)) ? false : true; }
        };
        
        struct Values
        {
            std::uint64_t               total {0};          // cumulative number = below + sum(inside) + beyond
            std::uint64_t               below {0};          // number of occurrences in ( -INF, config.min )
            std::uint64_t               beyond {0};         // number of occurrenced in [ inside.size() * config.step, +INF )
            std::vector<std::uint64_t>  inside;             // inside[i] contains the number of occurrences in [ config.min + i*config.step, config.min + (i+1)*config.step )  
            Values() = default;
        };
            
        
        Histogram();
        ~Histogram();
    
        bool init(const Config &config);
        
        bool add(std::uint64_t value);
        
        bool reset();  
        
        const vnt::tools::Histogram::Config * getconfig() const;
        const vnt::tools::Histogram::Values * getvalues() const;
        
        // it generates the pdf in a vector which is long Config::nsteps()+2 item. 
        // the first position contains probability that the value is < Config::min. 
        // the last position keeps probability that the value is >= Config::max. 
        // position i-th contain probability that value belongs inside [Config:min + i*Config::step, Config:min + (i+1)*Config::step).
        bool probabilitydensityfunction(std::vector<std::uint32_t> &values, const std::uint32_t scale) const;
        bool probabilitydensityfunction(std::vector<double> &values) const;
        
    private:        
        struct Impl;
        Impl *pImpl;    
    };    
    
} } // namespace vnt { namespace tools {




namespace vnt { namespace tools {
    
    // the object validates a given period expressed in micro-seconds.
    class PeriodValidator
    {
    public:
        
        struct Config
        {   
            vnt::core::Time                   period {0};                     // the period under test.
            vnt::core::Time                   alertvalue {0};                 // it is the value beyond which we produce an alert string. it must be > period.  
            vnt::core::Time                   reportinterval {0};             // if not zero, it keeps the value in usec between two reports
            vnt::tools::Histogram::Config     histoconfig {};                 // if it is valid(), then we produce a histogram. use uint64_t as usec 
            Config() = default;
            constexpr Config(vnt::core::Time pe, vnt::core::Time al, vnt::core::Time ri, const vnt::tools::Histogram::Config &hi) 
                : period(pe), alertvalue(al), reportinterval(ri), histoconfig(hi) {}
            bool isvalid() const { return ((0 == period) || (period >= alertvalue)) ? false : true; }
        };
        
        
        PeriodValidator();
        ~PeriodValidator();
    
        bool init(const Config &config);
        
        // it must be regularly called every Config.period micro-seconds.
        // it accepts the current time, returns delta time from previous call of tick(), it computes a histogram of deltas.
        // currtime_usec is the absolute time in micro-seconds (e.g., as generated by vnt::core::now() or by static_cast<std::uint64_t>(1000000.0*yarp::os::Time::now()))
        bool tick(vnt::core::Time currtime, vnt::core::Time &deltatime);
        
        // it removes all times that were previously added with tick().
        bool reset();  
        
        // if true it is time for the regular report because more  than Config.reportinterval micro-seconds have passed from previous report time.
        bool report() const;
        
        // if true: there is an alert because the current delta is > Config.alertvalue 
        bool alert(vnt::core::Time &deltatime) const;
        
        const vnt::tools::Histogram * histogram() const;
               
    private:        
        struct Impl;
        Impl *pImpl;    
    };    
    
} } // namespace vnt { namespace tools {






#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------

