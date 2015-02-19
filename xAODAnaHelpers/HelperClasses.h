#ifndef xAODAnaHelpers_HELPERCLASSES_H
#define xAODAnaHelpers_HELPERCLASSES_H

#include <map>
#include <iostream>

#include "TString.h"

namespace HelperClasses {

  enum class ContainerType{
      UNKNOWN      = 0,
      CONSTDV      = 1,
      CONSTCONT    = 2,
  };

  enum class ToolName{
      MUONSELECTOR,
      ELECTRONSELECTOR,
      JETSELECTOR,
      BJETSELECTOR,
      OVERLAPREMOVER,
  };

  /* template enum parser
  shamelessly copied from: http://stackoverflow.com/a/726681
  */
  template <typename T>
  class EnumParser
  {
     std::multimap <TString, T> enumMap;
   public:
     EnumParser();

     T parseEnum(const TString &value)
     { 
	/*
	for (auto it = enumMap.begin(); it != enumMap.end(); ++it){
	   std::cout << "element: " << (*it).first << std::endl;
	} 
        */
        typename std::multimap <TString, T>::const_iterator iValue = enumMap.find(value);
        if (iValue == enumMap.end()){
            std::cerr << "Could not find input TString in enum!" << std::endl;
	}   
        return iValue->second;
     }
  };
} // close namespace HelperClasses

# endif
