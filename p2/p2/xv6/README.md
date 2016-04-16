# CS537_P2b

Name: Xiaodong Wang

CS Login: xiaodong

UW ID: 9066383432

email: xwang322@wisc.edu

Name: Junwei Su

CS Login: junwei

UW ID: 9069851377 

email: jsu23@wisc.edu

This is part b for project 2, building a scheduler for xv6. We built a multilevel feedback queue for scheduling the priority management of scheduler. We pretty much changed the following part:

Kernel: proc.c, proc.h, sysproc.c, syscall.c, sysfunc.h, def.h;

Include: pstat.h, syscall.h;

User: testgetpinfo.c, makefile.mk

Most of the changes are in proc.c file and we passed all the 3 tests so far. Then we built two graphs figuring the priority of each process versus CPU time slice of each process. We include the plots in the xv6 folder with this README file. One is a test case with sleep command while the other is not a sleep command without sleep command. They are named graph1.pdf and graph2.pdf separately.

The workload file is also included in xv6 folder.
