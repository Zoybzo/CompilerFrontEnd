#include "inc/PDouble.h"

#include "inc/utils.h"

PDouble::PDouble() {}

PDouble::PDouble(double v) {
    value = v;
}

PDouble::~PDouble() {}

bool operator==(const PDouble &x, const PDouble &y) {
    return Sgn(x.value - y.value) == 0;
}
