#pragma once

#include "DiscreteController.hpp"
#include "DiscreteObserver.hpp"
#include "NoiseGenerator.hpp"
#include <DormandPrince.hpp>
#include <Time.hpp>
#include <TimeFunction.hpp>

#include <cassert>

/** 
 * @brief   An abstract class for general models that can be simulated.
 */
template <size_t Nx, size_t Nu, size_t Ny>
class Model {
  public:
    typedef ColVector<Nx> VecX_t;
    typedef ColVector<Nu> VecU_t;
    typedef ColVector<Ny> VecY_t;
    typedef ColVector<Ny> VecR_t;

    /**
     * @brief   Get the state change of the model, given current state
     *          @f$ x @f$ and input @f$ u @f$.
     */
    virtual VecX_t operator()(const VecX_t &x, const VecU_t &u) = 0;

    /** 
     * @brief   Get the output of the model, given the current state
     *          @f$ x @f$ and input @f$ u @f$.
     */
    virtual VecY_t getOutput(const VecX_t &x, const VecU_t &u) = 0;
};

/** 
 * @brief   An abstract class for Continuous-Time models.
 */
template <size_t Nx, size_t Nu, size_t Ny>
class ContinuousModel : public Model<Nx, Nu, Ny> {
  public:
    using VecX_t = typename Model<Nx, Nu, Ny>::VecX_t;
    using VecU_t = typename Model<Nx, Nu, Ny>::VecU_t;
    using VecY_t = typename Model<Nx, Nu, Ny>::VecY_t;
    using VecR_t = typename Model<Nx, Nu, Ny>::VecR_t;

    typedef TimeFunctionT<VecU_t> InputFunction;
    typedef TimeFunctionT<VecR_t> ReferenceFunction;
    typedef ODEResultX<VecX_t> SimulationResult;

    struct ControllerSimulationResult : public SimulationResult {
        std::vector<double> sampledTime;
        std::vector<VecU_t> control;
        std::vector<VecY_t> reference;
    };

    struct ObserverControllerSimulationResult
        : public ControllerSimulationResult {
        std::vector<VecX_t> estimatedSolution;
        std::vector<VecY_t> output;
    };

    /**
     * @brief   Simulate the continuous model starting from the given initial 
     *          state, evaluating the given input function, using the given
     *          integration options.  
     *          This version returns all intermediate points calculated by the
     *          ODE solver.
     * 
     * @param   u
     *          The input function that can be evaluated at any given time in 
     *          the integration interval.
     * @param   x_start
     *          The inital state, which is the initial condition for the ODE
     *          solver.
     * @param   opt
     *          A struct of options for the ODE solver.
     * 
     * @return
     *          A struct containing a vector of time points, and a vector of
     *          states that have equal lenghts.  
     *          A result code is given as well, to indicate whether the 
     *          integration was successful.
     */
    SimulationResult simulate(InputFunction &u, VecX_t x_start,
                              const AdaptiveODEOptions &opt) {
        // A lambda function that evaluates the input function and the dynamics
        // of the model
        ContinuousModel &model = *this;
        auto f                 = [&model, &u](double t, const VecX_t &x) {
            return model(x, u(t));
        };
        // Use the ODE solver to simulate the system
        return dormandPrince(f, x_start, opt);
    }

    /**
     * @brief   Simulate the continuous model starting from the given initial 
     *          state, evaluating the given input function, using the given
     *          integration options.  
     *          This version only returns the final time and state of the 
     *          simulation.
     * 
     * @param   u
     *          The input function that can be evaluated at any given time in 
     *          the integration interval.
     * @param   x_start
     *          The inital state, which is the initial condition for the ODE
     *          solver.
     * @param   opt
     *          A struct of options for the ODE solver.
     * 
     * @return
     *          A struct containing a vector of time points, and a vector of
     *          states that have equal lenghts.  
     *          A result code is given as well, to indicate whether the 
     *          integration was successful.
     */
    SimulationResult simulateEndResult(InputFunction &u, VecX_t x_start,
                                       const AdaptiveODEOptions &opt) {
        // A lambda function that evaluates the input function and the dynamics
        // of the model
        ContinuousModel &model = *this;
        auto f                 = [&model, &u](double t, const VecX_t &x) {
            return model(x, u(t));
        };
        // Use the ODE solver to simulate the system
        return dormandPrinceEndResult(f, x_start, opt);
    }

