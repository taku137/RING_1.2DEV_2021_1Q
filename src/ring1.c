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

//値域の制限
double Clip(
double clipping_point,
double value){
  double tmp;
  
  tmp=0.5*(clipping_point+fabs(value)-fabs(value-clipping_point));
  return tmp;
}

////main 関数
int main(void){
  
//変数定義
  IplImage *img;                                                      //opencv の構造体
  double *g_in,*g_out;                                                //作業用色配列
  int i,j;                                                            //カウンタ
  double value;                                                       //中間変数
  int radius_int=15;
  
//初期化
  gnInit(R_MODE);
  
//画像ファイルの読込み
  img=cvLoadImage("input.bmp",CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  
//領域確保
  g_in=(double*)malloc(sizeof(double)*img->width*img->height);
  g_out=(double*)malloc(sizeof(double)*img->width*img->height);
  
//作業用配列へのコピー
  for(j=0;j<img->height;j++){
    for(i=0;i<img->width;i++){
      value=img->imageData[i+j*img->widthStep];
      if(value < 0){
        g_in[i+j*img->width]=value+256.0;
      }
      else{
        g_in[i+j*img->width]=value;
      }
    }
  }
  
//鮮鋭化処理
  gnRing1(radius_int,img->width,img->height,g_in,g_out);
  
//作業用配列からコピー
  for(j=0;j<img->height;j++){
    for(i=0;i<img->width;i++){
      img->imageData[i+j*img->widthStep]=(unsigned char)Clip(255.0,g_out[i+j*img->width]);
    }
  }
  
//領域開放
  free(g_in);
  free(g_out);
  
//鮮鋭化画像の保存
  cvSaveImage("output.bmp",img,0);
  
//領域開放
  cvReleaseImage(&img);
  gnFree();
  printf("completed\n");
  getch();
  return EXIT_SUCCESS;
}