extern int isr_flag;

void interruptRoutine()
{
  isr_flag = 1;
}