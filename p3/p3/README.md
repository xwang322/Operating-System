# CS537_P3

Name: Xiaodong Wang

CS Login: xiaodong

UW ID: 9066383432

email: xwang322@wisc.edu


Name: Junwei Su

CS Login: junwei

UW ID: 9069851377 

email: jsu23@wisc.edu

We went through the codes about memory allocation part of kernel and figured out some files needed to be changed for the project requirement:

1. proc.h: we add a new sz_stack under proc to denote the stack top of the process;

2. proc.c: we understood how growproc works and similarily, we wrote a new function called growstack to grow stack from the end of USERTOP. We  also added one line under fork to initilized the child process stack top position;

3. exec.c: we made a new stack paged at the end of USERTOP and firstly assigned one page for that. We also initilized the stack size for the stack;

4. trap.c: we define the PGFLT (page fault) for both part a and b of this project, we want xv6 able to give the error of whether it is pointing to a null pointer or it is our of the boundary of the USERTOP or when it is pointing somewhere between growing stack and growting heap;

5. vm.c: we understood how allocuvm worked and changed the copyuvm to allocate memory for child process;

6. syscall.c: we changed the fetchint and fetchstr to make sure the int or str we want to fetch is in valid region and checked the condition in argptr to make sure pointer is as well;

We also made 8 test cases as required, we passed all of them.

They are all put in user folder with corresponding names: null, null-syscall, badaddress-syscall, small-stack, stack-growth, fault\ malloc, overcommit-heap, overcommit-stack
