# CS537_P5

Name: Xiaodong Wang

CS Login: xiaodong

UW ID: 9066383432

email: xwang322@wisc.edu

Name: Zhen Di

CS Login: zdi

UW ID:  di3

email: di3@wisc.edu

For p5, we made a bunch of changes:

./include/fcntl.h: add O_CHECKED;

./include/stat.h: add T_CHECKED and checksum;

./kernel/sysfile.c: change based on Remzi's viedo, revise create and sys_open to fit into new feature T_CHECKED;

./kernel/fs.c: change bmap, allow only using 24 bits to express the address, simply discard 8 bits for address when allocating;
               
               change itrunc, when free inode, deleting those 24 bits addresses;
               
               change stati, do XOR to every data in data block and return checksum;
   
               change readi/writei: similar to both functions, but able to handle T_CHECKED type file 

We built fine and passed all tests.

