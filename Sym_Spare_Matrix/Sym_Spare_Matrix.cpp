// Sym_Spare_Matrix.cpp : �������̨Ӧ�ó������ڵ㡣
//
#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include<vector>
using namespace std;
//�Գƾ���Ĵ洢
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
			//�洢������
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
		//ֱ������
		//�����������Ԫ��
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
//��Ԫ��
template<class T>struct Triple
{
	T _value;
	size_t _rows;//��
	size_t _cols;//��
	Triple(size_t rows = 0, size_t cols = 0, const T&x = T()) :_value(x), _rows(rows), _cols(cols)
	{}
};
template<class T>class SparseMatrix
{
protected:
	vector<Triple<T>>_v;
	size_t _row;//��
	size_t _col;//��
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
		//�����ת�ã�һ���㷨��
		SparseMatrix<T>tsm;
		tsm._row = _col;
		tsm._col = _row;
		tsm._v.reserve(_v.size());
		for (size_t i = 0; i < _col; i++)
		{
			//i�����¾������Ҳ���Ǿɾ������
			size_t index = 0;
			while (index < _v.size())
			{
				//index����ԭ����Ԫ���Ԫ�صĸ���
				if (_v[index]._cols == i)
				{
					//����ԭ������Ԫ��������������Ӧ�������Ԫ��������µ���Ԫ��
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
		//���ٷ���
		SparseMatrix<T>ftsm;
		ftsm._row = _col;
		ftsm._col = _row;
		ftsm._v.resize(_v.size());
		//�¾�����м��ɾ������
		//CountNumͳ���¾���ÿһ����Ч�ĸ���
		int*CountNum = new int[_col];
		memset(CountNum, 0, sizeof(int)*_col);
		for (size_t i = 0; i < _v.size(); i++)
		{
			int col = _v[i]._cols;
			++CountNum[col];
		}
		//StartPos��¼�¾����ÿһ�е�һ��Ԫ������Ԫ��Ĵ洢�±�
		int*StartPos = new int[_col];
		memset(StartPos, 0, sizeof(int)*_col);
		size_t i = 0;
		StartPos[i] = 0;
		for (i = 1; i < _col; i++)
		{
			StartPos[i] = StartPos[i - 1] + CountNum[i - 1];
		}
		//����ԭ������Ԫ�飬�ҵ����ݾ�ֱ�ӷ�������Ԫ��Ķ�Ӧ���±괦
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