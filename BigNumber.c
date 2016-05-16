/*///////////////////////////////////////////
 
 기본 베이스 
	만든이 : 전병모  

//////////////////////////////////////////////*/
#include <stdio.h>
#define VAR_MAX 10	//변수 최대갯수 
#define CIPHER_MAX 60	//부호 + 50 + 9 
#define NUM_MAX 100		//임시변수 
#define FLOAT 9			//소수 자릿수 
int Num[NUM_MAX][CIPHER_MAX]; //임시 변수 
int VAR[VAR_MAX][CIPHER_MAX];	//변수
char VARC[VAR_MAX];	//변수 기호 
char Solving[1000];		//가공 식
char Input[1000]; 	//가공전 식 
int one[CIPHER_MAX];	//계산에 직접사용할수1 
int two[CIPHER_MAX];	//계산에 직접사용할수2 

void remover(int,int);
void numberResgister(int,int);//등록 
int add(int,int);	//더하기 
int subtract(int,int);	//뺴기 
int multiply(int,int);	//곱하기 
int divide(int,int);	//나누기
void rest(int a,int b); //나머지 
void solve(void){};	//Input 의미 분석
void save();	//저장 
void load();	//불러오기 

int minus(int);	//단항- 
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



/*
numberResgister
	입력받은 변수변호 a,b를 해석 
	one 에 a  , two에 b 함수 저장 (직접사용하기) 
*/
void numberResgister(int a,int b){ 
	int i;
	if(a >= 10000){
		int t = a - 10000;
		for(i = 0 ;i < CIPHER_MAX; i++)
			one[i] = VAR[t][i];
		VAR[t][0] = -1;
		}
	else{
		for(i = 0; i < CIPHER_MAX; i++)
			one[i] = Num[a][i];
	}
	if(b >= 10000){
		int t = b - 10000;
		for( i = 0 ;i < CIPHER_MAX; i++)
			two[i] = VAR[t][i];
		VAR[t][0] = -1;
		}
	else{
		for(i = 0; i < CIPHER_MAX; i++)
			two[i] = Num[b][i];
	}
	
/*	printf("one :\n");	//테스트
	if(one[0] == 1)
		putchar('-');
	else
		putchar(' ');
	for(i = 1 ; i < CIPHER_MAX; i++)
		printf("%d",one[i]);
	putchar('\n');
	printf("two :\n");
	if(two[0] == 1)
		putchar('-');
	else
		putchar(' ');
	for(i = 1 ; i < CIPHER_MAX; i++)
		printf("%d",two[i]);
	putchar('\n');	//테스트 종료 */
} 
/*getNew
	비어있는 임시변수를 찾아서 변수번호 반환*/ 
int getNew(void){
	int new;
	int i;
	for(i = 0; i < NUM_MAX; i ++)	//비어잇는 임시 변수 찾기 
		if(Num[i][0] == -1){
			new = i;
			break;
		}
	for(i = 0; i < CIPHER_MAX; i++)	//사용할변수 깨끗하게 
		Num[new][0]  = 0;
	return new;
}
/*remover
	사용한 임시변수를 삭제하는 역할을함 
	*/ 
void remover(int a,int b){
	Num[a][0] = -1;
	Num[b][0] = -1;
}
