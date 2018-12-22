#include <gtest/gtest.h>

#include <DLQE.hpp>
#include <AlmostEqual.hpp>
#include <iostream>

using std::cout;
using std::endl;

TEST(DLQE, DLQE) {
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
    Matrix<3, 5> C          = {{
        {1117, 1433, 1439, 863, 877},
        {881, 571, 293, 229, 1559},
        {1567, 1087, 2011, 2017, 2027},
    }};
    Matrix<2, 2> W          = diag<2>({{{29, 31}}});
    Matrix<3, 3> V          = diag<3>({{{47, 53, 59}}});
    auto res                = dlqe(A, B, C, W, V);
    Matrix<5, 3> L          = res.L;
    Matrix<5, 5> P          = res.P;
    Matrix<5, 3> L_expected = {{
        {0.00067891971574704, 0.00039353007653236, -0.00051532978725010},
        {0.00066024852445439, 0.00021646475386967, -0.00040289143731722},
        {0.00003593784521684, -0.00075149119934991, 0.00039968349694189},
        {-0.00028764456147473, -0.00013633696031264, 0.00036529645486007},
        {-0.00061122697693250, 0.00047881726955696, 0.00033090942140618},
    }};
    Matrix<5, 5> P_expected = {{
        {153.00239164813299908, 397.00373141459760973, 885.00507118104849269,
         1431.00641094749380500, 1977.00775071393854887},
        {397.00373141459760973, 1036.00657348461868423, 2314.00941555456029164,
         3766.01225762447484158, 5218.01509969438848202},
        {885.00507118104849269, 2314.00941555456029164, 5172.01375992787234281,
         8436.01810430111436290, 11700.02244867435729248},
        {1431.00641094749380500, 3766.01225762447484158, 8436.01810430111436290,
         13860.02395097764019738, 19284.02979765416239388},
        {1977.00775071393854887, 5218.01509969438848202,
         11700.02244867435729248, 19284.02979765416239388,
         26868.03714663396385731},
    }};

    ASSERT_TRUE(isAlmostEqual(L, L_expected, 1e-10));
    ASSERT_TRUE(isAlmostEqual(P, P_expected, 1e-6));
}