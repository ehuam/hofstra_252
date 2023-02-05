using System;

////////////// Inheritance Fundamentals in C# ///////////////////
//  static and dynamic type information, how they're used.
//   different kind of dispatch, when exactly does dynamic dispatch occur
//   virtual-override versus new
//   variable scope stays static, dynamic dispatch only on virtual methods.
//   difference between dynamic dispatch and overloading
//   type casting; compiler versus runtime errors
//  how does C# differ from Java and from C++?

class AA
{
   private int x;
   public AA(int x) {this.x = x;}

   public void f() {Console.WriteLine("AA.f");}
   public virtual void g() {Console.WriteLine("AA.g, x=="+x);}
}//BB

class BB : AA   // BB is a subclass of AA
{
   public int x;  // how will this x coexist with x in AA?
   public BB(int x0, int x1) : base(x0)
   {   x = x1;    }

   public new void f() {Console.WriteLine("BB.f");}   // new
   public override void g() {Console.WriteLine("BB.g, my x=="+x);} //override
   public void h() {Console.WriteLine("BB.h");}  // completely new
}//BB

////////
public class inheritance
{
  static void q(BB x) {Console.WriteLine("q(BB)");}
  static void q(AA x) {Console.WriteLine("q(AA)");}  // overloading

  public static void Main()
  {
    AA m = new BB(1,2);
    // if (...) m = new BB(1,2); else m = new AA(1);
    m.f();  //  AA.f   // static dispatch
    m.g();  //  BB.g,x==2  // dynamic dispatch
  
    //    m = new AA(1);
    ((BB)m).h();  

    q(m); // which q will be called?  // no dynamic dispatch here!

  }//Main
}
