#ifndef GUARD_tfsf_h
#define GUARD_tfsf_h

namespace qbox {

    class Field1D;
    class Field2D;
    class grid_properties;

    //TFSF object created by the grid_properties class
    class tfsf {
    public:
        //*** This class needs rule of 3 (or Zero)
        tfsf(grid_properties grid, double dt);
        tfsf() = default;
        tfsf(const tfsf&) = default;
        tfsf& operator=(const tfsf&) = default;

        void pulse();               //update the source to 1D sim
        void updateD(Field2D* f);   //update the D field in 1D sim
        void updateH(Field2D* f);   //update the H field in 1D sim

    private:
       //*** Smart poiter here, and Field1D needs proper destructor
       Field1D *inc;       //A 1D field simulation to produce perfect plane wave
       int ia,ib,ja,jb;    //position  of corners
    };
}


#endif
