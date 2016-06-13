/*
	ê¸°ë³¸ ë² ì´ìŠ¤ ìƒˆë¡œìš´ ë²„ì ¼
	*/	
#include <stdio.h>
#include <string.h>
#define CIPHER_MAX 60	// ìˆ«ìžë°°ì—´ì˜ ì´ê¸¸ì´ 0 : ë¶€í˜¸ 1 ~ 50 : ì •ìˆ˜ 51 ~ 59 ì†Œìˆ˜ 
#define DECIMAL 9	//ì†Œìˆ˜ ë¶€ë¶„ 
#define VAR_MAX 10	//ë³€ìˆ˜
#define TVAR_MAX 100 //ìž„ì‹œë³€ìˆ˜
#define SVAR_MAX 3	//íŠ¹ë³„í•œ ë³€ìˆ˜
#define ONE	VAR_MAX + TVAR_MAX + 1
#define TEN VAR_MAX + TVAR_MAX + 2
#define ZERO VAR_MAX + TVAR_MAX + 3
#define TOTAL_VAR (VAR_MAX+TVAR_MAX+SVAR_MAX)
#define COMMAND_LENGTH 20
char Num[TOTAL_VAR][CIPHER_MAX];	//0 ~ TVAR_MAX - 1 : (ìž„ì‹œë³€ìˆ˜) ê¸°íƒ€ìƒëžµ
char signal[VAR_MAX];	//ë³€ìˆ˜ ê¸°í˜¸ 
char command[COMMAND_LENGTH];	//ëª…ë ¹ 
void init();	//ì‹œìž‘ í• ë•Œ í•„ìš”í•œê±° 

