unsigned char isAlphaNumeric(char c)
{
  return (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9'));
}

unsigned int indexOfElement(char *symbol)
{
  return ((symbol[0] - 'A') * 27) + ((symbol[1] == '\0') ? 0 : (symbol[1] - 'a' + 1));
}

char *symbolOfIndex(unsigned int index)
{
  char *ret = (char *)malloc(sizeof(char) * 3);
  ret[0] = 'A' + (index / 27);
  ret[1] = ((index % 27) == 0) ? '\0' : 'a' + (index % 27) - 1;
  ret[2] = '\0';

  return ret;
}

void grabElement(char *start, char *destination, char **end)
{
  if(!('A' <= *start && *start <= 'Z'))
  {
    return;
  }
  char *c = start;
  destination[0] = c[0];
  c++;
  unsigned int index = 1;
  while('a' <= *c && *c <= 'z')
  {
    destination[index] = *c;
    c++;
  }
  *end = c;
}

unsigned int _countFormulas(char *equation)
{
  unsigned int formulas = 1;

  char *c = equation;
  while(*c != '\0')
  {
    if(*c == '+' || *c == '=')
    {
      formulas++;
    }
    c++;
  }

  return formulas;
}

unsigned int countFormulas(char *equation, char **starts, char **ends)
{
  unsigned int formulaCount = _countFormulas(equation);
  if(starts == NULL || ends == NULL)
  {
    return formulaCount;
  }

  unsigned int index = 0;

  char *currentChar = equation;
  char prevChar = ' ';

  while(*currentChar != '\0')
  {
    int isCurrentCharOnFormulaStart = ('A' <= *currentChar && *currentChar <= 'Z') || *currentChar == '(';
    isCurrentCharOnFormulaStart &= prevChar == ' ' || prevChar == '+' || prevChar == '=';

    if(isCurrentCharOnFormulaStart)
    {
      starts[index] = currentChar;
    }
    else if(isAlphaNumeric(prevChar) && (*currentChar == ' ' || *currentChar == '+' || *currentChar == '='))
    {
      ends[index] = currentChar;
      index++;
    }

    prevChar = *currentChar;
    currentChar++;
  }

  ends[index] = currentChar;

  return formulaCount;
}

unsigned int formulasOnLeftHandSide(char *equation)
{
  unsigned int formulas = 1;

  char *c = equation;
  while(*c != '\0')
  {
    if(*c == '=')
    {
      return formulas;
    }
    formulas += *c == '+';
    c++;
  }

  return formulas;
}


unsigned int countElements(char *equation, unsigned int *elementInFormula)
{
  for(int i = 0; i < 26 * 27; i++)
  {
    elementInFormula[i] = 0;
  }
  char *currentChar = equation;
  char *endChar;

  while(*currentChar != '\0')
  {
    if('A' <= *currentChar && *currentChar <= 'Z')
    {
      char dest[3] = {'\0', '\0', '\0'};
      grabElement(currentChar, dest, &endChar);
      elementInFormula[indexOfElement(dest)] = 1;
      currentChar = endChar;
    }
    else
    {
      currentChar++;
    }
  }

  unsigned int total = 0;
  for(int i = 0; i < 26 * 27; i++)
  {
    total += elementInFormula[i];
  }

  return total;
}

char *matchingParethese(char *startParenthese)
{
  unsigned int open = 1;
  char *currentChar = startParenthese;
  while(*currentChar != '\0' && open != 0)
  {
    currentChar++;
    open += (*currentChar == '(');
    open -= (*currentChar == ')');
  }
  return currentChar;
}

unsigned char sameElement(char *a, char *b)
{
  if(a[0] != b[0])
  {
    return 0;
  }
  return a[1] == b[1];
}

unsigned int formulaElementCount(char *start, char *end, char *symbol)
{
  unsigned int count = 0;

  char *currentChar = start;

  while(currentChar != end)
  {
    char *endChar;
    if(*currentChar == '(')
    {
      unsigned int subscript = strtol(matchingParethese(currentChar) + 1, &endChar, 10);
      count += formulaElementCount(currentChar + 1, matchingParethese(currentChar), symbol) * subscript;
    }
    else
    {
      char dest[3] = {'\0', '\0', '\0'};
      grabElement(currentChar, dest, &endChar);
      currentChar = endChar;
      unsigned int subscript = ('0' <= *currentChar && *currentChar <= '9') ? strtol(currentChar, &endChar, 10) : 1;
      count += subscript * sameElement(dest, symbol);
    }

    currentChar = endChar;
  }
  
  return count;
}