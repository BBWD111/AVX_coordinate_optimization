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

NormalBox Normalize(const Rectangle& rec) {
    return {rec.w, rec.l};
}

double Length(const Point& p) {
    return std::sqrt(p.x * p.x + p.y * p.y);
}

Rectangle RectangleConvert(const CornerRectangle& cor_rec) {
    Rectangle rec;
    rec.w = Length(cor_rec.v1);
    rec.l = Length(cor_rec.v2);
    rec.center = cor_rec.corner;
    rec.center.x += (cor_rec.v1.x + cor_rec.v2.x) / 2;
    rec.center.y += (cor_rec.v1.y + cor_rec.v2.y) / 2;

    rec.vcos = cor_rec.v1.x / rec.w;
    rec.vsin = cor_rec.v1.y / rec.w;
    return rec;
}

PPoint_ASM PPoint_Shift_ASM(const PPoint_ASM& p, const Rectangle& rectangle) {
    __m256d new_x = _mm256_sub_pd(p.x, _mm256_set1_pd(rectangle.center.x));
    __m256d new_y = _mm256_sub_pd(p.y, _mm256_set1_pd(rectangle.center.y));
    return {new_x, new_y};
}

PPoint_ASM PPoint_Rotate_ASM(const PPoint_ASM& p, const Rectangle& rectangle) {
    __m256d vcos = _mm256_set1_pd(rectangle.vcos);
    __m256d vsin = _mm256_set1_pd(rectangle.vsin);
    __m256d new_x = _mm256_add_pd(_mm256_mul_pd(p.x, vcos), _mm256_mul_pd(p.y, vsin));
    __m256d new_y = _mm256_sub_pd(_mm256_mul_pd(p.y, vcos), _mm256_mul_pd(p.x, vsin));
    return {new_x, new_y};
}

PPoint_ASM PPoint_Transform_ASM(const PPoint_ASM& p, const Rectangle& rectangle) {
    PPoint_ASM new_p = PPoint_Shift_ASM(p, rectangle);
    new_p = PPoint_Rotate_ASM(new_p, rectangle);
    return new_p;
}

PPoint_CPP PPoint_Shift_CPP(const PPoint_CPP& p, const Rectangle& rectangle) {
    PPoint_CPP new_p;
    for (int i = 0; i < 4; i++) {
        new_p.x[i] = p.x[i] - rectangle.center.x;
        new_p.y[i] = p.y[i] - rectangle.center.y;
    }
    return new_p;
}

PPoint_CPP PPoint_Rotate_CPP(const PPoint_CPP& p, const Rectangle& rectangle) {
    PPoint_CPP new_p;
    for (int i = 0; i < 4; i++) {
        new_p.x[i] = p.x[i]*rectangle.vcos + p.y[i]*rectangle.vsin;
        new_p.y[i] = p.y[i]*rectangle.vcos - p.x[i]*rectangle.vsin;
    }
    return new_p;
}

PPoint_CPP PPoint_Transform_CPP(const PPoint_CPP& p, const Rectangle& rectangle) {
    PPoint_CPP new_p = PPoint_Shift_CPP(p, rectangle);
    new_p = PPoint_Rotate_CPP(new_p, rectangle);
    return new_p;
}
