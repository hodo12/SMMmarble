//
//  main.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"



//board configuration parameters
static int board_nr;
static int food_nr;
static int festival_nr;

static int player_nr;


typedef struct player {
        int energy;
        int position;
        char name[MAX_CHARNAME];
        int accumCredit;
        int flag_graduate;
         int labaratory;
} player_t;

//static player_t *cur_player;
static player_t cur_player[MAX_PLAYER];
static smmObjGrade_e gradeList[9];
static int end = 0;

#if 0
static int player_energy[MAX_PLAYER];
static int player_position[MAX_PLAYER];
static char player_name[MAX_PLAYER][MAX_CHARNAME];
#endif

//function prototypes
#if 0
int isGraduated(void); //check if any player is graduated
 //print grade history of the player
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
void printPlayerStatus(void); //print all player status at the beginning of each turn
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName, int credit); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
void printGrades(int player); //print all the grade history of the player

#endif
double findAverageGrade(int player);
double findGradePoint(smmObjGrade_e gradeName);

char* findGradeName(smmObjGrade_e gradeName);
int totalCredit(int player);
void printEndPoint(int player);
int findBoardUse(int player, char* boardName);
void printGrades(int player)
{
     int i;
     void *gradePtr;
     printf("--> Printing player a's grade (average %.2lf) ::::\n", findAverageGrade(player));
     for (i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
     {
         gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
          printf("=> %s (credit:%i) : %s\n",smmObj_getBoardName(gradePtr) ,smmObj_getGradeCredit(gradePtr) , findGradeName(smmObj_getGradeData(gradePtr)));
     }
}
int findBoardUse(int player, char* boardName)
{
     int i;
     void *gradePtr;
     for (i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
     {
         gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
         if(strcmp(smmObj_getBoardName(gradePtr), boardName) == 0) {
         	return 1;
		 }
     }
     return 0;
}

void printEndPoint(int player)
{
     int i;
     void *gradePtr;
     for (i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
     {
         gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
          printf("=> 강의명 : %s, 성적 : %s ,학점: %.2lf\n",smmObj_getBoardName(gradePtr) ,findGradeName(smmObj_getGradeData(gradePtr)) , findGradePoint(smmObj_getGradeData(gradePtr)));
     }
}

int totalCredit(int player)
{
     int i;
     void *gradePtr;
     int totalCredit = 0;
     for (i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
     {
         gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
          totalCredit += smmObj_getGradeCredit(gradePtr);
     }
     return totalCredit;
}
double findAverageGrade(int player) {
	int i;
     void *gradePtr;
     double totalGrade;
     int length = smmdb_len(LISTNO_OFFSET_GRADE + player);
     for (i=0;i<length;i++)
     {
         gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
         totalGrade+=findGradePoint(smmObj_getGradeData(gradePtr));
     }
     return (totalGrade)/(length);
}
void gen_grade()
{
	gradeList[0] = smmObjGrade_Ap;
	gradeList[1] = smmObjGrade_A0;
	gradeList[2] = smmObjGrade_Am;
	gradeList[3] = smmObjGrade_Bp;
	gradeList[4] = smmObjGrade_B0;
	gradeList[5] = smmObjGrade_Bm;
	gradeList[6] = smmObjGrade_Cp;
	gradeList[7] = smmObjGrade_C0;
	gradeList[8] = smmObjGrade_Cm;
	
}

void printPlayerStatus(void)
{
     int i;
     printf("\n\n========================== PLAYER STATUS ==========================\n");
     for (i=0;i<player_nr;i++)
     {
         //printf("%s : credit %i, energy %i, position %i\n", 
                      //cur_player[i].name,
                      //cur_player[i].accumCredit,
                      //cur_player[i].energy,
                      //cur_player[i].position);
            
        	printf("%s at %i.%s, credit: %i, energy: %i\n",
			cur_player[i].name,
                      cur_player[i].position,
                      smmdb_getData(LISTNO_NODE, returnPosition(i)),
					  cur_player[i].accumCredit,
					  cur_player[i].energy);
			
		}
        printf("========================== PLAYER STATUS ==========================\n\n\n");
     }

void generatePlayers(int n, int initEnergy) //generate a new player
{
     int i;
     //n time loop
     for (i=0;i<n;i++)
     {
         //input name
         printf("Input player %i's name:", i); //
         scanf("%s", cur_player[i].name);
         fflush(stdin);
         
         //set position
         //player_position[i] = 0;
         cur_player[i].position = 0;
         
         //set energy
         //player_energy[i] = initEnergy;
         cur_player[i].energy = initEnergy;
         cur_player[i].accumCredit = 0;
         cur_player[i].flag_graduate = 0;
         cur_player[i].labaratory = 0;
     }
}


int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    
#if 1
    if (c == 'g')
        printGrades(player);
#endif
    
    return (rand()%MAX_DIE + 1);
}

char* findGradeName(smmObjGrade_e gradeName) {
	switch(gradeName) {
		case smmObjGrade_Ap : return "A+";
		case smmObjGrade_A0 : return "A0";
		case smmObjGrade_Am : return "A-";
		case smmObjGrade_Bp : return "B+";
		case smmObjGrade_B0 : return "B0";
		case smmObjGrade_Bm : return "B-";
		case smmObjGrade_Cp : return "C+";
		case smmObjGrade_C0 : return "C0";
		case smmObjGrade_Cm : return "C-";
	}
}
double findGradePoint(smmObjGrade_e gradeName) {
	switch(gradeName) {
		case smmObjGrade_Ap : return 4.3;
		case smmObjGrade_A0 : return 4.0;
		case smmObjGrade_Am : return 3.7;
		case smmObjGrade_Bp : return 3.4;
		case smmObjGrade_B0 : return 3.0;
		case smmObjGrade_Bm : return 2.7;
		case smmObjGrade_Cp : return 2.4;
		case smmObjGrade_C0 : return 2.0;
		case smmObjGrade_Cm : return 1.7;
	}
}

int returnPosition(int player) {
      if(cur_player[player].position - 1 <= 0) {
      	return cur_player[player].position;
	  } else {
	  	return cur_player[player].position - 1;
	  }
					  
}
//action code when a player stays at a node
void actionNode(int player)
{
	int position = returnPosition(player);
    void *boardPtr = smmdb_getData(LISTNO_NODE, position);
    int type = smmObj_getNodeType( boardPtr );
    char *name = smmObj_getName( boardPtr );
    void *gradePtr;
    char a[4];
    char c;
    
    
    switch(type)
    {
        //case lecture:
        case SMMNODE_TYPE_LECTURE:
            
            
            //grade generation
            //gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getCredit( boardPtr ), 0, smmObjGrade_Ap);
            //smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
        	
			
			while(1){
				printf(" Lecture %s (credit:%i, energy:%i) starts! are you going to join? or drop? :",
					smmObj_getName(smmdb_getData(LISTNO_NODE, position)),
					 smmObj_getCredit(smmdb_getData(LISTNO_NODE, position)),
					 smmObj_getEnergy(smmdb_getData(LISTNO_NODE, position))//
				);
				scanf("%s", &a);
				fflush(stdin);
				if(strcmp(a, "join") == 0){
					
					if(cur_player[player].energy < smmObj_getEnergy( boardPtr )) {
						//에너지 비교
						 printf("-> %s is too hungry to take the lecture %s (remained:%i, required:%i)",cur_player[player].name, 
						 smmObj_getName(smmdb_getData(LISTNO_NODE, position)),cur_player[player].energy, smmObj_getEnergy(smmdb_getData(LISTNO_NODE, position)));
						break;
					}
					if(findBoardUse(player, smmObj_getName(smmdb_getData(LISTNO_NODE, position))) == 1) {
						//이미 들었던 항 목
						 printf("-> %s has already taken the lecture %s!!",cur_player[player].name ,smmObj_getName(smmdb_getData(LISTNO_NODE, position)));
						break;
					}
					cur_player[player].accumCredit += smmObj_getCredit( boardPtr );
            		cur_player[player].energy -= smmObj_getEnergy( boardPtr );
					smmObjGrade_e gradeName = gradeList[rand()%9];
					void* gradeObj = smmObj_genGrade(smmObj_getName(smmdb_getData(LISTNO_NODE, position)) ,gradeName, smmObj_getCredit(smmdb_getData(LISTNO_NODE, position)));
					smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradeObj);
					printf(" ->%s successfully takes the lecture %s with grade %s (average : %.2lf), (remained energy : %i)\n\n\n",
					cur_player[player].name,
						smmdb_getData(LISTNO_NODE, position),
						findGradeName(gradeName), 
						findAverageGrade(player),
						cur_player[player].energy
					);
					break;
				} else if(strcmp(a, "drop") == 0) {
					break;
				}	
				printf("-> invalid input! input \"drop\" or \"join\"!\n");
			}
            break;
            
	    case SMMNODE_TYPE_FOODCHANCE: //음식을 고르는 경 우
		 		printf("-> %s gets a food chance! press any key to pick a food card: ",cur_player[player].name);
		 		
			    c = getchar();
			    fflush(stdin);
			    int randomFoodIndex = rand()%14;
			    void *foodObj = smmdb_getData(LISTNO_FOODCARD, randomFoodIndex);
		 		printf("-> %s picks %s and charges %i (remained energy : %i)\n",
				 cur_player[player].name, smmdb_getData(LISTNO_FOODCARD, randomFoodIndex),smmObj_getfoodenergy(foodObj)
				 ,cur_player[player].energy + smmObj_getfoodenergy(foodObj) );
				 cur_player[player].energy += smmObj_getfoodenergy(foodObj);
		 		
	            break;
	    case SMMNODE_TYPE_RESTAURANT:
		 	
		 		
	            break;
	    case SMMNODE_TYPE_FESTIVAL:  //축제의 경우  
		 		printf("%s participates to Snow Festival! press any key to pick a festival card: ",cur_player[player].name); //이름이 이상하게 나옴 
		 		
			    scanf(" %s", &a);
			    fflush(stdin);
		 		printf("  -> %s participates to Snow Festival! press any key to pick a festival card:", a);
			  	c = getchar();
			    fflush(stdin);
			    int randomFestivalIndex = rand()%5;
			    void *festival = smmdb_getData(LISTNO_FESTCARD, randomFestivalIndex);
		 		printf("  -> MISSION : %s. !!\n",smmObj_getfestivalname(festival) );
   				printf("  (Press any key when mission is ended.)");
   				c = getchar();
			    fflush(stdin);
		 		
	            break;
	   	case SMMNODE_TYPE_GOTOLAB: //실험실로 이동  
		   	printf("-> OMG! This is experiment time!! Player a goes to the lab.\n");
		    	cur_player[player].labaratory = 1;
	   		break;
		case SMMNODE_TYPE_LABORATORY: //실험중  
			if(cur_player[player].labaratory == 1) {
				int temp = 4;
				printf("\n");
				printf("-> Experiment time! Let's see if you can satisfy professor (threshold: %i)\n", temp);
				int dice = rolldie(player);
				if(dice >= temp) {
					cur_player[player].labaratory = 0;
					
					printf("-> Experiment result : 6, success! a can exit this lab!", dice);
				} else {
					printf("-> Experiment result : %i, fail T_T. a needs more experiment......", dice);
				}
			}
	    	break;
	
        default:
            break;
    }
}

/*
16까지 돌아야함
16 넘으면 0으로 바꾸고 남은 횟수 반복
남은 횟수는 step - 움직인 횟수
합계 16 이하면 그냥 출력 
*/

void goForward(int player, int step)
{
	//int i; 
     void *boardPtr;
     int i;
     printf("--> result : %i \n", step);
     int fullposition =  cur_player[player].position;
     cur_player[player].position +=step;
     
     int index = 0;
     if(cur_player[player].position > 16) {
     	for (i = fullposition;i< 16;i++) {
     	boardPtr = smmdb_getData(LISTNO_NODE, i);
	 	printf("=> jumpe to %s \n",smmObj_getName(boardPtr));//지나간 곳이랑 도착한 곳도 나와야 함-if문 사용?
		
		}     
	
		for (i = 0;i< step - (16 - fullposition);i++) {
	     	boardPtr = smmdb_getData(LISTNO_NODE, i);
		 	printf("=> jumpe to %s \n",smmObj_getName(boardPtr));
		 		if(i == 0) {
	 		cur_player[player].energy += 18;
	 		printf("-> returned to HOME! energy charged by 18 (total : %i) \n", cur_player[player].energy);
	 		if(totalCredit(player) >= 30) {
	 			
	 			end = 1;
	 			break;
			 }
			} 
		}     
		cur_player[player].position =step - (16 - fullposition);
	 } else {
	 	for (i = cur_player[player].position - step;i< cur_player[player].position;i++) {
     	boardPtr = smmdb_getData(LISTNO_NODE, i);
	 	printf("=> jumpe to %s \n",smmObj_getName(boardPtr));
	
	}          
	 }
                                                                                                
	
                //cur_player[player].name, cur_player[player].position,smmObj_getName(boardPtr));
}

int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME]; 
	char foodname[MAX_CHARNAME];
	char festivalname[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    int i;
    int initEnergy;
    int turn=0;
    int foodenergy;
   
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while ( fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4 ) //read a node parameter set
    {
        //store the parameter set
        //(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj);
        if (type == SMMNODE_TYPE_HOME)
           initEnergy = energy;
        board_nr++;
    }
    fclose(fp);
    
    
    
    for (i = 0;i<board_nr;i++)
    {
        void *boardObj = smmdb_getData(LISTNO_NODE, i);
        
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
                     i, smmObj_getName(boardObj), 
                     smmObj_getNodeType(boardObj), smmObj_getNodeTypeName(smmObj_getNodeType(boardObj)),
                     smmObj_getCredit(boardObj), smmObj_getEnergy(boardObj));
    }
    //printf("(%s)", smmObj_getTypeName(SMMNODE_TYPE_LECTURE));
    printf("Total number of board nodes : %i\n", board_nr);
    
    
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    
    
    
    while ( fscanf(fp, "%s %i", foodname, &foodenergy) ==2 ) //read a node parameter set
    {
        //store the parameter set
        //(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
       //뭔가 초기화나 처음 실행할때 해야할 것 
    
        //void *foodObj = smmObj_genObject(foodname, smmObj_getfoodname, smmObj_getfoodenergy, 0);
        void *foodObj = smmObj_genfood(foodname, foodenergy);
    
        smmdb_addTail(LISTNO_FOODCARD, foodObj);
          
        food_nr++;
    }
    
     
    
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);
    for (i = 0;i< food_nr;i++)//음식 코드 출력 
    {
        void *foodObj = smmdb_getData(LISTNO_FOODCARD, i);
        
        printf("node %i : %s, charge : %i\n", 
                     i, smmObj_getfoodname(foodObj), 
                     smmObj_getfoodenergy(foodObj));
    }
    
    
    
    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n");
    
    
    
    while ( fscanf(fp, "%s", festivalname) == 1 ) //read a node parameter set
    {
        //store the parameter set
        //(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
        //뭔가 초기화나 처음 실행할때 해야할 것 
        void *festival = smmObj_genfestival(festivalname);
    
        smmdb_addTail(LISTNO_FESTCARD, festival);
        festival_nr++;
    }
    
    
    
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr);
    for (i = 0;i< festival_nr;i++)//축제  코드 출력 
    {
        void *festival = smmdb_getData(LISTNO_FESTCARD, i);
        
        printf("node %i : %s\n", 
                     i, smmObj_getfestivalname(festival));
    }
    gen_grade();
    printf("\n\n\n\n\n=======================================================================\n");
    printf("-----------------------------------------------------------------------\n");
    printf("       Sookmyung Marble !! Let's Graduate (total credit : 30)!!\n");
    printf("-----------------------------------------------------------------------\n");
	printf("=======================================================================\n\n\n");

    //2. Player configuration ---------------------------------------------------------------------------------
    
    do
    {
        //input player number to player_nr
        printf("input player no.:");
        scanf("%d", &player_nr);
        fflush(stdin);
    }
    while (player_nr < 0 || player_nr >  MAX_PLAYER);
    
    //cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
    	 generatePlayers(player_nr, initEnergy);

    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while (1) //is anybody graduated?
    {
        int die_result;
        
        
        //4-1. initial printing
        printPlayerStatus();
    	printf("This is %s's turn ::::", cur_player[turn].name);
        //4-2. die rolling (if not in experiment)        
        
        //4-3. go forward
        //type이 실험실이면 goForward건너뜀 
        if(smmObj_getNodeType( smmdb_getData(LISTNO_NODE, returnPosition(turn))) != SMMNODE_TYPE_LABORATORY || cur_player[turn].labaratory != 1) {
        	 die_result = rolldie(turn);
			 goForward(turn, die_result);
			 if(end == 1) {
			 	
			 	printEndPoint(turn);
			 	exit(0);
			 }
		}
		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        
        //4-5. next turn
        turn = (turn + 1)%player_nr;
    }
    
    
    free(cur_player);
    system("PAUSE");
    return 0;
}

