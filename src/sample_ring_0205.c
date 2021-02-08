 /*
 ============================================================================
 DATE    : 2021,8th Feburary.
 ============================================================================
 */

/*

+++++++++++++++++  Command Prompt +++++++++++++++++

When you install OpenCV3.1.0, then suddenly check dirs bellow:
 1). ~/opencv-3.1.0/build/install/include/
 2). ~/opencv-3.1.0/build/install/x64/mingw/bin
 3). ~/opencv-3.1.0/build/install/x64/mingw/lib

 Firstly, we have to copy the contents of their directories, to each corresponding mingw-dirs as bellows:
 1). Move the folder \opencv2 itself --> C:\TDM-GCC-64\x86_64-w64-mingw32\include
 2). Copy the contaents of \bin to -->   C:\TDM-GCC-64\x86_64-w64-mingw32\bin
 3). Copy the contaents of \lib to -->   C:\TDM-GCC-64\x86_64-w64-mingw32\lib

 After this procedure, you can run the commands, for example:
 > gcc src/sample_ring.c -o sample_ring -I./include -L. -lRING1.2_x64 -lopencv_world310 -O2

 Needed:
  ./hasp_windows_x64_98933.dll

*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <opencv2/highgui/highgui_c.h>
#include "ring.h"

//値域の制限
double Clip(
double clipping_point,
double value){
  double tmp;
  
  tmp=0.5*(clipping_point+fabs(value)-fabs(value-clipping_point));
  return tmp;
}
/* https://www.it-swarm-ja.tech/ja/c%2B%2B/rgb%E3%81%AE%E5%A4%89%E6%8F%9B%E6%96%B9%E6%B3%95gt-yuvgt-rgb%EF%BC%88%E4%B8%A1%E6%96%B9%E3%81%AE%E6%96%B9%E6%B3%95%EF%BC%89/1041024754
*/
// Trans
void RGB2YUV(double* YUV, double R, double G, double B)
{
  YUV[0] =  0.257 * R + 0.504 * G + 0.098 * B +  16;
  YUV[1] = -0.148 * R - 0.291 * G + 0.439 * B + 128;
  YUV[2] =  0.439 * R - 0.368 * G - 0.071 * B + 128;
}
void YUV2RGB(double* RGB, double Y, double U, double V)
{
  Y -= 16;
  U -= 128;
  V -= 128;
  RGB[0] = 1.164 * Y             + 1.596 * V;
  RGB[1] = 1.164 * Y - 0.392 * U - 0.813 * V;
  RGB[2] = 1.164 * Y + 2.017 * U;
}
// Trans
void _RGB2YUV(double* YUV, double R, double G, double B)
{
  YUV[0] =  0.299 * R + 0.587 * G + 0.114 * B;
  YUV[1] = -0.169 * R - 0.331 * G + 0.500 * B;
  YUV[2] =  0.500 * R - 0.419 * G - 0.081 * B;
}
void _YUV2RGB(double* RGB, double Y, double U, double V)
{
  RGB[0] = 1.000 * Y             + 1.402 * V;
  RGB[1] = 1.000 * Y - 0.344 * U - 0.714 * V;
  RGB[2] = 1.000 * Y + 1.772 * U;
}
// Trans
void __RGB2YUV(double* YUV, double R, double G, double B)
{
  YUV[0] =  0.299 * R + 0.587 * G + 0.114 * B;
}
void __YUV2RGB(double* RGB, double Y)
{
  RGB[0] = 1.000 * Y ;
}
////main 関数
int main(void){
  
//変数定義
  IplImage *img;                                                      //opencv の構造体
  //double *g_in,*g_out;                                                //作業用色配列
  //int i,j;                                                            //カウンタ
  double *g_in=NULL;
  double *R_in=NULL;
  double *G_in=NULL;
  double *B_in=NULL;
  double *Y_in=NULL;
  double *U_in=NULL;
  double *V_in=NULL;
  double *g_out=NULL;
  double *R_out=NULL;
  double *G_out=NULL;
  double *B_out=NULL;
  double *Y_out=NULL;
  double *U_out=NULL;
  double *V_out=NULL;
  double *YUV=NULL;
  double *RGB=NULL;
  int radius;
  int width,height,area,bit,widthstep;
  int i,j;
  double value;
  double a,sum=0.0;
  
//先鋭化強度                                                  //中間変数
  int radius_int=74;
  
//初期化
  gnInit(R_MODE);
  
//画像ファイルの読込み
  img=cvLoadImage("sample.bmp",CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  width=img->width;
  height=img->height;
  area=width*height;
  bit=8*img->nChannels;
 
//領域確保
    if(bit == 8){
        printf("CHECKER: GRAY: 1: malloc\n");                       /* CHECKER */
        g_in=(double*)malloc(sizeof(double)*area);
        g_out=(double*)malloc(sizeof(double)*area);
        }
    else{
        printf("CHECKER: RGB: 1: malloc\n");                       /* CHECKER */
        R_in=(double*)malloc(sizeof(double)*area);
        R_out=(double*)malloc(sizeof(double)*area);
        G_in=(double*)malloc(sizeof(double)*area);
        G_out=(double*)malloc(sizeof(double)*area);
        B_in=(double*)malloc(sizeof(double)*area);
        B_out=(double*)malloc(sizeof(double)*area);
        // YUV, Y-ch is only used for Blind decnv.
        Y_in=(double*)malloc(sizeof(double)*area);
        Y_out=(double*)malloc(sizeof(double)*area);
        U_in=(double*)malloc(sizeof(double)*area);
        V_in=(double*)malloc(sizeof(double)*area);;
        } 
//作業用配列へのコピー
  if(bit == 8){
    widthstep=width+(4-width%4)%4;
    for(j=0;j<height;j++){
      for(i=0;i<width;i++){
        value=img->imageData[i+j*widthstep];
        if(value < 0){
          g_in[i+j*width]=value+256.0;
        }
        else{
          g_in[i+j*width]=value;
        }
      }
    }
    printf("CHECKER: GRAY: 2: INPUT-DONE\n");                       /* CHECKER */
    cvReleaseImage(&img);
  }
  else{
    widthstep=3*width+width-4*(int)(width/4);
    for(j=0;j<height;j++){
      for(i=0;i<width;i++){
        value=img->imageData[3*i+j*widthstep];
        if(value < 0){
          B_in[i+j*width]=value+256.0;
        }
        else{
          B_in[i+j*width]=value;
        }
        value=img->imageData[1+3*i+j*widthstep];
        if(value < 0){
          G_in[i+j*width]=value+256.0;
        }
        else{
          G_in[i+j*width]=value;
        }
        value=img->imageData[2+3*i+j*widthstep];
        if(value < 0){
          R_in[i+j*width]=value+256.0;
        }
        else{
          R_in[i+j*width]=value;
        }
      }
    }
    printf("CHECKER: RGB: 2: INPUT-DONE\n");                       /* CHECKER */
    cvReleaseImage(&img);
  }


/*  No. 1 
    STRAT 
*/
long start_1 = clock();

  //鮮鋭化処理
  if(bit == 8){
    printf("CHECKER: GRAY: 3: Begin Deconvolution.\n");                       /* CHECKER */
    gnRing1(radius_int,img->width,img->height,g_in,g_out);
    printf("CHECKER: GRAY: 3: FINISH Deconvolution.\n");                       /* CHECKER */
    }
    else{  
    printf("CHECKER: RGB: 3: Begin Deconvolution.\n");                       /* CHECKER */
    YUV=(double*)malloc(sizeof(double)*3);
    RGB=(double*)malloc(sizeof(double)*3);
    for(j=0;j<height;j++){
      for(i=0;i<width;i++){
        _RGB2YUV(YUV,R_in[i+j*width],G_in[i+j*width],B_in[i+j*width]);
        Y_in[i+j*width] = YUV[0];
        U_in[i+j*width] = YUV[1];
        V_in[i+j*width] = YUV[2];
      }
    }
    gnRing1(radius_int,width,height,Y_in,Y_out);
    printf("CHECKER: RGB: 3: FINISH Deconvolution.\n");                       /* CHECKER */
    for(j=0;j<height;j++){
      for(i=0;i<width;i++){
        _YUV2RGB(RGB,Y_out[i+j*width],U_in[i+j*width],V_in[i+j*width]);
        R_out[i+j*width] = RGB[0];
        G_out[i+j*width] = RGB[1];
        B_out[i+j*width] = RGB[2];
      }
    }
  }

//作業用配列からコピー
if(bit == 8){
    img=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
    for(j=0;j<height;j++){
      for(i=0;i<width;i++){
        value=g_out[i+j*width];
        img->imageData[i+j*widthstep]=(unsigned char)Clip(255.0,value);
      }
    }
    //鮮鋭化画像の保存
    cvSaveImage("out_g.bmp",img,0);
    //領域開放
    cvReleaseImage(&img);
}else{
    img=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,3);
    for(j=0;j<height;j++){
      for(i=0;i<width;i++){
        value=B_out[i+j*width];
        img->imageData[3*i+j*widthstep]=(unsigned char)Clip(255.0,value);
        value=G_out[i+j*width];
        img->imageData[1+3*i+j*widthstep]=(unsigned char)Clip(255.0,value);
        value=R_out[i+j*width];
        img->imageData[2+3*i+j*widthstep]=(unsigned char)Clip(255.0,value);
      }
    }
    cvSaveImage("out_RGB_1.bmp",img,0);
    cvReleaseImage(&img);
}

long end_1 = clock();
/*  No. 1 
    END 
*/

/*  No. 2 
    STRAT 
*/
long start_2 = clock();

  //鮮鋭化処理
  if(bit == 8){
    printf("CHECKER: GRAY: 3: Begin Deconvolution.\n");                       /* CHECKER */
    gnRing1(radius_int,img->width,img->height,g_in,g_out);
    printf("CHECKER: GRAY: 3: FINISH Deconvolution.\n");                       /* CHECKER */
    }
    else{  
    printf("CHECKER: RGB: 3: Begin Deconvolution.\n");                       /* CHECKER */
    YUV=(double*)malloc(sizeof(double)*3);
    RGB=(double*)malloc(sizeof(double)*3);
    for(j=0;j<height;j++){
      for(i=0;i<width;i++){
        __RGB2YUV(YUV,R_in[i+j*width],G_in[i+j*width],B_in[i+j*width]);
        Y_in[i+j*width] = YUV[0];
      }
    }
    gnRing1(radius_int,width,height,Y_in,Y_out);
    printf("CHECKER: RGB: 3: FINISH Deconvolution.\n");                       /* CHECKER */
    for(j=0;j<height;j++){
      for(i=0;i<width;i++){
        __YUV2RGB(RGB,Y_out[i+j*width]-Y_in[i+j*width]);
        R_out[i+j*width] = R_in[i+j*width]+RGB[0];
        G_out[i+j*width] = G_in[i+j*width]+RGB[0];
        B_out[i+j*width] = B_in[i+j*width]+RGB[0];
      }
    }
  }

//作業用配列からコピー
if(bit == 8){
    img=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
    for(j=0;j<height;j++){
      for(i=0;i<width;i++){
        value=g_out[i+j*width];
        img->imageData[i+j*widthstep]=(unsigned char)Clip(255.0,value);
      }
    }
    //鮮鋭化画像の保存
    cvSaveImage("out_g.bmp",img,0);
    //領域開放
    cvReleaseImage(&img);
}else{
    img=cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,3);
    for(j=0;j<height;j++){
      for(i=0;i<width;i++){
        value=B_out[i+j*width];
        img->imageData[3*i+j*widthstep]=(unsigned char)Clip(255.0,value);
        value=G_out[i+j*width];
        img->imageData[1+3*i+j*widthstep]=(unsigned char)Clip(255.0,value);
        value=R_out[i+j*width];
        img->imageData[2+3*i+j*widthstep]=(unsigned char)Clip(255.0,value);
      }
    }
    cvSaveImage("out_RGB_2.bmp",img,0);
    cvReleaseImage(&img);
}
long end_2 = clock();
/*  No. 2 
    END 
*/



const double time_1 = (double)(end_1-start_1)/(CLOCKS_PER_SEC);
printf("CPU TIME :  %f\n",time_1);

const double time_2 = (double)(end_2-start_2)/(CLOCKS_PER_SEC);
printf("CPU TIME :  %f\n",time_2);

//領域開放
  if(bit == 8){
    free(g_in);
    free(g_out);
    }
  else{
    free(R_in);
    free(R_out);
    free(G_in);
    free(G_out);
    free(B_in);
    free(B_out);
    free(Y_in);
    free(Y_out);
    free(U_in);
    free(V_in);
    //
    free(YUV);
    free(RGB);

  } 
  

  gnFree();
  printf("completed\n");
  getch();
  return EXIT_SUCCESS;
}