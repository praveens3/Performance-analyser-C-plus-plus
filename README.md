# Performance-analyser-C-
A simple c++ library to calculate performance of a c++ code at granular level

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
Date: 15w-Dec-2018
Release: v1.0
