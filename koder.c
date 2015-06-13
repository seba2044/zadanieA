#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#define ERR        0xFF   
int counter=0;
unsigned char const encode_table[16] =   
{   
0x00,0x0b,0x16,0x1d,0x27,0x2c,0x31,0x3a,   
0x45,0x4e,0x53,0x58,0x62,0x69,0x74,0x7f   
};   
       
unsigned char const correction_table[8] =   
{   
0x00,0x01,0x02,0x08,0x04,0x40,0x10,0x20   
};   

unsigned char upper4bit(unsigned char x){
	x&=240;//11110000
	return x>>=4;
}
unsigned char lower4bit(unsigned char x){ 
	return x&=15;
}
unsigned char diff(unsigned char x,unsigned char y){
	int i=0;
	if (lower4bit(x)!=lower4bit(y)){
		i++;
	}
	if(upper4bit(x)!=upper4bit(y)){
		i++;
	}
	return i;
}
unsigned char randomize(unsigned char x,int p){
	int r_num=rand()%101;
	int y=x;
	for (int i=0;i<8;i++){
		r_num=rand()%100;
		//printf("%d %d\n",r_num,p);
		y=x;
		if (r_num < p){
			//printf("%d p:%d x:%d\n",r_num,p,x);
			y>>=i;
			int k=1;
			if(k&=y){//gdy 1 na i-tym miejscu 
				k<<=i;
				//printf("k:%d \n",k);
				k=~k;
				//printf("!k:%d \n",k);
				x&=k;
			}else{
				k=1;
				k<<=i;
				k=~k;
				x=~(x^k);
			}
			//printf("loosy:%d \n",x);
		}
	}
	return x;
}
            
unsigned char Encode_84(unsigned char x){   
	unsigned char c,d,p;   
	c=d=encode_table[x];   
	p=0;   
	while(d){   
		p^=d;
		d>>=1;
	}   
	return (c<<1)|(p&1);   
}   
unsigned char Decode_84(unsigned char x){   
	unsigned char parity,syndrom;   
	syndrom=x;   
	parity=0;   
	while(syndrom){   
		parity^=syndrom;   
		syndrom>>=1;   
	}   
	parity&=1;   
    //z=x;
    //printf("%d ",p);   
	syndrom=(x>>1)^encode_table[x>>4];   
    //printf("%d %d\n",x>>1 ,encode_table[x>>4]);	
	if(syndrom){   
		//printf("%d ",p);
		if(!parity){
			printf("Więcej niż 1 błąd\n");
			return ERR;     
		}
		puts("Wykryto i naprawiono błąd\n");
		return (x^(correction_table[syndrom]<<1))>>4;   
	}   
       
	return x>>4;   
}   
int main (int argc,char **argv){
	FILE *in = fopen(argv[2], "rb");
	FILE *out;
	if (argv[1][0]=='o'){
		puts("ws");
		out= fopen(argv[3], "rb");
	}else{
		out = fopen(argv[3], "wb");
	}
	unsigned char a[1],b[1],upper,lower;
	int freadc,all=0;
	int diff4bit=0;
	srand (time(NULL));
	while((freadc=fread(a,sizeof(unsigned char),1,in))>0){
		switch (argv[1][0]){
			case 'e':
				putc(Encode_84(upper4bit(a[0])),out);
				putc(Encode_84(lower4bit(a[0])),out);
				break;
			case 'd':
				upper=Decode_84(a[0]);
				upper<<=4;
				if((freadc=fread(a,sizeof(unsigned char),1,in))>0){
					//printf("%u",a[0]);
					lower=Decode_84(a[0]);
					//printf("%u ",upper|lower);
					putc(upper|lower,out);
				}else{
					putc(upper,out);
				}
				break;
			//mieszczas 
			case 'l':
				putc(randomize(a[0],argv[4][0]-48),out);
				break;
			case 'o':
				if((freadc=fread(b,sizeof(unsigned char),1,out))>0){
					diff4bit+=diff(a[0],b[0]);
				}
				break;
			default:
				break;
		}
		all++;
		//printf("%u  \n",upper4bit(a[0]),lower4bit(a[0]));
	}
	if (argv[1][0]=='o'){
		all*=2;
		float f=(float)diff4bit/(float)all;
		printf("Diff:%d; %f\n",diff4bit,f);
	}
	fclose(in);
	fclose(out);
	puts("koniec");
	return 0;
}