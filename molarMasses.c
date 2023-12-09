#include <stdio.h>
#include <stdlib.h>
#include "./elementsAndMasses.h"

int masses[26 * 27] = {0};

int massOfElement(char *elementName)
{
  int symbolLength;
  if(elementName[1] == '\0'){symbolLength = 1;}
  else if(elementName[2] == '\0'){symbolLength = 2;}
  
  return masses[indexOfElement(elementName, symbolLength)];
}

int isDigit(char c){return '0' <= c && c <= '9';}
int isUpper(char c){return 'A' <= c && c <= 'Z';}
int isLower(char c){return 'a' <= c && c <= 'z';}

void getElement(char **startingChar, char *destination)
{
  destination[0] = **startingChar;
  (*startingChar)++;
  destination[1] = '\0';
  destination[2] = '\0';
  if(isLower(**startingChar))
  {
    destination[1] = **startingChar;
    (*startingChar)++;
  }
}

char *matchingParentheseOf(char *start)
{
  int opened = 1;
  while(*start != '\0' && opened != 0)
  {
    start++;
    if(*start == ')'){opened--;}
    if(*start == '('){opened++;}
  }
  return start;
}

int molarMassStartEnd(char *start, char *end)
{
  char dest[3];
  char *currentChar = start;

  int totalMass = 0;

  while(currentChar != end)
  {
    int massOfCurrentElement;

    if(*currentChar == '(')
    {
      massOfCurrentElement = molarMassStartEnd(currentChar + 1, matchingParentheseOf(currentChar));
      char *endChar;
      massOfCurrentElement *= strtol(matchingParentheseOf(currentChar) + 1, &endChar, 10);
      currentChar = endChar;
      totalMass += massOfCurrentElement;
    }
    else
    {
      getElement(&currentChar, dest);
      massOfCurrentElement = massOfElement(dest);

      if(isDigit(*currentChar))
      {
        char *endChar;
        massOfCurrentElement *= strtol(currentChar, &endChar, 10);
        currentChar = endChar;
      }
      
      totalMass += massOfCurrentElement;
    }
  }
  
  return totalMass;
}

int molarMass(char *str)
{
  char *end = str;
  while(*end != '\0'){end++;}
  return molarMassStartEnd(str, end);
}

int main(int argc, char **argv)
{
  if(argc < 2)
  {
    printf("not enough args passed; requires a chemical formula\n");
    return 0;
  }

  initializeMasses(masses);
  int totalMass = molarMass(argv[1]);

  printf("molar mass: %d", totalMass);
  
  return 0;
}