#include "stdafx.h"
#include "DataContainer.h"
#include <shared/exception/JSONParse.hpp>
#include "exception/EmptyResult.hpp"


// Includes needed to compile tested classes

#include <shared/rapidjson/pointer.h>

//bench only
#include <shared/rapidjson/writer.h>
#include <shared/rapidjson/stringbuffer.h>
#include <shared/rapidjson/writer.h>
#include <shared/rapidjson/istreamwrapper.h>
#include <shared/rapidjson/ostreamwrapper.h>
#include <shared/rapidjson/prettywriter.h>
#include <shared/rapidjson/pointer.h>

namespace shared
{
   const CDataContainer CDataContainer::EmptyContainer;

   CDataContainer::CDataContainer()
   {
      m_document.SetObject();
   }

   CDataContainer::CDataContainer(const std::string& initialData)
   {
      m_document.SetObject();
      deserialize(initialData);
   }

   CDataContainer::CDataContainer(const std::map<std::string, std::string>& initialData)
   {
      m_document.SetObject();
      for (const auto& i : initialData)
         set(i.first, i.second);
   }


   CDataContainer::CDataContainer(const CDataContainer& initialData)
   {
      initializeWith(initialData);
   }

   CDataContainer::CDataContainer(boost::shared_ptr<rapidjson::Value> initialData)
   {
      initializeWith(initialData);
   }

   CDataContainer::CDataContainer(rapidjson::Value &initialData)
   {
      initializeWith(initialData);
   }

   CDataContainer::~CDataContainer()
   {
   }

   CDataContainer& CDataContainer::operator=(const CDataContainer& rhs)
   {
      initializeWith(rhs);
      return *this;
   }

   CDataContainer& CDataContainer::operator=(const std::string& rhs)
   {
      deserialize(rhs);
      return *this;
   }
   
   std::ostream& CDataContainer::operator<<(std::ostream& os)
   {
      os << serialize();
      return os;
   }

   std::istream& CDataContainer::operator>>(std::istream& is)
   {
      rapidjson::IStreamWrapper isw(is);
      m_document.ParseStream(isw);
      return is;
   }

   std::ostream& operator << (std::ostream& os, const CDataContainer & dc)
   {
      os << dc.serialize();
      return os;
   }

   std::istream& operator >> (std::istream& is, CDataContainer & dc)
   {
      rapidjson::IStreamWrapper isw(is);
      dc.m_document.ParseStream(isw);
      return is;
   }


   bool CDataContainer::operator ==(const CDataContainer &rhs) const
   {
      return serialize() == rhs.serialize();
   }

   bool CDataContainer::operator !=(const CDataContainer &rhs) const
   {
      return serialize() != rhs.serialize();
   }


   rapidjson::Value* CDataContainer::find(const std::string & parameterName, const char pathChar) const
   {
      rapidjson::Value *v = nullptr;
      if (parameterName.empty())
      {
         v = get();
      }
      else
      {
         std::string path = generatePath(parameterName, pathChar);
         v = (rapidjson::Value*)rapidjson::Pointer(path).Get(m_document);
      }
      return v;
   }

   rapidjson::Value* CDataContainer::findOrCreateArray(const std::string & parameterName, const char pathChar)
   {
      rapidjson::Value *v = nullptr;
      if (parameterName.empty())
      {
         m_document.SetArray();
         v = get();
      }
      else
      {
         std::string path = generatePath(parameterName, pathChar);
         bool alreadyExist;
         v = &rapidjson::Pointer(path).Create(m_document, &alreadyExist);
         if (!alreadyExist)
            v->SetArray();
         if (!v->IsArray())
            throw shared::exception::CException("Value is not an ARRAY : " + parameterName);
      }
      return v;
   }

   inline std::string CDataContainer::generatePath(const std::string & parameterName, const char pathChar) const
   {
      std::string res = "/";
      int c = parameterName.size();
      const char * s = parameterName.c_str();
      for (int i = 0; i < c; ++i)
         res += (s[i] == pathChar) ? '/' : s[i];
      return res;
   }


