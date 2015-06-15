#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

int main (int* argc,char **argv){
	FILE *old = fopen(argv[1], "rb");
	FILE *new1 = fopen(argv[2], "rb");
	int header[18],w,h;
	printf("%s %s \n",argv[1],argv[2]);
	float mse[3]={0},snr[3]={0},snr_n[3]={0};
	for (int i =0; i<18 ; i++){
 		header[i]=getc(old);
 		getc(new1);
 	}
 	w = (header[13]<<8)+header[12];
	h = (header[15]<<8)+header[14];
	for(int i=0;i<h;i++){
		for (int j=0;j<w;j++){
			for(int k=0;k<3;k++){
				long s_old=getc(old);
				long s_new=getc(new1);
				snr[k]+=pow(s_old,2);
				snr_n[k]+=pow(s_new,2);
				mse[k]+=pow(s_old-s_new,2);
			}
		}
	}
	int all=w*h;
	float mse_all=0,snr_all=0;
	printf("%f %f %f \n",snr[0],snr[1],snr[2]);
	for(int i=0;i<3;i++){
		snr[i]/=all;
		//printf("all %f %f \n",snr[i],all);
		mse[i]/=all;
		snr[i]/=mse[i];
		//printf("%f \n",snr[i]);
		mse_all+=mse[i];
		snr_all+=snr[i];
		printf("snr[%d]:%f   mse[%d]:%f \n",i,snr[i],i,mse[i]);
	}
	mse_all/=3;
	snr_all/=3;
	
	
}