/*
 * File:   QRDecomposition.h
 * Author: hedersb
 *
 * Created on November 7, 2010, 10:17 AM
 */

#ifndef _QRDECOMPOSITION_H
#define	_QRDECOMPOSITION_H

class QRDecomposition {
    public:
        QRDecomposition(double const* const* matrix, const int numberLines, const int numberColumns);
        QRDecomposition(const QRDecomposition& orig);
        virtual ~QRDecomposition();
        double** createH(int* numberLines, int* numberColumns);
        double** createR(int* numberLines, int* numberColumns);
        double** createQ(int* numberLines, int* numberColumns);
        double* solveLeastSquares(double const* rightHandSide, const int dimension);
		bool isFullRank( ) const;
    private:
        double** qr;
        int numberLines;
        int numberColumns;
        double* diagonalR;
        bool fullRank;
};

#endif	/* _QRDECOMPOSITION_H */

