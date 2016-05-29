/*
	기본 베이스 새로운 버젼
	*/	
#include <stdio.h>
#include <string.h>
#define CIPHER_MAX 60	// 숫자배열의 총길이 0 : 부호 1 ~ 50 : 정수 51 ~ 59 소수 
#define DECIMAL 9	//소수 부분 
#define VAR_MAX 10	//변수
#define TVAR_MAX 100 //임시변수
#define SVAR_MAX 3	//특별한 변수
#define ONE	VAR_MAX + TVAR_MAX + 1
#define TEN VAR_MAX + TVAR_MAX + 2
#define ZERO VAR_MAX + TVAR_MAX + 3
#define TOTAL_VAR (VAR_MAX+TVAR_MAX+SVAR_MAX)
#define COMMAND_LENGTH 20
char Num[TOTAL_VAR][CIPHER_MAX];	//0 ~ TVAR_MAX - 1 : (임시변수) 기타생략
char signal[VAR_MAX];	//변수 기호 
char command[COMMAND_LENGTH];	//명령 
void init();

void sendError(int a);	//오류 처리
int input(int a);
int getNew();
int getVarNew();
void show(int a);
int add(int a,int b);
int minus(int a);
int subtract(int a, int b);
int multiply(int a, int b);
int divide(int a, int b);
int rest(int a, int b);
int compare(int a, int b);
void remover(int a);
void transition(int a,int b);	//a에 b를 대입(복사)
void clear();
void save();
void load();
void interpreter();
void showVAR();
int main(void){
	init();

	while(1){
		printf("입력하세요 : "); 
		gets(command);
		if(strcmp(command,"end") == 0)
			break;
		else if(strcmp(command,"clear") == 0)
			clear();
		else if(strcmp(command,"save") == 0)
			save();
		else if(strcmp(command,"load") == 0)
			load();
		else if(strcmp(command,"var") == 0)
			showVAR();
		else
			interpreter();

		for(int i = 0 ;i < COMMAND_LENGTH; i++)
			command[i] = 0;
	}
	
}

