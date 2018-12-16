#ifndef PERFINSTANCEDEFINITION_H
#define PERFINSTANCEDEFINITION_H

#include "IPerf.h"

namespace PERF_INSTANCE_DEFINITION
{
const std::string THREAD_ID_TO_NAME_SEPERATOR = "[|#|]";

/*
* PerfInstanceDefinition class hold the IPerfInstance metadata
* gives definition of the Interface IPerfInstance
*/
class PerfInstanceDefinition : public IPERF::IPerfInstance, public std::enable_shared_from_this<PerfInstanceDefinition>
{
public:
   PerfInstanceDefinition(const std::string &pIdentifier);
   PerfInstanceDefinition(const std::string &pIdentifier, const std::thread::id &pThreadId);
   virtual perfInstancePtr start() override;
   virtual void stop() override;

private:
   /*
   * hold the IperfInstance metadata
   * metadata will be a unique name or unique name + thread Id
   */
   std::string mIdentifier;
};
}

#endif //PERFINSTANCEDEFINITION_H