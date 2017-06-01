#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

int main() {
  ifstream input;
  ofstream output;
  input.open("Big5-ZhuYin.map");
  output.open("ZhuYin-Big5.map");
  vector <vector<string> > zhuArr;
  map <string, vector<string> > final;
  vector <vector<string> > finalArr;
  int cnt = 1;
  while ( 1 ) {
    string line;
    getline( input, line );
    if ( line.size() == 0 ) break;
    string temp;
    string chin;
    vector <string> zhu;
    for ( int i = 0 ; i < line.size(); i++ ) {
      if ( (line[i] != ' ') && (temp.length() < 2) ) {
        temp += line[i];
      }
      else if ( line[i] == ' ' ) {
        chin = temp;
        temp = "";
      }
      if ( line[i] == '/' || i == line.size()-1 ) {
        bool same = 0;
        for ( int k = 0; k < zhu.size(); k++ ) {
          if ( zhu[k] == temp )
            same = 1;
        }
        if ( !same )
          zhu.push_back(temp);
        temp = "";
      }
    }
    for ( int j = 0; j < zhu.size(); j++ ) {
      bool exist = 0;
      for ( int i = 0; i < zhuArr.size(); i++ ) {
        if (zhu[j] == zhuArr[i][0]) {
          zhuArr[i].push_back(chin);
          exist = 1;
          break;  
        }               
      }                 
      if ( !exist ) {     
        vector <string> s;
        s.push_back(zhu[j]);
        s.push_back(chin);
        zhuArr.push_back(s);
      }
    } 
    cnt += 1;
  }
  sort( zhuArr.begin(), zhuArr.end() );
  for ( int i = 0; i < zhuArr.size(); i++ ) {
    for ( int j = 0; j < zhuArr[i].size(); j++ ) { 
      if ( j == 0 ) {
        if ( final.count(zhuArr[i][0]) <= 0 ){
          vector <string> temp;
          temp = zhuArr[i];
          temp.erase(temp.begin());
          final.insert(make_pair(zhuArr[i][0], temp ));
          finalArr.push_back(zhuArr[i]);
        }
      }
      else {
        if ( final.count(zhuArr[i][j]) <= 0 ){
          vector <string> temp;
          temp.push_back(zhuArr[i][j]);
          final.insert(make_pair(zhuArr[i][j], temp ));
          temp.push_back(zhuArr[i][j]);
          finalArr.push_back(temp);
        }
      }
    }                                           
  }
  for ( int i = 0; i < finalArr.size(); i++ ) {
    for ( int j = 0; j < finalArr[i].size(); j++ ) {
      if ( j  == 0 )
        output << finalArr[i][j] << "  ";
      else {
        if ( j == finalArr[i].size() - 1 ) output << finalArr[i][j];
        else output << finalArr[i][j] << " "; 
     }
    }
    output << endl;
  }
}
