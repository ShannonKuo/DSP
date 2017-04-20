#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main() {
  int cnt = 0;
  int total = 0;
  string line1, line2;
  fstream myAns;
  fstream answer;
  myAns.open( "../result.txt" );
  answer.open( "../testing_answer.txt" );
  while ( getline( myAns, line1 ) ){
    getline( answer, line2 );
    if ( line1 == line2 ){
      cnt ++;
    }
    total++;
  }
  cout << cnt  <<endl;
  cout << "testing score: " << double(cnt) / total << endl;
}
