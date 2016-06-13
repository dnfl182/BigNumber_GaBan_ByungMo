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
void init();	//시작 할때 필요한거 

void sendError(int a);	//오류 처리
int input(int a);	// 테스트용으로 사용(숫자받기) 
int getNew();	//새로운 임시 변수 가져오기 
int getVarNew(); // 새로운 변수 가져오기 
void show(int a);	//테스트용 (숫자 보여주기) 
int add(int a,int b);//더하기 
int minus(int a);//단항(마이너스) 
int subtract(int a, int b);//兮 
int multiply(int a, int b);//곱하기 
int divide(int a, int b);//나누기 
int rest(int a, int b);//나머지 
int compare(int a, int b);//비교(한주형 요청) 
void remover(int a);//임시변수 삭제 
void transition(int a,int b);	//a에 b를 대입(한주형 요청) 
void clear();	//화면 비우기 
void save();	//변수 저장 
void load();	//변수 불러오기 
void interpreter();	//복합 연산자(미완) 
void showVAR();	//변수 보여주기 
void findVarFromSignal(char c);	// 문자로 변수 찾기 
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
		//테스트
		for(int i = 0 ;i < COMMAND_LENGTH; i++)
			command[i] = 0;
	}
	
}
int add (int a, int b){	//�Ѵ� ��� ���� 
	
	int newNum = getNew();
	for(int i = 1 ; i < CIPHER_MAX; i++)
		Num[newNum][i] = Num[a][i] + Num[b][i];

	for(int i = CIPHER_MAX - 1; i >= 1; i--)
		if(Num[newNum][i] >= 10){
			if(i == 1)
				sendError(ERROR_NUMBER_OVERFLOW);	//������ 50�ڸ� ���� ũ�� 
			Num[newNum][i] -= 10;
			Num[newNum][i - 1]++;
		}
	
	return newNum; 
} 

int subtract(int a,int b){	//�Ѵ� ��� ����
	int newNum = getNew();
	
	if(compare(a,b) == -1){
		Num[newNum][0] = 1;	// - ��ȣ
		int t = a;	//Swap
		a = b;
		b = t;
	}
	for(int i = 1; i < CIPHER_MAX; i++)
		Num[newNum][i] = Num[a][i] - Num[b][i];
	
	for(int i =  CIPHER_MAX - 1; i >= 1; i--)
		if(Num[newNum][i] < 0){
			Num[newNum][i] += 10;
			Num[newNum][i - 1]--;
		}
	return newNum;
}
int multiply(int a,int b){	//���ϱ� �ݿø� ���� ���� 
	int temp[CIPHER_MAX  * 2 + 100];	//�׳� ũ�� ���� 
	int tempLen = CIPHER_MAX * 2 + 100; 
	int newNum = getNew();	//���ο� �� ������ 
	Num[newNum][0] = (Num[a][0] + Num[b][0]) % 2;
	
	for(int i = 0 ; i  < tempLen; i++)
		temp[i] = 0;
	
	for(int i = CIPHER_MAX - 1; i >= 0; i--)
		for(int j = CIPHER_MAX - 1; j >= 0; j--)
			temp[tempLen - (CIPHER_MAX - i - 1) - (CIPHER_MAX - j - 1) - 1] += Num[a][i] * Num[b][j];	//������ �ϳ��ϳ� ���ؼ����ϰ� 
		
		
	for(int i = tempLen - 1; i >= 1; i--)	//�ø��� 
		if(temp[i] >= 10 ){
			temp[i - 1] += temp[i] / 10;
			temp[i] %= 10;
		}
	
	if(temp[tempLen - DECIMAL] > 5){	//�ݿø� 
		temp[tempLen - DECIMAL - 1] += 1;
		for(int i = tempLen - 1; i >= 1; i--)
		if(temp[i] >= 10 ){
			temp[i - 1] += temp[i] / 10;
			temp[i] %= 10;
		}
	}///�ݿø� �ҽ� 
	int start = tempLen - DECIMAL- 1;	//���� �ֱ� 
	for(int i = start , j = CIPHER_MAX - 1; j >= 1; j--,i--)
		Num[newNum][j] = temp[i];
	return newNum;

}
int MultiTen(int a){	// * 10
	for(int i = 1; i <= CIPHER_MAX - 2; i++)
		Num[a][i] = Num[a][i + 1];
	Num[a][CIPHAR_MAX - 1] = 0;
}
int divide(int a, int b){
	int newNum = getNew();
	int size = 0;
	int temp[CIPHER_MAX  * 2 + 100];	//�̰͵� ũ�� 
	int tempLen = CIPHER_MAX * 2 + 100; 
	int ua = getNew();
	int ub = getNew();
	transition(ua,a);
	transition(ub,b);
	Num[newNum][0] = (Num[a][0] + Num[a][0]) % 2;
	int tempn = getNew();
	transition(tempn,ub);
	MultiTen(tempn);
	while(compare(a,tempn) >= 0){	// �Ի� �������ϱ����ؼ�  
		++size;
		MultiTen(ub);
		MultiTen(tempn);
	}
		remover(tempn);	//tempn ���� (���� ���ʿ���) 
	while(size != -10){		//�ǳ����� ���� ���� 
		int cal = 0;
		while(compare(ua,ub) >= 0 && compare(ua,ZERO) != 0){
			int ta;
			ta = ua;
			ua = subtract(ua,ub);
			remover(ta);
			cal++;
		}
		MultiTen(ua);
		temp[tempLen - 1 - DECIMAL - size] = cal;
		size--;
	}
	for(int i = CIPHER_MAX - 1, j = 1 ; i >= 0; i-- ,j++)
		Num[newNum][i] = temp[tempLen - j];
	 
	remover(ua);
	remover(ub);
	return newNum;
}
void clear(){
	printf("지워병 나중에 구현\n"); 
}
void init(){
	for(int i = 0 ; i < VAR_MAX + TVAR_MAX; i++)	// 부호 부분이 -1이면 없는것 취급 
		Num[i][0] = -1;
		
	for(int i = 0; i < VAR_MAX; i++)
		signal[i] = 0;
	for(int i = 0 ; i< TOTAL_VAR; i++)
		for(int j = 1; j < CIPHER_MAX; j++)	// 0 으로 전부 대입 
			Num[i][j] = 0;
			
	Num[TEN][CIPHER_MAX - DECIMAL - 2] = 1;	//10
	Num[ONE][CIPHER_MAX - DECIMAL - 1] = 1;	//1
	
}
int findVarFromSignal(char c){
	int var = -1;
	for(int i = 0; i < VAR_MAX; i++)
		if(signal[i] == c || signal[i] + 32 == c || signal[i] - 32 == c)	//대소문자 구별 X 귀찮아서 이렇게 
			var = TVAR_MAX + i;
	return var;
}
int getNew(){
	int newNum = -1;
	for(int i = 0 ; i < TVAR_MAX; i++)	//비어있는 임시변수 찾기 
		 if(Num[i][0] == -1){
		 	newNum = i;
		 	break;
		 }
	if(newNum == -1)	//에러 발생 
		sendError(ERROR_TVAR_OVERFLOW);
		
	for(int i = 0; i < CIPHER_MAX; i++)	// 초기화 
		Num[newNum][i] = 0;
	return newNum;
}

