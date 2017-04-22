#include <iostream>
#include <math.h>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "hmm.h"

using namespace std;

int T = 0;
int* ob;
double** delta;
//finalDelta
vector<vector <double> > finalDelta;
vector<int> ans;
vector<double> resultP;

void initialize ( HMM hmm, int len ) {
  T = len;
  ob = new int [T];
  for ( int i = 0; i < T; i++ ){
    ob[i] = 0;
  }
  //delta
  delta = new double* [hmm.state_num];
  for ( int i = 0; i < hmm.state_num; i++ ){
    delta[i] = new double [T];
  }
  for ( int i = 0; i < hmm.state_num; i++ ){
    for ( int t = 0; t < T; t++ ){
      delta[i][t] = 0;
    }
  }
}

void calOb( HMM hmm, string line ) {
  for ( int i = 0; i < T; i++ ){
    ob[i] = char(line[i]) - 65;
  } 
}

void calDelta( HMM hmm, int cnt, int model ) {
  for ( int i = 0; i < hmm.state_num; i++ ){
    delta[i][0] = hmm.initial[i] * hmm.observation[ob[0]][i];
  }
  for ( int t = 1; t < T; t++ ){
    for ( int j = 0; j < hmm.state_num; j++ ){
      double maxDelta = 0;
      for ( int i = 0; i < hmm.state_num; i++ ){
        double temp = delta[i][t-1] * hmm.transition[i][j]; 
        if ( temp > maxDelta ){ 
          maxDelta = temp;
          delta[j][t] = temp * hmm.observation[ob[t]][j];
        }
      }
    }
  }
}

void findP( HMM hmm, int model, int id ) {
  if ( model == 0 ){
    vector <double> P(5, 0);
    finalDelta.push_back(P);
  }
  double maxDelta = 0;
  for ( int i = 0 ; i < hmm.state_num; i++ ){
    if ( delta[i][T-1] > maxDelta ){
      finalDelta[id][model] = delta[i][T-1];
      maxDelta = delta[i][T-1];
    }
  }
  /*if ( model == 0 ){
    for ( int i = 0; i < hmm.state_num; i++ ){
      cout << delta[i][T-1]<<" ";
    }
    cout << endl;
  }*/
}

void compare() {
  for ( int i = 0; i < finalDelta.size(); i++ ){
    double maxP = -1;
    double temp = 0;
    for ( int j = 0; j < 5; j++ ){
      if ( finalDelta[i][j] > maxP ){
        maxP = finalDelta[i][j];
        temp = j + 1;
      }
    }
    ans.push_back( temp );
    resultP.push_back( maxP );
  }
}

void outputAns( FILE *fp ) {
  for ( int i = 0; i < finalDelta.size(); i++ ){
    //fprintf( fp, "model_0%d.txt %e\n", ans[i], resultP[i] );
    fprintf( fp, "model_0%d.txt\n", ans[i] );
  }
}

void outputAcc( string result ) {
  int cnt = 0;
  int total = 0;
  char* model;
  double accuracy = 0;
  char line1[20], line2[20];
  string accuracy_old;
  string accuracy_s;
  string file = "acc.txt"; 

  /*fstream myAns; 
  fstream answer; 
  fstream acc;
  ostringstream buff;
  myAns.open( result.c_str() );
  answer.open( "../testing_answer.txt" );
  acc.open( "acc.txt" );*/
  FILE *acc = open_or_die( file.c_str(), "w" );
  FILE *myAns = open_or_die( result.c_str(), "r");

  fstream test;
  test.open( "test.txt");
  while ( fscanf( myAns, "%s", line1 ) > 0 ){
    test << line1 << endl;
    /*getline( answer, line2 );
    int i = 0;
    bool same = 1;
    while ( int(line1[i]) != 32 && line1[i] != NULL && line1[i] != '\n' ){
      if ( line1[i] != line2[i] ){
        same = 0;
        break;
      }
      else i++;
    }
    if ( same ) cnt++;
    */
    total++;
  }
  cout << total<<endl;
  /*accuracy = double(cnt) / total;
  buff << accuracy;
  accuracy_s = buff.str();
  cout << "testing score: " << accuracy << endl;
  if ( getline( acc, accuracy_old ) ) {
    acc.close();
    if ( accuracy > atoi(accuracy_old.c_str()) ){
      FILE *fp = open_or_die( file.c_str(), "w");
      fprintf( fp, "%s", accuracy_s.c_str());
    }
  }
  else{ 
    FILE *fp = open_or_die( file.c_str(), "w");
    fprintf( fp, "%s", accuracy_s.c_str() );
  }*/
}

int main( int argc, char* argv[] ) {
  int cnt = 0;
  string modellist = argv[1];
  string testing_data = argv[2];
  string result = argv[3];
  string line;
  HMM model_train[5];
   
  load_models( modellist.c_str(), model_train, 5 );
  fstream testing;

  for ( int model = 0; model < 5; model++ ){
    cnt = 0;
    testing.open( testing_data.c_str() );
    if (testing.is_open()){
      while( getline(testing, line) ){
        initialize( model_train[model], line.length() );
        calOb( model_train[model], line );
        calDelta( model_train[model], cnt, model );
        findP( model_train[model], model, cnt );
        cnt ++;
      }
    }
    else cout << "Unable to open file";
    testing.close();
  }
  compare();
  FILE *fp = open_or_die( result.c_str(), "w");
  outputAns( fp );
  outputAcc( result );
}
