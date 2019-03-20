

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


#define MAX_STRING_LEN 32

typedef struct student
{
		char m_name[MAX_STRING_LEN];
		char m_sex;
		int m_age;
		struct list_head m_list;  /*把我们的学生对象组织成双向链表，就靠该节点了*/
}Student;



static int dbg_flg = 0;

LIST_HEAD(g_student_list);

static int add_stu(char* name,char sex,int age)
{
		Student *stu,*cur_stu;

		list_for_each_entry(cur_stu,&g_student_list,m_list){ //仅遍历是否有同名学生，所以用该接口
			if(0 == strcmp(cur_stu->m_name,name))
			{
						printf("Error:the name confict!\n");
						return -1;
			}
		}

		stu = malloc(sizeof(Student));
		if(!stu)
		{
				printf("malloc mem error!\n");
				return -1;
		}

		memset(stu,0,sizeof(Student));
		strncpy(stu->m_name,name,strlen(name));
		stu->m_sex = sex;
		stu->m_age = age;
		INIT_LIST_HEAD(&stu->m_list);

		if(dbg_flg)
				printf("(Add)name:[%s],\tsex:[%c],\tage:[%d]\n",stu->m_name,stu->m_sex,stu->m_age);

		list_add_tail(&stu->m_list,&g_student_list); //将新学生插入到链表尾部，很简单吧

		return 0;
}


static int del_stu(char *name)
{
		Student *cur,*next;
		int ret = -1;
		list_for_each_entry_safe(cur,next,&g_student_list,m_list){  //因为要删除链表的节点，所以必须有带有“safe”的宏接口
				if(0 == strcmp(name,cur->m_name))
				{
						list_del(&cur->m_list);
						printf("(Del)name:[%s],\tsex:[%c],\tage:[%d]\n",cur->m_name,\
										cur->m_sex,cur->m_age);
						free(cur);
						cur = NULL;
						ret = 0;
						break;
				}
		}
		return ret;
}

static void dump_students(void)
{
		Student *stu;
		int i = 1;
		printf("===================Student List================\n");
		list_for_each_entry(stu,&g_student_list,m_list){  //同样，也仅遍历链表而已
				printf("(%d)name:[%s],\tsex:[%c],\tage:[%d]\n",i++,stu->m_name,\
								stu->m_sex,stu->m_age);
		}
		printf("===============================================\n");
}

static void init_system(void)
{
		/*初始化时，向链表g_student_list里添加6个节点*/
		add_stu("Tom",'m',18);
		add_stu("Jerry",'f',17);
		add_stu("Alex",'m',18);
		add_stu("Conory",'f',18);
		add_stu("Frank",'m',17);
		add_stu("Marry",'f',17);
}

/*因为没有数据库，所以当我们的模块退出时，需要释放内存*/
static void clean_up(void)
{
		Student *stu,*next;
		list_for_each_entry_safe(stu,next,&g_student_list,m_list){
				list_del(&stu->m_list);
				printf("Destroy [%s]\n",stu->m_name);
				free(stu);
		}
}

/*模块初始化接口*/
static int student_mgt_init(void)
{
		printf("Student Managment System,Initializing...\n");

		init_system();
		dbg_flg = 1;   //从此以后，再调用add_stu()时，都会有有内核打印信息，详见实例训练
		dump_students();

		return 0;
}

static void student_mgt_exit(void)
{
		clean_up();
		printf("System Terminated!\n");
}

int main(void)
{
	student_mgt_init();

	del_stu("Tom");
	dump_students();

	del_stu("Frank");
	dump_students();

	student_mgt_exit();
	return 0;
}

