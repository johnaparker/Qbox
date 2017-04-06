#ifndef GUARD_tfsf_h
#define GUARD_tfsf_h

#include <memory>
#include "vec.h"
#include "sources/time_profile.h"

namespace qbox {

    class Field1D;
    class Field2D;
    class grid_properties;

    struct pmlBoundary{
        Eigen::Array2d left; 
        Eigen::Array2d right; 
    };

/// Total Field/Scattered Field class
/** 
 *  TFSF object, created by the grid_properties class.
 *  It contains a 1D Field, and updates the D and H fields
 *  alongside the 2D Field udate.
 *
 *  @todo passing in Field2D* f to update is not good.
 *  Better to add a member in f to allow this
 */
    class tfsf {
    public:
        tfsf(const grid_properties &grid, const time_profile &tp, const volume &vol, double dt);

        /// update the source in 1D sim
        void pulse();               
        /// @brief update the D field in 1D sim
        /// @param f pointer to owning Field2D object
        void updateD(Field2D* f);
        /// @brief update the H field in 1D sim
        /// @param f pointer to owning Field2D object
        void updateH(Field2D* f);

    private:
        int ia,ib,ja,jb;    ///< position of 4 corners
        std::unique_ptr<time_profile> tp;       ///< A 1D field simulation to produce perfect plane wave

        int Nx;
        double dx;
        double dt,t;

        Array Ez,Dz,Hx,Iz,ca,cb;
        pmlBoundary pml;
    };
}


#endif
