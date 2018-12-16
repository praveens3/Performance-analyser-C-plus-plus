#pragma once

#define __Interface class
#define Interface(name) __Interface name {\
   public:\
   virtual ~name() =default;
#define EndInterface };