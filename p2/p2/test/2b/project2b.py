import toolspath
from testing import Xv6Build, Xv6Test

class Getpinfo(Xv6Test):
   name = "getpinfo"
   description = "compile and run a user program with a getpinfo call"
   tester = name + ".c"
   timeout = 30

class Getpinfo2(Xv6Test):
   name = "getpinfo2"
   description = "check that getpinfo returns some reasonable values"
   tester = name + ".c"
   timeout = 30

class Usertests(Xv6Test):
   name = "usertests"
   description = "run usertests to check for regressions"
   tester = name + ".c"
   timeout = 500

all_tests = [Getpinfo, Getpinfo2]

import toolspath
from testing.runtests import main
main(Xv6Build, all_tests)
