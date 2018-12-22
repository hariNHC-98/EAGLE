#include <gtest/gtest.h>

#include <DLQR.hpp>
#include <LQR.hpp>
#include <AlmostEqual.hpp>

TEST(LQR, LQR) {
    Matrix<9, 9> A          = {{
        {0, 0, 0, 0.5, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0.5, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0.5, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 2.76914, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 2.61439, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, -1.67014},
        {0, 0, 0, 0, 0, 0, -28.5714, -0, -0},
        {0, 0, 0, 0, 0, 0, -0, -28.5714, -0},
        {0, 0, 0, 0, 0, 0, -0, -0, -28.5714},
    }};
    Matrix<9, 3> B          = {{
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 211.441},
        {3330, 0, 0},
        {0, 3330, 0},
        {0, 0, 3330},
    }};
    Matrix<9, 9> Q          = {{
        {3, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 3, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 3, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0.000671492, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0.000671492, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0.000671492},
    }};
    Matrix<3, 3> R          = {{
        {9.12149, 0, 0},
        {0, 9.12149, 0},
        {0, 0, 9.12149},
    }};
    Matrix<3, 9> K          = -lqr(A, B, Q, R).K;
    Matrix<3, 9> K_expected = {{
        {
            -0.573493,
            -0.000000,
            -0.000000,
            -0.056737,
            -0.000000,
            -0.000000,
            -0.006963,
            -0.000000,
            -0.000000,
        },
        {
            -0.000000,
            -0.573493,
            -0.000000,
            -0.000000,
            -0.058212,
            -0.000000,
            -0.000000,
            -0.006868,
            -0.000000,
        },
        {
            -0.000000,
            -0.000000,
            -0.573493,
            -0.000000,
            -0.000000,
            -0.134089,
            -0.000000,
            -0.000000,
            0.004065,
        },
    }};
    bool passed             = isAlmostEqual(K, K_expected, 1e-6);
    ASSERT_TRUE(passed);
}

TEST(DLQR, DLQR) {
    Matrix<5, 5> A          = {{
        {11, 12, 13, 14, 15},
        {21, 22, 23, 24, 25},
        {31, 32, 33, 34, 35},
        {41, 42, 43, 44, 45},
        {51, 52, 53, 54, 55},
    }};
    Matrix<5, 2> B          = {{
        {1, 2},
        {3, 5},
        {7, 11},
        {13, 17},
        {19, 23},
    }};
    Matrix<5, 5> Q          = diag<5>({{{29, 31, 37, 41, 43}}});
    Matrix<2, 2> R          = diag<2>({{{47, 51}}});
    Matrix<2, 5> K          = -dlqr(A, B, Q, R).K;
    Matrix<2, 5> K_expected = {{
        {4.55855402465455, 5.01609563355864, 5.47363724240869, 5.93117885125875,
         6.38872046013582},
        {-6.02972054883736, -6.45263097488142, -6.87554140088499,
         -7.29845182688857, -7.72136225291238},
    }};
    bool passed             = isAlmostEqual(K, K_expected, 1e-6);
    ASSERT_TRUE(passed);
}