void sendError(int a);	//ì˜¤ë¥˜ ì²˜ë¦¬
int input(int a);	// í…ŒìŠ¤íŠ¸ìš©ìœ¼ë¡œ ì‚¬ìš©(ìˆ«ìžë°›ê¸°) 
int getNew();	//ìƒˆë¡œìš´ ìž„ì‹œ ë³€ìˆ˜ ê°€ì ¸ì˜¤ê¸° 
int getVarNew(); // ìƒˆë¡œìš´ ë³€ìˆ˜ ê°€ì ¸ì˜¤ê¸° 
void show(int a);	//í…ŒìŠ¤íŠ¸ìš© (ìˆ«ìž ë³´ì—¬ì£¼ê¸°) 
int add(int a,int b);//ë”í•˜ê¸° 
int minus(int a);//ë‹¨í•­(ë§ˆì´ë„ˆìŠ¤) 
int subtract(int a, int b);//Â•å…® 
int multiply(int a, int b);//ê³±í•˜ê¸° 
int divide(int a, int b);//ë‚˜ëˆ„ê¸° 
int rest(int a, int b);//ë‚˜ë¨¸ì§€ 
int compare(int a, int b);//ë¹„êµ(í•œì£¼í˜• ìš”ì²­) 
void remover(int a);//ìž„ì‹œë³€ìˆ˜ ì‚­ì œ 
void transition(int a,int b);	//aì— bë¥¼ ëŒ€ìž…(í•œì£¼í˜• ìš”ì²­) 
void clear();	//í™”ë©´ ë¹„ìš°ê¸° 
void save();	//ë³€ìˆ˜ ì €ìž¥ 
void load();	//ë³€ìˆ˜ ë¶ˆëŸ¬ì˜¤ê¸° 
void interpreter();	//ë³µí•© ì—°ì‚°ìž(ë¯¸ì™„) 
void showVAR();	//ë³€ìˆ˜ ë³´ì—¬ì£¼ê¸° 
void findVarFromSignal(char c);	// ë¬¸ìžë¡œ ë³€ìˆ˜ ì°¾ê¸° 
int main(void){
	init();

	while(1){
		printf("ìž…ë ¥í•˜ì„¸ìš” : "); 
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
		//í…ŒìŠ¤íŠ¸
		for(int i = 0 ;i < COMMAND_LENGTH; i++)
			command[i] = 0;
	}
	
}void sendError(int a){
	switch(a){
		case ERROR_TVAR_OVERFLOW : 
		printf("임시 변수가 다 꽉찻다");
		break;
		case ERROR_NUMBER_OVERFLOW:
		printf("최대 자리수를 벗어남");	
		break;
		case ERROR_FILE:
		printf("파일 관련 오류");
		break;
		case ERROR_VAR_OVERFLOW:
		printf("변수 가 다 꽉찿다");
		break;
		case ERROR_CAL:
		printf("계산 오류");
		break; 
		case ERROR_UNKNOWN:
		printf("존재하지 않는 변수");
		break; 
	}
	printf("\n");
}
int isAlpha(char c){
	if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
		return 1;
	return 0;
}
int isNumber(char c){
	if(c >= '0' && c <= '9' || c == '.')
		return 1;
	return 0;
}
int findVarFromSignal(char c){
	int var = -1;
	for(int i = 0; i < VAR_MAX; i++)
		if(signal[i] == c || signal[i] + 32 == c || signal[i] - 32 == c)	//대소문자 구별 X 귀찮아서 이렇게 
			var = TVAR_MAX + i;
	return var;
}
void interpreter(){
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
	int calHeader = -1;
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
			char whiteList[5] = {'+','-','*','/','%'};
			for(int i = 0 ; i < 5; i++)
				if(command[header] == whiteList[i])
					can = 1;
			if(can == 0){	//정해진 기호가 아닐경우
				printf("%c",command[header]);
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
	for(int i = 0 ; i <= calHeader;){
		int newNum;
		if(cal[i] == '*' + 1000)
			newNum = multiply(cal[i-1],cal[i+1]);
		else if(cal[i] == '/' + 1000)
			newNum = divide(cal[i-1],cal[i+1]);
		else if(cal[i] == '%' + 1000)
			newNum = rest(cal[i-1],cal[i+1]);
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
	printf(" = ");
	show(cal[0]);
	remover(cal[0]);
}
int add (int a, int b){	//µÑ´Ù ¾ç¼ö ±âÁØ 
	
	int newNum = getNew();
	for(int i = 1 ; i < CIPHER_MAX; i++)
		Num[newNum][i] = Num[a][i] + Num[b][i];

	for(int i = CIPHER_MAX - 1; i >= 1; i--)
		if(Num[newNum][i] >= 10){
			if(i == 1)
				sendError(ERROR_NUMBER_OVERFLOW);	//Á¤¼ö°¡ 50ÀÚ¸® º¸´Ù Å©´Ù 
			Num[newNum][i] -= 10;
			Num[newNum][i - 1]++;
		}
	
	return newNum; 
} 
//test
int subtract(int a,int b){	//µÑ´Ù ¾ç¼ö ±âÁØ
	int newNum = getNew();
	
	if(compare(a,b) == -1){
		Num[newNum][0] = 1;	// - ºÎÈ£
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
int multiply(int a,int b){	//°öÇÏ±â ¹Ý¿Ã¸² ¾ø´Â ¹öÁ¯ 
	int temp[CIPHER_MAX  * 2 + 100];	//±×³É Å©°Ô ¸¸µë 
	int tempLen = CIPHER_MAX * 2 + 100; 
	int newNum = getNew();	//»õ·Î¿î °ª ¾ò¾î¿À±â 
	Num[newNum][0] = (Num[a][0] + Num[b][0]) % 2;
	
	for(int i = 0 ; i  < tempLen; i++)
		temp[i] = 0;
	
	for(int i = CIPHER_MAX - 1; i >= 0; i--)
		for(int j = CIPHER_MAX - 1; j >= 0; j--)
			temp[tempLen - (CIPHER_MAX - i - 1) - (CIPHER_MAX - j - 1) - 1] += Num[a][i] * Num[b][j];	//ÀÏÀÏÀÌ ÇÏ³ªÇÏ³ª °öÇØ¼­´õÇÏ°í 
		
		
	for(int i = tempLen - 1; i >= 1; i--)	//¿Ã¸®±â 
		if(temp[i] >= 10 ){
			temp[i - 1] += temp[i] / 10;
			temp[i] %= 10;
		}
	
	if(temp[tempLen - DECIMAL] > 5){	//¹Ý¿Ã¸² 
		temp[tempLen - DECIMAL - 1] += 1;
		for(int i = tempLen - 1; i >= 1; i--)
		if(temp[i] >= 10 ){
			temp[i - 1] += temp[i] / 10;
			temp[i] %= 10;
		}
	}///¹Ý¿Ã¸² ¼Ò½º 
	int start = tempLen - DECIMAL- 1;	//Á¤´ä ³Ö±â 
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
	int temp[CIPHER_MAX  * 2 + 100];	//ÀÌ°Íµµ Å©°Ô 
	int tempLen = CIPHER_MAX * 2 + 100; 
	int ua = getNew();
	int ub = getNew();
	transition(ua,a);
	transition(ub,b);
	Num[newNum][0] = (Num[a][0] + Num[a][0]) % 2;
	int tempn = getNew();
	transition(tempn,ub);
	MultiTen(tempn);
	while(compare(a,tempn) >= 0){	// °Ô»ê ºü¸£°ÔÇÏ±âÀ§ÇØ¼­  
		++size;
		MultiTen(ub);
		MultiTen(tempn);
	}
		remover(tempn);	//tempn »èÁ¦ (ÀÌÁ¦ ¾µÇÊ¿ä¾ø¾î¼­) 
	while(size != -10){		//¸Ç³¡±îÁö °¡¸é Á¾·á 
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
int rest(int a, int b){
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
void load(){
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
void save(){
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
int minus(int a){
	Num[a][0] = (Num[a][0] + 1) % 2;
	return a;	
}
void show(int a){
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
void clear(){	//삭제후 등록 
	system("clear");
}
void init(){
	for(int i = 0 ; i < VAR_MAX + TVAR_MAX; i++)	// ë¶€í˜¸ ë¶€ë¶„ì´ -1ì´ë©´ ì—†ëŠ”ê²ƒ ì·¨ê¸‰ 
		Num[i][0] = -1;
		
	for(int i = 0; i < VAR_MAX; i++)
		signal[i] = 0;
	for(int i = 0 ; i< TOTAL_VAR; i++)
		for(int j = 1; j < CIPHER_MAX; j++)	// 0 ìœ¼ë¡œ ì „ë¶€ ëŒ€ìž… 
			Num[i][j] = 0;
			
	Num[TEN][CIPHER_MAX - DECIMAL - 2] = 1;	//10
	Num[ONE][CIPHER_MAX - DECIMAL - 1] = 1;	//1
	
}
int findVarFromSignal(char c){
	int var = -1;
	for(int i = 0; i < VAR_MAX; i++)
		if(signal[i] == c || signal[i] + 32 == c || signal[i] - 32 == c)	//ëŒ€ì†Œë¬¸ìž êµ¬ë³„ X ê·€ì°®ì•„ì„œ ì´ë ‡ê²Œ 
			var = TVAR_MAX + i;
	return var;
}
int getNew(){
	int newNum = -1;
	for(int i = 0 ; i < TVAR_MAX; i++)	//ë¹„ì–´ìžˆëŠ” ìž„ì‹œë³€ìˆ˜ ì°¾ê¸° 
		 if(Num[i][0] == -1){
		 	newNum = i;
		 	break;
		 }
	if(newNum == -1)	//ì—ëŸ¬ ë°œìƒ 
		sendError(ERROR_TVAR_OVERFLOW);
		
	for(int i = 0; i < CIPHER_MAX; i++)	// ì´ˆê¸°í™” 
		Num[newNum][i] = 0;
	return newNum;
}

int getVarNew(){
	int newNum = -1;
	for(int i = TVAR_MAX ; i < TVAR_MAX + VAR_MAX; i++)	//ë¹„ì–´ìžˆëŠ” ìž„ì‹œë³€ìˆ˜ ì°¾ê¸° 
		 if(Num[i][0] == -1){
		 	newNum = i;
		 	break;
		 }
	if(newNum == -1)	//ì—ëŸ¬ ë°œìƒ 
		sendError(ERROR_VAR_OVERFLOW);
		
	for(int i = 0; i < CIPHER_MAX; i++)	// ì´ˆê¸°í™” 
		Num[newNum][i] = 0;
	return newNum;
}

//í…ŒìŠ¤íŠ¸ìš© 
void show(int a){
	printf("%c",Num[a][0] ? '-' : '+');
	for(int i = 1; i <= CIPHER_MAX - DECIMAL - 1; i++)
	printf("%d",Num[a][i]);
	printf(".");
	for(int i = CIPHER_MAX - DECIMAL; i < CIPHER_MAX; i++)
	printf("%d",Num[a][i]);
	printf("\n");
}
int compare(int a,int b){	// 1 : (a ê°€ í¬ë‹¤) 0  : (ê°™ë‹¤) -1 : (bê°€ í¬ë‹¤)
	
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
