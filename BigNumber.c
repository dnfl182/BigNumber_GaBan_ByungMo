/*///////////////////////////////////////////
 
 기본 베이스 
	만든이 : 전병모  

//////////////////////////////////////////////*/
#include <stdio.h>
#define VAR_MAX 10		//변수의 갯수 
char Num[100][59]; //임시 변수 
char VAR[VAR_MAX][59];	//변수
char VARC[VAR_MAX];	//변수 기호 
char Solving[1000];		//가공 식
char Input[1000]; 	//가공전 식 

///한주형 부분 
void add(int a,int b);	//더하기 //a,b = 임시변수에서 숫자번호		10001부터는 변수 번호 README.md 를 통해 나중에 설명 
void multiply(int a,int b);	//곱하기 

///정명훈 부분 
void divide(int a,int b);	//나누기
void rest(int a,int b); //나머지 

///전병모 부분 
void solve(void){};	//Input 의미 분석
void save();	//저장 
void load();	//불러오기

///김상록 부분 
void minus(int a);	//단항- 
void subtract(int a,int b);	//뺴기 
void print(); 	//콤마 조건에맞게 출력 


char EndString[] = "end";
int main(void){
	printf("Start....\n");
	
	while(1){
		printf("(Input) ");
		scanf("%s",Input);
		
		//////////////////////////	end 치면 끝나는 문장 
		int end = 1;
		int i = 0; 
		for(i = 0; i < 3; i++)
			if(Input[i] != EndString[i])
				end = 0;
		if(end)
			break;
		///////////////////////////
		
		solve();
		
		
	}
	
	return 0;
}
