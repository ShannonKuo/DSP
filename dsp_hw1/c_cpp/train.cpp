#include <iostream>
#include <math.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "hmm.h"

using namespace std;

// global 
int T = 50;
int* ob;
double P_O_lambda = 0;
double** alpha;
double** beta;
double** Gamma;
double*** epsilon;
double* sigmaGamma;
double** sigmaGammaNum;
double** sigmaEpsilon;

void initialize( HMM hmm ) {
  //alpha
  alpha = new double* [hmm.state_num];
  for ( int i = 0; i < hmm.state_num; i++ ){
    alpha[i] = new double [T];  
  }
  for (int i = 0; i < hmm.state_num; i++){
    for (int j = 0; j < T; j++){
      alpha[i][j] = 0;
    }
  }
  //beta
  beta = new double* [hmm.state_num];
  for ( int i = 0; i < hmm.state_num; i++ ){
    beta[i] = new double [T];
  }
  for ( int i = 0; i < hmm.state_num; i++ ){
    for ( int j = 0; j < T; j++ ){
      beta[i][j] = 0;
    }
  }
  //gamma
  Gamma = new double* [hmm.state_num];
  for ( int i = 0; i < hmm.state_num; i++ ){
    Gamma[i] = new double [T];
  }
  for ( int i = 0; i < hmm.state_num; i++ ){
    for ( int j = 0; j < T; j++ ){
      Gamma[i][j] = 0;
    }
  }
  //epsilon
  epsilon = new double** [T-1];
  for ( int i = 0; i < T-1; i++ ){
    epsilon[i] = new double* [hmm.state_num];
  }
  for ( int i = 0; i < T-1; i++ ){
    for ( int j = 0; j < hmm.state_num; j++ ){
      epsilon[i][j] = new double [hmm.state_num];
    }
  }
  for ( int t = 0; t < T-1; t++ ){
    for ( int i = 0; i < hmm.state_num; i++ ){
      for ( int j = 0; j < hmm.state_num; j++ ){
        epsilon[t][i][j] = 0;
      }
    }
  }
  //sigma
  sigmaGamma = new double [hmm.state_num];
  sigmaGammaNum = new double* [hmm.state_num];
  for ( int i = 0; i < hmm.state_num; i++ ){
    sigmaGammaNum[i] = new double [hmm.state_num];
  }
  sigmaEpsilon = new double* [hmm.state_num];
  for ( int i = 0; i < hmm.state_num; i++ ){
    sigmaEpsilon[i] = new double [hmm.state_num];
  }
  P_O_lambda = 0;
}

void calOb( HMM hmm, string line ) {
  int T = line.length();
  ob = new int [T];
  for ( int i = 0; i < T; i++ ){
    ob[i] = char(line[i]) - 65;
  } 
}

void calAlpha( HMM hmm, string line ) {
  int T = line.length();
  for ( int i = 0; i < hmm.state_num; i++ ){
    for ( int t = 0; t < T; t++ ){
      alpha[i][t] = 0;
    }
  }
  for ( int i = 0; i < hmm.state_num; i++ ){
    alpha[i][0] = hmm.initial[i] * hmm.observation[ob[0]][i];
  } 
  for (int t = 1; t < T; t++){ 
    for (int j = 0; j < hmm.state_num; j++){
      for (int i = 0; i < hmm.state_num; i++){
        alpha[j][t] += alpha[i][t-1] * hmm.transition[i][j];
      }
      alpha[j][t] *= hmm.observation[ob[t]][j];
      //cout << alpha[j][t] << " ";
    }
    //cout << endl;
  }
  /*for (int i = 0; i < hmm.state_num; i++){
    for (int j = 0; j < T; j++){
      cout << alpha[i][j] << " ";
    }
    cout << endl;
  }
  */
}

void calBeta( HMM hmm, string line ) {
  for ( int i = 0; i < hmm.state_num; i++ ){
    for ( int t = 0; t < T; t++ ){
      beta[i][t] = 0;
    }
  }
  for ( int i = 0; i < hmm.state_num; i++ ){
    beta[i][T-1] = 1;
  }
  for ( int t = T-2; t >= 0; t-- ){ 
    for ( int i = 0; i < hmm.state_num; i++ ){
      for ( int j = 0; j < hmm.state_num; j++ ){
        beta[i][t] += hmm.transition[i][j] * hmm.observation[ob[t+1]][j] * beta[j][t+1];
      }
    }
  }
  /*for ( int i = 0; i < hmm.state_num; i++ ){
    for ( int j = 0; j < T; j++ ){
      cout << beta[i][j] << " ";
    }
    cout << endl;
  }*/
}