    /**
     * @brief   Simulate the continuous model starting from the given initial 
     *          state, evaluating the given input function, using the given
     *          integration options.  
     *          This version appends the results to existing time and states
     *          vectors.
     * 
     * @param   timeresult 
     *          The time vector that the time results will be appended to.
     * @param   xresult
     *          The states vector that the state results will be appended to.
     * @param   u
     *          The input function that can be evaluated at any given time in 
     *          the integration interval.
     * @param   x_start
     *          The inital state, which is the initial condition for the ODE
     *          solver.
     * @param   opt
     *          A struct of options for the ODE solver.
     * 
     * @return
     *          A result code that indicates whether the integration was
     *          successful.
     */
    auto
    simulate(typename std::back_insert_iterator<std::vector<double>> timeresult,
             typename std::back_insert_iterator<std::vector<VecX_t>> xresult,
             InputFunction &u, VecX_t x_start, const AdaptiveODEOptions &opt) {
        ContinuousModel &model = *this;
        auto f                 = [&model, &u](double t, const VecX_t &x) {
            return model(x, u(t));
        };
        return dormandPrince(timeresult, xresult, f, x_start, opt);
    }

    /**
     * @brief   Simulate the continuous model starting from the given initial 
     *          state, with a given constant input, using the given
     *          integration options.  
     *          This version returns all intermediate points calculated by the
     *          ODE solver.
     * 
     * @param   u
     *          The constant input.
     * @param   x_start
     *          The inital state, which is the initial condition for the ODE
     *          solver.
     * @param   opt
     *          A struct of options for the ODE solver.
     * 
     * @return
     *          A struct containing a vector of time points, and a vector of
     *          states that have equal lenghts.  
     *          A result code is given as well, to indicate whether the 
     *          integration was successful.
     */
    SimulationResult simulate(VecU_t u, VecX_t x_start,
                              const AdaptiveODEOptions &opt) {
        ContinuousModel &model = *this;
        auto f                 = [&model, u](double /* t */, const VecX_t &x) {
            return model(x, u);
        };
        return dormandPrince(f, x_start, opt);
    }

    /**
     * @brief   Simulate the continuous model starting from the given initial 
     *          state, with a given constant input, using the given
     *          integration options.  
     *          This version only returns the final time and state of the 
     *          simulation.
     * 
     * @param   u
     *          The constant input.
     * @param   x_start
     *          The inital state, which is the initial condition for the ODE
     *          solver.
     * @param   opt
     *          A struct of options for the ODE solver.
     * 
     * @return
     *          A struct containing a vector of time points, and a vector of
     *          states that have equal lenghts.  
     *          A result code is given as well, to indicate whether the 
     *          integration was successful.
     */
    SimulationResult simulateEndResult(VecU_t u, VecX_t x_start,
                                       const AdaptiveODEOptions &opt) {
        ContinuousModel &model = *this;
        auto f                 = [&model, u](double /* t */, const VecX_t &x) {
            return model(x, u);
        };
        return dormandPrinceEndResult(f, x_start, opt);
    }

    /**
     * @brief   Simulate the continuous model starting from the given initial 
     *          state, with a given constant input, using the given integration
     *          options.  
     *          This version appends the results to existing time and states
     *          vectors.
     * 
     * @param   timeresult 
     *          The time vector that the time results will be appended to.
     * @param   xresult
     *          The states vector that the state results will be appended to.
     * @param   u
     *          The constant input.
     * @param   x_start
     *          The inital state, which is the initial condition for the ODE
     *          solver.
     * @param   opt
     *          A struct of options for the ODE solver.
     * 
     * @return
     *          A result code that indicates whether the integration was
     *          successful.
     */
    auto
    simulate(typename std::back_insert_iterator<std::vector<double>> timeresult,
             typename std::back_insert_iterator<std::vector<VecX_t>> xresult,
             VecU_t u, VecX_t x_start, const AdaptiveODEOptions &opt) {
        ContinuousModel &model = *this;
        auto f                 = [&model, &u](double /* t */, const VecX_t &x) {
            return model(x, u);
        };
        return dormandPrince(timeresult, xresult, f, x_start, opt);
    }

