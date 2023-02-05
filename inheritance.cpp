using namespace std;   
#include<iostream>     

class AA                //// WARNING: THIS PROGRAM CONTAINS SCENES OF HORROR
{                       //// THAT MAY NOT BE SUITABLE FOR MINORS
private:
   int x;
public:
   AA(int x) {this->x = x;}

   void f() {cout << "AA.f\n";}
   virtual void g() {cout << "AA.g\n";}
};

class BB : public AA  // "public" needed for "is a" relationship with AA
{  private:
     double y;
   public:
    BB(int x0) : AA(x0) { y = 2.5;}
    void f() {cout << "BB.f\n";}   
    void g() {cout << "BB.g\n";}
    void h() {cout << "BB.h, y==" << y << endl;} 
}; //BB

int main()
{
  AA n = BB(1);  // n is not a pointer
  n.g(); // no dynamic dispatch!  but note that the program compiled.

  AA *x = new BB(1);  // object allocated on heap
  x->f();  // still prints AA.f
  x->g();  // now get dynamic dispatch because of "virtual"
  
  x = new AA(1);   // change x to an AA object
  ((BB*)x)->h();   // RUN FOR YOUR LIFES!
  return 0;
}