void calGamma( HMM hmm, string line) {
  int T = line.length();
  double* sigma = new double [T];
  
  for ( int i = 0; i < T; i++ ){
    sigma[i] = 0;
  }
  for ( int i = 0; i < hmm.state_num; i++ ){
    for ( int j = 0; j < T; j++ ){
      sigma[j] += alpha[i][j] * beta[i][j];
    }
  }
  for ( int i = 0; i < hmm.state_num; i++ ){
    for ( int j = 0; j < T; j++ ){
      Gamma[i][j] = alpha[i][j] * beta[i][j] / sigma[j];  
    }
  }
}

void calEpsilon( HMM hmm, string line ) {
  int T = line.length();
  double* sigma = new double [T];
  for ( int t = 0; t < T-1; t++ ){
    for ( int i = 0; i < hmm.state_num; i++ ){
      for ( int j = 0; j < hmm.state_num; j++ ){
        sigma[t] += alpha[i][t] * hmm.transition[i][j]
                  * hmm.observation[ob[t+1]][j] * beta[j][t+1];
      }
    }
  }
  for ( int t = 0; t < T-1; t++ ){
    for ( int i = 0; i < hmm.state_num; i++ ){
      for ( int j = 0; j < hmm.state_num; j++ ){
        epsilon[t][i][j] = alpha[i][t] * hmm.transition[i][j]
                         * hmm.observation[ob[t+1]][j] * beta[j][t+1] / sigma[t];
      }
    }
  }
  
}

void accuGamma( HMM hmm ) {
  for ( int i = 0; i < hmm.state_num; i++ ){
    for ( int t = 0; t < T-1; t++ ){ 
      sigmaGamma[i] += Gamma[i][t]; 
      sigmaGammaNum[ob[t]][i] += Gamma[i][t];
    }
  }
}
void accuEpsilon( HMM hmm ) {
  for ( int t = 0; t < T-1; t++ ){
    for ( int i = 0; i < hmm.state_num; i++ ){
      for ( int j = 0; j < hmm.state_num; j++ ){
        sigmaEpsilon[i][j] += epsilon[t][i][j];
      }
    }
  } 
}

void calPOlambda( HMM hmm ) {
  for ( int i = 0; i < hmm.state_num; i++ ){
    P_O_lambda += alpha[i][T-1];
  }
}

HMM reEstimate( HMM hmm ) {
   for ( int i = 0; i < hmm.state_num; i++ ){
     hmm.initial[i] = Gamma[i][0];
   }
   for ( int i = 0; i < hmm.state_num; i++ ){
     for ( int j = 0; j < hmm.state_num; j++ ){
       hmm.transition[i][j] = sigmaEpsilon[i][j] / sigmaGamma[i];
     }
   }
   for ( int k = 0; k < hmm.state_num; k++ ){
     for ( int j = 0; j < hmm.state_num; j++ ){
       hmm.observation[k][j] = sigmaGammaNum[k][j] / sigmaGamma[j];
     }
   }
   return hmm;
}


int main( int argc, char* argv[]) {

 /*
	HMM hmms[5];
	load_models( "modellist.txt", hmms, 5);
	dump_models( hmms, 5);
*/
 
	//load initial model
  HMM hmm_initial;
  HMM newModel; 
  
  string iteration = argv[1];
  int iteration_int = atoi(iteration.c_str());
  string model_init = argv[2];
  string seq_model = argv[3];
  string output = argv[4];
  loadHMM( &hmm_initial, model_init.c_str() );
  
  for ( int iter = 0; iter < iteration_int; iter++ ){
    cout << "iter" << iter << " ";
    if ( iter > 1 )
      hmm_initial = newModel;
    fstream seq;
    string line;
    //sprintf(filename, "../seq_model_0%d.txt", i + 1);
    seq.open(seq_model.c_str());
    if (seq.is_open()){
      initialize( hmm_initial);
      while( getline(seq, line) ){
        calOb( hmm_initial, line );
        calAlpha( hmm_initial, line );
        calBeta( hmm_initial, line );
        calGamma( hmm_initial, line );
        calEpsilon( hmm_initial, line );
        accuGamma( hmm_initial );
        accuEpsilon( hmm_initial );
        calPOlambda( hmm_initial );
      }
      seq.close();
    }
    else cout << "Unable to open file";
    newModel = reEstimate( hmm_initial );
    cout << P_O_lambda << endl;
  }
  //sprintf(output, "model_0%d.txt", i + 1);
  FILE *fp = open_or_die( output.c_str(), "w" );
  dumpHMM( fp, &newModel );
	return 0;

}