    /**
     * @brief   Simulate the closed-loop continuous model using the given 
     *          state-less discrete controller, starting from the given
     *          initial state, evaluating the given input function, 
     *          using the given integration options.
     * 
     * @param   controller
     *          The state-less discrete controller that produces a control 
     *          output u, given the system state x and reference state r.
     * @param   r
     *          The reference function that can be evaluated at any given time
     *          in the integration interval.
     * @param   x_start
     *          The inital state, which is the initial condition for the ODE
     *          solver.
     * @param   opt
     *          A struct of options for the ODE solver.
     * 
     * @return
     *          A struct containing a vector of time points, and a vector of
     *          states that have equal lenghts.  
     *          A result code is given as well, to indicate whether the 
     *          integration was successful.
     */
    ControllerSimulationResult
    simulate(DiscreteController<Nx, Nu, Ny> &controller, ReferenceFunction &r,
             VecX_t x_start, const AdaptiveODEOptions &opt) {
        ControllerSimulationResult result = {};
        double Ts                         = controller.Ts;
        size_t N = numberOfSamplesInTimeRange(opt.t_start, Ts, opt.t_end);
        // pre-allocate memory for result vectors
        result.sampledTime.reserve(N);
        result.control.reserve(N);
        result.reference.reserve(N);

        // actual state = inital state
        VecX_t curr_x               = x_start;
        AdaptiveODEOptions curr_opt = opt;
        // For each time step
        for (size_t i = 0; i < N; ++i) {
            // current time, and integration range
            double t         = opt.t_start + Ts * i;
            curr_opt.t_start = t;
            curr_opt.t_end   = t + Ts;
            curr_opt.maxiter = opt.maxiter - result.iterations;
            // reference signal
            VecR_t curr_ref = r(t);
            // calculate the control signal, based on current state
            // and current reference
            VecU_t curr_u = controller(curr_x, curr_ref);
            // add the time, control signal and reference to the
            // result vectors
            result.sampledTime.push_back(t);
            result.control.push_back(curr_u);
            result.reference.push_back(curr_ref);
            // simulate the continuous system over this time step [t, t + Ts]
            // and add the time points and states to the result
            auto curr_result = this->simulate(
                std::back_inserter(result.time),
                std::back_inserter(result.solution), curr_u, curr_x, curr_opt);
            result.resultCode |= curr_result.first;
            result.iterations += curr_result.second;
            // update the actual state using the result of the continuous
            // simulation at t + Ts
            curr_x = result.solution.back();
            // start and end point are included by `simulate`, so remove doubles
            result.time.pop_back();
            result.solution.pop_back();
        }
        return result;
    }

    template <class F>
    ODEResultCode simulateRealTime(DiscreteController<Nx, Nu, Ny> &controller,
                                   ReferenceFunction &r, VecX_t x_start,
                                   const AdaptiveODEOptions &opt, F &callback) {
        ODEResultCode resultCode;
        double Ts     = controller.Ts;
        size_t N      = numberOfSamplesInTimeRange(opt.t_start, Ts, opt.t_end);
        VecX_t curr_x = x_start;
        AdaptiveODEOptions curr_opt = opt;
        for (size_t i = 0; i < N; ++i) {
            double t         = opt.t_start + Ts * i;
            curr_opt.t_start = t;
            curr_opt.t_end   = t + Ts;
            VecR_t curr_ref  = r(t);
            VecU_t curr_u    = controller(curr_x, curr_ref);
            if (!callback(t, curr_x, curr_u))
                break;
            auto result = this->simulateEndResult(curr_u, curr_x, curr_opt);
            curr_opt.maxiter -= result.iterations;
            resultCode |= result.resultCode;
            if (resultCode & ODEResultCodes::MAXIMUM_ITERATIONS_EXCEEDED)
                break;
            curr_x = result.solution[0];
        }
        return resultCode;
    }

