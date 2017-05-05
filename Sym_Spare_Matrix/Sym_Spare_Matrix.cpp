// Sym_Spare_Matrix.cpp : 定义控制台应用程序的入口点。
//
#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include<vector>
using namespace std;
//对称矩阵的存储
template<typename T>class SymMatrix
{
protected:
	T*_matrix;
	size_t _size;
	size_t _n;
public:
	SymMatrix(T*matrix, size_t n) :_matrix(new T[n*(n + 1) / 2]), _size(n*(n + 1) / 2), _n(n)
	{
		for (size_t i = 0; i < n; ++i)
		{
			//存储下三角
			for (size_t j = 0; j < n; ++j)
			{
				if (i >= j)
				{
					_matrix[i*(i + 1) / 2 + j] = matrix[i*n + j];
				}
			}
		}
	}
	T&Access(size_t i, size_t j)
	{
		//直接引用
		//如果是上三角元素
		if (i < j)
		{
			swap(i, j);
		}
		return _matrix[i*(i + 1) / 2 + j];
	}
	void PrintSymMatrix()
	{
		for (size_t i = 0; i < _n; i++)
		{
			for (size_t j = 0; j < _n; j++)
			{
				cout << Access(i, j) << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
};
//三元组
template<class T>struct Triple
{
	T _value;
	size_t _rows;//行
	size_t _cols;//列
	Triple(size_t rows = 0, size_t cols = 0, const T&x = T()) :_value(x), _rows(rows), _cols(cols)
	{}
};
template<class T>class SparseMatrix
{
protected:
	vector<Triple<T>>_v;
	size_t _row;//行
	size_t _col;//列
	T _invalid;
public:
	SparseMatrix() :_row(0), _col(0), _invalid(T()), _v(NULL)
	{}
	SparseMatrix(T*a, size_t m, size_t n, const T&invalid) :_row(m), _col(n), _invalid(invalid)
	{
		for (size_t i = 0; i < m; i++)
		{
			for (size_t j = 0; j < n; j++)
			{
				if (a[i*n + j] != invalid)
				{
					Triple<T>t(i, j, a[i*n + j]);
					_v.push_back(t);
				}
			}
		}
	}
	SparseMatrix<T>Transport()
	{
		//矩阵的转置（一般算法）
		SparseMatrix<T>tsm;
		tsm._row = _col;
		tsm._col = _row;
		tsm._v.reserve(_v.size());
		for (size_t i = 0; i < _col; i++)
		{
			//i控制新矩阵的行也就是旧矩阵的列
			size_t index = 0;
			while (index < _v.size())
			{
				//index控制原来三元组的元素的个数
				if (_v[index]._cols == i)
				{
					//遍历原来的三元组如果存在满足对应下坐标的元素则进入新的三元组
					Triple<T>tmp(_v[index]._cols, _v[index]._rows, _v[index]._value);
					tsm._v.push_back(tmp);
				}
				index++;
			}
		}
		return tsm;
	}
	SparseMatrix<T>FastTransport()
	{
		//快速方法
		SparseMatrix<T>ftsm;
		ftsm._row = _col;
		ftsm._col = _row;
		ftsm._v.resize(_v.size());
		//新矩阵的行即旧矩阵的列
		//CountNum统计新矩阵每一行有效的个数
		int*CountNum = new int[_col];
		memset(CountNum, 0, sizeof(int)*_col);
		for (size_t i = 0; i < _v.size(); i++)
		{
			int col = _v[i]._cols;
			++CountNum[col];
		}
		//StartPos记录新矩阵的每一行第一个元素在三元组的存储下标
		int*StartPos = new int[_col];
		memset(StartPos, 0, sizeof(int)*_col);
		size_t i = 0;
		StartPos[i] = 0;
		for (i = 1; i < _col; i++)
		{
			StartPos[i] = StartPos[i - 1] + CountNum[i - 1];
		}
		//遍历原来的三元组，找到数据就直接放入新三元组的对应的下标处
		for (size_t i = 0; i < _v.size(); i++)
		{
			int col = _v[i]._cols;
			size_t tmp = StartPos[col];
			ftsm._v[tmp]._rows = _v[i]._cols;
			ftsm._v[tmp]._cols = _v[i]._rows;
			ftsm._v[tmp]._value = _v[i]._value;
			++StartPos[col];
		}
		delete[]CountNum;
		delete[]StartPos;
		return ftsm;
	}
	void PrintSpareMatrix()
	{
		size_t index = 0;
		for (size_t i = 0; i < _row; i++)
		{
			for (size_t j = 0; j < _col; j++)
			{
				if ((index < _v.size()) && (_v[index]._rows == i) && (_v[index]._cols == j))
				{
					cout << _v[index]._value << " ";
					index++;
				}
				else
				{
					cout << _invalid << " ";
				}
			}
			cout << endl;
		}
		cout << endl;
	}
};
void testSymMatrix()
{
	int arr[5][5] = {
		{ 0, 1, 2, 3, 4 },
		{ 1, 0, 1, 2, 3 },
		{ 2, 1, 0, 1, 2 },
		{ 3, 2, 1, 0, 1 },
		{ 4, 3, 1, 2, 0 }
	};
	SymMatrix<int>sm((int*)arr, 5);
	sm.PrintSymMatrix();
}
void testSparseMatrix()
{
	int arr[6][5]=
	{
		{ 1, 0, 3, 0, 5 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 },
		{ 1, 0, 3, 0, 5 },
		{ 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 }
	};
	SparseMatrix<int>sm1((int*)arr, 6, 5, 0);
	sm1.PrintSpareMatrix();
	SparseMatrix<int>sm2;
	sm2 = sm1.Transport();
	sm2.PrintSpareMatrix();
	SparseMatrix<int>sm3;
	sm3 = sm1.FastTransport();
	sm3.PrintSpareMatrix();
}
int main()
{
	//testSymMatrix();
	testSparseMatrix();
	system("pause");
	return 0;
}