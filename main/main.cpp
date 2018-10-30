#include "InputSignals.hpp"
#include "PrintCSV.hpp"
#include <Matrix/DLQR.hpp>
#include <Matrix/LQR.hpp>
#include <Model/System.hpp>
#include <ODE/ODEEval.hpp>
#include <iostream>

#include "Config.hpp"

#include "Plot.hpp"

using namespace std;
using namespace Config;

int main(int argc, char const *argv[]) {
    (void) argc, (void) argv;

    /* ------ Drone full non-linear and linear models ----------------------- */
    auto nonLinFullmodel  = drone.getNonLinearFullModel();
    auto linFullContModel = drone.getLinearFullContinuousModel();

    /* ------ Design controller --------------------------------------------- */
    auto fullContinuousLQRController = drone.getContinuousController(Q, R);
    auto fullDiscreteLQRController =
        drone.getDiscreteController(Q, R, Ts, DiscretizationMethod::Bilinear);
    auto fullClampedDiscreteLQRController = drone.getClampedDiscreteController(
        Q, R, Ts, DiscretizationMethod::Bilinear);

    /* ------ Reference function -------------------------------------------- */
    TestReferenceFunction ref = {};

    /* ------ Simulate the drone with the controller ------------------------ */
    auto &simulationController =
        simulateContinuousController
            ? (LQRController &) fullContinuousLQRController
            : clampController
                  ? (LQRController &) fullClampedDiscreteLQRController
                  : (LQRController &) fullDiscreteLQRController;

    auto &simulationModel =
        simulateLinearModel
            ? (ContinuousModel<drone.Nx, drone.Nu> &) linFullContModel
            : (ContinuousModel<drone.Nx, drone.Nu> &) nonLinFullmodel;

    LQRController::SimulationResult result =
        simulationController.simulate(simulationModel, ref, x0, odeopt);

    double t_end = odeopt.t_end;
    if (result.resultCode & ODEResultCodes::MAXIMUM_ITERATIONS_EXCEEDED) {
        std::cerr << "Error: maximum number of iterations exceeded" << endl;
        t_end = result.time.back();  // TODO
    }
    if (result.resultCode & ODEResultCodes::MINIMUM_STEP_SIZE_REACHED)
        std::cerr << "Error: minimum step size reached" << endl;

    /* ------ Export the simulation result as CSV --------------------------- */

    if (exportCSV) {
        // Sample/interpolate the simulation result using a fixed time step
        auto sampled = sampleODEResult(result, odeopt.t_start, CSV_Ts, t_end);
        vector<EulerAngles> sampledOrientation;
        sampledOrientation.resize(sampled.size());
        transform(sampled.begin(), sampled.end(), sampledOrientation.begin(),
                  NonLinearFullDroneModel::stateToEuler);
        // Export to the given output file
        printCSV(outputFile, 0.0, CSV_Ts, sampledOrientation);
    }

    /* ------ Plot the simulation result ------------------------------------ */

    // Sample/interpolate the simulation result using a fixed time step
    auto sampled = sampleODEResult(result, odeopt.t_start, Ts, t_end);
    // Decide what data to use: either the sampled version or the raw result
    auto t =
        plotSampled ? makeTimeVector(odeopt.t_start, Ts, t_end) : result.time;
    auto data = plotSampled ? sampled : result.solution;

    // Calculate controller output
    auto u = simulationController.getControlSignal(t, data, ref);

    // Convert the quaternions of the state to euler angles
    vector<EulerAngles> orientation;
    orientation.resize(data.size());
    transform(data.begin(), data.end(), orientation.begin(),
              NonLinearFullDroneModel::stateToEuler);

    vector<EulerAngles> reference;
    reference.resize(t.size());
    transform(t.begin(), t.end(), reference.begin(), [&ref](double t) {
        return quat2eul(getBlock<0, 4, 0, 1>(ref(t)));
    });

    // Plot all results
    plt::subplot(5, 1, 1);
    plotResults(t, reference, {0, 3}, {"z", "y", "x"}, {"b-", "g-", "r-"},
                "Reference orientation");
    plt::xlim(odeopt.t_start, odeopt.t_end);
    plt::subplot(5, 1, 2);
    plotResults(t, orientation, {0, 3}, {"z", "y", "x"}, {"b-", "g-", "r-"},
                "Orientation of drone");
    plt::xlim(odeopt.t_start, odeopt.t_end);
    plt::subplot(5, 1, 3);
    plotResults(t, data, {4, 7}, {"x", "y", "z"}, {"r-", "g-", "b-"},
                "Angular velocity of drone");
    plt::xlim(odeopt.t_start, odeopt.t_end);
    plt::subplot(5, 1, 4);
    plotResults(t, data, {7, 10}, {"x", "y", "z"}, {"r-", "g-", "b-"},
                "Angular velocity of motors");
    plt::xlim(odeopt.t_start, odeopt.t_end);
    plt::subplot(5, 1, 5);
    plotResults(t, u, {0, 3}, {"x", "y", "z"}, {"r-", "g-", "b-"},
                "Control signal");
    plt::xlim(odeopt.t_start, odeopt.t_end);

    plt::tight_layout();
    plt::show();

    /* ---------------------------------------------------------------------- */

    return EXIT_SUCCESS;
}
