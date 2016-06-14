//########################
//
//조장 : 
//	전병모
//조원 :
//	김상록
//	정명훈
//	조한주
//###############################
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CIPHER_MAX 60	// 숫자배열의 총길이 0 : 부호 1 ~ 50 : 정수 51 ~ 59 소수 
#define DECIMAL 9	//소수 부분 
#define VAR_MAX 10	//변수
#define TVAR_MAX 100 //임시변수
#define SVAR_MAX 3	//특별한 변수
#define ONE	VAR_MAX + TVAR_MAX + 0	//1
#define TEN VAR_MAX + TVAR_MAX + 1	//10
#define ZERO VAR_MAX + TVAR_MAX + 2	//0
#define TOTAL_VAR (VAR_MAX+TVAR_MAX+SVAR_MAX)
#define COMMAND_LENGTH 30
char Num[TOTAL_VAR][CIPHER_MAX];	//0 ~ TVAR_MAX - 1 : 실제 계산에 쓰이는 변수들과 그냥 변수와 특변한 변수 저장하는곳
char signal[VAR_MAX];	//변수 기호 
char command[COMMAND_LENGTH];	//명령 
void init();

void sendError(int a);	//오류 처리(상수로 분류)
#define ERROR_TVAR_OVERFLOW 1
#define ERROR_NUMBER_OVERFLOW 2
#define ERROR_FILE 3
#define ERROR_VAR_OVERFLOW 4
#define ERROR_CAL 5
#define ERROR_UNKNOWN 6
#define ERROR_DIVIDEZERO 7
int getNew();			//새로운 수 저장소를 가져온다
int getVarNew();		//새로운 변수 저장소를 가져온다
void show(int a);		//콤마를 찍어서 수를 보여준다
int add(int a,int b);	//더하기
int minus(int a);		//단항 마이너스
int subtract(int a, int b);	//빼기
int multiply(int a, int b);	//곱하기
int divide(int a, int b);	//나누기
int rest(int a, int b);		//나머지
int compare(int a, int b);	//비교하기
void remover(int a);		//수 저장소 삭제
void transition(int a,int b);	//a에 b를 대입(복사)
void clear();	//화면지우기
void save();		//저장하기
void load();		//불러오기
void interpreter();		//식 해석기
void showVAR();			//(현재)변수 보여주기
int findVarFromSignal(char c);	//기호로 변수찾기
int main(void){
	init();
	printf("Start.... \n"); 
	while(1){		//명령 루프
		printf("(input) ");
		gets(command);
		if(strcmp(command,"end") == 0)
			break;
		else if(strcmp(command,"clear") == 0)
			clear();
		else if(strcmp(command,"save") == 0)
			save();
		else if(strcmp(command,"load") == 0)
			load();
		else if(strcmp(command,"VAR") == 0)
			showVAR();
		else
			interpreter();
		for(int i = 0 ;i < COMMAND_LENGTH; i++)
			command[i] = 0;	//명령어 지우기
	}
	
}
int isAlpha(char c){	//c 가 알파벳인가?
	if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
		return 1;
	return 0;
}
int isNumber(char c){	// c가 숫자인가?
	if(c >= '0' && c <= '9' || c == '.')
		return 1;
	return 0;
}
void interpreter(){	//made : 전병모
	int header = -1;
	int condition = 0;	//0 : 단순 연산 1 : 대입연산
	int target = -1;	// 대입할경우 대상 
	
	for(int i = 0 ; i < COMMAND_LENGTH; i++)	//문제 쪼개기 
		if(command[i] == '=')
			condition = 1;
			
	if(condition == 1){	//오류 해결및 대입할 대상을 정함 
			while(command[++header] != '='){
				if(command[header] == ' ')
					continue;
				if(isAlpha(command[header])){
					if(target != -1){
						sendError(ERROR_CAL);
						return;
					}
					target = command[header];
				}
				else{
						sendError(ERROR_CAL);
						return;
				}	
			}
	}
	int cal[1000] = {0};
	int calHeader = -1;	//cal을 쓰고 읽는 역할
	int state = 0;	// 0 숫자를 받을 차례 1 부호를 받을 차례 2  마이너스 특수 
	while(command[++header] != 0){
		if(command[header] == ' ')
			continue;
		
		if(state == 0 || state == 2){
			char in[CIPHER_MAX] = {0};
			int inHeader = -1;
			if(command[header] == '-'){		//마이너스 특수 처리 
				if(state == 0){
					state = 2;
					continue;
				}
				else{
					sendError(ERROR_CAL);
					return;
				}
			}
			else if(isAlpha(command[header])){	//알파벳일경우
				int varNum;
				if((varNum = findVarFromSignal(command[header])) == -1){	//없는 변수일경우
				 	sendError(ERROR_UNKNOWN);
				 	return;
				 }
				cal[++calHeader] = varNum;
			}
			else if(!isNumber(command[header])){	//숫자가 아닐경우
				sendError(ERROR_CAL);
				return;
			}
			else{
				in[++inHeader] = command[header];
				while(isNumber(command[header + 1])){	//숫자 계산 
					++header;
					in[++inHeader] = command[header];
				}
				///////문자 숫자화 
				int toPoint = -1;	//정수부 숫자 갯수
				int newNum = getNew();
				int isMinus = 0;
				if(in[0] == '-'){
					Num[newNum][0] = 1;
					isMinus = 1;
				}
				while(in[++toPoint]!='.' && in[toPoint] != 0);	// 정수부분 갯수 찾기 (마이너스 부호 포함) 
				for(int i = CIPHER_MAX - DECIMAL - toPoint +  isMinus , j = isMinus; j < toPoint; i++ , j++)
					Num[newNum][i] = in[j] -'0';	//문자열이므로 
				for(int i = CIPHER_MAX - DECIMAL , j = toPoint + 1; j < strlen(in); i++ , j++)
					Num[newNum][i] = in[j] -'0';
				///////////
				if(state == 2)
					minus(newNum);
				cal[++calHeader] = newNum;
			}
			state = 1;
		}
		else{
			int can = 0;
			char whiteList[5] = {'+','-','*','/','%'};//허용 부호들
			for(int i = 0 ; i < 5; i++)
				if(command[header] == whiteList[i])
					can = 1;
			if(can == 0){	//정해진 기호가 아닐경우
				sendError(ERROR_CAL);
				return;
			}
			cal[++calHeader] = command[header] + 1000;	// + 1000 이유 숫자랑 구분하기 위해서 
			state = 0;
		}	
	}
	if(state != 1){
		sendError(ERROR_CAL);
		return;
	}
	//여기부터 연산시작//곱하기 나누기 나머지 부터
	for(int i = 0 ; i <= calHeader;){
		int newNum;
		if(cal[i] == '*' + 1000)
			newNum = multiply(cal[i-1],cal[i+1]);
		else if(cal[i] == '/' + 1000){
			if(compare(cal[i + 1] , ZERO) == 0){
				sendError(ERROR_DIVIDEZERO);
				return;
			}
			newNum = divide(cal[i-1],cal[i+1]);
		}
		else if(cal[i] == '%' + 1000){
			if(compare(cal[i + 1] , ZERO) == 0){
				sendError(ERROR_DIVIDEZERO);
				return;
			}
			newNum = rest(cal[i-1],cal[i+1]);
		}
		else{
			i++;
			continue;
		}
		remover(cal[i-1]);
		remover(cal[i+1]);
		cal[i - 1] = newNum;
		calHeader -= 2;
		for(int j = i; j <= calHeader; j++)
			cal[j] = cal[j + 2];
		
	}
	//빼기와 더하기만 남았다. 
	while(calHeader != 0){
		int newNum;
		//특별한 더하기 그리고 빼기
		if(cal[1] == '-' + 1000)
		 minus(cal[2]);
			 
		if(Num[cal[0]][0] == 0 && Num[cal[2]][0] == 0)	// 둘다 양수 일경우 
			newNum = add(cal[0],cal[2]); 
		else if(Num[cal[0]][0] == 1 && Num[cal[2]][0] == 1)	//둘다 음수 
			newNum = minus(add(cal[0],cal[2]));
		else if(Num[cal[0]][0] == 0 && Num[cal[2]][0] == 1)	//한수가 양수 
			newNum = subtract(cal[0],cal[2]);
		else
			newNum = subtract(cal[2],cal[1]);
		remover(cal[0]);
		remover(cal[2]);
		cal[0] = newNum;
		calHeader -= 2;
		for(int j = 1; j <= calHeader; j++)
			cal[j] = cal[j + 2];
		
	}
	if(target != -1){
		int temp;
		if((temp = findVarFromSignal(target)) != -1)
			target = temp;
		else{
			temp = getVarNew();
			signal[temp - TVAR_MAX] = target;
			target = temp;
		}
		transition(target,cal[0]);
	}
	printf("        = ");
	show(cal[0]);
	remover(cal[0]);
	
}
void clear(){	//made : 김상록
	system("clear");
}
void init(){	//특수변수 설정(0,10,1)
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

void sendError(int a){	//made : 전병모
	switch(a){
		case ERROR_TVAR_OVERFLOW : 
		printf("프로그램 내부에서 임시 변수 저장공간이 부족합니다.");
		break;
		case ERROR_NUMBER_OVERFLOW:
		printf("최대 자리수를 벗어남");	
		break;
		case ERROR_FILE:
		printf("파일 관련 오류");
		break;
		case ERROR_VAR_OVERFLOW:
		printf("변수를 더 이상 저장할수 없습니다.");
		break;
		case ERROR_CAL:
		printf("계산 오류.");
		break; 
		case ERROR_UNKNOWN:
		printf("undefined");
		break; 
		case ERROR_DIVIDEZERO:
		printf("0으로 나눌수 없습니다.");
		break;
	}
	printf("\n");
}

int getNew(){	//made 전병모
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

int getVarNew(){	//made 전병모
	int newNum = -1;	
	for(int i = TVAR_MAX ; i < TVAR_MAX + VAR_MAX; i++)	//비어있는 임시변수 찾기 
		 if(Num[i][0] == -1){//부호에서 -1이면 없는거 취급
		 	newNum = i;
		 	break;
		 }
	if(newNum == -1)	//에러 발생(남은수자리가 없음) 
		sendError(ERROR_VAR_OVERFLOW);
		
	for(int i = 0; i < CIPHER_MAX; i++)	// 초기화 
		Num[newNum][i] = 0;
	return newNum;
}
void show(int a){	//made 김상록
	int start = CIPHER_MAX - DECIMAL - 1;
	int end = CIPHER_MAX - DECIMAL - 1;
	for(int i = 1; i < CIPHER_MAX - DECIMAL - 1; i++)
		if(Num[a][i] != 0){
			start = i;
			break;
		}
	for(int i = CIPHER_MAX - 1; i >= CIPHER_MAX - DECIMAL - 1; i--)
		if(Num[a][i] != 0){
			end = i;
			break;
	}
	if(Num[a][0] == 1)
		printf("-");
	for(int i = start; i <= end; i++){
		if(i == CIPHER_MAX - DECIMAL)
			printf(".");
		else if(i % 3 == 0 && i != start)
			printf(",");
		printf("%d",Num[a][i]);
	}
	printf("\n"); 
}

int add (int a, int b){	//made 조한주 (a,b)는 양수여야함 
	
	int newNum = getNew();
	for(int i = 1 ; i < CIPHER_MAX; i++)
		Num[newNum][i] = Num[a][i] + Num[b][i];

	for(int i = CIPHER_MAX - 1; i >= 1; i--)
		if(Num[newNum][i] >= 10){
			if(i == 1)
				sendError(ERROR_NUMBER_OVERFLOW);	//정수가 50자리 보다 크다 
			Num[newNum][i] -= 10;
			Num[newNum][i - 1]++;
		}	
	return newNum; 
} 

int subtract(int a,int b){	//made 조한주 (a,b)는 양수 (a > b)
	int newNum = getNew();
	if(compare(a,b) == -1){
		Num[newNum][0] = 1;	// - 부호
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

int compare(int a,int b){	//made : 조한주   1 : (a 가 크다) 0  : (같다) -1 : (b가 크다)
	
	for(int i = 1; i < CIPHER_MAX; i++)
	{
		if(Num[a][i] > Num[b][i])
			return 1;
		else if(Num[a][i] < Num[b][i])
			return -1;
	}
	return 0;
}

int multiply(int a,int b){	//made 정명훈
	int temp[CIPHER_MAX + DECIMAL + 200];
	int tempLen = CIPHER_MAX + DECIMAL + 200;
	int newNum = getNew();
	Num[newNum][0] = (Num[a][0] + Num[b][0]) % 2;
	for(int i = 0 ; i  < tempLen; i++)
		temp[i] = 0;
	
	for(int i = CIPHER_MAX - 1; i >= 0; i--)
		for(int j = CIPHER_MAX - 1; j >= 0; j--){
			temp[tempLen - (CIPHER_MAX - i - 1) - (CIPHER_MAX - j - 1) - 1] += Num[a][i] * Num[b][j];
		}
		
	for(int i = tempLen - 1; i >= 1; i--)
		if(temp[i] >= 10 ){
			temp[i - 1] += temp[i] / 10;
			temp[i] %= 10;
		}
	if(temp[tempLen - DECIMAL] > 5){
		temp[tempLen - DECIMAL - 1] += 1;
		for(int i = tempLen - 1; i >= 1; i--)
		if(temp[i] >= 10 ){
			temp[i - 1] += temp[i] / 10;
			temp[i] %= 10;
		}
	}
	int start = tempLen - DECIMAL- 1;
	for(int i = start , j = CIPHER_MAX - 1; j >= 1; j--,i--)
		Num[newNum][j] = temp[i];
	return newNum;

}

int minus(int a){	//made :김상록
	Num[a][0] = (Num[a][0] + 1) % 2;
	return a;	
}

void transition(int a, int b){ //made : 조한주
	for(int i = 0 ; i < CIPHER_MAX; i++)
		Num[a][i] = Num[b][i];
}
void remover(int a){ //made : 전병모
	if(a < TVAR_MAX)
		Num[a][0] = -1;
}

int MultiTen(int a){	// * 10
	for(int i = 1; i <= CIPHER_MAX - 2; i++)
		Num[a][i] = Num[a][i + 1];
}
int divide(int a, int b){	//made : 정명훈
	int newNum = getNew();
	int size = 0;
	int temp[200] = {0};
	int tempLen = 200;
	int ua = getNew();
	int ub = getNew();
	transition(ua,a);
	transition(ub,b);
	Num[newNum][0] = (Num[a][0] + Num[b][0]) % 2;
	int tempn = getNew();
	transition(tempn,ub);
	MultiTen(tempn);
	while(compare(a,tempn) >= 0){
		++size;
		MultiTen(ub);
		MultiTen(tempn);
	}
	remover(tempn); 
	while(size != -10){
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
int rest(int a, int b){		//made : 김상록
	int newNum = getNew();
	int r = divide(a,b);
	
	for(int i = CIPHER_MAX - DECIMAL; i < CIPHER_MAX; i++)
		Num[r][i] = 0;
	int m = multiply(r,b);
	newNum = subtract(a,m);
	remover(r);
	remover(m);
	return newNum;
}
void load(){	//made 김상록
	FILE *fp;
	char varChar;
	char in[CIPHER_MAX];
	
	if((fp = fopen("save","r")) == NULL){
		sendError(ERROR_FILE);
		return;
	}
	while(fscanf(fp,"%c %s\n",&varChar,in) != EOF){
		int varNum = findVarFromSignal(varChar);
		if(varNum == -1){
			varNum = getVarNew();
			signal[varNum - TVAR_MAX] = varChar;
		}
		for(int i = 0 ; i < CIPHER_MAX; i++)
			Num[varNum][i] = in[i] - '0';
	}
	fclose(fp);
}
void save(){		//made 김상록
	FILE *fp;
	if((fp = fopen("save","w")) == NULL){
		sendError(ERROR_FILE);
		return;
	}
	for(int i = 0; i < VAR_MAX; i++)
		if(Num[TVAR_MAX + i][0] != -1){
			fprintf(fp,"%c ",signal[i]);
			for(int j = 0 ; j< CIPHER_MAX; j++)
				fprintf(fp,"%d",Num[TVAR_MAX + i][j]);
			fprintf(fp,"\n");
		}
	fclose(fp);
}
void showVAR(){	//made 전병모
	for(int i = 0; i < VAR_MAX ; i++)
		if(Num[TVAR_MAX + i][0] != -1){
			printf("        %c = ",signal[i]);
			show(TVAR_MAX + i);
		}
}

int findVarFromSignal(char c){	//made 전병모
	int var = -1;
	for(int i = 0; i < VAR_MAX; i++)
		if(signal[i] == c || signal[i] + 32 == c || signal[i] - 32 == c)	//대소문자 구별 X 귀찮아서 이렇게 
			var = TVAR_MAX + i;
	return var;
}
//기타도움 : 전병모
