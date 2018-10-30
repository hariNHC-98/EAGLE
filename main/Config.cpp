#include "Config.hpp"

namespace Config {

/* ------ CSV export settings ----------------------------------------------- */

/** Filename for simulation output. */
const std::string outputFile =
    home + "/PO-EAGLE/Groups/ANC/Blender/Animation-Data/rotation.csv";
/** Export the simulation data as CSV. */
const bool exportCSV = true;
/** Sample frequency for CSV output (fps). */
const double CSV_fs = 30.0;
/** Time step for discrete controller. */
const double CSV_Ts = 1.0 / CSV_fs;

/* ------ Discretization options -------------------------------------------- */

/** Sample frequency for discrete controller. */
const double fs = 238.0;
/** Time step for discrete controller. */
const double Ts = 1.0 / fs;

/* ------ Plot settings ----------------------------------------------------- */

/** Plot the samled version instead of "continuous" result of simulation. */
const bool plotSampled = false;

/* ------ Simulation settings ----------------------------------------------- */

/** Plot the result of the continuous controller instead of the discrete one. */
const bool simulateContinuousController = false;
/** Plot the linearized model instead of the non-linear real model. */
const bool simulateLinearModel = false;
/** Clamp the motor control outputs between 0 and 1. */
const bool clampController = true;

/* ------ LQR weighting matrices Q and R ------------------------------------ */

static auto invsq = [](double x) { return 1.0 / (x * x); };

const RowVector<3> Qn     = invsq(n_att_max) * ones<1, 3>();
const RowVector<3> Qomega = 0.04 * ones<1, 3>();
const RowVector<3> Qq     = 40.0 * ones<1, 3>();

/** Weighting matrix for states in LQR design. */
const Matrix<9, 9> Q = diag(hcat(Qq, Qomega, Qn));
/** Weighting matrix for inputs in LQR design. */
const Matrix<3, 3> R = invsq(u_att_max) * eye<3>();

/* ------ Simulation options (for ODE solver) ------------------------------- */

/** Options for numerical integration for simulation. */
const AdaptiveODEOptions odeopt = {
    .t_start = 0,
    .t_end   = 18,
    .epsilon = 1e-6,
    .h_start = 1e-2,
    .h_min   = 1e-6,
    .maxiter = (unsigned long) 1e6,
};

}  // namespace Config