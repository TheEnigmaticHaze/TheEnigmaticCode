#include <iostream>
#include <string>
using namespace std;

int squishBits(bool bits[],int length) // {1,0,1,1,0} -> 10110
{
  int result = 0;

  for(int i = 0; i < length; i++)
  {
    result += bits[i];
    result <<= 1;
  }

  return result >> 1;
}


int grabBitsFromBitChooser(int genocode, int bitChooser, int geneCount)
{
  bool bitsToSquish[geneCount];

  for(int i = 0; i < geneCount; i++)
  {
    bitsToSquish[geneCount-1-i] = (genocode >> (i*2 + ((bitChooser >> i)%2)))%2;
  }

  return squishBits(bitsToSquish, geneCount);
}

int crossGenotypes(int genocode1, int genocode2, int bitChooser1, int bitChooser2, int geneCount)
{
  int gamete1 = grabBitsFromBitChooser(genocode1, bitChooser1, geneCount);
  int gamete2 = grabBitsFromBitChooser(genocode2, bitChooser2, geneCount);

  bool bitsToSquish[geneCount*2];

  for(int i = 0; i < geneCount; i++)
  {
    bitsToSquish[geneCount*2 - i*2 - 1] = (gamete1 >> i)%2;
    bitsToSquish[geneCount*2 - i*2 - 2] = (gamete2 >> i)%2;
  }

  for(int i = 0; i < geneCount*2; i+=2)
  {
    if((bitsToSquish[i] == false) && (bitsToSquish[i+1] == true))
    {
      bitsToSquish[i] = true;
      bitsToSquish[i+1] = false;
    }
  }

  return squishBits(bitsToSquish, geneCount*2);
}

int genocodeToTrinary(int genocode, int geneCount)
{
  int placeValueMultiplier = 1;

  int result = 0;

  for(int i = 0; i < geneCount; i++)
  {
    if((genocode >> 2*i) % 4 == 0 || (genocode >> 2*i) % 4 == 1)
    {
      result += ((genocode >> 2*i) % 4)*placeValueMultiplier;
    } else {
      result += (((genocode >> 2*i) % 4) - 1)*placeValueMultiplier;
    }

    placeValueMultiplier*=3;
  }

  return result;
}

void logGenotypes(int genotype1, int genotype2, int geneCount, int genoList[])
{
  int counter = 0;

  for(int bitChooser1 = 0; bitChooser1 < 1<<(geneCount); bitChooser1++)
  {
    for(int bitChooser2 = 0; bitChooser2 < 1<<(geneCount); bitChooser2++)
    {
      int newGenotype = crossGenotypes(genotype1,genotype2,bitChooser1,bitChooser2,geneCount);
      genoList[counter] = newGenotype;
      counter++;
    }
  }
}

bool isUpper(char a)
{
  return a <= 'Z' && a >= 'A';
}
bool isLower(char a)
{
  return a <= 'z' && a >= 'a';
}
char toUpper(char a)
{
  return isLower(a) ? a-('a'-'A') : a;
}
char toLower(char a)
{
  return isUpper(a) ? a+('a'-'A') : a;
}

bool checkGenotype(string genotype) // checks if a genotype is valid
{
  if(genotype.length() % 2 != 0)
  {
    cout << "Genotype is not an even length" << endl;
    return false;
  }

  for(int i = 0; i < genotype.length(); i+=2)
  {
    if(toUpper(genotype.at(i)) != toUpper(genotype.at(i+1)))
    {
      cout << "unmatched allele" << endl;
      return false;
    }
  }

  return true;
}

void grabGenotype(string &genotype)
{
  cout << "input a genotype: ";
  cin >> genotype;

  while(!checkGenotype(genotype))
  {
    cout << "try again: ";
    cin >> genotype;
  }
}

bool areListsEqual(char a[], char b[], int lengthOfA, int lengthOfB)
{
  if(lengthOfA != lengthOfB)
  {
    cout << "lists are different sizes: " << lengthOfA << " and " << lengthOfB << endl;
    return false;
  }

  for(int i = 0; i < lengthOfA; i++)
  {
    if(a[i] != b[i])
    {
      cout << a[i] << " != " << b[i] << endl;
      return false;
    }
  }
  return true;
}

int genotypeToGenocode(string genotype)
{
  int genocode = 0;

  for(int i = 0; i < genotype.length(); i++)
  {
    genocode += isUpper(genotype.at(i));
    genocode <<= 1;
  }

  return (genocode >> 1);
}

string genocodeToGenotype(int genocode, char geneList[], int geneCount)
{
  string genotype;
  genotype.resize(geneCount*2);
  bool appendUpper;
  
  for(int i = 0; i < geneCount*2; i+=2)
  {
    appendUpper = (genocode >> (geneCount*2 - i - 1))%2;
    genotype[i] = appendUpper ? geneList[i/2] : toLower(geneList[i/2]);

    appendUpper = (genocode >> (geneCount*2 - i - 2))%2;
    genotype[i+1] = appendUpper ? geneList[i/2] : toLower(geneList[i/2]);
  }

  return genotype;
}

int main()
{
  string genotype1;
  string genotype2;

  grabGenotype(genotype1);

  char geneList[genotype1.length()/2];
  for(int i = 0; i < genotype1.length(); i+=2)
  {
    geneList[i/2] = toUpper(genotype1.at(i));
  }

  grabGenotype(genotype2);
  char geneList2[genotype2.length()/2];
  for(int i = 0; i < genotype2.length(); i+=2)
  {
    geneList2[i/2] = toUpper(genotype2.at(i));
  }

  while(!areListsEqual(geneList2,geneList,genotype2.length()/2,genotype1.length()/2))
  {
    grabGenotype(genotype2);
    for(int i = 0; i < genotype2.length(); i++)
    {
      geneList2[i] = toUpper(genotype2.at(i));
    }
  }

  cout << "genotypes are: " << genotype1 << " " << genotype2 << endl;

  int genocode1 = genotypeToGenocode(genotype1);
  int genocode2 = genotypeToGenocode(genotype2);

  int geneCount = genotype1.length()/2;

  int totalGenes = (1<<(geneCount*2)); // this is (2^geneCount)^2

  int genoList[totalGenes];

  logGenotypes(genocode1, genocode2, geneCount, genoList);

  for(int i = 0; i < totalGenes; i++)
  {
    cout << genocodeToGenotype(genoList[i],geneList,geneCount) << endl;
    //cout << genoList[i] << endl;
  }

  return 0;
}