#include <array>
#include <cstdint>
#include <immintrin.h>
#include <utility>
#include <cmath>

struct PPoint_ASM {
    __m256d x;
    __m256d y;
};

struct Point {
    double x;
    double y;
};

struct PPoint_CPP {
    std::array<double, 4> x;
    std::array<double, 4> y;
};

struct CornerRectangle {
    Point corner;
    Point v1;
    Point v2;
};

struct Rectangle {
    double w;
    double l;
    Point center;
    double vcos;
    double vsin;
};

struct NormalBox {
    double w;
    double l;
};

NormalBox Normalize(const Rectangle& rec);

double Length(const Point& p);

Rectangle RectangleConvert(const CornerRectangle& cor_rec);

PPoint_ASM PPoint_Shift_ASM(const PPoint_ASM& p, const Rectangle& rectangle);

PPoint_ASM PPoint_Rotate_ASM(const PPoint_ASM& p, const Rectangle& rectangle);

PPoint_ASM PPoint_Transform_ASM(const PPoint_ASM& p, const Rectangle& rectangle);

PPoint_CPP PPoint_Shift_CPP(const PPoint_CPP& p, const Rectangle& rectangle);

PPoint_CPP PPoint_Rotate_CPP(const PPoint_CPP& p, const Rectangle& rectangle);

PPoint_CPP PPoint_Transform_CPP(const PPoint_CPP& p, const Rectangle& rectangle);
