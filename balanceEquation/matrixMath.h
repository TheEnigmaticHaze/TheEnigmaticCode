#include <stdlib.h>
#include <malloc.h>

typedef struct
{
  int numerator;
  int denominator;
} Fraction;

void simplifyFraction(Fraction *fraction)
{
  if(fraction->denominator < 0)
  {
    fraction->numerator *= -1;
    fraction->denominator *= -1;
  }

  if(fraction->numerator == 0)
  {
    fraction->denominator = 1;
  }

  int negative;
  if(fraction->numerator < 0)
  {
    negative = -1;
  }
  else
  {
    negative = 1;
  }

  fraction->numerator *= negative;

  int i = 2;
  while(i <= fraction->numerator && i <= fraction->denominator)
  {
    if(fraction->numerator % i == 0 && fraction->denominator % i == 0)
    {
      fraction->numerator /= i;
      fraction->denominator /= i;
    }
    else
    {
      i++;
    }
  }

  fraction->numerator *= negative;
}

Fraction *fractionCreate(int numerator, int denominator)
{
  Fraction *newFraction;
  newFraction = (Fraction *)malloc(sizeof(Fraction));

  newFraction->numerator = numerator;
  newFraction->denominator = denominator;

  simplifyFraction(newFraction);

  return newFraction;
}

Fraction *fractionAddFraction(Fraction *a, Fraction *b)
{
  int newNumerator = a->numerator * b->denominator + b->numerator * a->denominator;
  int newDenominator = a->denominator * b->denominator;

  return fractionCreate(newNumerator, newDenominator); 
}

Fraction *fractionMultiplyFraction(Fraction *a, Fraction *b)
{
  return fractionCreate(a->numerator * b->numerator, a->denominator * b->denominator);
}

Fraction *fractionDivideFraction(Fraction *a, Fraction *b)
{
  return fractionCreate(a->numerator * b->denominator, a->denominator * b->numerator);
}

unsigned char fractionLT(Fraction *a, Fraction *b)
{
  float aValue = (float)(a->numerator) / (float)(a->denominator);
  float bValue = (float)(b->numerator) / (float)(b->denominator);

  return aValue > bValue ? 0 : 1;
}

unsigned char fractionGT(Fraction *a, Fraction *b)
{
  return !fractionLT(a, b);
}

Fraction *fractionAbs(Fraction *a)
{
  return fractionCreate(abs(a->numerator), abs(a->denominator));
}

typedef struct 
{
  unsigned int rows;
  unsigned int columns;
  Fraction **entries;
} Matrix;


Fraction *getEntry(Matrix *matrix, unsigned int row, unsigned int column)
{
  return &((matrix->entries)[row][column]);
}

void setEntry(Matrix *matrix,  unsigned int row, unsigned int column, Fraction *newEntry)
{
  (matrix->entries)[row][column] = *newEntry;
}

Matrix *blankMatrix(unsigned int rows, unsigned int columns)
{
  Matrix *newMatrix = (Matrix *)malloc(sizeof(Matrix));

  newMatrix->rows = rows;
  newMatrix->columns = columns;

  newMatrix->entries = (Fraction **)malloc((sizeof(Fraction *)) * rows);

  for(int i = 0; i < rows; i++)
  {
    (newMatrix->entries)[i] = (Fraction *)malloc((sizeof(Fraction)) * columns);
    for(int entry = 0; entry < columns; entry++)
    {
      setEntry(newMatrix, i, entry, fractionCreate(0, 1));
    }
  }

  return newMatrix;
}

Matrix *fromList(unsigned int rows, unsigned int columns, Fraction *entries)
{
  Matrix *newMatrix = blankMatrix(rows, columns);
  for(int row = 0; row < rows; row++)
  {
    for(int column = 0; column < columns; column++)
    {
      setEntry(newMatrix, row, column, &(entries[row * columns + column]));
    }
  }
  return newMatrix;
}

Matrix *identity(unsigned int rows)
{
  Matrix *newMatrix = blankMatrix(rows, rows);

  for(int i = 0; i < rows; i++)
  {
    setEntry(newMatrix, i, i, fractionCreate(1, 1));
  }

  return newMatrix;
}

void rowAddRow(Matrix *matrix, unsigned int row1, unsigned int row2, Fraction *scalar)
{
  Fraction *fractionRow1 = (matrix->entries)[row1];
  Fraction *fractionRow2 = (matrix->entries)[row2];

  for(int i = 0; i < matrix->columns; i++)
  {
    Fraction *toAdd = fractionMultiplyFraction(&fractionRow1[i], scalar);
    Fraction *newFraction = fractionAddFraction(&fractionRow2[i], toAdd);
    fractionRow2[i] = *newFraction;
    free(toAdd);
    free(newFraction);
  }
}

void rowScale(Matrix *matrix, unsigned int row, Fraction *scalar)
{
  for(int column = 0; column < matrix->columns; column++)
  {
    setEntry(matrix, row, column, fractionMultiplyFraction(getEntry(matrix, row, column), scalar));
  }
}

