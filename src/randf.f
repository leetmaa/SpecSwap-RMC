
c****************************************************************
c     subroutine RAN1(S, ANS)
c     A "minimal standard" random number generator after the model
c     "ran0(idum)" in "numberical recipes in fortran" page 270.
c     http://apps.nrbook.com/fortran/index.html
c****************************************************************
      SUBROUTINE RND1(S, ANS)

      IMPLICIT NONE

      INTEGER S
      DOUBLE PRECISION ANS

      INTEGER SEED, FIRST

      INTEGER A, M, Q, R
      DOUBLE PRECISION MINV
      PARAMETER (A=16807, M=2147483647, Q=127773, R=2836, MINV=1.0/M)

      INTEGER K

      SAVE SEED, FIRST
      DATA SEED /524286/, FIRST /1/

      IF (FIRST .EQ. 1) THEN
         SEED=S+SEED
         FIRST=0
      ENDIF

      K=SEED/Q
      SEED=A*(SEED-K*Q)-K*R

      IF (SEED .LT. 0) SEED = SEED + M

      ANS=SEED*MINV

      RETURN
      END


