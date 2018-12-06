#pragma once

class Matrix
{
private:
	int rows;
	int columns;
public:
	double* Data;

	Matrix(int rows, int columns, double* data);

	void Dispose();

	double Get(int i, int j) const;

	int GetRows() const;

	int GetColumns() const;

	Matrix Transpose() const;

	Matrix operator* (const Matrix& m) const;
};

