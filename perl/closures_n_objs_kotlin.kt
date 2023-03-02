// accumulator objects in Kotlin  (Java + extra creme)

open class accumulator(dollar_0:Int)  // just to be funny
{
   private var x = dollar_0;  // instance var with type inference
   private val x0 = dollar_0;  // val means immutable

   public open fun inc(y:Int): Int
   {
      x += y
      return x  // no need for ; but need return
   }

   // but write mult directly as a lambda term
   public var mult = {y:Int -> x*=y; x};

   public fun reset() { x=x0;}
   public fun transfer(target:accumulator)
   {
      target.x += this.x; // direct access to private
      this.x = 0;
   }
}//class
// what's the most important difference between this java-ish program
// and the Perl closure?  "public"

fun main() //av:Array<String>) // ha! it's still java beneath
{
  var ac1 = accumulator(20)
  var ac2 = accumulator(200)
  println( ac1.inc(5) )
  println("ac1 after mult(10): "+ ac1.mult(10))
  ac1.transfer(ac2)
  ac1.reset()
  println("ac2 after transfer: "+ ac2.inc(0))
  println(ac1.inc(0))
}//main

// kotlinc accumulator.kt -include-runtime -d accum.jar
// java -jar accum.jar
