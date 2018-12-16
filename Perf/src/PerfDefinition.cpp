#include "PerfDefinition.h"
#include "PerfInstanceDefinition.h"
#include <chrono>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <mutex>
#include <map>

// Check windows
#if _WIN32 || _WIN64
#if _WIN64
const std::string BUILD_ARCHITECTURE = " x64 ";
#else
const std::string BUILD_ARCHITECTURE = " x86 ";
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
const std::string BUILD_ARCHITECTURE = " x64 ";
#else
const std::string BUILD_ARCHITECTURE = " x86 ";
#endif
#endif

/*
* **************************************************************************************************
* IPerfManager singleton instance creation
* **************************************************************************************************
*/
IPERF::IPerfManager* IPERF::getPerf()
{
   static PERF_DEFINITION::PerfDefinition lPerf;
   return &lPerf;
}

namespace PERF_DEFINITION
{
const std::string FILE_PATH = "Perf.log";
const int NAME_MAX_SIZE = 18;
std::mutex gMutextStart;

/*
* **************************************************************************************************
* IPerfManager's derived class definition member definitions
* **************************************************************************************************
*/

PerfDefinition::PerfDefinition() :
   mpath(FILE_PATH)
{
}

PerfDefinition::~PerfDefinition()
{
   for (const auto& itter : mNameToPerfDataMap)
   {
      stop(itter.first);
   }
   save();
}

/*
* **************************************************************************************************
* Performance Definition Interface API definitions
* **************************************************************************************************
*/

std::shared_ptr<IPERF::IPerfInstance> PerfDefinition::getInstance(
   const std::string &pIdentifier,
   const std::thread::id &pThreadId)
{
   return std::make_shared<PERF_INSTANCE_DEFINITION::PerfInstanceDefinition>(
      pIdentifier,
      pThreadId);
}

std::shared_ptr<IPERF::IPerfInstance> PerfDefinition::getInstance(
   const std::string &pIdentifier)
{
   return std::make_shared<PERF_INSTANCE_DEFINITION::PerfInstanceDefinition>(
      pIdentifier);
}

void PerfDefinition::setPath(const std::string &pPath)
{
   mpath = pPath;
}

void PerfDefinition::reset()
{
   std::lock_guard<std::mutex> lock(gMutextStart);
   mNameToPerfDataMap.clear();
}

void PerfDefinition::start(const std::string &pIdentifier)
{
   PerfData* lDataPtr = nullptr;
   {
      std::lock_guard<std::mutex> lock(gMutextStart);
      lDataPtr = &mNameToPerfDataMap[pIdentifier];
   }

   if (lDataPtr != nullptr && lDataPtr->mStatus == STATUS::RUNNING)
      return;

   ++lDataPtr->mInvokeCount;
   lDataPtr->mStatus = STATUS::RUNNING;
   lDataPtr->mStartTime = std::chrono::high_resolution_clock::now();
}

void PerfDefinition::stop(const std::string &pIdentifier)
{
   const PerfTime &lStopTime = std::chrono::high_resolution_clock::now();
   //std::find result is undefined behaviour when the container is geting modified parallely
   //seems to be not thread safe
   //gMutextStart.lock();
   auto& itter = mNameToPerfDataMap.find(pIdentifier);
   //gMutextStart.unlock();
   if (itter != mNameToPerfDataMap.end() && itter->second.mStatus == STATUS::RUNNING)
   {
      itter->second.mTimeConsumed += std::chrono::duration_cast<std::chrono::milliseconds>(
         lStopTime - itter->second.mStartTime).count();
      itter->second.mStatus = STATUS::STOP;
   }
}

std::string getSpace(const std::string &pString)
{
   std::string lReturnStr;
   const int lNameLen = pString.length();
   for (int i = 0; i < (NAME_MAX_SIZE - lNameLen)+1; ++i)
   {
      lReturnStr.append(" ");
   }
   return lReturnStr;
}

std::string trimName(const std::string &pIdentifier)
{
   size_t lPos = pIdentifier.find_first_of(PERF_INSTANCE_DEFINITION::THREAD_ID_TO_NAME_SEPERATOR);
   if (lPos != std::string::npos)
   {
      return pIdentifier.substr(lPos + PERF_INSTANCE_DEFINITION::THREAD_ID_TO_NAME_SEPERATOR.length(), pIdentifier.length());
   }
   return pIdentifier;
}

void PerfDefinition::save()
{
   std::ofstream lFile;
   lFile.open(mpath, std::ofstream::out | std::ofstream::app);
   if (lFile.is_open() == false)
      return;

   //ToDo: the following 3 for loop should be optimize
   //2.copy of a container to save time, since container may gets modified by other threads
   //3. merging threaded identifier
   //4. iterating element for file write

   std::map<std::string, PerfData> lNameToPerfDataMap;
   gMutextStart.lock();
   auto lNameToPerfDataMapCopy = mNameToPerfDataMap;
   gMutextStart.unlock();

   std::string lFullName, lTrimmedName;
   for (const auto& itter : lNameToPerfDataMapCopy)
   {
      lFullName = itter.first;
      lTrimmedName = trimName(lFullName);
      if (lTrimmedName != lFullName)
      {
         PerfData &lData = lNameToPerfDataMap[lTrimmedName];
         lData.mInvokeCount += itter.second.mInvokeCount;
         lData.mTimeConsumed += itter.second.mTimeConsumed;
         lData.mIsThread = true;
      }
      else
      {
         lNameToPerfDataMap[lFullName] = lNameToPerfDataMapCopy[lFullName];
      }
   }

   auto in_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
   tm lTM;
   localtime_s(&lTM, &in_time_t);
   std::stringstream ss;
   ss << std::put_time(&lTM, "%Y-%m-%d %X");

   lFile << std::endl;
   lFile << "----- Performance analysis result" + BUILD_ARCHITECTURE + "-----" << std::endl;
   lFile << "[" << ss.str() << "]" << std::endl;
   lFile << "[name]             " << "[invoke count]     " << "[time consumed]" << std::endl;
   std::string lName;
   std::for_each(
      lNameToPerfDataMap.begin(),
      lNameToPerfDataMap.end(),
      [&lFile, &lName](const std::pair<std::string, PerfData> &pPair) {
         lName = ((pPair.second.mIsThread) ? "[t]" : "") + pPair.first;
         lFile << lName << getSpace(lName) <<
            pPair.second.mInvokeCount << getSpace(std::to_string(pPair.second.mInvokeCount)) <<
            pPair.second.mTimeConsumed <<std::endl;
      }
   );

   lFile << std::endl;
   lFile.close();
}
}
