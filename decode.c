
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
void put3c(int a,int b,int c,FILE * out){
	putc(a,out);
	putc(b,out);
	putc(c,out);
	return;
}
int get_mask(int n){
	int mask=0;
	for(int i=0;i<n;i++){
		mask <<=1;
		mask |= 1;
	}
	mask <<= 8-n;
	return mask;
}
int half(int n,int num){
	int tmp=0;
	tmp |=1;
	tmp<<=7-n;
	num |=tmp;
	return num;
}
int quant (int c,int mask, int bit){
	return  half(bit,c&mask);
		
}
int main (int argc,char **argv){	
	FILE *fp = fopen(argv[1], "rb");
	FILE *fp2 = fopen("decode.tga", "wb");
	int w, h, mask_r,mask_g,mask_b;
	long long r=0,g=0,b=0;
	//int r_bit=argv[3][0]-48,g_bit=argv[4][0]-48,b_bit=argv[5][0]-48;
	//printf("%d %d %d",r_bit,g_bit,b_bit);
	int r_bit=3,g_bit=3,b_bit=2;
	float mse,mse_r,mse_g,mse_b,snr,snr_r,snr_g,snr_b;
	mse_r=mse_g=mse_b=mse=snr=snr_r=snr_g=snr_b=0;
	mask_r=get_mask(r_bit);
	mask_g=get_mask(g_bit);
	mask_b=get_mask(b_bit);
	int header[18];	
	int header2[18];
	for (int i =0; i<18 ; i++){
 		header[i]=getc(fp);
 		header2[i]=header[i];
 	}
 	//pobieranie nagłówka
 	w = (header[13]<<8)+header[12];
	h = (header[15]<<8)+header[14];
	for (int i =0; i<18 ; i++){
                putc(header2[i],fp2);
    }
	int i=0;
	int j=0;
	int rgb[3];
	int rgb_l[3];
	int rgb_u[3];
	int nrgb[3];
	int prgb[3]={0};
	for (i=0;i<h;i++){
		for( j=0;j < w;j++){
			for(int k=0;k<3;k++){
				rgb_l[k]=getc(fp);
			}
			for(int k=0;k<3;k++){
				rgb_u[k]=getc(fp)+prgb[k];
				putc(rgb_l[k]-rgb_u[k],fp2);
			}
			for(int k=0;k<3;k++){
				putc(rgb_l[k]+rgb_u[k],fp2);
			}
			for(int l=0;l<3;l++) prgb[l]=rgb_u[l];
		}
	}
	int all=w*h;
	return 0;
}