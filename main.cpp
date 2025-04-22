#include <iostream>
#include "sketch.h"

int main() {
    Sketch sketch(1024); // r = 2^10

    for (int i = 0; i < 1000; ++i) {
        sketch.Update(i % 100, i);
    }

    std::cout << "Estimated ||f||^2 = " << sketch.Query() << std::endl;
    return 0;
}
