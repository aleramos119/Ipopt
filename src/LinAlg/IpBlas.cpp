// Copyright (C) 2004, 2006 International Business Machines and others.
// All Rights Reserved.
// This code is published under the Eclipse Public License.
//
// Authors:  Carl Laird, Andreas Waechter     IBM    2004-08-13

#include "IpoptConfig.h"
#include "IpBlas.hpp"

/* we currently have no separate check for Blas, but assume that Blas comes with Lapack
 * thus, we use the nameing convention of Lapack for Blas, too
 */
#ifndef IPOPT_BLAS_FUNC
#define IPOPT_BLAS_FUNC(name,NAME) IPOPT_LAPACK_FUNC(name,NAME)
#endif

#ifdef IPOPT_SINGLE
#define IPOPT_BLAS_FUNCP(x,X) IPOPT_BLAS_FUNC(s ## x,S ## X)
#else
#define IPOPT_BLAS_FUNCP(x,X) IPOPT_BLAS_FUNC(d ## x,D ## X)
#endif

#include <cstring>

// Prototypes for the BLAS routines
extern "C"
{
   /** BLAS Fortran function XDOT */
   ipfreal IPOPT_BLAS_FUNCP(dot, DOT)(
      ipfint*        n,
      const ipfreal* x,
      ipfint*        incX,
      const ipfreal* y,
      ipfint*        incY
   );

   /** BLAS Fortran function XNRM2 */
   ipfreal IPOPT_BLAS_FUNCP(nrm2, NRM2)(
      ipfint*        n,
      const ipfreal* x,
      ipfint*        incX
   );

   /** BLAS Fortran function XASUM */
   ipfreal IPOPT_BLAS_FUNCP(asum, ASUM)(
      ipfint*        n,
      const ipfreal* x,
      ipfint*        incX
   );

#ifdef IPOPT_SINGLE
   /** BLAS Fortran function ISAMAX */
   ipfint IPOPT_BLAS_FUNC(isamax, ISAMAX)(
#else
   /** BLAS Fortran function IDAMAX */
   ipfint IPOPT_BLAS_FUNC(idamax, IDAMAX)(
#endif
      ipfint*        n,
      const ipfreal* x,
      ipfint*        incX
   );

   /** BLAS Fortran subroutine XCOPY */
   void IPOPT_BLAS_FUNCP(copy, COPY)(
      ipfint*        n,
      const ipfreal* x,
      ipfint*        incX,
      ipfreal*       y,
      ipfint*        incY
   );

   /** BLAS Fortran subroutine XAXPY */
   void IPOPT_BLAS_FUNCP(axpy, AXPY)(
      ipfint*        n,
      const ipfreal* alpha,
      const ipfreal* x,
      ipfint*        incX,
      ipfreal*       y,
      ipfint*        incY
   );

   /** BLAS Fortran subroutine XSCAL */
   void IPOPT_BLAS_FUNCP(scal, SCAL)(
      ipfint*        n,
      const ipfreal* alpha,
      const ipfreal* x,
      ipfint*        incX
   );

   /** BLAS Fortran subroutine XGEMV */
   void IPOPT_BLAS_FUNCP(gemv, GEMV)(
      char*          trans,
      ipfint*        m,
      ipfint*        n,
      const ipfreal* alpha,
      const ipfreal* a,
      ipfint*        lda,
      const ipfreal* x,
      ipfint*        incX,
      const ipfreal* beta,
      ipfreal*       y,
      ipfint*        incY,
      int            trans_len
   );

   /** BLAS Fortran subroutine XSYMV */
   void IPOPT_BLAS_FUNCP(symv, SYMV)(
      char*          uplo,
      ipfint*        n,
      const ipfreal* alpha,
      const ipfreal* a,
      ipfint*        lda,
      const ipfreal* x,
      ipfint*        incX,
      const ipfreal* beta,
      ipfreal*       y,
      ipfint*        incY,
      int            uplo_len
   );

   /** BLAS Fortran subroutine XGEMM */
   void IPOPT_BLAS_FUNCP(gemm, GEMM)(
      char*          transa,
      char*          transb,
      ipfint*        m,
      ipfint*        n,
      ipfint*        k,
      const ipfreal* alpha,
      const ipfreal* a,
      ipfint*        lda,
      const ipfreal* b,
      ipfint*        ldb,
      const ipfreal* beta,
      ipfreal*       c,
      ipfint*        ldc,
      int            transa_len,
      int            transb_len
   );

   /** BLAS Fortran subroutine XSYRK */
   void IPOPT_BLAS_FUNCP(syrk, SYRK)(
      char*          uplo,
      char*          trans,
      ipfint*        n,
      ipfint*        k,
      const ipfreal* alpha,
      const ipfreal* a,
      ipfint*        lda,
      const ipfreal* beta,
      ipfreal*       c,
      ipfint*        ldc,
      int            uplo_len,
      int            trans_len
   );

   /** BLAS Fortran subroutine XTRSM */
   void IPOPT_BLAS_FUNCP(trsm, TRSM)(
      char*          side,
      char*          uplo,
      char*          transa,
      char*          diag,
      ipfint*        m,
      ipfint*        n,
      const ipfreal* alpha,
      const ipfreal* a,
      ipfint*        lda,
      const ipfreal* b,
      ipfint*        ldb,
      int            side_len,
      int            uplo_len,
      int            transa_len,
      int            diag_len
   );
}

namespace Ipopt
{
Number IpBlasDot(
   Index         size,
   const Number* x,
   Index         incX,
   const Number* y,
   Index         incY
)
{
   if( incX > 0 && incY > 0 )
   {
      ipfint n = size, INCX = incX, INCY = incY;
      return IPOPT_BLAS_FUNCP(dot, DOT)(&n, x, &INCX, y, &INCY);
   }
   else
   {
      Number s = 0.0;

      for( ; size; --size, x += incX, y += incY )
      {
         s += *x * *y;
      }

      return s;
   }
}

Number IpBlasNrm2(
   Index         size,
   const Number* x,
   Index         incX
)
{
   ipfint n = size, INCX = incX;
   return IPOPT_BLAS_FUNCP(nrm2, NRM2)(&n, x, &INCX);
}

Number IpBlasAsum(
   Index         size,
   const Number* x,
   Index         incX
)
{
   ipfint n = size, INCX = incX;
   return IPOPT_BLAS_FUNCP(asum, ASUM)(&n, x, &INCX);
}

/** interface to FORTRAN routine IXAMAX */
Index IpBlasIamax(
   Index         size,
   const Number* x,
   Index         incX
)
{
   ipfint n = size, INCX = incX;
#ifdef IPOPT_SINGLE
   return (Index) IPOPT_BLAS_FUNC(isamax, ISAMAX)(&n, x, &INCX);
#else
   return (Index) IPOPT_BLAS_FUNC(idamax, IDAMAX)(&n, x, &INCX);
#endif
}

/** interface to FORTRAN routine XCOPY */
void IpBlasCopy(
   Index         size,
   const Number* x,
   Index         incX,
   Number*       y,
   Index         incY
)
{
   if( incX > 0 )
   {
      ipfint N = size, INCX = incX, INCY = incY;
      IPOPT_BLAS_FUNCP(copy, COPY)(&N, x, &INCX, y, &INCY);
   }
   else if( incY == 1 )
   {
      for( ; size; --size, ++y )
      {
         *y = *x;
      }
   }
   else
   {
      for( ; size; --size, y += incY )
      {
         *y = *x;
      }
   }
}

void IpBlasAxpy(
   Index         size,
   Number        alpha,
   const Number* x,
   Index         incX,
   Number*       y,
   Index         incY
)
{
   if( incX > 0 )
   {
      ipfint N = size, INCX = incX, INCY = incY;
      IPOPT_BLAS_FUNCP(axpy, AXPY)(&N, &alpha, x, &INCX, y, &INCY);
   }
   else if( incY == 1 )
   {
      for( ; size; --size, ++y )
      {
         *y += alpha * *x;
      }
   }
   else
   {
      for( ; size; --size, y += incY )
      {
         *y += alpha * *x;
      }
   }
}

void IpBlasScal(
   Index   size,
   Number  alpha,
   Number* x,
   Index   incX
)
{
   ipfint N = size, INCX = incX;
   IPOPT_BLAS_FUNCP(scal, SCAL)(&N, &alpha, x, &INCX);
}

void IpBlasGemv(
   bool          trans,
   Index         nRows,
   Index         nCols,
   Number        alpha,
   const Number* A,
   Index         ldA,
   const Number* x,
   Index         incX,
   Number        beta,
   Number*       y,
   Index         incY
)
{
   ipfint M = nCols, N = nRows, LDA = ldA, INCX = incX, INCY = incY;

   char TRANS;
   if( trans )
   {
      TRANS = 'T';
   }
   else
   {
      TRANS = 'N';
   }
   IPOPT_BLAS_FUNCP(gemv, GEMV)(&TRANS, &M, &N, &alpha, A, &LDA, x, &INCX, &beta, y, &INCY, 1);
}

void IpBlasSymv(
   Index         n,
   Number        alpha,
   const Number* A,
   Index         ldA,
   const Number* x,
   Index         incX,
   Number        beta,
   Number*       y,
   Index         incY
)
{
   ipfint N = n, LDA = ldA, INCX = incX, INCY = incY;

   char UPLO = 'L';
   IPOPT_BLAS_FUNCP(symv, SYMV)(&UPLO, &N, &alpha, A, &LDA, x, &INCX, &beta, y, &INCY, 1);
}

void IpBlasGemm(
   bool          transa,
   bool          transb,
   Index         m,
   Index         n,
   Index         k,
   Number        alpha,
   const Number* A,
   Index         ldA,
   const Number* B,
   Index         ldB,
   Number        beta,
   Number*       C,
   Index         ldC
)
{
   ipfint M = m, N = n, K = k, LDA = ldA, LDB = ldB, LDC = ldC;

   char TRANSA;
   if( transa )
   {
      TRANSA = 'T';
   }
   else
   {
      TRANSA = 'N';
   }
   char TRANSB;
   if( transb )
   {
      TRANSB = 'T';
   }
   else
   {
      TRANSB = 'N';
   }
   IPOPT_BLAS_FUNCP(gemm, GEMM)(&TRANSA, &TRANSB, &M, &N, &K, &alpha, A, &LDA, B, &LDB, &beta, C, &LDC, 1, 1);
}

void IpBlasSyrk(
   bool          trans,
   Index         ndim,
   Index         nrank,
   Number        alpha,
   const Number* A,
   Index         ldA,
   Number        beta,
   Number*       C,
   Index         ldC
)
{
   ipfint N = ndim, K = nrank, LDA = ldA, LDC = ldC;

   char UPLO = 'L';
   char TRANS;
   if( trans )
   {
      TRANS = 'T';
   }
   else
   {
      TRANS = 'N';
   }
   IPOPT_BLAS_FUNCP(syrk, SYRK)(&UPLO, &TRANS, &N, &K, &alpha, A, &LDA, &beta, C, &LDC, 1, 1);
}

void IpBlasTrsm(
   bool          trans,
   Index         ndim,
   Index         nrhs,
   Number        alpha,
   const Number* A,
   Index         ldA,
   Number*       B,
   Index         ldB
)
{
   ipfint M = ndim, N = nrhs, LDA = ldA, LDB = ldB;

   char SIDE = 'L';
   char UPLO = 'L';
   char TRANSA;
   if( trans )
   {
      TRANSA = 'T';
   }
   else
   {
      TRANSA = 'N';
   }
   char DIAG = 'N';
   IPOPT_BLAS_FUNCP(trsm, TRSM)(&SIDE, &UPLO, &TRANSA, &DIAG, &M, &N, &alpha, A, &LDA, B, &LDB, 1, 1, 1, 1);
}

} // namespace Ipopt
