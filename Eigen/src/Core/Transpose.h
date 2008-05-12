// This file is part of Eigen, a lightweight C++ template library
// for linear algebra. Eigen itself is part of the KDE project.
//
// Copyright (C) 2006-2008 Benoit Jacob <jacob@math.jussieu.fr>
//
// Eigen is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// Alternatively, you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// Eigen is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License or the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License and a copy of the GNU General Public License along with
// Eigen. If not, see <http://www.gnu.org/licenses/>.

#ifndef EIGEN_TRANSPOSE_H
#define EIGEN_TRANSPOSE_H

/** \class Transpose
  *
  * \brief Expression of the transpose of a matrix
  *
  * \param MatrixType the type of the object of which we are taking the transpose
  *
  * This class represents an expression of the transpose of a matrix.
  * It is the return type of MatrixBase::transpose() and MatrixBase::adjoint()
  * and most of the time this is the only way it is used.
  *
  * \sa MatrixBase::transpose(), MatrixBase::adjoint()
  */
template<typename MatrixType>
struct ei_traits<Transpose<MatrixType> >
{
  typedef typename MatrixType::Scalar Scalar;
  typedef typename ei_nested<MatrixType>::type MatrixTypeNested;
  typedef typename ei_unref<MatrixTypeNested>::type _MatrixTypeNested;
  enum {
    RowsAtCompileTime = MatrixType::ColsAtCompileTime,
    ColsAtCompileTime = MatrixType::RowsAtCompileTime,
    MaxRowsAtCompileTime = MatrixType::MaxColsAtCompileTime,
    MaxColsAtCompileTime = MatrixType::MaxRowsAtCompileTime,
    Flags = _MatrixTypeNested::Flags ^ RowMajorBit,
    CoeffReadCost = _MatrixTypeNested::CoeffReadCost
  };
};

template<typename MatrixType> class Transpose
  : public MatrixBase<Transpose<MatrixType> >
{
  public:

    EIGEN_GENERIC_PUBLIC_INTERFACE(Transpose)

    inline Transpose(const MatrixType& matrix) : m_matrix(matrix) {}

    EIGEN_INHERIT_ASSIGNMENT_OPERATORS(Transpose)

  private:

    inline int _rows() const { return m_matrix.cols(); }
    inline int _cols() const { return m_matrix.rows(); }

    inline int _stride(void) const { return m_matrix.stride(); }

    inline Scalar& _coeffRef(int row, int col)
    {
      return m_matrix.const_cast_derived().coeffRef(col, row);
    }

    inline const Scalar _coeff(int row, int col) const
    {
      return m_matrix.coeff(col, row);
    }

    template<int LoadMode>
    inline const PacketScalar _packetCoeff(int row, int col) const
    {
      return m_matrix.template packetCoeff<LoadMode>(col, row);
    }

    template<int LoadMode>
    inline void _writePacketCoeff(int row, int col, const PacketScalar& x)
    {
      m_matrix.const_cast_derived().template writePacketCoeff<LoadMode>(col, row, x);
    }

  protected:
    const typename MatrixType::Nested m_matrix;
};

/** \returns an expression of the transpose of *this.
  *
  * Example: \include MatrixBase_transpose.cpp
  * Output: \verbinclude MatrixBase_transpose.out
  *
  * \sa adjoint(), class DiagonalCoeffs */
template<typename Derived>
inline Transpose<Derived>
MatrixBase<Derived>::transpose()
{
  return Transpose<Derived>(derived());
}

/** This is the const version of transpose(). \sa adjoint() */
template<typename Derived>
inline const Transpose<Derived>
MatrixBase<Derived>::transpose() const
{
  return Transpose<Derived>(derived());
}

/** \returns an expression of the adjoint (i.e. conjugate transpose) of *this.
  *
  * Example: \include MatrixBase_adjoint.cpp
  * Output: \verbinclude MatrixBase_adjoint.out
  *
  * \sa transpose(), conjugate(), class Transpose, class ei_scalar_conjugate_op */
template<typename Derived>
inline const Transpose<Temporary<
                     CwiseUnaryOp<ei_scalar_conjugate_op<typename ei_traits<Derived>::Scalar>, Derived >
                   > >
MatrixBase<Derived>::adjoint() const
{
  return conjugate().temporary().transpose();
}

#endif // EIGEN_TRANSPOSE_H