    /**
     * @brief   Simulate the closed-loop continuous model using the given 
     *          state-less discrete controller, starting from the given
     *          initial state, evaluating the given input function, 
     *          using the given integration options.
     * 
     * @param   controller
     *          The state-less discrete controller that produces a control 
     *          output u, given the system state x and reference state r.
     * @param   observer
     *          The discrete observer that estimates the system state, given
     *          the control signal u and a noisy measurement of the system 
     *          output y.
     * @param   randFnW
     *          A function that returns a random vector of system disturbances.
     * @param   randFnV
     *          A function that returns a random vector of sensor noise.
     * @param   r
     *          The reference function that can be evaluated at any given time
     *          in the integration interval.
     * @param   x_start
     *          The inital state, which is the initial condition for the ODE
     *          solver.
     * @param   opt
     *          A struct of options for the ODE solver.
     * 
     * @return
     *          A struct containing a vector of time points, and a vector of
     *          states that have equal lenghts.  
     *          It also contains the time, estimated state, control output, and
     *          sensor measurement for each time step.
     *          A result code is given as well, to indicate whether the 
     *          integration was successful.
     */
    ObserverControllerSimulationResult
    simulate(DiscreteController<Nx, Nu, Ny> &controller,
             DiscreteObserver<Nx, Nu, Ny> &observer,
             NoiseGenerator<Nu> &randFnW, NoiseGenerator<Ny> &randFnV,
             ReferenceFunction &r, VecX_t x_start,
             const AdaptiveODEOptions &opt) {
        assert(controller.Ts == observer.Ts);
        ObserverControllerSimulationResult result = {};
        double Ts                                 = controller.Ts;
        size_t N = numberOfSamplesInTimeRange(opt.t_start, Ts, opt.t_end);
        // pre-allocate memory for result vectors
        result.sampledTime.reserve(N);
        result.control.reserve(N);
        result.reference.reserve(N);
        result.estimatedSolution.reserve(N);
        result.output.reserve(N);

        // actual state = inital state
        VecX_t curr_x = x_start;
        // estimated state = inital state
        VecX_t curr_x_hat = x_start;
        // For each time step
        AdaptiveODEOptions curr_opt = opt;
        for (size_t k = 0; k < N; ++k) {
            // current time, and integration range
            double t         = opt.t_start + Ts * k;
            curr_opt.t_start = t;
            curr_opt.t_end   = t + Ts;
            curr_opt.maxiter = opt.maxiter - result.iterations;
            // reference signal
            VecR_t curr_ref = r(t);
            // calculate the control signal, based on current estimated state
            // and current reference
            VecU_t curr_u = controller(curr_x_hat, curr_ref);
            // the output of the real system is the output of the system, given
            // the actual state and the control signal, plus the sensor
            // noise
            VecY_t clean_y = this->getOutput(curr_x, curr_u);  // no noise
            VecY_t y       = randFnV(t, clean_y);  // add sensor noise
            // add the time, estimate state, control signal and output to the
            // result vectors
            result.sampledTime.push_back(t);
            result.estimatedSolution.push_back(curr_x_hat);
            result.control.push_back(curr_u);
            result.output.push_back(y);
            result.reference.push_back(curr_ref);

            // calculate the estimated state for the next time step
            //  k+1                                   k      k    k
            curr_x_hat = observer.getStateChange(curr_x_hat, y, curr_u);

            // disturbances
            VecU_t disturbed_u = randFnW(t, curr_u);
            // simulate the continuous system over this time step [t, t + Ts]
            // and add the time points and states to the result
            auto curr_result =
                this->simulate(std::back_inserter(result.time),
                               std::back_inserter(result.solution), disturbed_u,
                               curr_x, curr_opt);
            result.resultCode |= curr_result.first;
            result.iterations += curr_result.second;
            // update the actual state using the result of the continuous
            // simulation at t + Ts
            curr_x = result.solution.back();
            // start and end point are included by `simulate`, so remove doubles
            result.time.pop_back();
            result.solution.pop_back();
        }
        return result;
    }
};

#include <System.hpp>

/** 
 * @brief   Continuous-Time Linear Time-Invariant Model.
 */
template <size_t Nx, size_t Nu, size_t Ny>
class CTLTIModel : public ContinuousModel<Nx, Nu, Ny>,
                   public CTLTISystem<Nx, Nu, Ny> {
  public:
    using VecX_t = typename ContinuousModel<Nx, Nu, Ny>::VecX_t;
    using VecU_t = typename ContinuousModel<Nx, Nu, Ny>::VecU_t;
    using VecY_t = typename ContinuousModel<Nx, Nu, Ny>::VecY_t;
    using VecR_t = typename ContinuousModel<Nx, Nu, Ny>::VecR_t;

    CTLTIModel(const Matrix<Nx, Nx> &A, const Matrix<Nx, Nu> &B,
               const Matrix<Ny, Nx> &C, const Matrix<Ny, Nu> &D)
        : CTLTISystem<Nx, Nu, Ny>{A, B, C, D} {}

    CTLTIModel(const CTLTISystem<Nx, Nu, Ny> &sys)
        : CTLTISystem<Nx, Nu, Ny>{sys} {}

    VecX_t operator()(const VecX_t &x, const VecU_t &u) override {
        return this->getStateChange(x, u);
    }

    VecY_t getOutput(const VecX_t &x, const VecU_t &u) override {
        return this->getSystemOutput(x, u);
    }
};