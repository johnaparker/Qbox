#ifndef GUARD_source_h
#define GUARD_source_h

#include <vector>

//*** Think about whether this is the best way to do sources
//*** Also think about how to add TFSF into these sources
//*** Lastly, the best way to couple these sources with the field class.

//*** Definitely consider functions external to classes doing some of the work
//*** use "=0" on virtual base, and add virtual destructor

//*** Point source = line source default with one point?


namespace apine {

    class Field2D;

    //source base class
    class source {
    public:
        source();
        void set_F(Field2D *F);   //set field ownership
        virtual void pulse() {};  //add the source to the fields

    public:
        Field2D *F;    //field owner reference
        double *t;     //current time (from F)
    };


    class continuous_point_source: public source {
    public:
        continuous_point_source(std::vector<int> p, double f);
        void pulse();
    public:
        std::vector<int> p;    //vector position
        double freq;           //frequency
    };

    class gaussian_point_source: public source {
    public:
        gaussian_point_source(std::vector<int> p, double T0, double sig);
        void pulse();
    public:
        std::vector<int> p;    //vector position
        double T0, sig;        //temporal center, width
    };

    class custom_point_source: public source {
    public:
        std::vector<int> p;
        double (*time_func)(double);
    public:
        custom_point_source(std::vector<int> p, double (*time_func)(double));
        void pulse();
    };


    class continuous_line_source: public source {
    public:
        std::vector<int> p1, p2;
        double freq;
    public:
        continuous_line_source(std::vector<int> p1, std::vector<int> p2, double f);
        void pulse();
    };

    class gaussian_line_source: public source {
    public:
        std::vector<int> p1, p2;
        double T0, sig;
    public:
        gaussian_line_source(std::vector<int> p1, std::vector<int> p2, double T0, double sig);
        void pulse();
    };

    class custom_line_source: public source {
    public:
        std::vector<int> p1, p2;
        double (*time_func)(double);
    public:
        custom_line_source(std::vector<int> p1, std::vector<int> p2, double (*time_func)(double));
        void pulse();
    };
}

#endif
