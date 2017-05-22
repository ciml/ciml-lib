#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "gausselimination.h"

void gauss(double **a, double *b, double *x)
{
  int i, j, k, m, n = 3, rowx;
  double xfac, temp, temp1, amax;

  /*
  _______________________________________

    Do the forward reduction step.
  _______________________________________

  */

  rowx = 0;   /* Keep count of the row interchanges */
  for (k = 0; k < n - 1; k++)
  {
    amax = (double) fabs(a[k][k]);
    m = k;
    for (i = k + 1; i < n; i++)
    {   /* Find the row with largest pivot */
      xfac = (double) fabs(a[i][k]);
      if(xfac > amax)
        amax = xfac;
        m = i;
    }
    if(m != k)
    {  /* Row interchanges */
      rowx = rowx + 1;
      temp1 = b[k];
      b[k]  = b[m];
      b[m]  = temp1;
      for(j = k; j < n; j++)
      {
        temp = a[k][j];
        a[k][j] = a[m][j];
        a[m][j] = temp;
      }
    }
    for(i = k + 1; i < n; i++)
    {
      xfac = a[i][k] / a[k][k];
      for (j = k; j < n; j++)
      {
        a[i][j] = a[i][j] - xfac * a[k][j];
      }
        b[i] = b[i] - xfac * b[k];
    }
  }
  /*
  _______________________________________

    Do the back substitution step
  _______________________________________

  */

  for (j = 0; j < n; j++)
  {
    k = n - j - 1;
    x[k] = b[k];
    for(i = k + 1; i < n; i++)
    {
      x[k] = x[k] - a[k][i] * x[i];
    }
    x[k] = x[k] / a[k][k];
  }
}
