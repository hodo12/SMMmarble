//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>

#define MAX_NODETYPE    7
#define MAX_GRADE       9
#define MAX_NODE        100


static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
      "����",
	  "�Ĵ�",
	  "�����",
	  "��",
	  "����Ƿ��̵�",
	  "��������",
	  "�����ð�" 
};

static char smmNodefoodname[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
      "ġŲ"
	  "����" ,
	  "���ķ�",
	  "�Ŷ��",
	  "������",
	  "�Ź�",
	  "¥���",
	  "�н�",
	  "�Ľ�Ÿ",
	  "����",
	  "�ܹ���",
	  "������ġ",
	  "���Ʈ",
	  "�����"
};

char* smmObj_getNodeTypeName(int type)
{
      return (char*)smmNodeName[type];
}

char* smmObj_getNodeTypefoodname(int type)
{
      return (char*)smmNodefoodname[type];
}

//1. ����ü ���� ���� 
typedef struct smmObject {
       char name[MAX_CHARNAME];
       smmObjType_e objType; 
       int type;
       int credit;
       int energy;
       smmObjGrade_e grade;
       
} smmObject_t;
typedef struct smmGrade {
	char name[MAX_CHARNAME];
      smmObjGrade_e grade;
      int credit;
       
} smmGrade_t;

typedef struct smmfood {
	char foodname[MAX_CHARNAME]; 
	int foodenergy;
	
	}smmfood_t;
	
typedef struct smmfestival {
	char festivalname[MAX_CHARNAME]; 
	
	}smmfestival_t;

//static smmObject_t smm_node[MAX_NODE];
//static int smmObj_noNode = 0;

//3. ���� �Լ� ���� 
//object generation

void* smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
{    
    smmObject_t* ptr;
    
    ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
    
    strcpy(ptr->name, name);
    ptr->objType = objType;
    ptr->type = type;
    ptr->credit = credit;
    ptr->energy = energy;
    ptr->grade = grade;

    return ptr;
}
void* smmObj_genGrade(char* name,smmObjGrade_e grade, int credit) {
	smmGrade_t* ptr;
	 ptr = (smmGrade_t*)malloc(sizeof(smmGrade_t));
    strcpy(ptr->name, name);
    ptr->grade = grade;
    ptr->credit = credit;

    return ptr;
}

//#if 0
void* smmObj_genfood( char* foodname, int foodenergy )
{    
    smmfood_t* ptr;
    
    ptr = (smmfood_t*)malloc(sizeof(smmfood_t));
    strcpy(ptr->foodname, foodname);
    ptr->foodenergy = foodenergy;

    return ptr;
}

void* smmObj_genfestival( char* festivalname)
{    
    smmfestival_t* ptr;
   
    ptr = (smmfestival_t*)malloc(sizeof(smmfestival_t));
   
    strcpy(ptr->festivalname, festivalname);

    return ptr;
}
//#endif
	

//3. ���� �Լ� ����
char* smmObj_getName(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->name;
}
char* smmObj_getBoardName(void* obj) {
	  smmGrade_t* ptr = (smmGrade_t*)obj;
    
    return ptr->name;
}
int smmObj_getGradeCredit(void* obj) {
	  smmGrade_t* ptr = (smmGrade_t*)obj;
    
    return ptr->credit;
}
smmObjGrade_e smmObj_getGradeData(void* obj) {
	  smmGrade_t* ptr = (smmGrade_t*)obj;
    
    return ptr->grade;
}
//3. ���� �Լ� ����
int smmObj_getNodeType(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->type;
}

int smmObj_getCredit(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->credit;
}

int smmObj_getEnergy(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->energy;
}

smmObjGrade_e smmObj_getGrade(void* obj)
{
    smmObject_t* ptr = (smmObject_t*)obj;
    
    return ptr->grade;
}

char* smmObj_getfoodname(void* obj)
{
    smmfood_t* ptr = (smmfood_t*)obj;
    
    return ptr->foodname;
}

char* smmObj_getfoodenergy(void* obj)
{
    smmfood_t* ptr = (smmfood_t*)obj;
    
    return ptr->foodenergy;
}

char* smmObj_getfestivalname(void* obj)
{
    smmfestival_t* ptr = (smmfestival_t*)obj;
    
    return ptr->festivalname;
}
