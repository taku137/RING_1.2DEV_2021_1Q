/*
* ring.h
*
*  Created on: 2020/04/04
*      Author: Jun Nishigata
*/

#ifndef RING_H_
#define RING_H_

#define D_MODE 0
#define R_MODE 1

#if defined(__cplusplus)
extern "C"
{
  #endif
  int ErrorMessage(int);
  int gnDiscovery2(int,int,int,double*,double*);
  void gnFree(void);
  int gnInit(int);
  int gnRing1(int,int,int,double*,double*);
  #if defined(__cplusplus)
}
#endif

#endif
