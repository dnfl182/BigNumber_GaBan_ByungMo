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

void clear(){
	printf("ì§€ì›Œë³‘ ë‚˜ì¤‘ì— êµ¬í˜„\n"); 
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
///í…ŒìŠ¤íŠ¸ìš© 
int input(){ 
	char in[100];
	int toPoint = -1;	//ì •ìˆ˜ë¶€ ìˆ«ìž ê°¯ìˆ˜
	int newNum = getNew();
	int isMinus = 0;
	gets(in);
	
	if(in[0] == '-'){
		Num[newNum][0] = 1;
		isMinus = 1;
	}
	
	while(in[++toPoint]!='.' && in[toPoint] != 0);	// ì •ìˆ˜ë¶€ë¶„ ê°¯ìˆ˜ ì°¾ê¸° (ë§ˆì´ë„ˆìŠ¤ ë¶€í˜¸ í¬í•¨) 
	
	
	for(int i = CIPHER_MAX - DECIMAL - toPoint +  isMinus , j = isMinus; j < toPoint; i++ , j++)
		Num[newNum][i] = in[j] -'0';	//ë¬¸ìžì—´ì´ë¯€ë¡œ 
	for(int i = CIPHER_MAX - DECIMAL , j = toPoint + 1; j < strlen(in); i++ , j++){
		Num[newNum][i] = in[j] -'0';
	}
	return newNum;
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
void showVAR(){
	for(int i = 0; i < VAR_MAX ; i++)
		if(Num[TVAR_MAX + i][0] != -1){
			printf("%c = ",signal[i]);
			show(TVAR_MAX + i);
		}
}
