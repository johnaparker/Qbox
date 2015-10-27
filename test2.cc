#include <iostream>
#include <string>

#include "h5out.h"

int main() {
    h5out file("test.h5");
    file.create_node("a", {10}, false);
    double *data = new double[10];
    for (int i = 0; i != 10; i++)
        data[i] =1;
    file.write_to_node("a", data);
}
