#ifndef IPERF_H
#define IPERF_H

/**************************************************************************************************/
/*
Brief:
IPerf is a simple c++ library to calculate performance of a c++ code at granular level

Description:
IPerf will help developer to identify and segregate time consuming code area to optimize.
Yes its not a high level profiler to automatically detect the source code and generate the report during
run time, but it will do calculation granular level with respect to the programmer's interest by a
set of manual entries in your source code.

Ok lets speak about the granularity, actually granularity in the aspect of calculating performance
for the following  events,
      for a statement,
      for a set of statements,
      for a function,
      for a recursive function,
      from one function to another function,
      from one class method to another class method,
      .....,
      etc.
In simple terms, In the code base at any place of a definition area from one line to the another
line performance can be calculated no matters where the line exist in the code.

Performance calculation is based on the following term
        1)number of time the selected events get executed and
        2) the time taken for the selected events to execute

IPerf is thread safe
At the same time, IPerf core is designed as singleton in order to deliver consistency across a
whole client application instance.

Author: Praveen S
Date: 16-Dec-2018
Release: v1.0
*/
/**************************************************************************************************/

#include <string>
#include <thread>
#include <memory>

namespace IPERF
{
#define perfInstancePtr std::shared_ptr<IPERF::IPerfInstance>

   //returns an instance for the given identifier(name)
#define getPerfInstance(pIdentifier) getPerf()->getInstance(pIdentifier)

   //register and starting the time for the given identifier(name)
#define startPerf(pIdentifier) getPerfInstance(pIdentifier)->start()

   //returns an instance for the given identifier(name) with a threadId
#define getPerfInstance_t(pIdentifier, pThreadId) getPerf()->getInstance(pIdentifier,pThreadId)

   //register and starting the time for the given identifier(name) and threadId
#define startPerf_t(pIdentifier, pThreadId) getPerfInstance_t(pIdentifier, pThreadId)->start()

/*
* @brief Interface: IPerfInstance
* exposing two API's ::start() and ::stop() for every instantiated identifier(name)
*/
class IPerfInstance
{
public:
   virtual ~IPerfInstance() = default;

   /*
   * @brief starts time for the instance
   */
   virtual perfInstancePtr start() = 0;

   /*
   * @brief stops time for the instance
   */
   virtual void stop() = 0;
};

/*
* @brief Interface: IPerfManager
* This Interface will expose API's for generating IPerfInstance and some general configuration
* create Instance for this Interface by using ::getPerf() API
*/
class IPerfManager
{
public:
   virtual ~IPerfManager() = default;

   /*
   * @brief configuration related API's
   */
   /*
   * @brief sets path - a file absolute/ relative path to write the calculated performance data
   * default path is .\Perf.log
   */
   virtual void setPath(const std::string &pPath) = 0;

   /*
   @ brief reset the Performance data so far calculated and re-initialize the IPerf as new
   */
   virtual void reset() = 0;

   /*
   @brief save the calculated performance data to a configured file ::setPath()
   */
   virtual void save() = 0;

   /*
   * @brief returns instance to IPerfInstance for the given Identifier/ Identifier+Thread Id
   */
   virtual perfInstancePtr getInstance(const std::string &pIdentifier) = 0;
   virtual perfInstancePtr getInstance(
      const std::string &pIdentifier,
      const std::thread::id &pThreadId) = 0;
};

/*
* @brief returns the IPerfManager's singleton instance
*/
IPERF::IPerfManager* getPerf();
}
#endif //IPERF_H