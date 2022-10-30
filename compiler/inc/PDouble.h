#ifndef SCANNER_INC_PDOUBLE_H_
#define SCANNER_INC_PDOUBLE_H_

class PDouble {
public:
    double value;

    PDouble();
    PDouble(double v);
    virtual ~PDouble();
};

bool operator == (const PDouble &x, const PDouble &y);

#endif //SCANNER_INC_PDOUBLE_H_