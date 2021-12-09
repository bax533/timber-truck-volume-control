#ifndef PROFILER_H
#define PROFILER_H

#include <chrono>
#include <iostream>
#include <map>
#include <string>


    class Timer
    {
    private:
        std::map<std::string, double> timers;
        std::map<std::string, std::chrono::time_point<std::chrono::high_resolution_clock> > startPoints;

    public:
        Timer() 
        {}

        void Start(const std::string& name)
        {
            startPoints[name] = std::chrono::high_resolution_clock::now(); 
        }

        void Stop(const std::string& name)
        {
            auto endPoint = std::chrono::high_resolution_clock::now();

            auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(startPoints[name]).time_since_epoch().count();
            auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(endPoint).time_since_epoch().count();

            double duration = end - start;
            if(!timers.count(name))
                timers[name] = duration;
            else
                timers[name] += duration;
        }

        void getTime(const std::string& name, bool seconds = false)
        {
            double t = timers[name];
            if(seconds)
                 t *= 0.001;
            std::cout<<name<<": "<<t;
            if(seconds)
                std::cout<<"s\n";
            else
                std::cout<<"ms\n";
        }
    };
namespace TimberControl
{
    static Timer profiler;
}

#endif
