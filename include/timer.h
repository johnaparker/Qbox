#include <chrono>
#include <iostream>
#include <map>
#include <string>
#include <stdexcept>

namespace qbox {
    enum class clock_name {
        fourier,looping,hdf5,self
    };


    class timer {
    public:
        timer(): duration_(0) {};
        void begin() {
            if (ticking)
                throw std::runtime_error(std::string("Cannot start a clock already ticking"));
            start = std::chrono::system_clock::now();
            ticking = true;
        }
        void end() {
            if (!ticking)
                throw std::runtime_error(std::string("Cannot stop a clock that isn't ticking"));
            duration_ += std::chrono::system_clock::now() - start;
            ticking = false;
        }
        double duration() const{return duration_.count();}

    private:
        std::chrono::duration<double> duration_;
        std::chrono::time_point<std::chrono::system_clock> start;
        bool ticking = false;
    };

    class timers {
    public:
        timers();
        ~timers();
        void start(clock_name);
        void stop(clock_name);
        void display() const;
    private:
        std::string clock_string_name(clock_name name) const;

    private:
        std::map<clock_name, timer> time_map;
    };
}
