import toolspath
from testing import Xv6Build, Xv6Test

class Basic(Xv6Test):
   name = "basic"
   description = "call getsyscallnumtotal() and getsyscallnumsuccess() from a user program"
   tester = "basic.c"
   make_qemu_args = "CPUS=1"

class Getsyscallnumtotal(Xv6Test):
   name = "getsyscallnumtotal"
   description = "repeated calls of getsyscallnumtotal()"
   tester = "getsyscallnumtotal.c"
   make_qemu_args = "CPUS=1"

class Getsyscallnumsuccess(Xv6Test):
   name = "getsyscallnumsuccess"
   description = "repeated calls of getsyscallnumsuccess()"
   tester = "getsyscallnumsuccess.c"
   make_qemu_args = "CPUS=1"

class Many_getsyscallnumtotal(Xv6Test):
   name = "many_getsyscallnumtotal"
   description = "count calls to many different syscalls"
   tester = "many_getsyscallnumtotal.c"
   make_qemu_args = "CPUS=1"

class Many_getsyscallnumsuccess(Xv6Test):
   name = "many_getsyscallnumsuccess"
   description = "count calls to many different syscalls, both successful and unsuccessful"
   tester = "many_getsyscallnumsuccess.c"
   make_qemu_args = "CPUS=1"

class Usertests(Xv6Test):
   name = "usertests"
   description = "run usertests to check for regressions"
   tester = "usertests.c"
   timeout = 300

import toolspath
from testing.runtests import main
main(Xv6Build, [Basic, Getsyscallnumtotal, Getsyscallnumsuccess, Many_getsyscallnumtotal, Many_getsyscallnumsuccess])
