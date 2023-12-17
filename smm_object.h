//
//  smm_object.h
//  SMMarble object
//
//  Created by Juyeop Kim on 2023/11/05.
//

#ifndef smm_object_h
#define smm_object_h

#define SMMNODE_TYPE_LECTURE            0
#define SMMNODE_TYPE_RESTAURANT         1
#define SMMNODE_TYPE_LABORATORY         2
#define SMMNODE_TYPE_HOME               3
#define SMMNODE_TYPE_GOTOLAB            4
#define SMMNODE_TYPE_FOODCHANCE         5
#define SMMNODE_TYPE_FESTIVAL           6

#define SMMNODE_TYPE_MAX                7
typedef enum smmObjType {
	smmObjType_board = 0, 
	smmObjType_card,
	smmObjType_grade
	}smmObjType_e;

typedef enum smmObjGrade{
	
	smmObjGrade_Ap,
	smmObjGrade_A0,
	smmObjGrade_Am,
	smmObjGrade_Bp,
	smmObjGrade_B0,
	smmObjGrade_Bm,
	smmObjGrade_Cp,
	smmObjGrade_C0,
	smmObjGrade_Cm
}smmObjGrade_e;
/*
typedef enum smmObjGrade{
	
	
}smmObjGrade_e;
	smmObjGrade_Ap = 4.3,
	smmObjGrade_A0 = 4.0, 
	smmObjGrade_Am = 3.7,
	smmObjGrade_Bp = 3.4, 
	smmObjGrade_B0 = 3.0,
	smmObjGrade_Bm = 2.7,
	smmObjGrade_Cp = 2.4, 
	smmObjGrade_C0 = 2.0,
	smmObjGrade_Cm = 1.7
    A+,
    A0,
    A-,
    B+,
    B0,
    B-,
    C+,
    C0,
    C-
*/


//object generation
void* smmObj_genObject(char* name,smmObjType_e objType, int type, int credit, int energy,smmObjGrade_e grade);
void* smmObj_genfood( char* foodname, int foodenergy );
void* smmObj_genGrade(char* name,smmObjGrade_e grade, int credit);
void* smmObj_genfestival( char* festivalname);
//member retrieving
char* smmObj_getName(void* node_nr);
int smmObj_getNodeType(void* node_nr);
int smmObj_getCredit(void* node_nr);
int smmObj_getEnergy(void* node_nr);
char* smmObj_getBoardName(void* obj);
int smmObj_getGradeCredit(void* obj);
smmObjGrade_e smmObj_getGradeData(void* obj);
 
//element to string
char* smmObj_getNodeTypeName(int type);
char* smmObj_getNodeTypefoodname(int type);

#endif /* smm_object_h */
