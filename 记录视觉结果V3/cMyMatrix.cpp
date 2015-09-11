#include "cMyMatrix.h"


void cMyMatrix::InitMyMatrix(int row, int colum)
{
	if (mat)freeMatrix(mat);
	mat = newMatrix(row, colum);
	mRow = row;
	mColum = colum;
}
//bool cMyMatrix::InitMyMatrix(double* data,int row, int colum)
//{
//	if (mat)freeMatrix(mat);
//	mat = newMatrix(row, colum);
//	mRow = row;
//	mColum = colum;
//
//	return false;
//}


cMyMatrix::cMyMatrix()
{
	mat = NULL;
}


cMyMatrix::~cMyMatrix()
{
	if (mat)freeMatrix(mat);
}


//======================================================
cMyMatrix cMyMatrix::operator = (const cMyMatrix &A)
{
	mRow = A.mRow;
	mColum = A.mColum;
	copyMatrix(mat, A.mat);

	return *this;
}

cMyMatrix cMyMatrix::operator + (const cMyMatrix &A)
{
	cMyMatrix out;
	out.InitMyMatrix(this->mRow, this->mColum);
	out.mat = algMatrix(this->mat, A.mat,1);
	return out;
}

cMyMatrix cMyMatrix::operator - (const cMyMatrix &A)
{
	cMyMatrix out;
	out.InitMyMatrix(this->mRow, this->mColum);
	out.mat = algMatrix(this->mat, A.mat, 2);
	return out;
}

cMyMatrix cMyMatrix::operator * (const cMyMatrix &A)
{
	cMyMatrix out;
	out.InitMyMatrix(this->mRow, this->mColum);
	out.mat = MdotM(this->mat, A.mat);
	return out;
}

cMyMatrix cMyMatrix::Transpose()
{
	cMyMatrix out;
	out.InitMyMatrix(this->mRow, this->mColum);
	copyMatrix(out.mat ,transposeMatrix(this->mat));
	return out;
}

cMyMatrix cMyMatrix::Inv()
{
	cMyMatrix out;
	out.InitMyMatrix(this->mRow, this->mColum);
	copyMatrix(out.mat, invMatrix(this->mat));
	return out;
}
//======================================================