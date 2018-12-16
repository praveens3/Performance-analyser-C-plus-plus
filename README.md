# C++ Performance analyzer

### Brief
A simple c++ utility library to calculate performance of a c++ code at granular level.

### Description
IPerf will help developer to identify and segregate time consuming code area to optimize.\
Before going into it, Let it be clear this is not a high level profiler which automatically detect the source code and generate the report during run time.

##### So what this library will do for us?
It will do calculation at granular level with respect to the programmer's interest by a set of manual entries in your source code.

##### Okay lets speak about the granularity, actually granularity here is in the aspect of calculating performance for the following events,
	for a statement,
	for a set of statements,
	for a function,
	for a recursive function,
	from one function to another function,
	from one class method to another class method,
	for threaded functions,
	inside threaded functions,
	.....,
	etc.
	
In simple terms, In the code base at any place of a definition area from one line to the another
line performance can be calculated no matters where the line exist in the code.

##### erformance calculation is based on the following term
- number of time the selected events get executed and
- the time taken for the selected events to execute

### Features
- IPerf is thread safe
- At the same time, IPerf core is designed as singleton in order to deliver consistency across a whole client application instance.

#### Log
	Author: Praveen S
	Date: 16-Dec-2018
	Release: v1.0
