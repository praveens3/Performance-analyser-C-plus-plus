#include "PerfInstanceDefinition.h"
#include "PerfDefinition.h"
#include <sstream>

namespace PERF_INSTANCE_DEFINITION
{
PerfInstanceDefinition::PerfInstanceDefinition(const std::string &pIdentifier) :
   mIdentifier(pIdentifier)
{
}

PerfInstanceDefinition::PerfInstanceDefinition(const std::string &pIdentifier, const std::thread::id &pThreadId)
{
   std::stringstream lThreadIdSS;
   lThreadIdSS << pThreadId;
   mIdentifier = lThreadIdSS.str() + THREAD_ID_TO_NAME_SEPERATOR + pIdentifier;
}

perfInstancePtr PerfInstanceDefinition::start()
{
   static_cast<PERF_DEFINITION::PerfDefinition*> (IPERF::getPerf())->start(mIdentifier);
   return shared_from_this();
}

void PerfInstanceDefinition::stop()
{
   return (static_cast<PERF_DEFINITION::PerfDefinition*>(IPERF::getPerf()))->stop(mIdentifier);
}
}

