#pragma once

#include <algorithm>
#include <vector>

template <size_t N, size_t M, typename T = int64_t>
class Matrix {
public:
  // Constructors...
  Matrix() { matrix_.resize(N, std::vector<T>(M, 0)); }

  Matrix(const Matrix<N, M, T>& copy) { this->matrix_ = copy.matrix_; }

  Matrix<N, M, T>& operator=(const Matrix<N, M, T>& other) {
    this->matrix_ = other.matrix_;
    return *this;
  }

  Matrix(const T& elem) { matrix_.resize(N, std::vector<T>(M, elem)); }

  Matrix(std::vector<std::vector<T>>& vectors) { this->matrix_ = vectors; }

  ~Matrix() = default;
  // Addition, subtraction, operators +=, -=.
  // Addition and subtraction of matrices of inappropriate sizes should not be
  // compiled.
  // It is guaranteed that the types in the matrices in the tests will support
  // the corresponding operations.
  friend Matrix<N, M, T> operator+(const Matrix<N, M, T>& left,
                                   const Matrix<N, M, T>& right) {
    Matrix sum(left);
    sum += right;
    return sum;
  }
  friend Matrix<N, M, T> operator-(const Matrix<N, M, T>& left,
                                   const Matrix<N, M, T>& right) {
    Matrix diff(left);
    diff -= right;
    return diff;
  }

  Matrix<N, M, T>& operator+=(const Matrix<N, M, T>& other) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        this->matrix_[i][j] += other(i, j);
      }
    }
    return *this;
  }

  Matrix<N, M, T>& operator-=(const Matrix<N, M, T>& other) {
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        this->matrix_[i][j] -= other(i, j);
      }
    }
    return *this;
  }

  // Multiplication by an element of type T (it is guaranteed that the *
  // operator is defined for T)
  friend Matrix<N, M, T> operator*(const Matrix<N, M, T>& matrix,
                                   const T& number) {
    Matrix mul(matrix);
    for (std::vector<T>& row : mul.matrix_) {
      for (T& elements : row) {
        elements *= number;
      }
    }
    return mul;
  }
  friend Matrix<N, M, T> operator*(const T& number,
                                   const Matrix<N, M, T>& matrix) {
    return matrix * number;
  }

  // The Transposed() method, which returns the transposed matrix.
  Matrix<M, N, T> Transposed() const {
    Matrix<M, N, T> transposed_matrix;
    for (size_t i = 0; i < N; ++i) {
      for (size_t j = 0; j < M; ++j) {
        transposed_matrix(j, i) = this->matrix_[i][j];
      }
    }
    return transposed_matrix;
  }

  // The Trace() method is to calculate the trace of the matrix.
  T Trace() const {
    T trace = 0;
    for (size_t i = 0; i < N; ++i) {
      trace += matrix_[i][i];
    }

    return trace;
  }

  // Operator (i, j) that returns an element of the matrix in the i-th row and
  // in the j-th column. It is necessary to be able to change the value for
  // non-constant matrices.
  T& operator()(size_t index_i, size_t index_j) {
    return this->matrix_[index_i][index_j];
  }
  const T& operator()(size_t index_i, size_t index_j) const {
    return this->matrix_[index_i][index_j];
  }

  // Equality check operator.
  bool operator==(const Matrix& other) {
    return this->matrix_ == other.matrix_;
  }

private:
  std::vector<std::vector<T>> matrix_;
};

// Multiplication of two matrices. An attempt to multiply matrices of
// inappropriate sizes should lead to a compilation error.
template <size_t N, size_t M, size_t K, typename T = int64_t>
Matrix<N, M, T> operator*(const Matrix<N, K, T>& left,
                          const Matrix<K, M, T>& right) {
  Matrix<N, M, T> res;
  for (size_t index_x = 0; index_x < N; ++index_x) {
    for (size_t index_y = 0; index_y < M; ++index_y) {
      for (size_t index_z = 0; index_z < K; ++index_z) {
        res(index_x, index_y) +=
                left(index_x, index_z) * right(index_z, index_y);
      }
    }
  }
  return res;
}
