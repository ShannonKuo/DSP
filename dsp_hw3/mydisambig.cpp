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

map <string, map<string, double> > dic;
map <string, vector<string> > mymap;

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
      map <string, double> m;
      m.insert(p);
      if ( dic.count( temp[1] ) < 0 ) {
        dic.insert( make_pair( temp[1], m ) );
      }
      else{
        dic[ temp[1] ].insert( p );
      }
    }
    if ( line == "\\2-grams:") {
      start = 1; 
    }
    cnt++;
  }
}

void readMap() {
  ifstream mymapFile;
  mymapFile.open("./ZhuYin-Big5.map");

  string line;
  while ( getline(mymapFile, line) ) {
    vector <string> temp;
     string s;
     for ( int i = 0; i < line.length(); i++ ) {
       if ( line[i] == ' ' ) {
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
  ofstream output;
  output.open( "./testdata/1_output.txt" );

  prev.resize(data.size());
  for ( int i = 0;  i < data.size() - 1; i++ ) {
    prob.empty();
    prob.resize( mymap[data[i+1]].size() );
    for (  int j = 0; j < mymap[data[i+1]].size(); j++ ) {
      output << mymap[data[i+1]][j] << " ";
      if ( i == 0 ) {
        double probability = 0;
        string key;
        vector <string> v = mymap[data[i+1]];
        for ( int k = 0; k < v.size(); k++ ) {
          for ( map<string, double> :: iterator it = dic[v[j]].begin(); it != dic[v[j]].end(); ++it ){
            probability += it->second;
            key = v[j];
          }
        }
        p = make_pair( key, probability);
        prob_prev.push_back( p );
      }
      double probability = 0;
      string key;
      for ( int k = 0; k < prob_prev.size(); k++ ) {
        p = prob_prev[k];
        double temp = p.second * dic[ p.first ][ mymap[data[i+1]][j] ] ;
        map <string, string> m;
        m.insert( make_pair( mymap[data[i+1]][j], p.first ) );
        if ( k == 0 ) {
          probability = temp;
          prev[i] = m;
        }
        else if ( temp  > probability ) {
          probability = temp;
          prev[i] = m;
        }
        key = mymap[data[i+1]][j];
      }
      p = make_pair( key, probability );
      prob.push_back( p );
    } 
    prob_prev.empty();
    prob_prev = prob;
  }
  double maxProb = 0;
  vector <string> maxKey;
  maxKey.resize(data.size()); 
  for ( int i = 0; i < prob.size(); i++ ) {
    if ( prob[i].second > maxProb ) {
      maxProb = prob[i].second;
      maxKey[maxKey.size()-1] = prob[i].first;
    }
  }
  for ( int i = data.size() - 1; i > 0; i-- ) {
    maxKey[i-1] = prev[i][maxKey[i]];
  } 
  return maxKey;
}


int main() {
  string testFile = "./testdata/1.txt";
  string outputFile = "./testdata/1_output.txt";

  cout << "constructing dictionary..." << endl;
  constructDic();
  cout << "finish constructing dictionary" << endl;

  cout << "readMap" << endl;
  readMap();
  cout << "finish read map" << endl;

  ifstream test;
  test.open( "./testdata/1.txt" );
  //ofstream output;
  //output.open( "./testdata/1_output.txt" );
  string line;
  string s;
  vector <string> ans;
  int cnt = 1;
  while ( getline( test, line ) ) {
    cout << "line:" << cnt << endl;
    cnt++;
    if ( cnt > 1 ) break;
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
    ans = viterbi( temp );
    for ( int i = 0; i < ans.size(); i++ )
      cout << ans[i] << " ";
    cout << endl;
  }
}
