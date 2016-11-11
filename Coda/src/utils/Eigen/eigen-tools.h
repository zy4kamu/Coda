/**
 * @file	eigen-tools.h
 * @brief	header file containing all header with eigen API and tools functions
 *
 * .. invisible:
 *     _   _ _____ _     _____ _____
 *
 *    | | | |  ___| |   |  ___/  ___|
 *
 *    | | | | |__ | |   | |__ \ `--.
 *
 *    | | | |  __|| |   |  __| `--. \
 *
 *    \ \_/ / |___| |___| |___/\__/ /
 *
 *     \___/\____/\_____|____/\____/
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License
 *
 */


#ifndef EIGEN_TOOLS
#define EIGEN_TOOLS

#include "eigen-data-types.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include "time.h"

using namespace Eigen;
using std::vector;
using std::ios;

namespace eigentools
{
/**
 *Splitts sparse matrix (columns of i_mat) to mat1 and mat2 in portion (0.8 by default)
 */
template<typename Derived>
void splitSparseMatrix(const SparseMatrix<Derived> &i_mat, SparseMatrix<Derived> &o_mat1,SparseMatrix<Derived> &o_mat2, double portion = 0.8)
{
    int num = (int)(portion * (double)i_mat.cols());
    o_mat1 = i_mat.leftCols(num);
    o_mat2 = i_mat.rightCols(i_mat.cols()-num);
}

template <typename Derived>
Matrix<Derived, Dynamic, Dynamic> shuffleByIndexes(const Matrix<Derived, Dynamic, Dynamic>& mat, const std::vector<int>& indexes)
{
    if (indexes.empty())
        return mat;
    PermutationMatrix<Dynamic,Dynamic> perm(mat.cols());
    for (int i = 0; i < perm.indices().size(); i++)
    {
        *(perm.indices().data()+i) = indexes[i];
    }
    return mat * perm;
}


template <typename Derived>
SparseMatrix<Derived> shuffleByIndexes(const SparseMatrix<Derived>& mat, const std::vector<int>& indexes)
{
    if (indexes.empty())
        return mat;
    PermutationMatrix<Dynamic,Dynamic> perm(mat.cols());
    for (int i = 0; i < perm.indices().size(); i++)
    {
        *(perm.indices().data()+i) = indexes[i];
    }
    return mat * perm;
}

template <class M>
M randomShuffleMatrix(const M& mat, std::vector<int>& indexes)
{
    indexes.clear();
    indexes = vector<int>(mat.cols(),0);
    for (size_t i = 0; i < indexes.size();i++)
        indexes[i] = i;
    std::random_shuffle(indexes.begin(), indexes.end());
    return eigentools::shuffleByIndexes(mat,indexes);
}

template <typename Derived>
void randomStohasticMatrix(Matrix<Derived, Dynamic, Dynamic>& m, int rows, int cols, bool NormByRows = true)
{
    srand(time(NULL));
    Matrix<Derived, Dynamic, Dynamic> tmp;
    if (NormByRows)
    {
       tmp = (Matrix<Derived, Dynamic, Dynamic>::Random(rows, cols).array()+1).matrix()/2;
    }
    else
    {
        tmp = (Matrix<Derived, Dynamic, Dynamic>::Random(cols,rows).array()+1).matrix()/2;
    }
    Array<Derived, Dynamic, 1> c = tmp.rowwise().sum().array().pow(-1);
    tmp = (tmp.array().colwise() * c).matrix();
    if (!NormByRows)
        m = tmp.transpose();
    else
        m = tmp;
}

template <typename Derived>
void makeStohastic(Matrix<Derived, Dynamic, Dynamic>& m, bool NormByRows = true)
{
    srand(time(NULL));
    if (NormByRows)
    {
        Array<Derived, Dynamic, 1> c = m.rowwise().sum().array();
        c = (c.array() ==0).select(-1,c);
        m = m.array().colwise() / c;
    }
    else if (!NormByRows)
    {
        Array<Derived, Dynamic, 1> c = m.colwise().sum().array();
        c = (c.array() ==0).select(-1,c);
        m = m.array().rowwise() / c.transpose();
    }
}

template <typename Derived>
void makeStohastic(const Matrix<Derived, Dynamic, Dynamic>& m, Matrix<Derived, Dynamic, Dynamic>& res, bool NormByRows = true)
{
    srand(time(NULL));
    if (NormByRows)
    {
        Array<Derived, Dynamic, 1> c = m.rowwise().sum().array();
        c = (c.array() ==0).select(-1,c);
        res = m.array().colwise() / c;
    }
    else if (!NormByRows)
    {
        Array<Derived, Dynamic, 1> c = m.colwise().sum().array();
        c = (c.array() ==0).select(-1,c);
        res = m.array().rowwise() / c.transpose();
    }
}

template <typename Derived>
ScalarType
density(const Matrix<Derived,Dynamic,Dynamic>& i_mat, ScalarType i_precision = 0)
{
    Matrix<Derived,Dynamic,Dynamic>  a(i_mat.rows(),i_mat.cols());
    a = (i_mat.array() > i_precision).select(1,i_mat);
    a = (i_mat.array() <= i_precision).select(0,a);
    return a.sum()/(i_mat.cols()*i_mat.rows());
}

template <typename Derived>
ScalarType density(const SparseMatrix<Derived>& i_mat)
{
    return (ScalarType)i_mat.nonZeros()/(i_mat.cols()*i_mat.rows());
}

template <typename Derived, typename T>
void eigenVec2StlVecOfPair(const DenseBase<Derived> &i_mat, vector<std::pair<int,T> >& result)
{
    if (i_mat.cols()==1)
    {
        result = vector<std::pair<int,T> >(i_mat.rows(),std::make_pair(0,(T)0));
        for (int i = 0; i < i_mat.rows(); i++)
            result[i] = std::make_pair(i,(T)i_mat.coeff(i,0));
    }
    else if (i_mat.rows()==1)
    {
        result = vector<std::pair<int,T> >(i_mat.cols(),std::make_pair(0,(T)0));
        for (int i = 0; i < i_mat.cols(); i++)
            result[i] = std::make_pair(i,(T)i_mat.coeff(0,i));
    }
    else
    {
        result = vector<std::pair<int,T> >();
    }
}

template <typename Derived, typename T>
void eigenVec2StlVec(const DenseBase<Derived> &i_mat, vector<T>& result)
{
    if (i_mat.cols()==1)
    {
        result = vector<T>(i_mat.rows(),(T)0);
        for (int i = 0; i < i_mat.rows(); i++)
            result[i] = (T)i_mat.coeff(i,0);
    }
    else if (i_mat.rows()==1)
    {
        result = vector<T>(i_mat.cols(),(T)0);
        for (int i = 0; i < i_mat.cols(); i++)
            result[i] = i_mat.coeff(0,i);
    }
    else
    {
        result = vector<T>();
    }
}

template <class M>
void removeColumn(M &io_mat, int col)
{
    int rows = io_mat.rows();
    int cols = io_mat.cols();
    if (col < 0 || col>=cols)
        return;
    io_mat.block(0,col,rows,cols-col-1) = io_mat.block(0,col+1,rows,cols-col-1);
    io_mat.conservativeResize(rows,cols-1);
}

template <class M>
void removeRow(M &io_mat, int row)
{
    int rows = io_mat.rows();
    int cols = io_mat.cols();
    if (row < 0 || row>=rows)
        return;
    io_mat.block(row,0,rows-row-1,cols) =io_mat.block(row+1,0,rows-row-1,cols);
    io_mat.conservativeResize(rows-1,cols);
}

template <typename Derived>
void saveMatrix(const DenseBase<Derived> &i_mat, const std::string& filename)
{
    std::ofstream oStream(filename);
    if (!oStream)
        return;
    oStream<<i_mat.rows()<<std::endl;
    oStream<<i_mat.cols()<<std::endl;
    oStream<<i_mat;
    oStream.close();
}


/**
 *template function for loading from file matrix
 * ATTENTION: correspondence of scalar types in loading and saving matrix is obligation
 */
template <typename Derived>
void loadMatrix(DenseBase<Derived> &i_mat, const std::string &filename)
{
    typedef typename DenseBase<Derived>::Scalar Scalar;
    std::ifstream iStream(filename);
    if (!iStream)
        return;
//        std::cerr<<"Can't open file for writing " << filename << "\n";
    int rows,cols;
    Scalar val;
    std::string line;
    int count = -1;
    char* pEnd;
    while( std::getline(iStream,line) )
    {
        count++;
        if (count == 0)
        {
            rows = (int)strtof(line.c_str(),&pEnd);
            continue;
        }
        if (count == 1)
        {
            cols = (int)strtof(line.c_str(),&pEnd);
            i_mat.derived().resize(rows,cols);
            i_mat.setZero();
            continue;
        }
        pEnd = (char*)line.c_str();
        for (int i = 0; i < cols; i++)
        {
            val = (Scalar)strtod(pEnd,&pEnd);
            i_mat(count-2,i) = val;
        }
    }
    iStream.close();
}

template<typename Derived>
void saveSparseMatrix(SparseMatrix<Derived> &i_mat, const std::string &filename)
{
    std::ofstream oStream(filename);
    if (!oStream)
        return;
    oStream<<i_mat.rows()<<std::endl;
    oStream<<i_mat.cols()<<std::endl;
    for (int k=0; k<i_mat.outerSize(); ++k)
    {
        int count = 0;
        std::string line;
        std::stringstream sstream(line);

        for (typename SparseMatrix<Derived>::InnerIterator it(i_mat,k); it; ++it)
        {
            sstream<<"\t"<<it.row()<<"\t"<<it.value();
            count++;
        }
        oStream<<k<<"\t"<<count<<sstream.str();
        oStream<<std::endl;
    }
    oStream.close();
}

/**
 *template function for loading from file sparse matrix
 * ATTENTION: correspondence of scalar types in loading and saving matrix is obligation
 */
template<typename Derived>
void loadSparseMatrix(SparseMatrix<Derived> &i_mat, const std::string &filename)
{
    typedef typename SparseMatrix<Derived>::Scalar Scalar;
    std::ifstream iStream(filename);
    if (!iStream)
        return;
    int rows,cols,col,row;
    vector<Eigen::Triplet<Scalar> > t;
    std::string line;
    int count = -1,num;
    Scalar value;
	char* pEnd;
    while( std::getline(iStream,line) )
    {
		count++;
		if (count == 0)
		{
            rows = (int)strtof(line.c_str(),&pEnd);
			continue;
		}
		if (count == 1)
		{
            cols = (int)strtof(line.c_str(),&pEnd);
			i_mat = SparseMatrix<Scalar>(rows,cols);
			if (rows == 0 || cols == 0)
			{
				return;
			}
			continue;
		}
		col = (int)strtof(line.c_str(),&pEnd);
		num = (int)strtof(pEnd, &pEnd);
		for (size_t i = 0; i < num; i++)
		{
			row = (int)strtof(pEnd, &pEnd);
            value = (Scalar)strtod(pEnd, &pEnd);
			t.push_back(Eigen::Triplet<Scalar>(row, col, value));
		}
	}
    i_mat.setFromTriplets(t.begin(),t.end());
	iStream.close();
}

template<typename Derived>
void concatenateMatrix(const DenseBase<Derived> &i_mat1, const DenseBase<Derived> &i_mat2, DenseBase<Derived>& res)
{
    res.derived().resize(i_mat1.rows(),i_mat1.cols()+i_mat2.cols());
    res.block(0,0,i_mat1.rows(),i_mat1.cols()) = i_mat1;
    res.block(0,i_mat1.cols(),i_mat2.rows(),i_mat2.cols()) = i_mat2;
}


template<typename Derived>
void removeFirstNCols(const DenseBase<Derived> &i_mat, DenseBase<Derived> &o_mat, int N)
{
    if (N < 0)
    {
        o_mat = i_mat;
        return;
    }
    if (N > i_mat.cols())
    {
        o_mat = i_mat;
        return;
    }
    o_mat.derived().resize(i_mat.rows(),i_mat.cols()-N);
    o_mat = i_mat.block(0,N,i_mat.rows(),i_mat.cols()-N);
}

//template<typename Derived>
//ScalarType cosineMeasure(const MatrixBase<Derived> &a, const MatrixBase<Derived> &b)
template<typename Derived>
void saveSparseMatrixBinary(SparseMatrix<Derived> &i_mat, const std::string &filename)
{
    std::ofstream oStream(filename,ios::out|ios::binary);
    if (!oStream)
        return;
    saveSparseMatrixBinary(i_mat,oStream);
    oStream.close();
}

template<typename Derived>
void saveSparseMatrixBinary(SparseMatrix<Derived> &i_mat, std::ofstream& i_oStream)
{
    if (!i_oStream)
        return;
    int num = i_mat.rows();
    i_oStream.write((char*)& num,4);
    num =i_mat.cols();
    i_oStream.write((char*)& num,4);
    int SizeOfScalarType = (int)sizeof(typename SparseMatrix<Derived>::Scalar);
    i_oStream.write((char*)& SizeOfScalarType,4);

    for (int k=0; k<i_mat.outerSize(); ++k)
    {
        i_oStream.write((char*)& k,4);

        for (typename SparseMatrix<Derived>::InnerIterator it(i_mat,k); it; ++it)
        {
            num = it.row();
            i_oStream.write((char*)& num,4);
            i_oStream.write((char*)& it.value(),SizeOfScalarType);
        }
        num = -1;
        i_oStream.write((char*)&  num,4);
    }
}

template<typename Derived>
void loadSparseMatrixBinary(SparseMatrix<Derived> &o_mat, const std::string &filename)
{
    std::ifstream iStream(filename,ios::in|ios::binary);
    if (!iStream)
        return;
    loadSparseMatrixBinary(o_mat,iStream);
    iStream.close();
}

template<typename Derived>
void loadSparseMatrixBinary(SparseMatrix<Derived> &o_mat, std::ifstream& i_iStream)
{
    if (!i_iStream)
        return;
    typedef typename SparseMatrix<Derived>::Scalar Scalar;
    Scalar value;
    vector<Eigen::Triplet<Scalar> > t;
    int rows,cols, SizeOfScalarType, col,row;
    i_iStream.read((char*)& rows, 4);
    i_iStream.read((char*)& cols, 4);
    i_iStream.read((char*)& SizeOfScalarType, 4);
    if (SizeOfScalarType != (int)sizeof(Scalar))
    {
        std::cerr<<"Sizes of data type stored in matreces are not equal"<<std::endl;
    }
    o_mat.derived().resize(rows,cols);
    while (1)
    {
        i_iStream.read((char*)& col, 4);
        while (1)
        {
            i_iStream.read((char*)& row, 4);
            if (row == -1)
                break;
            i_iStream.read((char*)& value, SizeOfScalarType);
            t.push_back(Eigen::Triplet<Scalar>(row,col,(Scalar)value));
        }
        if (col == cols - 1)
            break;
    }
    o_mat.setFromTriplets(t.begin(),t.end());
}

ScalarType cosineMeasure(DenseMat &a,DenseMat &b);

}

#endif // EIGEN_TOOLS
