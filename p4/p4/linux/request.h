#ifndef __REQUEST_H__

struct requestStat {
  int is_static; //whether request is static(1) or dynamic(0)
  char filename[MAXLINE];
  int file_descriptor;
  int error_code;
  // may need to add more variables here
};

void requestHandle(struct requestStat *request);

#endif
