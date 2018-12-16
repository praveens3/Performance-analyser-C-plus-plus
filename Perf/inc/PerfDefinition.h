#ifndef PERFDEFINITION_H
#define PERFDEFINITION_H

#include "IPerf.h"
#include <chrono>
#include <unordered_map>

namespace PERF_DEFINITION
{
#define PerfTime std::chrono::time_point<std::chrono::system_clock>

   /*
   * @brief This class gives the definition for the Interface exposed API's
     Defining two API's ::start() and ::stop() which is invoked by IPerfInstanceDefinition
   */
class PerfDefinition : public IPERF::IPerfManager
{
public:
   PerfDefinition();
   virtual ~PerfDefinition();

   /*
   * @brief sets path - a file absolute/ relative path to write the calculated performance data
   * default path is .\Perf.log
   */
   virtual void setPath(const std::string &pPath) override;

   /*
    @brief reset the Performance data so far calculated and re-initialize the IPerf as new
   */
   virtual void reset() override;


   /*
    @brief save the calculated performance data to a file
   */
   virtual void save() override;

   /*
   * @brief returns a shared pointer of IPerfInstance
   */
   virtual std::shared_ptr<IPERF::IPerfInstance> getInstance(const std::string &pIdentifier) override;
   virtual std::shared_ptr<IPERF::IPerfInstance> getInstance(
      const std::string &pIdentifier,
      const std::thread::id &pThreadId) override;
   /*
    @brief starts/stops the time for the given identifier
   */
   void start(const std::string &pIdentifier);
   void stop(const std::string &pIdentifier);
private:
   std::string mpath;
   enum class STATUS
   {
      RUNNING,
      STOP
   };

   /*
   * @brief This struct hold the data list for an initiated identifier
   mStartTime - started time for the identifier
   mInvokeCount - no of time the identifier gets invoked
   mStatus - status of an identifier (Running/Stopped)
   mIsThread - true if an identifier is from a thread
   */
   struct PerfData
   {
      PerfTime mStartTime;
      long long mTimeConsumed;
      unsigned int mInvokeCount;
      STATUS mStatus;
      bool mIsThread;

      PerfData() :
         mStartTime(PerfTime()),
         mTimeConsumed(0),
         mInvokeCount(0),
         mStatus(STATUS::STOP),
         mIsThread(false)
      {}
   };

   /*
   * @brief a map of key: PerfInstanceDefinition->mName to value:PerfData
   */
   std::unordered_map<std::string, PerfData> mNameToPerfDataMap;
};
}
#endif //PERFDEFINITION_H