int getVarNew(){
	int newNum = -1;
	for(int i = TVAR_MAX ; i < TVAR_MAX + VAR_MAX; i++)	//비어있는 임시변수 찾기 
		 if(Num[i][0] == -1){
		 	newNum = i;
		 	break;
		 }
	if(newNum == -1)	//에러 발생 
		sendError(ERROR_VAR_OVERFLOW);
		
	for(int i = 0; i < CIPHER_MAX; i++)	// 초기화 
		Num[newNum][i] = 0;
	return newNum;
}

//테스트용 
void show(int a){
	printf("%c",Num[a][0] ? '-' : '+');
	for(int i = 1; i <= CIPHER_MAX - DECIMAL - 1; i++)
	printf("%d",Num[a][i]);
	printf(".");
	for(int i = CIPHER_MAX - DECIMAL; i < CIPHER_MAX; i++)
	printf("%d",Num[a][i]);
	printf("\n");
}
///테스트용 
int input(){ 
	char in[100];
	int toPoint = -1;	//정수부 숫자 갯수
	int newNum = getNew();
	int isMinus = 0;
	gets(in);
	
	if(in[0] == '-'){
		Num[newNum][0] = 1;
		isMinus = 1;
	}
	
	while(in[++toPoint]!='.' && in[toPoint] != 0);	// 정수부분 갯수 찾기 (마이너스 부호 포함) 
	
	
	for(int i = CIPHER_MAX - DECIMAL - toPoint +  isMinus , j = isMinus; j < toPoint; i++ , j++)
		Num[newNum][i] = in[j] -'0';	//문자열이므로 
	for(int i = CIPHER_MAX - DECIMAL , j = toPoint + 1; j < strlen(in); i++ , j++){
		Num[newNum][i] = in[j] -'0';
	}
	return newNum;
}

int compare(int a,int b){	// 1 : (a 가 크다) 0  : (같다) -1 : (b가 크다)
	
	for(int i = 1; i < CIPHER_MAX; i++)
	{
		if(Num[a][i] > Num[b][i])
			return 1;
		else if(Num[a][i] < Num[b][i])
			return -1;
	}
	return 0;
}
void transition(int a, int b){
	for(int i = 0 ; i < CIPHER_MAX; i++)
		Num[a][i] = Num[b][i];
}
void remover(int a){
	if(a < TVAR_MAX)
		Num[a][0] = -1;
}

void showVAR(){
	for(int i = 0; i < VAR_MAX ; i++)
		if(Num[TVAR_MAX + i][0] != -1){
			printf("%c = ",signal[i]);
			show(TVAR_MAX + i);
		}
}
