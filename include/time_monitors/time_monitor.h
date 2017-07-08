#ifndef GUARD_time_monitor_h
#define GUARD_time_monitor_h

#include <string>
#include <memory>
#include "h5cpp.h"
#include <unordered_map>

namespace qbox {

    struct every {
        every(unsigned int n_repeat = 0): n_repeat(n_repeat), n(1) {
            never = n_repeat == 0 ? true : false;
        };

        bool check_repeat() {
            if (never) return false;
            else if (n % n_repeat == 0) {
                n = 1;
                return true;
            }
            else {
                n += 1;
                return false;
            }
        }

        int n_repeat;
        int n;
        bool never;
    };

    class Field2D;
    enum class fields;

    class time_monitor {

    public:
        time_monitor(const std::string& name, const fields& Fields, const every& out_freq = every(0)): 
                    name(name), Fields(Fields), out_freq(out_freq) {};

        void write_if_ready();

        void write();

        virtual void set_F(Field2D *newF);        //set the owning field
        h5cpp::h5group get_group() const;

    protected:
        virtual void write_impl(const fields& A) = 0;

    private:
        void write(const fields& A);

    protected:
        std::string name;    //Monitor name; to call output from main
        fields Fields;
        every out_freq;
        Field2D *F;      //pointer to owning field object
        std::unique_ptr<h5cpp::h5file> outFile;
        std::unordered_map<fields, h5cpp::h5dset> dsets;

        static int _num_created;

    };

}

#endif
