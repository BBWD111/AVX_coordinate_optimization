#include "coordinate_transform.h"

#include <catch2/catch_message.hpp>
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <immintrin.h>
#include <iostream>
#include <random>

bool operator==(const PPoint_ASM& a, const PPoint_ASM& b) {
    bool res = true;
    for (int i = 0; i < 4; i++) {
        res &= (std::abs(b.x[i] - a.x[i]) <= std::numeric_limits<float>::epsilon());
        std::cout << a.x[i] << " " << b.x[i] << "\n";
    }
    std::cout << "\n\n";
    for (int i = 0; i < 4; i++) {
        res &= (std::abs(b.y[i] - a.y[i]) <= std::numeric_limits<float>::epsilon());
        std::cout << a.y[i] << " " << b.y[i] << "\n";
    }

    return res;
}

bool operator==(const PPoint_CPP& a, const PPoint_CPP& b) {
    bool res = true;
    for (int i = 0; i < 4; i++) {
        res &= (std::abs(b.x[i] - a.x[i]) <= std::numeric_limits<float>::epsilon());
        std::cout << a.x[i] << " " << b.x[i] << "\n";
    }
    std::cout << "\n\n";
    for (int i = 0; i < 4; i++) {
        res &= (std::abs(b.y[i] - a.y[i]) <= std::numeric_limits<float>::epsilon());
        std::cout << a.y[i] << " " << b.y[i] << "\n";
    }

    return res;
}

bool operator==(const Rectangle& a, const Rectangle& b) {
    bool res = true;

    res &= (std::abs(b.w - a.w) <= std::numeric_limits<float>::epsilon());
    std::cout << a.w << " " << b.w << "\n";
    res &= (std::abs(b.l - a.l) <= std::numeric_limits<float>::epsilon());
    std::cout << a.l << " " << b.l << "\n";
    res &= (std::abs(b.center.x - a.center.x) <= std::numeric_limits<float>::epsilon());
    std::cout << a.center.x << " " << b.center.x << "\n";
    res &= (std::abs(b.center.y - a.center.y) <= std::numeric_limits<float>::epsilon());
    std::cout << a.center.y << " " << b.center.y << "\n";
    res &= (std::abs(b.vcos - a.vcos) <= std::numeric_limits<float>::epsilon());
    std::cout << a.vcos << " " << b.vcos << "\n";
    res &= (std::abs(b.vsin - a.vsin) <= std::numeric_limits<float>::epsilon());
    std::cout << a.vsin << " " << b.vsin << "\n";

    return res;
}

TEST_CASE("Simple_asm_shift"){
    Rectangle rec(1, 1, {0, 0}, 1, 0);
    __m256d x = {3, 9, 7, 24}; 
    __m256d y = {8, 2, 9, 14};
    
    PPoint_ASM p(x, y);

    PPoint_ASM new_p = PPoint_Shift_ASM(p, rec);
    REQUIRE(p == new_p);
}

TEST_CASE("Advanced_asm_shift"){
    Rectangle rec(1, 1, {1, 1}, 1, 0);
    __m256d x = {3, 9, 7, 24}; 
    __m256d y = {8, 2, 9, 14};
    
    PPoint_ASM p(x, y);
    PPoint_ASM ep(x-1, y-1);

    PPoint_ASM new_p = PPoint_Shift_ASM(p, rec);
    REQUIRE(ep == new_p);
}


TEST_CASE("Simple_asm_rotate"){
    Rectangle rec(1, 1, {0, 0}, 1, 0);
    __m256d x = {3, 9, 7, 24}; 
    __m256d y = {8, 2, 9, 14};
    
    PPoint_ASM p(x, y);

    PPoint_ASM new_p = PPoint_Rotate_ASM(p, rec);
    REQUIRE(p == new_p);
}

TEST_CASE("Advanced_asm_rotate"){
    Rectangle rec(1, 1, {0, 0}, 0, 1);
    __m256d x = {3, 9, 7, 24}; 
    __m256d y = {8, 2, 9, 14};
    
    PPoint_ASM p(x, y);
    PPoint_ASM ep(y, -x);

    PPoint_ASM new_p = PPoint_Rotate_ASM(p, rec);
    REQUIRE(ep == new_p);
}

TEST_CASE("Advanced_asm_transform"){
    Rectangle rec(1, 1, {1, 1}, 0, 1);
    __m256d x = {3, 9, 7, 24}; 
    __m256d y = {8, 2, 9, 14};
    
    PPoint_ASM p(x, y);
    PPoint_ASM ep(y-1, -x+1);

    PPoint_ASM new_p = PPoint_Transform_ASM(p, rec);
    REQUIRE(ep == new_p);
}

TEST_CASE("Advanced_cpp_shift"){
    Rectangle rec(1, 1, {1, 1}, 1, 0);
    PPoint_CPP p({{3, 9, 7, 24}}, {{8, 2, 9, 14}});
    PPoint_CPP ep({{2, 8, 6, 23}}, {{7, 1, 8, 13}});
    PPoint_CPP new_p = PPoint_Shift_CPP(p, rec);

    REQUIRE(new_p == ep);
}

TEST_CASE("Advanced_cpp_rotate"){
    Rectangle rec(1, 1, {0, 0}, 0, 1);

    PPoint_CPP p({{3, 9, 7, 24}}, {{8, 2, 9, 14}});
    PPoint_CPP ep({{8, 2, 9, 14}}, {{-3, -9, -7, -24}});
    PPoint_CPP new_p = PPoint_Rotate_CPP(p, rec);

    REQUIRE(new_p == ep);
}

TEST_CASE("Advanced_cpp_transform"){
    Rectangle rec(1, 1, {1, 1}, 0, 1);

    PPoint_CPP p({{3, 9, 7, 24}}, {{8, 2, 9, 14}});
    PPoint_CPP ep({{7, 1, 8, 13}}, {{-2, -8, -6, -23}});
    PPoint_CPP new_p = PPoint_Transform_CPP(p, rec);

    REQUIRE(new_p == ep);
}

TEST_CASE("Rectangle_convert"){
    CornerRectangle rec({0.5, 1.5}, {1, 0}, {0, -1});
    Rectangle erec(1, 1, {1, 1}, 1, 0);

    Rectangle new_rec = RectangleConvert(rec);
    REQUIRE(new_rec == erec);
}

TEST_CASE("Random") {
    std::mt19937 gen{4242};
    std::uniform_real_distribution<double> dis(0.1, 20);

    PPoint_ASM pASM;
    PPoint_CPP pCPP;

    Point v1(dis(gen), dis(gen));
    Point v2(v1.y, -v1.x);
    Point corner(dis(gen), dis(gen));
    CornerRectangle crec(corner, v1, v2);
    Rectangle rec = RectangleConvert(crec);

    for (int i = 0; i < 4; i++) {
        pASM.x[i] = dis(gen);
        pASM.y[i] = dis(gen);
        pCPP.x[i] = pASM.x[i];
        pCPP.y[i] = pASM.y[i];
    }

    BENCHMARK_ADVANCED("AVX Random")(Catch::Benchmark::Chronometer meter) {
        meter.measure([&] { return PPoint_Transform_ASM(pASM, rec); });
    };

    BENCHMARK_ADVANCED("CPP Random")(Catch::Benchmark::Chronometer meter) {
        meter.measure([&] { return PPoint_Transform_CPP(pCPP, rec); });
    };
    //REQUIRE(0 == 1);
}