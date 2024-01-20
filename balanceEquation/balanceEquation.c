#include <stdio.h>
#include <stdlib.h>
#include "./matrixMath.h"
#include "./elements.h"

unsigned int _gcd(unsigned int a, unsigned int b)
{
  if(a == 0)
  {
    return b;
  }
  if(b == 0)
  {
    return a;
  }
  int greater = a > b ? a : b;
  int smaller = a > b ? b : a;
  return _gcd(greater - smaller, smaller);
}

unsigned int gcd(unsigned int *list, unsigned int length)
{
  if(length == 2)
  {
    return _gcd(list[0], list[1]);
  }
  return _gcd(list[0], gcd(&(list[1]), length - 1));
}

unsigned int *divideToInts(Fraction *list, unsigned int length)
{
  for(unsigned int i = 0; i < length - 1; i++)
  {
    if((list[i].numerator < 0) ^ (list[i + 1].numerator < 0))
    {
      printf("cannot make all positive");
      return NULL;
    }
  }

  int productOfDenominators = 1;
  for(unsigned int i = 0; i < length; i++)
  {
    productOfDenominators *= (list[i]).denominator;
  }

  if(productOfDenominators == 0)
  {
    printf("division by 0...");
    return NULL;
  }

  unsigned int *output = (unsigned int *)malloc(sizeof(unsigned int) * length);
  Fraction *newFraction = fractionCreate(productOfDenominators, 1);
  for(unsigned int i = 0; i < length; i++)
  {
    Fraction *product = fractionMultiplyFraction(&(list[i]), newFraction);
    output[i] = product->numerator;
    free(product);
  }
  free(newFraction);

  unsigned int outputGCD = gcd(output, length);

  for(int i = 0; i < length; i++)
  {
    output[i] /= outputGCD;
  }
  
  return output;
}

unsigned int *coefficients(char *equation)
{
  unsigned int elementInEquation[26 * 27];
  int f = _countFormulas(equation);

  char **formulaStarts = (char **)malloc(sizeof(char *) * f);
  char **formulaEnds = (char **)malloc(sizeof(char *) * f);

  unsigned int formulaCount = countFormulas(equation, formulaStarts, formulaEnds);
  unsigned int elementCount = countElements(equation, elementInEquation);
  unsigned int reactantCount = formulasOnLeftHandSide(equation);

  if(elementCount < formulaCount - 1)
  {
    printf("infinite ways to balance the equation");
    return NULL;
  }

  Matrix *matrix = blankMatrix(formulaCount - 1, formulaCount);

  Fraction *vector = (Fraction *)malloc(sizeof(Fraction) * formulaCount);
  unsigned int elementIndex;
  unsigned int matrixRowIndex = 0;
  for(elementIndex = 0; elementIndex < 27 * 26; elementIndex++)
  {
    if(!elementInEquation[elementIndex])
    {
      continue;
    }
    Fraction elementCounts[formulaCount];
    unsigned int formulaIndex;
    for(formulaIndex = 0; formulaIndex < formulaCount; formulaIndex++)
    {
      unsigned int numerator = formulaElementCount(formulaStarts[formulaIndex], formulaEnds[formulaIndex], symbolOfIndex(elementIndex));
      int denominator = (formulaIndex < reactantCount && formulaIndex != 0) ? 1 : -1;
      Fraction *myFraction = fractionCreate(numerator, denominator);
      elementCounts[formulaIndex] = *myFraction;
      free(myFraction);
    }

    unsigned char isCollinearWithRow = 0;
    unsigned int rowIndex;
    for(rowIndex = 0; rowIndex < matrixRowIndex; rowIndex++)
    {
      isCollinearWithRow |= areVectorsCollinear((matrix->entries)[rowIndex], elementCounts, formulaCount);
    }

    if(!isCollinearWithRow)
    {
      for(int i = 0; i < formulaCount; i++)
      {
        if(matrixRowIndex >= formulaCount - 1)
        {
          printf("cannot balance equation...\n");
          return NULL;
        }
        setEntry(matrix, matrixRowIndex, i, &elementCounts[i]);
      }
      matrixRowIndex++;
    }
  }

  if(matrixRowIndex < (formulaCount - 1))
  {
    printf("infinite ways to balance equation");
    return NULL;
  }

  for(int i = 0; i < matrix->rows; i++)
  {
    vector[i] = *getEntry(matrix, i, 0);
    (matrix->entries)[i] = &((matrix->entries)[i][1]);
  }

  matrix->columns--;
  invert(matrix);

  Fraction *newVector = matrixMultiplyVector(matrix, vector);
  Fraction *coefficientsFractions = (Fraction *)malloc(sizeof(Fraction) * formulaCount);
  Fraction *newFraction = fractionCreate(1, 1);
  coefficientsFractions[0] = *newFraction;
  free(newFraction);
  for(int i = 1; i < formulaCount; i++)
  {
    coefficientsFractions[i] = newVector[i - 1];
  }

  unsigned int *output = divideToInts(coefficientsFractions, formulaCount);
  free(matrix);
  return output;
}


int main(int argc, char **argv)
{
  char *equation;
  if(argc == 1)
  {
    equation = "PbCO3 + HC2H3O2 = CO2 + H2O + Pb(C2H3O2)2";
    //equation = "C2H2 + O2 = CO2 + H2O";
    //equation = "K = KO2";
  }
  else
  {
    equation = argv[1];
  }
  unsigned int *balancedEquationCoefficients = coefficients(equation);
  if(balancedEquationCoefficients == NULL)
  {
    return 1;
  }
  for(int i = 0; i < _countFormulas(equation); i++)
  {
    printf("%d ", balancedEquationCoefficients[i]);
  }

  return 0;
}