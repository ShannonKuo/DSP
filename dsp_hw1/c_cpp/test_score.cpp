#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main( int argc, char* argv[]) {
  int cnt = 0;
  int total = 0;
  string line1, line2;
  string result = argv[1];
  string score = argv[2];
  fstream myAns;
  fstream answer; 
  ofstream output ( score.c_str(), ios_base::app );
  myAns.open( result.c_str() );
  answer.open( "../testing_answer.txt" );
  while ( getline( myAns, line1 ) ){
    getline( answer, line2 );
    if ( line1 == line2 ){
      cnt ++;
    }
    total++;
  }
  cout << "testing score: " << double(cnt) / total << endl;
  output << double(cnt) / total;
  output << "\n";
  output.close();
}
