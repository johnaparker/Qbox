#include <chrono>
#include <iostream>
#include <map>
#include <string>
#include <stdexcept>


namespace qbox {

    /// @brief Types of clocks
    enum class clock_name {
        fourier,  ///< Clock for fourier transforming
        looping,  ///< Clock for main loops
        hdf5,     ///< Clock for HDF5 output
        self      ///< Clock for everything
    };

    /// A single timer class
    /** 
     *  The timer acts as a stopwatch. It can be started or stopped.
     *  When stopped, the total time is incremented by time since start.
     */
    class timer {
    public:
        /// Construcs a timer, starting at time 0
        timer(): duration_(0) {};
        
        /// Start timer
        void begin() {
            if (ticking)
                throw std::runtime_error(std::string("Cannot start a clock already ticking"));
            start = std::chrono::system_clock::now();
            ticking = true;
        }

        /// Stop timer
        void end() {
            if (!ticking)
                throw std::runtime_error(std::string("Cannot stop a clock that isn't ticking"));
            duration_ += std::chrono::system_clock::now() - start;
            ticking = false;
        }

        /// Get the current duration
        double duration() const{return duration_.count();}

    private:
        std::chrono::duration<double> duration_;    ///< Time spent ticking
        std::chrono::time_point<std::chrono::system_clock> start;   ///< Keeps track of start time
        bool ticking = false;  ///< True when ticking
    };

    /// A class for managing several timers
    /** 
     *  Has a timer for each of the clock_name types.
     *  Each timer can be started, stopped, and displayed
     */
    class timers {
    public:
        /// Construct each timer
        timers();
        /// Before destruction, display the timers
        ~timers();

        /// Start timer of type clock_name
        void start(clock_name);

        /// Stop timer of type clock_name
        void stop(clock_name);

        /// Display all clock durations
        void display() const;
    private:
        /// Get string name for given clock_name
        std::string clock_string_name(clock_name name) const;
        void display_timer(clock_name c_name) const;
        void display_timer_string(std::string name, double duration) const;
        double duration(clock_name c_name) const;

    private:
        std::map<clock_name, timer> time_map;   ///< map of timers
    };
}
