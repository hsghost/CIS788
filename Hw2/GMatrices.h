/*
/
*/

#include <Eigen/Sparse>

#ifndef __GMatrices_H__
#define __GMarrices_H__

typedef Eigen::SparseMatrix<double> MatrixSd;
typedef Eigen::SparseVector<double> VectorSd;
typedef Eigen::Triplet<double> GTri;
typedef std::tuple<int, int> GTup;

struct GC_hash : public std::unary_function<GTup, std::size_t>
{
	std::size_t operator()(const GTup& tup) const
	{
		return std::get<0>(tup) ^ std::get<1>(tup);
	}
};

#endif
