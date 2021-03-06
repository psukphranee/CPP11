//#include "RangeFilter.h"
//#include "TemporalFilter.h"
#include <iostream>
#include <iterator>
#include <cstdlib>
#include <cstring>
using namespace std;

int determineNflag(int argc, char *argv[]);
int determineDflag(int argc, char *argv[]);
int determineFilterFlag(int argc, char *argv[]);

void applyRange(istream_iterator<float> &);
void applyTemporal(istream_iterator<float> &);

int main(int argc, char *argv[]){

    int N = determineNflag(argc, argv); //length of reading vector
    int D = determineDflag(argc, argv); //number of previous readings to consider for temporal filter

    istream_iterator<float> inputFloat(cin); //create input stream interator for cin

    if(determineFilterFlag(argc, argv)){
      applyTemporal(inputFloat);
    }
}

int determineNflag(int argc, char *argv[]){
  //loop through argument list
  for(int i = 1; i < argc; ++i){
    if(strncmp(argv[i], "-N=", 3) == 0){
      return atoi(&argv[i][3]);
    }
  }
  return 1;
}

int determineDflag(int argc, char *argv[]){
  //loop through argument list
  for(int i = 1; i < argc; ++i){
    if(strncmp(argv[i], "-D=", 3) == 0){
      return atoi(&argv[i][3]);
    }
  }

  return 1;
}

int determineFilterFlag(int argc, char *argv[]){
  //loop through argument list
  for(int i = 1; i < argc; ++i){
    if(strcmp(argv[i], "-temporal") == 0){
      return 1;
    }
  }

  return 0;
}

void applyRange(istream_iterator<float> &inputStream){
  
}
