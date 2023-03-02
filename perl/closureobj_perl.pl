## what can we do with closures in perl?
## we can create OBJECTS (but not enough to have object oriented programming)

sub makeaccount
{  my $balance = $_[0];  # initial balance set by parameter

   my $deposit = sub { $balance += $_[0]; $balance; };

   my $withdraw = sub { if ($_[0]<=$balance) 
			     { $balance-=$_[0]; $balance; } 
                        else { print "insufficient funds\n"; }
                      };

   my $inquiry = sub { $balance };

   my $dispatch;  # use hash table as an interface

   $dispatch->{"withdraw"} = $withdraw;
   $dispatch->{"deposit"} = $deposit;   # strings are ok too
   $dispatch->{"inquiry"} = $inquiry;

   $dispatch;  # makeaccount returns dispatching hash table
}

my $ac1 = makeaccount(100);
my $ac2 = makeaccount(500);
$ac1->{"withdraw"}->(50);  
$ac2->{"deposit"}->(100);
print $ac1->{"inquiry"}->();
