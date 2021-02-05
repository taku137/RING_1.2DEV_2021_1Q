 /*
 ============================================================================
 Name        : sample_ring.c
 Author      : Jun Nishigata
 Version     : 1.2
 Copyright   : Casley Consulting, Inc.
 Description : サンプルプログラム
 ============================================================================
 */
#include <cstdio>
#include <cstdlib> 
#include <cmath>
#include <opencv2/highgui/highgui.hpp>

#include "ring.h"

using namespace std;
using namespace cv;

//値域の制限
double Clip(
double clipping_point,
double value){
  double tmp;
  
  tmp=0.5*(clipping_point+fabs(value)-fabs(value-clipping_point));
  return tmp;
}

////main 関数
int main(){
    gnInit(R_MODE);

  return 0;
}