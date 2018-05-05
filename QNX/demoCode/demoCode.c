/*printf time, nanosecond level*/
void print_time(const char *info)
{
  static struct timespec preTime;
  struct timespec nowTime;
  static int fps = 0;
  static int flag = 0;
  int timeGap = 0;

  fps++;

  if (clock_gettime(CLOCK_REALTIME, &nowTime) == -1)
  {
    perror("clock gettime");
  }
  else
  {
    printf("%s: sec = %ld nsec = %ld\n",nowTime.tv_sec,nowTime.tv_nsec);
  }
}