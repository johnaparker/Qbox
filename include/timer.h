#include <chrono>
#include <iostream>
#include <map>
#include <string>

namespace qbox {

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
        void start(std::string);
        void stop(std::string);
        void display() const;

    private:
        std::map<std::string, timer> time_map;
    };
}
