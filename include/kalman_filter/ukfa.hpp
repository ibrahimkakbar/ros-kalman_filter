/// \file kalman_filter/ukfa.hpp
/// \brief Defines the kalman_filter::ukfa_t class.
#ifndef KALMAN_FILTER___UKFA_H
#define KALMAN_FILTER___UKFA_H

#include <kalman_filter/base.hpp>

/// \brief Includes objects for Kalman Filtering.
namespace kalman_filter {

/// \brief An Unscented Kalman Filter with augmented state.
/// \details An augmented state UKF can model non-additive noise at the cost of higher computation.
class ukfa_t
    : public base_t
{
public:
    // CONSTRUCTORS
    /// \brief Instantiates a new ukfa_t object.
    /// \param n_variables The number of variables in the state vector.
    /// \param n_observers The number of state observers.
    ukfa_t(uint32_t n_variables, uint32_t n_observers);

    // MODEL FUNCTIONS
    /// \brief Predicts a new state by transitioning from a prior state.
    /// \param xp The prior state to transition from.
    /// \param q The prediction's noise vector.
    /// \param x (OUTPUT) The predicted new state.
    /// \note This function must not make changes to any external object.
    virtual void state_transition(const Eigen::VectorXd& xp, const Eigen::VectorXd& q, Eigen::VectorXd& x) const = 0;
    /// \brief Predicts an observation from a state.
    /// \param x The state to predict an observation from.
    /// \param r The prediction's noise vector.
    /// \param z (OUTPUT) The predicted observation.
    /// \note This function must not make changes to any external object.
    virtual void observation(const Eigen::VectorXd& x, const Eigen::VectorXd& r, Eigen::VectorXd& z) const = 0;

    // FILTER METHODS
    void iterate() override;

    // PARAMETERS
    /// \brief The alpha parameter of the UKF.
    double_t alpha;
    /// \brief The kappa parameter of the UKF.
    double_t kappa;
    /// \brief The beta parameter of the UKF.
    double_t beta;

private:   
    // DIMENSIONS
    /// \brief The number of variables in the augemented state (x q z).
    uint32_t n_a;
    /// \brief The number of sigma points.
    uint32_t n_s;

    // STORAGE: WEIGHTS
    /// \brief The mean recovery weight vector.
    Eigen::VectorXd wm;
    /// \brief The covariance recovery weight vector.
    Eigen::VectorXd wc;

    // STORAGE: PREDICTION
    /// \brief The variable covariance sigma matrix (positive half).
    Eigen::MatrixXd Xp;
    /// \brief The process noise sigma matrix (positive half).
    Eigen::MatrixXd Xq;
    /// \brief The evaluated variable sigma matrix.
    Eigen::MatrixXd X;
    /// \brief The evaluated variable sigma matrix minus it's mean.
    Eigen::MatrixXd dX;

    // STORAGE: UPDATE
    /// \brief The observation noise sigma matrix (positive half).
    Eigen::MatrixXd Xr;
    /// \brief The evaluated observation sigma matrix.
    Eigen::MatrixXd Z;

    // STORAGE: INTERFACES
    /// \brief An interface to the prior state vector.
    Eigen::VectorXd i_xp;
    /// \brief An interface to the process noise vector.
    Eigen::VectorXd i_q;
    /// \brief An interface to the current state vector.
    Eigen::VectorXd i_x;
    /// \brief An interface to the observation noise vector.
    Eigen::VectorXd i_r;
    /// \brief An interface to the predicted observation vector.
    Eigen::VectorXd i_z;

    // STORAGE: TEMPORARIES
    /// \brief A temporary working matrix of size z,z.
    Eigen::MatrixXd t_zz;
    /// \brief A temporary working matrix of size x,s.
    Eigen::MatrixXd t_xs;
    /// \brief A temporary working matrix of size z,s.
    Eigen::MatrixXd t_zs;

    // UTILITY
    /// \brief An LLT object for storing results of Cholesky decompositions.
    mutable Eigen::LLT<Eigen::MatrixXd> llt;
};

}

#endif