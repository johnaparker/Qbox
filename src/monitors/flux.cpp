#include "monitors/flux.h"

namespace qbox {

    Flux::Flux(const Array& S, const h5cpp::h5file &file, const h5cpp::h5group &group ): S(S) {
        file_name = file.get_name();
        group_path = group.get_path();
    }

    Flux::Flux(const Array& S, const std::string &file_name, const std::string &group_path): S(S), file_name(file_name), group_path(group_path) {};

    void Flux::write() {
        h5cpp::h5file f(file_name, h5cpp::io::rw);
        auto g = f.open_group(group_path);
        h5cpp::write_array<double>(S, g, "flux");
    }
}
