#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <utility>
#include "Ngram.h"
using namespace std;

map <string, vector<string> > mymap;
int ngram_order = 2; Vocab voc; Ngram lm( voc, ngram_order );

void readLM( Ngram& );
void readMP( ifstream& );
void viterbi( ofstream&, string& );
double getProb( string&, string = " ", int = 2 );


void readLM( Ngram& lm, string bigram ) {
  File lmFile( bigram.c_str(), "r" );
  lm.read( lmFile );
  lmFile.close();
}

double getProb( string& s1, string s2, int n ) {
  VocabIndex wid = voc.getIndex( s1.c_str() );
  if ( wid == Vocab_None )
    return -1000000;
  if ( n == 1 ) {
    VocabIndex context[] = { Vocab_None };
    return lm.wordProb( wid, context );
  }
  VocabIndex context[] = { voc.getIndex( s2.c_str() ), Vocab_None };
  return lm.wordProb( wid, context );
}

void readMap( ifstream& mymapFile ) {
  string line;
  while ( getline(mymapFile, line) ) {
    vector <string> temp;
     string s;
     for ( int i = 0; i < line.length(); i++ ) {
       if ( line[i] == ' ' ) {
         if ( s != "" && s != " ") {
           temp.push_back(s);
           s = ""; 
         }
       }
       else if ( i == line.length() - 1 ) {
         s += line[i];
         temp.push_back(s);
       } 
       else 
         s += line[i];
     }
     
     for ( int i = 1; i < temp.size(); i++ ) {
       if ( mymap.count( temp[0] ) < 0 ) {
         vector <string> v;
         v.push_back( temp[i] );
         mymap.insert( make_pair( temp[0], v ) );
       }
       else{
         vector <string> v = mymap[ temp[0] ];
         v.push_back(temp[i]);
         mymap[ temp[0] ] = v;
       }
     }
  }
}


vector<string> viterbi( vector <string> data ) {
  vector <pair<string, double> > prob;
  vector <pair<string, double> > prob_prev;
  vector <map<string, string> >prev;
  pair <string, double> p;
  prev.resize(data.size() + 1);

  for ( int i = 0;  i < data.size() - 1; i++ ) {
    prob.clear();
    if ( i == 0 ) {
      for ( int j = 0; j < mymap[data[i]].size(); j++ ) {
        double probability = 0;
        string key = mymap[data[i]][j];
        string dummy = "<s>";
        probability = getProb( key, dummy );
        p = make_pair( key, probability);
        prob_prev.push_back( p );
      }
    }
    
    for ( int j = 0; j < mymap[data[i+1]].size(); j++ ) {
      double probability = -10000000000;
      string key;
      pair <string, string> newpair;
      for ( int k = 0; k < prob_prev.size(); k++ ) {
        double bigramProb = 0;
        p = prob_prev[k];
        bigramProb = getProb( mymap[data[i+1]][j], p.first);
        double temp = p.second + bigramProb;
        if ( temp  > probability ) {
          newpair = make_pair( mymap[data[i+1]][j], p.first );
          probability = temp;
        }
      }
      prev[i+1].insert(newpair);
      key = mymap[data[i+1]][j];
      p = make_pair( key, probability );
      prob.push_back( p );
    } 
    prob_prev.clear();
    prob_prev = prob;
  }
  double probability = -10000000000;
  string key;
  pair <string, string> newpair;
  prob.clear();
  for ( int k = 0; k < prob_prev.size(); k++ ) {
    double bigramProb = 0;
    p = prob_prev[k];
    string dummy = "</s>";
    bigramProb = getProb( dummy, p.first );
    double temp = p.second + bigramProb;
    if ( temp  > probability ) {
      newpair = make_pair( "</s>", p.first );
      probability = temp;
    }
  }
  prev[data.size()].insert(newpair);
  p = make_pair( "</s>", probability );
  prob.push_back( p );

  double maxProb = -10000000000;
  vector <string> maxKey;
  maxKey.resize(data.size() + 1); 
  for ( int i = 0; i < prob.size(); i++ ) {
    if ( i == 0 ) {
      maxProb = prob[i].second;
      maxKey[maxKey.size()-1] = prob[i].first;
    }
    if ( prob[i].second > maxProb ) {
      maxProb = prob[i].second;
      maxKey[maxKey.size()-1] = prob[i].first;
    }
  }
  for ( int i = data.size(); i > 0; i-- ) {
    maxKey[i-1] = prev[i][maxKey[i]];
  } 
  return maxKey;
}


int main( int argc, char* argv[]) {
  ifstream bigram ( argv[6] );
  readLM(lm, argv[6]);
  ifstream mymapFile ( argv[4] );
  readMap( mymapFile );

  ifstream test;
  test.open( argv[2] );
  ofstream output;
  output.open( argv[10] );
  string line;
  string s;
  vector <string> ans;
  vector <string> ansTemp;
  int cnt = 1;
  while ( getline( test, line ) ) {
    vector <string> testData;
    
    for ( int i = 0; i < line.length(); i++ ) {
      if ( s.length() < 2 ) {
        if ( line[i] != ' ' )
          s += line[i];
      }
      if ( s.length() == 2 ) {
        testData.push_back( s );
        s = "";
      } 
    }
    
    ans = viterbi( testData );
    cout << "<s> ";
    for ( int i = 0; i < ans.size(); i++ )
      if ( i == ans.size() - 1 )
        cout << ans[i];
      else cout << ans[i] << " ";
	cout << endl;
    cnt++;
  }
}
