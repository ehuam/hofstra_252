# Sample program: Closures in Perl
# Perl, which has a garbage collector, belongs to a family languages
# that allows you to form closures (Python, Ruby, C# 4.0, but not C,Java)

# A closure is a function with an local environment attached.

# function that returns an accumulator function WITH STATE

sub newaccumulator
{
    my $x = 0; # initial value of accumulator
    sub
    {  $x += $_[0]; $x }  # returns pointer to this inlined subroutine 
}


my $accum1 = newaccumulator();
print $accum1->(3), "\n";
print $accum1->(3), "\n";
my $accum2 = newaccumulator();
print $accum2->(2), "\n";
print $accum1->(2), "\n";

# each accumulator is an "instance" of newaccumulator that each carries STATE

## note that a garbarge collector has to be smart enough to not "pop off"
## the my $x after newaccumulator returns!  $x must be copied somewhere, and
## must be distinguished from other "instances" of $x.

### don't change "my $x " to "local $x"!

#### closures typically used as call-back functions in event-driven programs
