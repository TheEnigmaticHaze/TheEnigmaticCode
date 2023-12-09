int indexOfElement(char *elementName, unsigned int symbolLength)
{
  int index = 0;
  index += 27 * (elementName[0] - 'A');
  if(symbolLength == 2)
  {
    index += elementName[1] - 'a' + 1;
  }
  return index;
}

void setMass(char *symbol, int mass, int *masses)
{
  if(symbol[1] == '\0')
  {
    masses[indexOfElement(symbol, 1)] = mass;
  }
  else
  {
    masses[indexOfElement(symbol, 2)] = mass;
  }
}

void initializeMasses(int *masses)
{
  setMass("H", 1, masses);
  setMass("He", 4, masses);
  setMass("Li", 7, masses);
  setMass("Be", 9, masses);
  setMass("B", 11, masses);
  setMass("C", 12, masses);
  setMass("N", 14, masses);
  setMass("O", 16, masses);
  setMass("F", 19, masses);
  setMass("Ne", 20, masses);
  setMass("Na", 23, masses);
  setMass("Mg", 24, masses);
  setMass("Al", 27, masses);
  setMass("Si", 28, masses);
  setMass("P", 31, masses);
  setMass("S", 32, masses);
  setMass("Cl", 35, masses);
  setMass("Ar", 40, masses);
  setMass("K", 39, masses);
  setMass("Ca", 40, masses);
  setMass("Sc", 45, masses);
  setMass("Ti", 48, masses);
  setMass("V", 51, masses);
  setMass("Cr", 52, masses);
  setMass("Mn", 55, masses);
  setMass("Fe", 56, masses);
  setMass("Co", 59, masses);
  setMass("Ni", 59, masses);
  setMass("Cu", 64, masses);
  setMass("Zn", 65, masses);
  setMass("Ga", 70, masses);
  setMass("Ge", 73, masses);
  setMass("As", 75, masses);
  setMass("Se", 79, masses);
  setMass("Br", 80, masses);
  setMass("Kr", 84, masses);
  setMass("Rb", 85, masses);
  setMass("Sr", 88, masses);
  setMass("Y", 89, masses);
  setMass("Zr", 91, masses);
  setMass("Nb", 93, masses);
  setMass("Mo", 96, masses);
  setMass("Tc", 98, masses);
  setMass("Ru", 101, masses);
  setMass("Rh", 103, masses);
  setMass("Pd", 106, masses);
  setMass("Ag", 108, masses);
  setMass("Cd", 112, masses);
  setMass("In", 115, masses);
  setMass("Sn", 119, masses);
  setMass("Sb", 122, masses);
  setMass("Te", 128, masses);
  setMass("I", 127, masses);
  setMass("Xe", 131, masses);
  setMass("Cs", 133, masses);
  setMass("Ba", 137, masses);
  setMass("La", 139, masses);
  setMass("Ce", 140, masses);
  setMass("Pr", 141, masses);
  setMass("Nd", 144, masses);
  setMass("Pm", 145, masses);
  setMass("Sm", 150, masses);
  setMass("Eu", 152, masses);
  setMass("Gd", 157, masses);
  setMass("Tb", 159, masses);
  setMass("Dy", 163, masses);
  setMass("Ho", 165, masses);
  setMass("Er", 167, masses);
  setMass("Tm", 169, masses);
  setMass("Yb", 173, masses);
  setMass("Lu", 175, masses);
  setMass("Hf", 178, masses);
  setMass("Ta", 181, masses);
  setMass("W", 184, masses);
  setMass("Re", 186, masses);
  setMass("Os", 190, masses);
  setMass("Ir", 192, masses);
  setMass("Pt", 195, masses);
  setMass("Au", 197, masses);
  setMass("Hg", 201, masses);
  setMass("Tl", 204, masses);
  setMass("Pb", 207, masses);
  setMass("Bi", 209, masses);
  setMass("Po", 209, masses);
  setMass("At", 210, masses);
  setMass("Rn", 222, masses);
  setMass("Fr", 223, masses);
  setMass("Ra", 226, masses);
  setMass("Ac", 227, masses);
  setMass("Th", 232, masses);
  setMass("Pa", 231, masses);
  setMass("U", 238, masses);
  setMass("Np", 237, masses);
  setMass("Pu", 244, masses);
  setMass("Am", 243, masses);
  setMass("Cm", 247, masses);
  setMass("Bk", 247, masses);
  setMass("Cf", 251, masses);
  setMass("Es", 252, masses);
  setMass("Fm", 257, masses);
  setMass("Md", 258, masses);
  setMass("No", 259, masses);
  setMass("Lr", 262, masses);
  setMass("Rf", 267, masses);
  setMass("Db", 262, masses);
  setMass("Sg", 269, masses);
  setMass("Bh", 264, masses);
  setMass("Hs", 269, masses);
  setMass("Mt", 278, masses);
  setMass("Ds", 281, masses);
  setMass("Rg", 282, masses);
  setMass("Cn", 285, masses);
  setMass("Nh", 286, masses);
  setMass("Fl", 289, masses);
  setMass("Mc", 289, masses);
  setMass("Lv", 293, masses);
  setMass("Ts", 294, masses);
  setMass("Og", 294, masses);
}