#!/usr/local/bin/python
# coding: utf-8
#-------------------------------------------------------------------------------
# Cpacity declaration

import cppClass


#-------------------------------------------------------------------------------
class capacityType(cppClass.CppType):
   """ Object for generating a capacity """

   def __init__(self, name, unit = "shared::plugin::yPluginApi::CStandardUnits::NoUnit()", keywordDataType = "shared::plugin::yPluginApi::EKeywordDataType::kEnum"):
      super(capacityType, self).__init__(name + "Capacity")
      self.__name = name
      self.__unit = unit
      self.__keywordDataType = keywordDataType

   def instance(self):
      return self._cppTypeName + "()"

   def generateHeader(self, f):
      pass

   def generateSource(self, f):
      f.write("DECLARE_CAPACITY(" + self._cppTypeName + ", \"" + self.__name + "\", " + self.__unit + ", " + self.__keywordDataType + ");\n")
