#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
int main(){
	FILE *in = fopen("sample.data", "wb");
	int x=1;
	for (int i=0; i<256;i++){
		putc(i,in);
		//x<<=1;
		//x|=1;
		//printf("%d ",x);
		
	}
}