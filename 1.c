#include<stdio.h>
int main()
{
	char num_1[61]="";
	char num_2[61]="";
	int i,t;

	scanf("%s",num_1);
	for(i=0;i<61;i++)
		if(num_1[i]=='\0')
			t=i;

	for(i=0;num_1[i]!='\0';i+=3){
		printf("%c%c%c",num_1[i],num_1[i+1],num_1[i+2]);
		if(num_1[i+3]!='\0')
			printf(",");
	}
	return 0;
}


