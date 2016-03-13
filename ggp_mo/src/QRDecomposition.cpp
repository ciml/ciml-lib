/*
 * File:   QRDecomposition.cpp
 * Author: Heder Soares Bernardino
 *
 * Created on November 7, 2010, 10:17 AM
 */

#include "QRDecomposition.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <memory.h>

using namespace std;

 /**
  *  QR Decomposition, structure to access R and the Householder vectors and compute Q.
  *  It is computed using Householder reflections.
  *  @param matrix           Rectangular matrix;
  *  @param numberLines      Number of lines of the matrix;
  *  @param numberColumns    Number of columns of the matrix;
  */
 QRDecomposition::QRDecomposition(double const* const* matrix, const int numberLines, const int numberColumns) {
     int i, j, k;
     this->qr = new double*[numberLines];
     for (i = 0; i < numberLines; i++) {
         this->qr[i] = new double[numberColumns];
         memcpy(this->qr[i], matrix[i], numberColumns * sizeof (double));
     }
     this->numberLines = numberLines;
     this->numberColumns = numberColumns;
     this->diagonalR = new double[numberColumns];
     this->fullRank = true;

     for (k = 0; k < numberColumns; k++) {
         // Compute 2-norm of k-th column
         double nrm = 0;
         for (i = k; i < numberLines; i++) {
             nrm = hypot(nrm, this->qr[i][k]);
             //std::cout << nrm << "\t";
         }
         //std::cout << std::endl;

         if (nrm != 0.0) {
             // Make k-th Householder vector
             if (this->qr[k][k] < 0) {
                 nrm = -nrm;
             }
             for (i = k; i < numberLines; i++) {
                 this->qr[i][k] /= nrm;
             }
             this->qr[k][k] += 1.0;

             // Apply transformation to remaining columns
             for (j = k + 1; j < numberColumns; j++) {
                 double s = 0.0;
                 for (i = k; i < numberLines; i++) {
                     s += this->qr[i][k] * this->qr[i][j];
                 }
                 s = -s / this->qr[k][k];
                 for (i = k; i < numberLines; i++) {
                     this->qr[i][j] += s * this->qr[i][k];
                 }
             }
         }
         this->diagonalR[k] = -nrm;
         this->fullRank &= nrm != 0;
     }
 }

 QRDecomposition::QRDecomposition(const QRDecomposition& orig) {
 }

 QRDecomposition::~QRDecomposition() {
     for (int i = 0; i < this->numberLines; i++) {
         delete[] this->qr[i];
     }
     delete[] this->qr;
     delete[] this->diagonalR;
 }

 double** QRDecomposition::createH(int* numberLines, int* numberColumns) {
     *numberLines = this->numberLines;
     *numberColumns = this->numberColumns;
     double** h = new double*[this->numberLines];
     int i, j;
     for (i = 0; i < this->numberLines; i++) {
         h[i] = new double[this->numberColumns];
         for (j = 0; j < this->numberColumns; j++) {
             if (i >= j) {
                 h[i][j] = this->qr[i][j];
             } else {
                 h[i][j] = 0.0;
             }
         }
     }
     return h;
 }

 double** QRDecomposition::createR(int* numberLines, int* numberColumns) {
	*numberLines = this->numberColumns;
	*numberColumns = this->numberColumns;
     double** r = new double*[this->numberColumns];
     int i, j;
     for (i = 0; i < this->numberColumns; i++) {
         r[i] = new double[this->numberColumns];
         for (j = 0; j < this->numberColumns; j++) {
             if (i < j) {
                 r[i][j] = this->qr[i][j];
             } else if (i == j) {
                 r[i][j] = this->diagonalR[i];
             } else {
                 r[i][j] = 0.0;
             }
         }
     }
     return r;
 }

 double** QRDecomposition::createQ(int* numberLines, int* numberColumns) {
	*numberLines = this->numberLines;
	*numberColumns = this->numberColumns;
     double** q = new double*[this->numberLines];
     int i, j, k;
     double sum;
     for (i = 0; i< this->numberLines; i++) {
         q[i] = new double[this->numberColumns];
     }
     for (k = this->numberColumns - 1; k >= 0; k--) {
         for (i = 0; i < this->numberLines; i++) {
             q[i][k] = 0.0;
         }
         q[k][k] = 1.0;
         for (j = k; j < this->numberColumns; j++) {
             if (this->qr[k][k] != 0) {
                 sum = 0.0;
                 for (i = k; i < this->numberLines; i++) {
                     sum += this->qr[i][k] * q[i][j];
                 }
                 sum = -sum / this->qr[k][k];
                 for (i = k; i < this->numberLines; i++) {
                     q[i][j] += sum * this->qr[i][k];
                 }
             }
         }
     }
     return q;
 }

 double* QRDecomposition::solveLeastSquares(double const* rightHandSide, const int dimension) {
     if (dimension != this->numberLines) {
//         cout << "The right-hand side vector and matrix row dimensions must agree." << endl;
         exit(12);
     }
     if (!this->fullRank) {
//         cout << "Matrix is rank deficient." << endl;
         return NULL;
     }

     int i, k;
     double sum;
     // Copy right hand side
     double* x = new double[this->numberLines];
     memcpy(x, rightHandSide, dimension * sizeof (double));

     // Compute Y = transpose(Q)*B
     for (k = 0; k < this->numberColumns; k++) {
         sum = 0.0;
         for (i = k; i < this->numberLines; i++) {
             sum += this->qr[i][k] * x[i];
         }
         sum = -sum / this->qr[k][k];
         for (i = k; i < this->numberLines; i++) {
             x[i] += sum * this->qr[i][k];
         }
     }
     // Solve R*X = Y;
     for (k = this->numberColumns - 1; k >= 0; k--) {
         x[k] /= this->diagonalR[k];
         for (i = 0; i < k; i++) {
             x[i] -= x[k] * this->qr[i][k];
         }
		if ( isnan( x[k] )) {
//			delete[] x;
            x[k] = INFINITY;
//			cout <<"The resulting vector contains nan elements." << endl;
		}
		else if (isinf( x[k] ) ) {
//			delete[] x;
//			cout <<"The resulting vector contains inf elements." << endl;
		}
     }
     return x;
 }


bool QRDecomposition::isFullRank( ) const {

	return this->fullRank;

}
