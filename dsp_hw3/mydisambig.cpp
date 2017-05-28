#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <utility>
using namespace std;

map <string, pair <string, double> > dic;

void constructDic() {
  bool start = 0;
  map <string, double> voc;
  ifstream bigram;
  bigram.open("./bigram.lm");
  int cnt = 0;
  string line;
  pair <string, double> p;
  while( getline( bigram, line )) {
    if ( start && line.length() > 0 && line != "\\end\\") {
      vector <string> temp;
      int begin = 0; 
      string s;
      for ( int i = 0; i < line.length(); i++ ) {
        if ( line[i] == ' ' || line[i] == '	') {
          if ( s != "") {
            temp.push_back(s);
            s = " "; 
          }
        }
        else if ( i == line.length() - 1 ) {
          s += line[i];
          temp.push_back(s);
        } 
        else 
          s += line[i];
      }
      p = make_pair( string(temp[2]), atof(temp[0].c_str()) );
      if ( dic.count( temp[1] ) < 0 ) {
        dic.insert( make_pair( temp[1], p ) );
      }
      else{
        dic[ temp[1] ] = p;
      }
    }
    if ( line == "\\2-grams:") {
      start = 1; 
    }
    cnt++;
  }
}

void viterbi( vector <string> data ) {
  
  vector <double> prob;
  vector <pair> prob_prev;
  for ( int i = 0;  i < data.size() - 1; i++ ) {
    prob.resize( dic[ data[i+1] ].count );
    for ( map<string, double> :: iterator it = dic[data[i+1]].begin(); it != dic[data[i+1]].end(); ++it ) {
      if ( i == 0 ) {
        int j = 0;
        double probability = 0;
        for ( map<string, double> :: iterator it = dic[data[0]].begin(); it != dic[data[0]].end(); ++it ) {
          probability += it -> second;
          j++;
        }
        
      }
      for ( int k = 0; k < prob_prev.size(); k++ ) {
        double temp = prob_prev[j] * dic[data[0]];
        if ( temp  > prob[j] )
          prob[j] = temp;
      }
    } 
    prob_prev = prob;
  }
}


int main() {
  string testFile = "./testdata/1.txt";
  string outputFile = "./testdata/1_output.txt";
  cout << "constructing dictionary..." << endl;
  constructDic();
  cout << "finish constructing dictionary" << endl;
  ifstream test;
  test.open( "./testdata/1.txt" );
  ofstream output;
  output.open( "./testdata/1_output.txt" );
  string line;
  string s;
  while ( getline( test, line ) ) {
    vector <string> temp;
    for ( int i = 0; i < line.length(); i++ ) {
      if ( s.length() < 2 ) {
        if ( line[i] != ' ' )
          s += line[i];
      }
      if ( s.length() == 2 ) {
        temp.push_back( s );
        s = "";
      } 
    }
    for ( int i = 0; i < temp.size(); i++ ) {
      output << temp[i] << " ";
    }
    output << endl;
    viterbi( temp );
  }
I}
