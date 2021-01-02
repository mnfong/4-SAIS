#include<iostream>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <iomanip>
#include <cfloat>

using namespace std;
using std::cin;
using std::cout;
using std::endl;


void SAIS(vector<int>&SA, const vector<int>&T, int alphabet_size);
vector<int> fillNames(const vector<int> &T, const vector<int> &t, const vector<int> &SA, int &alphabet_size, int sizeT);
bool isEqualLMS(const vector<int> &T, const vector<int> &t, int previous, int p);

int main(){
  string S("");
  string aline;

  while (getline(cin, aline))
  {
    S = S + aline;
  }

  vector<int> count(257, 0);

  int size = (int)S.size();

  for (int i = 0; i < size; i++)
  {
    int index = (int)S[i];
    count[index] = 1;
  }

  int name = 1;

  for (int j = 0; j < 257; j++)
  {
    if (count[j] > 0)
    {
      count[j] = name;
      name++;
    }
  }

  vector<int> T;

  for (int i = 0; i < size; i++)
  {
    int index = (int)S[i];
    int aname = count[index];
    T.push_back(aname);
  }
  T.push_back(0);

  vector<int> SA(T.size(), -1);
  int alphabet_size = name;

  SAIS(SA, T, alphabet_size);

  vector<char>BWT;

  for(int i = 0; i < (int) SA.size(); i++)
  {
    int p = SA[i];

    if(p != 0)
      BWT.push_back(S[p - 1]);
  }

  for(int i = 0; i < (int) BWT.size(); i++)
    cout << BWT[i];
  cout << endl;

  return 0;
}

void SAIS(vector<int> &SA, const vector<int> &T, int alphabet_size)
{
  if(alphabet_size >= (int) T.size())
  {
    for(int i = 0; i < (int) T.size(); i++)
      SA[T[i]] = i;

    return;
  }


  vector<int> A(alphabet_size, 0);
  vector<int> C(alphabet_size, 0);

  vector<int> t((int)T.size());

  for(int i = 0; i < (int) T.size(); i++)
    A[T[i]]++;

  for(int i = 1; i < (int) A.size(); i++)
    C[i] = C[i-1] + A[i-1];

  vector<int> B = C;
  for(int c = 1; c < (int) B.size(); c++)
    B[c] += A[c] - 1;

  vector<int> ends = B;

  t[T.size() - 1] = 1;

  for(int i = (int) T.size() - 2; i >= 0; i--)
  {
    if (T[i] < T[i + 1])
      t[i] = 1;

    else if (T[i] > T[i + 1])
    {
      t[i] = 0;

      if (t[i + 1] == 1)
      {
        SA[B[T[i + 1]]] = i + 1;
        B[T[i + 1]]--;
      }
    }

    else
      t[i] = t[i + 1];
  }

  B = C;

  for(int i = 0; i < (int) SA.size(); i++)
  {
    int p = SA[i]; 

    if(p <= 0)
      p = (int) T.size();

    if(p != -1)
    {
      if(t[p - 1] == 0)
      {
        SA[B[T[p - 1]]] = p - 1;
        B[T[p - 1]]++;
      }
    }
  }

  B = ends;

  for(int i = (int) SA.size() - 1; i >= 0; i--)
  {
    int p = SA[i];

    if(p <= 0)
      p = (int) T.size();

    if(p != -1)
    {
      if(t[p - 1] == 1)
      {
        SA[B[T[p - 1]]] = p - 1;
        B[T[p - 1]]--;
      }
    } 
  }


  vector<int> T1 = fillNames(T, t, SA, alphabet_size, (int) T.size());
  vector<int>SA1(T1.size(), -1);

  SAIS(SA1, T1, alphabet_size + 1);

  int j = 0;
  for(int i = 1; i < (int) t.size(); i++)
  {
    if(t[i] == 1 && t[i - 1] == 0)
    {
      T1[j] = i;
      j++;
    }
  }

  for(int i = 0; i < (int) SA.size(); i++)
    SA[i] = -1;

  B = C;
  for(int c = 0; c < (int) B.size(); c++)
    B[c] += A[c] - 1;

  ends = B;

  for(int j = (int) SA1.size() - 1; j >= 0; j--)
  {
    int p = T1[SA1[j]];
    SA[B[T[p]]] = p;
    B[T[p]]--;
  }

  B = C;

  for(int j = 0; j < (int) SA.size(); j++)
  {
    int p = SA[j]; 

    if(p <= 0)
      p = (int) T.size();

    if(p != -1)
    {
      if(t[p - 1] == 0)
      {
        SA[B[T[p - 1]]] = p - 1;
        B[T[p - 1]]++;
      }
    }
  }

  B = ends;

  for(int j = (int) SA.size() - 1; j >= 0; j--)
  {
    int p = SA[j]; 

    if(p <= 0)
      p = (int) T.size();

    if(p != -1)
    {
      if(t[p - 1] == 1)
      {
        SA[B[T[p - 1]]] = p - 1;
        B[T[p - 1]]--;
      }
    }
  }

  return;
}// SAIS

vector<int> fillNames(const vector<int> &T, const vector<int> &t, const vector<int> &SA, int &alphabet_size, int sizeT)
{
  vector<int>T1;

  vector<int> N(sizeT, -1);
  N[N.size()-1] = 0;

  int prev = SA[0];
  int name = 0;

  for(int i = 1; i < (int) N.size(); i++)
  {
    int p = SA[i];

    if(p > 0)
    {
      if(t[p] == 1 )
      {
        if(t[p - 1] == 0)
        {
          if (!isEqualLMS(T, t, prev, p))
            name = N[prev] + 1;

          N[p] = name;
          prev = p;
        }
      }
    }
  }


  alphabet_size = N[0];
  N[N.size()-1] = 0;

  for(int i = 0; i<(int)N.size(); i++)
  {
    if(N[i] > alphabet_size)
      alphabet_size = N[i];

    if(N[i] != -1){
      T1.push_back(N[i]);
    }
  }

  return T1;
}

bool isEqualLMS(const vector<int> &T, const vector<int> &t, int previous, int p)
{
  if (T[previous] != T[p])
    return false;

  previous++;
  p++;
  int sizeT = (int)T.size();

  while (1)
  {
    if (previous >= sizeT || p >= sizeT)
      break;

    if ((t[previous] == 1 && t[previous - 1] == 0) && (t    [p] == 1 && t[p - 1] == 0))
    {
      if (T[previous] != T[p])
        return false;

      break;
    }

    if (T[previous] != T[p])
      return false;

    previous++;
    p++;
  }

  return true;
}//isEqualLMS
