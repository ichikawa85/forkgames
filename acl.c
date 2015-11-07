#include "common.h"

int     allow_severity = LOG_INFO;
int     deny_severity = LOG_WARNING;

int is_access_granted(char* daemon, int s)
{
  struct request_info req;

  request_init(&req, RQ_DAEMON, daemon, RQ_FILE, s, NULL);
  fromhost(&req);

  return hosts_access(&req);
  return 0;
}

