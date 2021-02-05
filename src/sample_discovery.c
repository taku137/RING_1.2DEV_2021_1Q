 /*
 ============================================================================
 Name        : sample_discovery.c
 Author      : Jun Nishigata
 Version     : 2.3
 Copyright   : Casley Consulting, Inc.
 Description : �T���v���v���O����
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <opencv2/highgui/highgui_c.h>
#include "ring.h"

//�l��̐���
double Clip(
double clipping_point,
double value){
  double tmp;
  
  tmp=0.5*(clipping_point+fabs(value)-fabs(value-clipping_point));
  return tmp;
}

////main �֐�
int main(void){
  
//�ϐ���`
  IplImage *img;                                                      //opencv �̍\����
  double *g_in,*g_out;                                                //��Ɨp�F�z��
  int i,j;                                                            //�J�E���^
  double value;                                                       //���ԕϐ�
  int radius_int=15;
  
//������
  gnInit(D_MODE);
  
//�摜�t�@�C���̓Ǎ���
  img=cvLoadImage("input.bmp",CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  
//�̈�m��
  g_in=(double*)malloc(sizeof(double)*img->width*img->height);
  g_out=(double*)malloc(sizeof(double)*img->width*img->height);
  
//��Ɨp�z��ւ̃R�s�[
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
  
//�N�s������
  gnDiscovery2(radius_int,img->width,img->height,g_in,g_out);
  
//��Ɨp�z�񂩂�R�s�[
  for(j=0;j<img->height;j++){
    for(i=0;i<img->width;i++){
      img->imageData[i+j*img->widthStep]=(unsigned char)Clip(255.0,g_out[i+j*img->width]);
    }
  }
  
//�̈�J��
  free(g_in);
  free(g_out);
  
//�N�s���摜�̕ۑ�
  cvSaveImage("output.bmp",img,0);
  
//�̈�J��
  cvReleaseImage(&img);
  gnFree();
  printf("completed\n");
  getch();
  return EXIT_SUCCESS;
}