void rowSwap(Matrix *matrix, unsigned int row1, unsigned int row2)
{
  Fraction *tmp = (matrix->entries)[row1];
  (matrix->entries)[row1] = (matrix->entries)[row2];
  (matrix->entries)[row2] = tmp;
}
void debugPrintFraction(Fraction *f)
{
  printf("%d/%d", f->numerator, f->denominator);
}
void debugPrintMatrix(Matrix *matrix)
{
  for(int i = 0; i < matrix->rows; i++)
  {
    for(int j = 0; j < matrix->columns; j++)
    {
      printf("%d/%d ", getEntry(matrix, i, j)->numerator, getEntry(matrix, i, j)->denominator);
    }
    printf("\n");
  }
}

/*
void invert(Matrix *matrix)
{
  Matrix *inverseMatrix = identity(matrix->columns);

  for(int row = 0; row < matrix->rows - 1; row++)
  {
    for(int toZero = row + 1; toZero < matrix->columns; toZero++)
    {
      Fraction *scalar = fractionDivideFraction(getEntry(matrix, toZero, row), getEntry(matrix, row, row));
      scalar->numerator *= -1;
      rowAddRow(matrix, row, toZero, scalar);
      rowAddRow(inverseMatrix, row, toZero, scalar);
    }
  }

  for(int row = matrix->rows - 1; row > 0; row--)
  {
    for(int toZero = row - 1; toZero >= 0; toZero--)
    {
      Fraction *scalar = fractionDivideFraction(getEntry(matrix, toZero, row), getEntry(matrix, row, row));
      scalar->numerator *= -1;
      rowAddRow(matrix, row, toZero, scalar);
      rowAddRow(inverseMatrix, row, toZero, scalar);
    }
  }

  for(int row = 0; row < matrix->rows; row++)
  {
    Fraction *scalar = fractionDivideFraction(fractionCreate(1, 1), getEntry(matrix, row, row));

    rowScale(matrix, row, scalar);
    rowScale(inverseMatrix, row, scalar);
  }

  *matrix = *inverseMatrix;
}
*/

void invert(Matrix *m)
{
  Matrix *inverseMatrix = identity(m->rows);
  unsigned int pivotRow = 0;
  unsigned int pivotColumn = 0;

  while(pivotRow < m->rows && pivotColumn < m->columns)
  {
    unsigned int pivotMax = 0;
    Fraction *maxEntry = fractionCreate(INT_MIN, 1);
    for(int i = pivotRow; i < m->rows; i++)
    {
      pivotMax = fractionGT(maxEntry, fractionAbs(getEntry(m, i, pivotColumn))) ? pivotMax : i;
      maxEntry = fractionGT(maxEntry, fractionAbs(getEntry(m, i, pivotColumn))) ? maxEntry : fractionAbs(getEntry(m, i, pivotColumn));
    }
    if(getEntry(m, pivotMax, pivotColumn)->numerator == 0)
    {
      pivotColumn++;
    }
    else
    {
      rowSwap(m, pivotRow, pivotMax);
      rowSwap(inverseMatrix, pivotRow, pivotMax);
      for(unsigned int i = pivotRow + 1; i < m->rows; i++)
      {
        Fraction *f = fractionDivideFraction(getEntry(m, i, pivotColumn), getEntry(m, pivotRow, pivotColumn));
        f->numerator *= -1;
        //setEntry(m, i, pivotColumn, fractionCreate(0, 1));
        rowAddRow(m, pivotRow, i, f);
        rowAddRow(inverseMatrix, pivotRow, i, f);
      }
    }
    pivotRow++;
    pivotColumn++;
  }
  for(int row = m->rows - 1; row > 0; row--)
  {
    for(int toZero = row - 1; toZero >= 0; toZero--)
    {
      Fraction *scalar = fractionDivideFraction(getEntry(m, toZero, row), getEntry(m, row, row));
      scalar->numerator *= -1;
      rowAddRow(m, row, toZero, scalar);
      rowAddRow(inverseMatrix, row, toZero, scalar);
    }
  }

  for(int row = 0; row < m->rows; row++)
  {
    Fraction *scalar = fractionDivideFraction(fractionCreate(1, 1), getEntry(m, row, row));

    rowScale(m, row, scalar);
    rowScale(inverseMatrix, row, scalar);
  }

  *m = *inverseMatrix;
}

Fraction *matrixMultiplyVector(Matrix *matrix, Fraction *vector)
{
  Fraction *newVector = (Fraction *)malloc(sizeof(Fraction) * matrix->columns);
  for(int row = 0; row < matrix->columns; row++)
  {
    newVector[row] = *fractionCreate(0, 1);
    for(int column = 0; column < matrix->columns; column++)
    {
      newVector[row] = *fractionAddFraction(&newVector[row], fractionMultiplyFraction(getEntry(matrix, row, column), &vector[column]));
    }
  }
  return newVector;
}

unsigned char areVectorsCollinear(Fraction *a, Fraction *b, unsigned int length)
{
  unsigned int index = 0;
  while(a[index].numerator != 0 && b[index].numerator != 0)
  {
    index++;
  }
  Fraction *ratio = fractionDivideFraction(&(a[index]), &(b[index]));

  unsigned char result = 1;
  for(; index < length; index++)
  {
    if(a[index].numerator == 0 && b[index].numerator == 0)
    {
      continue;
    }
    else
    {
      Fraction *ratio2 = fractionDivideFraction(&(a[index]), &(b[index]));
      result = result && (ratio->numerator == ratio2->numerator) && (ratio->denominator == ratio2->denominator);
    }
  }

  free(ratio);
  return result;
}