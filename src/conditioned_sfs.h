#ifndef CONDITIONED_SFS_H
#define CONDITIONED_SFS_H

#include <exception>
#include <map>
#include <random>
#include <gmpxx.h>

#include "common.h"
#include "piecewise_constant_rate_function.h"
#include "moran_eigensystem.h"
#include "mpq_support.h"
#include "demography.h"

typedef struct 
{
    MatrixXq coeffs;
} below_coeff;

typedef struct { Matrix<double> X0, X2, M0, M1; } MatrixCache;

class improper_sfs_exception : public std::exception
{
    virtual const char* what() const throw()
    {
        return "SFS is not a probability distribution";
    }
};

template <typename T>
class ConditionedSFS
{
    public:
    virtual std::vector<Matrix<T> > compute() const;
    void setDemography(const Demography<T> demo);

    protected:
    Demography<T> demo;
};

template <typename T, size_t P>
class NPopConditionedSFS : public ConditionedSFS<T>
{
    public:
};

template <typename T>
class OnePopConditionedSFS : public NPopConditionedSFS<T, 1>
{
    public:
    OnePopConditionedSFS(int, int);
    std::vector<Matrix<T> > compute() const;

    // private:
    std::vector<Matrix<T> > compute_below() const;
    std::vector<Matrix<T> > compute_above() const;

    // Variables
    static MatrixCache& cached_matrices(int n);
    static std::map<int, MatrixCache> matrix_cache;

    const int n, H;
    const MoranEigensystem mei;
    const MatrixCache mcache;
    // std::vector<Matrix<T> > csfs, csfs_below, csfs_above, C_above;
    const Matrix<double> Uinv_mp0, Uinv_mp2;
};

template <typename T>
std::vector<Matrix<T> > incorporate_theta(const std::vector<Matrix<T> > &, const double);

#endif