   bool CDataContainer::getBool(const std::string &parameterName, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsBool())
            return v->GetBool();
         else
         {
            if (v->IsString()) {
               return boost::iequals(v->GetString(), "true") || boost::iequals(v->GetString(), "1");
            } 
            else
            {
               if (v->IsNumber()) {
                  return v->GetDouble() == 1;
               }
            }
            throw shared::exception::CException("Value is not a BOOL or convertible to BOOL : " + parameterName);
         }
      }
      throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
   }

   int CDataContainer::getInt(const std::string &parameterName, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsInt())
            return v->GetInt();
         else
         {
            if (v->IsNumber())
               return (int)v->GetDouble();
            if (v->IsString())
               return atoi(v->GetString());
            throw shared::exception::CException("Value is not a INT : " + parameterName);
         }
      }
      throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
   }

   unsigned int CDataContainer::getUInt(const std::string &parameterName, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsUint())
            return v->GetUint();
         else
         {
            if (v->IsNumber())
               return (unsigned int)v->GetDouble();
            if (v->IsString())
               return (unsigned int)atoi(v->GetString());

            throw shared::exception::CException("Value is not a UINT : " + parameterName);
         }
      }
      throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
   }

   int64_t CDataContainer::getInt64(const std::string &parameterName, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsInt64())
            return v->GetInt64();
         else
         {
            if (v->IsNumber())
               return (int64_t)v->GetDouble();
            if (v->IsString())
               return (int64_t)atoi(v->GetString());
            throw shared::exception::CException("Value is not a INT64 : " + parameterName);
         }
      }
      throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
   }

   uint64_t CDataContainer::getUInt64(const std::string &parameterName, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsUint64())
            return v->GetUint64();
         else
         {
            if (v->IsNumber())
               return (uint64_t)v->GetDouble();
            if (v->IsString())
               return (uint64_t)atoi(v->GetString());
            throw shared::exception::CException("Value is not a INT64 : " + parameterName);
         }
      }
      throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
   }

   float CDataContainer::getFloat(const std::string &parameterName, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsFloat() || v->IsDouble())
            return v->GetFloat();
         else
         {
            if(v->IsNumber())
               return v->GetFloat();
            if (v->IsString())
               return static_cast<float>(atof(v->GetString()));

            throw shared::exception::CException("Value is not a FLOAT : " + parameterName);
         }
      }
      throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
   }

   double CDataContainer::getDouble(const std::string &parameterName, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsDouble())
            return v->GetDouble();
         else
         {
            if (v->IsString())
               return atof(v->GetString());
            throw shared::exception::CException("Value is not a DOUBLE : " + parameterName);
         }
      }
      throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
   }


   bool CDataContainer::getBoolWithDefault(const std::string &parameterName, bool defaultValue, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsBool())
            return v->GetBool();
         if (v->IsString())
            return boost::iequals(v->GetString(), "true") || boost::iequals(v->GetString(), "1");
         if (v->IsNumber())
            return v->GetDouble() == 1;
      }
      return defaultValue;
   }

   int CDataContainer::getIntWithDefault(const std::string &parameterName, int defaultValue, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsInt())
            return v->GetInt();
         if (v->IsNumber())
            return (int)v->GetDouble();
         if (v->IsString())
            return (int)atoi(v->GetString());
      }
      return defaultValue;
   }

   unsigned int CDataContainer::getUIntWithDefault(const std::string &parameterName, unsigned int defaultValue, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsUint())
            return v->GetUint();
         if (v->IsNumber())
            return (unsigned int)v->GetDouble();
         if (v->IsString())
            return (unsigned int)atoi(v->GetString());
      }
      return defaultValue;
   }

   int64_t CDataContainer::getInt64WithDefault(const std::string &parameterName, int64_t defaultValue, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsInt64())
            return v->GetInt64();
         if (v->IsNumber())
            return (int64_t)v->GetDouble();
         if (v->IsString())
            return (int64_t)atoi(v->GetString());
      }
      return defaultValue;
   }

   uint64_t CDataContainer::getUInt64WithDefault(const std::string &parameterName, uint64_t defaultValue, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsUint64())
            return v->GetUint64();
         if (v->IsNumber())
            return (uint64_t)v->GetDouble();
         if (v->IsString())
            return (uint64_t)atoi(v->GetString());
      }
      return defaultValue;
   }

   float CDataContainer::getFloatWithDefault(const std::string &parameterName, float defaultValue, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsFloat())
            return v->GetFloat();
         if (v->IsNumber())
            return v->GetFloat();
         if (v->IsString())
            return static_cast<float>(atof(v->GetString()));
      }
      return defaultValue;
   }
   double CDataContainer::getDoubleWithDefault(const std::string &parameterName, double defaultValue, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsDouble())
            return v->GetDouble();
         if (v->IsString())
            return atof(v->GetString());
      }
      return defaultValue;
   }
   std::string CDataContainer::getStringWithDefault(const std::string &parameterName, const std::string & defaultValue, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsString())
            return v->GetString();
      }
      return defaultValue;
   }

   std::string CDataContainer::getString(const std::string &parameterName, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsString())
            return v->GetString();
         else
         {
            throw shared::exception::CException("Value is not a String : " + parameterName);
         }
      }
      throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
   }

   CDataContainer CDataContainer::getChild(const std::string &parameterName, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsObject())
            return *v;
         else
         {
            throw shared::exception::CException("Value is not a DOUBLE : " + parameterName);
         }
      }
      throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
   }

   void CDataContainer::getChilds(const std::string &parameterName, std::vector<CDataContainer> & result, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsArray())
         {
            for (auto itr = v->Begin(); itr != v->End(); ++itr)
               result.push_back(CDataContainer(*itr));
         }
         else
         {
            throw shared::exception::CException("Value is not an ARRAY : " + parameterName);
         }
      }
      else
      {
         throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
      }
   }


   void CDataContainer::getChilds(const std::string &parameterName, std::vector<bool> & result, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsArray())
         {
            for (auto itr = v->Begin(); itr != v->End(); ++itr)
               if(itr->IsBool())
                  result.push_back(itr->GetBool());
         }
         else
         {
            throw shared::exception::CException("Value is not an ARRAY : " + parameterName);
         }
      }
      else
      {
         throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
      }
   }
   void CDataContainer::getChilds(const std::string &parameterName, std::vector<int> & result, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsArray())
         {
            for (auto itr = v->Begin(); itr != v->End(); ++itr)
               if (itr->IsInt())
                  result.push_back(itr->GetInt());
         }
         else
         {
            throw shared::exception::CException("Value is not an ARRAY : " + parameterName);
         }
      }
      else
      {
         throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
      }
   }

   void CDataContainer::getChilds(const std::string &parameterName, std::vector<int64_t> & result, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsArray())
         {
            for (auto itr = v->Begin(); itr != v->End(); ++itr)
               if (itr->IsInt64())
                  result.push_back(itr->GetInt64());
         }
         else
         {
            throw shared::exception::CException("Value is not an ARRAY : " + parameterName);
         }
      }
      else
      {
         throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
      }
   }
   void CDataContainer::getChilds(const std::string &parameterName, std::vector<unsigned int> & result, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsArray())
         {
            for (auto itr = v->Begin(); itr != v->End(); ++itr)
               if (itr->IsUint())
                  result.push_back(itr->GetUint());
         }
         else
         {
            throw shared::exception::CException("Value is not an ARRAY : " + parameterName);
         }
      }
      else
      {
         throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
      }
   }

   void CDataContainer::getChilds(const std::string &parameterName, std::vector<uint64_t> & result, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsArray())
         {
            for (auto itr = v->Begin(); itr != v->End(); ++itr)
               if (itr->IsUint64())
                  result.push_back(itr->GetUint64());
         }
         else
         {
            throw shared::exception::CException("Value is not an ARRAY : " + parameterName);
         }
      }
      else
      {
         throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
      }
   }
   void CDataContainer::getChilds(const std::string &parameterName, std::vector<float> & result, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsArray())
         {
            for (auto itr = v->Begin(); itr != v->End(); ++itr)
               if (itr->IsFloat())
                  result.push_back(itr->GetFloat());
         }
         else
         {
            throw shared::exception::CException("Value is not an ARRAY : " + parameterName);
         }
      }
      else
      {
         throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
      }
   }
   void CDataContainer::getChilds(const std::string &parameterName, std::vector<double> & result, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsArray())
         {
            for (auto itr = v->Begin(); itr != v->End(); ++itr)
               if (itr->IsDouble())
                  result.push_back(itr->GetDouble());
         }
         else
         {
            throw shared::exception::CException("Value is not an ARRAY : " + parameterName);
         }
      }
      else
      {
         throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
      }
   }
   void CDataContainer::getChilds(const std::string &parameterName, std::vector<std::string> & result, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsArray())
         {
            for (auto itr = v->Begin(); itr != v->End(); ++itr)
               if (itr->IsString())
                  result.push_back(itr->GetString());
         }
         else
         {
            throw shared::exception::CException("Value is not an ARRAY : " + parameterName);
         }
      }
      else
      {
         throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
      }
   }

   void CDataContainer::getChilds(const std::string &parameterName, std::vector<boost::posix_time::ptime> & result, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         if (v->IsArray())
         {
            for (auto itr = v->Begin(); itr != v->End(); ++itr)
               if (itr->IsString())
                  result.push_back(boost::posix_time::from_iso_string(itr->GetString()));
         }
         else
         {
            throw shared::exception::CException("Value is not an ARRAY : " + parameterName);
         }
      }
      else
      {
         throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
      }
   }

   void CDataContainer::getChilds(const std::string &parameterName, std::map<std::string, std::string> & result, const char pathCar) const
   {
      auto v = find(parameterName, pathCar);
      if (v != nullptr)
      {
         //use serialization to ensure all items (at level1 only) are stringized
         rapidjson::StringBuffer sb;
         rapidjson::Writer<rapidjson::StringBuffer> writer(sb, 0, 1);
         v->Accept(writer);
         CDataContainer allLevel1ItemsAsStrings (sb.GetString());
         for (auto itr = allLevel1ItemsAsStrings.m_document.MemberBegin(); itr != allLevel1ItemsAsStrings.m_document.MemberEnd(); ++itr)
            result[itr->name.GetString()] = itr->value.GetString();
      }
      else
      {
         throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
      }
   }

   std::vector<std::string> CDataContainer::getKeys() const
   {
      std::vector<std::string> result;
      for (auto itr = m_document.MemberBegin(); itr != m_document.MemberEnd(); ++itr)
         result.push_back(itr->name.GetString());
      return result;
   }

   std::map<std::string, std::string> CDataContainer::getAsMap(const std::string &parameterName, const char pathCar) const
   {
      std::map<std::string, std::string> local;
      getChilds(parameterName, local);
      return local;
   }

   int CDataContainer::getEnumValue(const std::string& parameterName, const EnumValuesNames& valuesNames, const char pathCar) const
   {
      auto stringValue = getString(parameterName);
      auto it = valuesNames.find(stringValue);
      if (it != valuesNames.end())
         return it->second;

      throw exception::COutOfRange(std::string("Value ") + stringValue + " was not found for " + parameterName + " parameter");
   }

   int CDataContainer::getEnumValueWithDefault(const std::string& parameterName, const EnumValuesNames& valuesNames, int defaultValue, const char pathCar) const
   {
      auto stringValue = getStringWithDefault(parameterName, "");
      auto it = valuesNames.find(stringValue);
      if (it != valuesNames.end())
         return it->second;

      return defaultValue;
   }

   boost::posix_time::ptime CDataContainer::getBoostPTime(const std::string &parameterName, const char pathCar) const
   {
      return boost::posix_time::from_iso_string(getString(parameterName));
   }
     
   boost::posix_time::ptime CDataContainer::getBoostPTimeWithDefault(const std::string &parameterName, const boost::posix_time::ptime & defaultValue, const char pathCar) const
   {
      if(find(parameterName, pathCar) != nullptr)
         return boost::posix_time::from_iso_string(getString(parameterName));
      return defaultValue;
   }

   void CDataContainer::get(const std::string &parameterName, shared::IDataContainable & value, const char pathCar)
   {
      CDataContainer a = getChild(parameterName);
      value.fillFromContent(a);
   }

   void CDataContainer::get(const std::string &parameterName, boost::shared_ptr<shared::IDataContainable> & value, const char pathCar)
   {
      CDataContainer a = getChild(parameterName);
      value->fillFromContent(a);
   }

   

   void CDataContainer::set(const std::string &parameterName, bool value, const char pathCar)
   {
      std::string p = generatePath(parameterName, pathCar);
      rapidjson::Pointer(p).Set(m_document, value);
   }


   void CDataContainer::set(const std::string &parameterName, int value, const char pathCar)
   {
      std::string p = generatePath(parameterName, pathCar);
      rapidjson::Pointer(p).Set(m_document, value);
   }

   void CDataContainer::set(const std::string &parameterName, unsigned int value, const char pathCar)
   {
      std::string p = generatePath(parameterName, pathCar);
      rapidjson::Pointer(p).Set(m_document, value);
   }

   void CDataContainer::set(const std::string &parameterName, int64_t value, const char pathCar)
   {
      std::string p = generatePath(parameterName, pathCar);
      rapidjson::Pointer(p).Set(m_document, value);
   }

   void CDataContainer::set(const std::string &parameterName, uint64_t value, const char pathCar)
   {
      std::string p = generatePath(parameterName, pathCar);
      rapidjson::Pointer(p).Set(m_document, value);
   }

   void CDataContainer::set(const std::string &parameterName, float value, const char pathCar)
   {
      std::string p = generatePath(parameterName, pathCar);
      rapidjson::Pointer(p).Set(m_document, value);
   }
   void CDataContainer::set(const std::string &parameterName, double value, const char pathCar)
   {
      std::string p = generatePath(parameterName, pathCar);
      rapidjson::Pointer(p).Set(m_document, value);
   }
   void CDataContainer::set(const std::string &parameterName, const std::string & value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      rapidjson::Value v(rapidjson::kStringType);
      v.SetString(value, a);
      std::string p = generatePath(parameterName, pathCar);
      rapidjson::Pointer(p).Set(m_document, v);
   }
   void CDataContainer::set(const std::string &parameterName, const char * value, const char pathCar)
   {
      std::string s(value);
      set(parameterName, s);
   }

   void CDataContainer::set(const std::string &parameterName, const CDataContainer & value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      rapidjson::Value v(rapidjson::kObjectType);
      v.CopyFrom(value.m_document, a);
      std::string p = generatePath(parameterName, pathCar);
      rapidjson::Pointer(p).Set(m_document, v);
   }

   void CDataContainer::set(const std::string &parameterName, const shared::IDataContainable & value, const char pathCar)
   {
      CDataContainer obj;
      value.extractContent(obj);
      set(parameterName, obj);
   }

   void CDataContainer::set(const std::string &parameterName, boost::posix_time::ptime value, const char pathCar)
   {
      std::string date = boost::posix_time::to_iso_string(value);
      set(parameterName, date);
   }

   void CDataContainer::set(const std::string &parameterName, boost::shared_ptr<shared::IDataContainable> & value, const char pathCar)
   {
      CDataContainer obj;
      if(value)
         value->extractContent(obj);
      set(parameterName, obj);
   }

   void CDataContainer::set(const std::string &parameterName, const std::vector<bool> & values, const char pathCar)
   {
      auto& allocator = m_document.GetAllocator();
      rapidjson::Value * v = findOrCreateArray(parameterName, pathCar);

      for (auto i = values.begin(); i != values.end(); ++i)
      {
         v->PushBack(*i, allocator);
      }
   }

   void CDataContainer::set(const std::string &parameterName, const std::vector<int> & values, const char pathCar)
   {
      auto& allocator = m_document.GetAllocator();
      rapidjson::Value * v = findOrCreateArray(parameterName, pathCar);

      for (auto i = values.begin(); i != values.end(); ++i)
      {
         v->PushBack(*i, allocator);
      }
   }

   void CDataContainer::set(const std::string &parameterName, const std::vector<int64_t> & values, const char pathCar)
   {
      auto& allocator = m_document.GetAllocator();
      rapidjson::Value * v = findOrCreateArray(parameterName, pathCar);

      for (auto i = values.begin(); i != values.end(); ++i)
      {
         v->PushBack(*i, allocator);
      }
   }

   void CDataContainer::set(const std::string &parameterName, const std::vector<float> & values, const char pathCar)
   {
      auto& allocator = m_document.GetAllocator();
      rapidjson::Value * v = findOrCreateArray(parameterName, pathCar);

      for (auto i = values.begin(); i != values.end(); ++i)
      {
         v->PushBack(*i, allocator);
      }
   }

   void CDataContainer::set(const std::string &parameterName, const std::vector<double> & values, const char pathCar)
   {
      auto& allocator = m_document.GetAllocator();
      rapidjson::Value * v = findOrCreateArray(parameterName, pathCar);

      for (auto i = values.begin(); i != values.end(); ++i)
      {
         v->PushBack(*i, allocator);
      }
   }
   void CDataContainer::set(const std::string &parameterName, const std::vector<std::string> & values, const char pathCar)
   {
      auto& allocator = m_document.GetAllocator();
      rapidjson::Value * v = findOrCreateArray(parameterName, pathCar);

      for (auto i = values.begin(); i != values.end(); ++i)
      {
         rapidjson::Value val(i->c_str(), i->size());
         v->PushBack(val, allocator);
      }
   }
   void CDataContainer::set(const std::string &parameterName, const std::vector<CDataContainer> & values, const char pathCar)
   {
      auto& allocator = m_document.GetAllocator();
      rapidjson::Value * v = findOrCreateArray(parameterName, pathCar);

      for (auto i = values.begin(); i != values.end(); ++i)
      {
         rapidjson::Value val(rapidjson::kObjectType);
         val.CopyFrom(*(i->get()), allocator);
         v->PushBack(val, allocator);
      }
   }

   void CDataContainer::set(const std::string &parameterName, const std::vector<shared::IDataContainable> & values, const char pathCar)
   {
      auto& allocator = m_document.GetAllocator();
      rapidjson::Value * v = findOrCreateArray(parameterName, pathCar);

      for (auto i = values.begin(); i != values.end(); ++i)
      {
         CDataContainer local;
         i->extractContent(local);
         v->PushBack(local.m_document, allocator);
      }
   }  
   
   void CDataContainer::set(const std::string &parameterName, const std::vector<boost::posix_time::ptime> & values, const char pathCar)
   {
      auto& allocator = m_document.GetAllocator();
      rapidjson::Value * v = findOrCreateArray(parameterName, pathCar);

      for (auto i = values.begin(); i != values.end(); ++i)
      {
         std::string date = boost::posix_time::to_iso_string(*i);
         rapidjson::Value val(date.c_str(), date.size());
         v->PushBack(val, allocator);
      }
   }

   void CDataContainer::set(const std::string &parameterName, const std::vector<boost::shared_ptr<shared::IDataContainable>> & values, const char pathCar)
   {
      auto& allocator = m_document.GetAllocator();
      rapidjson::Value * v = findOrCreateArray(parameterName, pathCar);
      for (auto i = values.begin(); i != values.end(); ++i)
      {
         CDataContainer local;
         (*i)->extractContent(local);
         v->PushBack(local.m_document, allocator);
      }
   }



   void CDataContainer::push(const std::string &parameterName, bool value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
         v->PushBack(value, a);
   }
   void CDataContainer::push(const std::string &parameterName, int value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
         v->PushBack(value, a);
   }
   void CDataContainer::push(const std::string &parameterName, int64_t value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
         v->PushBack(value, a);
   } 
   void CDataContainer::push(const std::string &parameterName, unsigned int value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
         v->PushBack(value, a);
   }
   void CDataContainer::push(const std::string &parameterName, uint64_t value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
         v->PushBack(value, a);
   }
   void CDataContainer::push(const std::string &parameterName, float value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
         v->PushBack(value, a);
   }
   void CDataContainer::push(const std::string &parameterName, double value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
         v->PushBack(value, a);
   } 
   void CDataContainer::push(const std::string &parameterName, const std::string & value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
      {
         rapidjson::Value vs(rapidjson::kStringType);
         vs.SetString(value, a);
         v->PushBack(vs, a);
      }
   }
   void CDataContainer::push(const std::string &parameterName, const char * value, const char pathCar)
   {
      std::string v(value);
      push(parameterName, v);
   }
   void CDataContainer::push(const std::string &parameterName, const CDataContainer & value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
      {
         rapidjson::Value v2(value.m_document, a);
         v->PushBack(v2, a);
      }
   }
   void CDataContainer::push(const std::string &parameterName, const shared::IDataContainable & value, const char pathCar)
   {
      CDataContainer a;
      value.extractContent(a);
      push(parameterName, a);
   }
   void CDataContainer::push(const std::string &parameterName, boost::shared_ptr<shared::IDataContainable> & value, const char pathCar)
   {
      CDataContainer a;
      value->extractContent(a);
      push(parameterName, a);
   }

   void CDataContainer::push(const std::string &parameterName, std::vector<bool> & value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
      {
         for(auto i : value)
            v->PushBack(i, a);
      }
   }
   void CDataContainer::push(const std::string &parameterName, std::vector<int> & value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
      {
         for(auto i : value)
            v->PushBack(i, a);
      }
   }
   void CDataContainer::push(const std::string &parameterName, std::vector<int64_t> & value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
      {
         for(auto i : value)
            v->PushBack(i, a);
      }
   }
   void CDataContainer::push(const std::string &parameterName, std::vector<unsigned int> & value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
      {
         for (auto i : value)
            v->PushBack(i, a);
      }
   }
   void CDataContainer::push(const std::string &parameterName, std::vector<uint64_t> & value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
      {
         for (auto i : value)
            v->PushBack(i, a);
      }
   }
   void CDataContainer::push(const std::string &parameterName, std::vector<float> & value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
      {
         for (auto i : value)
            v->PushBack(i, a);
      }
   } 
   void CDataContainer::push(const std::string &parameterName, std::vector<double> & value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
      {
         for (auto i : value)
            v->PushBack(i, a);
      }
   }   
   void CDataContainer::push(const std::string &parameterName, std::vector<std::string> & value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
      {
         for (auto & i : value)
         {
            rapidjson::Value vs(rapidjson::kStringType);
            vs.SetString(i, a);
            v->PushBack(vs, a);
         }
      }
   } 
   
   void CDataContainer::push(const std::string &parameterName, std::vector<CDataContainer> & value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
      {
         for (auto & i : value)
         {
            CDataContainer copy(i);

            v->PushBack(*copy.get(), a);
         }
      }
   }   

   void CDataContainer::push(const std::string &parameterName, std::vector<shared::IDataContainable> & value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
      {
         for (shared::IDataContainable & i : value)
         {
            CDataContainer cont;
            i.extractContent(cont);
            v->PushBack(*(cont.get()), a);
         }
      }
   }

   void CDataContainer::push(const std::string &parameterName, std::vector<boost::shared_ptr<shared::IDataContainable>> & value, const char pathCar)
   {
      auto & a = m_document.GetAllocator();
      auto v = findOrCreateArray(parameterName, pathCar);
      if (v != nullptr)
      {
         for (boost::shared_ptr<shared::IDataContainable> & i : value)
         {
            CDataContainer cont;
            i->extractContent(cont);
            v->PushBack(*(cont.get()), a);
         }
      }
   }

   bool CDataContainer::empty() const
   {
      return m_document.MemberCount() == 0;
   }

   bool CDataContainer::exists(const std::string &parameterName, const char pathCar) const
   {
      return find(parameterName, pathCar) != nullptr;
   }

   void CDataContainer::printToLog(std::ostream& os) const
   {
      os << std::endl;
      rapidjson::OStreamWrapper osw(os);
      rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
      m_document.Accept(writer);
   }




   std::string CDataContainer::serialize(bool prettyOutput) const
   {
      rapidjson::StringBuffer sb;
      if (prettyOutput)
      {
         rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
         m_document.Accept(writer);
      }
      else
      {
         rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
         m_document.Accept(writer);
      }
      return sb.GetString();
   }

   void CDataContainer::deserialize(const std::string & data)
   {
      m_document.RemoveAllMembers();

      if (m_document.Parse(data.c_str()).HasParseError())
         throw shared::exception::CJSONParse("Fail to parse CDataContainer", m_document.GetErrorOffset());
   }

   void CDataContainer::serializeToFile(const std::string & filename) const
   {
      std::ofstream ofs(filename);
      rapidjson::OStreamWrapper osw(ofs);
      rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
      m_document.Accept(writer);
   }

   void CDataContainer::deserializeFromFile(const std::string & file)
   {
      m_document.RemoveAllMembers();
      std::ifstream ifs(file);
      rapidjson::IStreamWrapper isw(ifs);
      m_document.ParseStream(isw);
   }

   bool CDataContainer::containsChild(const std::string& parameterName, const char pathCar) const
   {
      rapidjson::Value* found = find(parameterName, pathCar);
      if (found)
         return !found->IsNull() && found->IsObject();
      return false;
   }

   bool CDataContainer::containsValue(const std::string& parameterName, const char pathCar) const
   {
      rapidjson::Value* found = find(parameterName, pathCar);

      if (found != NULL && !found->IsNull())
      {
         return found->IsBool() || found->IsDouble() || found->IsFloat() || found->IsInt() || found->IsInt64() ||
            found->IsNumber() || found->IsString() || found->IsUint() || found->IsUint64();
      }
      return false;
   }

   void CDataContainer::setNull(const std::string &parameterName, const char pathCar)
   {
      rapidjson::Value* found = find(parameterName, pathCar);
      if(found != nullptr)
         found->SetNull();
   }

   void CDataContainer::mergeObjects(rapidjson::Value &dstObject, rapidjson::Value &srcObject, rapidjson::Document::AllocatorType &allocator)
   {
      for (auto srcIt = srcObject.MemberBegin(); srcIt != srcObject.MemberEnd(); ++srcIt)
      {
         auto dstIt = dstObject.FindMember(srcIt->name);
         if (dstIt != dstObject.MemberEnd())
         {
            assert(srcIt->value.GetType() == dstIt->value.GetType());
            if (srcIt->value.IsArray())
            {
               for (auto arrayIt = srcIt->value.Begin(); arrayIt != srcIt->value.End(); ++arrayIt)
               {
                  dstIt->value.PushBack(*arrayIt, allocator);
               }
            }
            else if (srcIt->value.IsObject())
            {
               mergeObjects(dstIt->value, srcIt->value, allocator);
            }
            else
            {
               dstIt->value = srcIt->value;
            }
         }
         else
         {
            dstObject.AddMember(srcIt->name, srcIt->value, allocator);
         }
      }
   }

   void CDataContainer::mergeFrom(const CDataContainer& source)
   {
      auto& allocator = m_document.GetAllocator();
      mergeObjects(m_document, *source.get(), allocator);
   }

   void CDataContainer::initializeWith(const CDataContainer &rhs)
   {
      m_document.CopyFrom(rhs.m_document, m_document.GetAllocator());
   }

   void CDataContainer::initializeWith(const rapidjson::Value &rhs)
   {
      m_document.CopyFrom(rhs, m_document.GetAllocator());
   }

   void CDataContainer::initializeWith(boost::shared_ptr<rapidjson::Value> rhs)
   {
      m_document.CopyFrom(*rhs.get(), m_document.GetAllocator());
   }
} // namespace shared
