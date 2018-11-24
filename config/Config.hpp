#pragma once

#include <Matrix/Matrix.hpp>
#include <ODE/ODEOptions.hpp>
#include <filesystem>

namespace Config {

// Home directory
const std::filesystem::path home = getenv("HOME");

/* ------ CSV export settings ----------------------------------------------- */
// Filenames for simulation output
extern const std::filesystem::path rotationCSVFile;
extern const std::filesystem::path locationCSVFile;
// Export the simulation data as CSV.
extern const bool exportCSV;
// Sample frequency for CSV output (fps).
extern const double CSV_fs;
// Time step for discrete controller.
extern const double CSV_Ts;

/* ------ Matrix & Parameter data loading ----------------------------------- */
extern const std::filesystem::path loadPath;

/* ------ Attitude LQR & LQE ------------------------------------------------ */

namespace Attitude {
extern const RowVector<3> Qq;
extern const RowVector<3> Qomega;
extern const RowVector<3> Qn;

extern const Matrix<9, 9> Q;
extern const Matrix<3, 3> R;

extern const RowVector<3> varDynamics;
extern const RowVector<6> varSensors;
}  // namespace Attitude

/* ------ Altitude PI controller and LQE ------------------------------------ */

namespace Altitude {
extern const Matrix<1, 3> K_p;
extern const Matrix<1, 3> K_i;

extern const RowVector<1> varDynamics;
extern const RowVector<1> varSensors;
}  // namespace Altitude

/* ------ Simulation options (for ODE solver) ------------------------------- */
extern const AdaptiveODEOptions odeopt;

/* ------ Tuner settings ---------------------------------------------------- */
namespace Tuner {
/* ------ Matrix & Parameter data loading ----------------------------------- */
extern const std::filesystem::path loadPath;

extern const AdaptiveODEOptions odeopt;
extern const AdaptiveODEOptions odeoptdisp;

/* ------ LQR --------------------------------------------------------------- */
extern const ColVector<9> Q_diag_initial;
extern const ColVector<3> R_diag_initial;

/* ------ Tuner mutation variance ------------------------------------------- */
extern const ColVector<9> varQ;
extern const ColVector<3> varR;

extern const ColVector<9> Qmin;
extern const ColVector<3> Rmin;

extern const ColVector<9> Qmax;
extern const ColVector<3> Rmax;

/* ------ Genetic algorithm settings ---------------------------------------- */
extern const size_t population;
extern const size_t generations;
extern const size_t survivors;

/* ------ Image export dimensions ------------------------------------------- */
extern const size_t px_x;
extern const size_t px_y;

}  // namespace Tuner

}  // namespace Config