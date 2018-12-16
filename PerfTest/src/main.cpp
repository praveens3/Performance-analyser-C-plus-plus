#include "IPerf.h"
#include <thread>

void fun(int pDistance)
{
   auto lThreadId = std::this_thread::get_id();
   std::shared_ptr<IPERF::IPerfInstance> lPerfInstance = IPERF::getPerfInstance_t("apple_t", lThreadId);
   lPerfInstance->start();
   for (int i = 0; i < pDistance - 1; ++i)
   {
   }
   lPerfInstance->stop();

   auto lPtr3 = IPERF::getPerf()->getInstance("star_t", lThreadId);
   auto lPtr2 = IPERF::startPerf_t("mango_t", lThreadId);
   for (int i = 0; i < pDistance - 1; ++i)
   {
      lPtr3->start();
      lPtr3->stop();
   }
   lPtr2->stop();
}

int main(int argv, char* argc[]) {

   perfInstancePtr ptr1 = IPERF::getPerf()->getInstance("apple_main")->start();
   for (unsigned int i = 0; i < 100000-1; ++i)
   {
   }
   ptr1->stop();

   auto ptr3 = IPERF::getPerfInstance("star___main");
   auto ptr2 = IPERF::startPerf("mango___main");
   for (unsigned int i = 0; i < 100000 - 1; ++i)
   {
      ptr3->start();
      ptr3->stop();
   }
   ptr2->stop();

   //star and immediate stop
  perfInstancePtr ptr5 = IPERF::startPerf("superman");
  ptr5->stop();

  //thread test
  std::thread t1(fun, 100000 * 2);
  std::thread t2(fun, 100000 * 3);
  std::thread t3(fun, 100000);

   t1.join();
   t2.join();
   t3.join();
   return 0;
}
