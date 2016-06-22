#include <chrono>
#include <iostream>
#include <map>
#include <string>

namespace qbox {
    enum class clock_name {
        fourier,looping,hdf5
    };


    class timer {
    public:
        timer(): duration_(0) {};
        void begin() {start = std::chrono::system_clock::now();}
        void end() {duration_ += std::chrono::system_clock::now() - start;}
        double duration() const{return duration_.count();}

    private:
        std::chrono::duration<double> duration_;
        std::chrono::time_point<std::chrono::system_clock> start;
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
