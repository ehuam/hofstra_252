#include<stdio.h>
#include<stdlib.h>

#define Nil 0
#define bool int

struct cell
{
  int item;
  struct cell* next;
};

typedef struct cell* list;

// constructor
list cons(int i, list n)
{
  list newcell = (list)malloc(sizeof(struct cell));
  newcell->item = i;
  newcell->next = n;
  return newcell;
}
// accessors
int car(list m) { return m->item; }
list cdr(list m) { return m->next; }
bool null(list m) { return m==Nil; }

int prod(list m) { if (null(m)) return 1; else return car(m)*prod(cdr(m)); }
int product(list m)
{
  int prodi(list m, int ax)
  { if (null(m)) return ax; else return prodi(cdr(m),ax*car(m)); }
  return prodi(m,1);
}

list reverse(list m, list stack)    // stack must be initially Nil
{ if (null(m)) return stack; else return reverse(cdr(m),cons(car(m),stack)); }


int last(list m)
{
  if (null(cdr(m))) return car(m); else return last(cdr(m));
}

void printlist(list m)
{
  for(;m!=Nil;m=cdr(m)) printf("%d ",car(m));
  printf("\n");			  
}

typedef bool (*intpred)(int);   // defines integer predicate type

bool exists(intpred p, list m)
{
  return !null(m) && ( p(car(m)) || exists(p,cdr(m)) );
}
bool forall(intpred p, list m)
{
  bool notp(int x) { return !p(x); }
  return !exists(notp,m);
}

void dealloc(list m)
{
  if (!null(m)) { dealloc(cdr(m)); free(m); }
}

int main()
{
  list m = cons(2,cons(3,cons(5,cons(7,cons(11,Nil)))));
  printlist(m);
  printf("last : %d\n",last(m));
  printf("product of m is %d\n",product(m));

  bool even(int x) { return x%2==0; }
  bool odd(int x) { return x%2==1; }
  
  printf("exists even %d\n", exists(even,m));
  printf("cdr all odd %d\n", forall(odd,cdr(m)));

  list B = cons(1,cdr(m));   // what is B?
  
  list temp = m;
  m = reverse(m,Nil);
  dealloc(temp);
  printlist(m);

  printlist(B);   // what is B now?

                  // what's the consequence of using a language without garbage collection?
                  // ... maybe can't have real abstract data types.  Must think at level of
                  // pointers and memory allocation; unavoidable low-level programming.
  return 0;
}




