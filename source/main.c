#include <coreinit/thread.h>
#include <coreinit/time.h>

//Homebrew libraries
#include <whb/proc.h>
#include <whb/log.h>
#include <whb/log_console.h>

//Input Library
#include <vpad/input.h>

// C Standard Libraries
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
   int last_tm_sec = -1;
   OSCalendarTime tm;

   WHBProcInit();
   WHBLogConsoleInit();
   WHBLogPrintf("Hello World!");

   while(WHBProcIsRunning()) {
      WHBLogPrintf("Running...");
      WHBLogConsoleDraw();
      OSSleepTicks(OSMillisecondsToTicks(100));
   }

   WHBLogPrintf("Exiting... good bye.");
   WHBLogConsoleDraw();
   OSSleepTicks(OSMillisecondsToTicks(1000));

   WHBLogConsoleFree();
   WHBProcShutdown();
   return 0;
}
