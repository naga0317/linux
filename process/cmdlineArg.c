#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<unistd.h>

struct std
{
  char name[100];
  int age;
  int salary;
};	

void func(struct std);

int main(int argv, char* argc[])
{
  struct std st;

  strcpy(st.name, argc[1]);
  printf("after strcpy");
  st.age = atoi(argc[2]);
  st.salary = atoi(argc[3]);
  func(st);

  return 0;
}	

void func(struct std a)
{
  printf("name =%s,age=%d,salary=%d\n", a.name, a.age, a.salary);
}	
