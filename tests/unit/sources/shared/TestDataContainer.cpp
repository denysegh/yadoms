#include "stdafx.h"
#include <boost/test/unit_test.hpp>

#include <Poco/Stopwatch.h>
#include <Poco/JSON/Object.h>
#include <shared/exception/JSONParse.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/DataContainer.h"
#include "../../../../sources/shared/shared/rapidJson/document.h"
#include "../../../../sources/shared/shared/rapidJson/pointer.h"

//bench only
#include "../../../../sources/shared/shared/rapidJson/writer.h"
#include "../../../../sources/shared/shared/rapidJson/stringbuffer.h"
#include "../../../../sources/shared/shared/rapidJson/writer.h"
#include "../../../../sources/shared/shared/rapidJson/istreamwrapper.h"
#include "../../../../sources/shared/shared/rapidJson/ostreamwrapper.h"
#include "../../../../sources/shared/shared/rapidJson/prettywriter.h"
#include "../../../../sources/shared/shared/rapidJson/pointer.h"


class Json : public rapidjson::Document
{
public:
   Json()
   {
      SetObject();
   }

   Json(rapidjson::Value &v)
   {
      SetObject();
      initializeWith(v);
   }  
   
   Json(Json &v)
   {
      SetObject();
      initializeWith(v);
   }

   virtual ~Json()
   {
   }

private:
   inline std::string generatePath(const std::string & parameterName, const char pathChar) const
   {
      std::string path = "/";
      std::string pathCarStl = "";
      pathCarStl += pathChar;
      path += boost::replace_all_copy(parameterName, pathCarStl, "/");
      return path;
   }

public:

   bool getBool(const std::string &parameterName)
   {
      auto iter = FindMember(parameterName);
      if (iter != MemberEnd())
      {
         if (iter->value.IsBool())
            return iter->value.GetBool();
         else
         {
            throw shared::exception::CException("Value is not a BOOL : " + parameterName);
         }
      }
      else
      {
         std::string path = generatePath(parameterName, '.');
         auto a = (rapidjson::Value*)rapidjson::Pointer(path).Get(*this);
         if (a != nullptr)
         {
            if (a->IsBool())
               return a->GetBool();
            else
            {
               throw shared::exception::CException("Value is not a BOOL : " + parameterName);
            }
         }
         else
         {
            throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
         }
      }
   }

   int getInt(const std::string &parameterName)
   {
      auto iter = FindMember(parameterName);
      if (iter != MemberEnd())
      {
         if (iter->value.IsInt())
            return iter->value.GetInt();
         else
         {
            throw shared::exception::CException("Value is not a INT : " + parameterName);
         }
      }
      else
      {
         std::string path = generatePath(parameterName, '.');
         auto a = (rapidjson::Value*)rapidjson::Pointer(path).Get(*this);
         if (a != nullptr)
         {
            if (a->IsInt())
               return a->GetInt();
            else
            {
               throw shared::exception::CException("Value is not a BOOL : " + parameterName);
            }
         }
         else
         {
            throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
         }
      }
   }

   int64_t getInt64(const std::string &parameterName)
   {
      auto iter = FindMember(parameterName);
      if (iter != MemberEnd())
      {
         if (iter->value.IsInt64())
            return iter->value.GetInt64();
         else
         {
            throw shared::exception::CException("Value is not a INT64 : " + parameterName);
         }
      }
      else
      {
         throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
      }
   }

   float getFloat(const std::string &parameterName)
   {
      auto iter = FindMember(parameterName);
      if (iter != MemberEnd())
      {
         if (iter->value.IsFloat())
            return iter->value.GetFloat();
         else
         {
            throw shared::exception::CException("Value is not a FLOAT : " + parameterName);
         }
      }
      else
      {
         throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
      }
   }

   double getDouble(const std::string &parameterName)
   {
      auto iter = FindMember(parameterName);
      if (iter != MemberEnd())
      {
         if (iter->value.IsDouble())
            return iter->value.GetDouble();
         else
         {
            throw shared::exception::CException("Value is not a DOUBLE : " + parameterName);
         }
      }
      else
      {
         throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
      }
   }

   std::string getString(const std::string &parameterName)
   {
      auto iter = FindMember(parameterName);
      if (iter != MemberEnd())
      {
         if (iter->value.IsString())
            return iter->value.GetString();
         else
         {
            throw shared::exception::CException("Value is not a String : " + parameterName);
         }
      }
      else
      {
         std::string path = generatePath(parameterName, '.');
         auto a = (rapidjson::Value*)rapidjson::Pointer(path).Get(*this);
         if (a != nullptr)
         {
            if (a->IsString())
               return a->GetString();
            else
            {
               throw shared::exception::CException("Value is not a String : " + parameterName);
            }
         }
         else
         {
            throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
         }
      }
   }

   Json getChild(const std::string &parameterName)
   {
      auto iter = FindMember(parameterName);
      if (iter != MemberEnd())
      {
         return iter->value;
      }
      else
      {
         throw shared::exception::COutOfRange("Cannot find parameter " + parameterName);
      }
   }
   



   void set(const std::string &parameterName, bool value)
   {
      auto & a = GetAllocator();
      rapidjson::Value name(parameterName, a);
      AddMember(name, value, a);
   }


   void set(const std::string &parameterName, int value)
   {
      auto & a = GetAllocator();
      rapidjson::Value name(parameterName, a);
      AddMember(name, value, a);
   }
   void set(const std::string &parameterName, int64_t value)
   {
      auto & a = GetAllocator();
      rapidjson::Value name(parameterName, a);
      AddMember(name, value, a);
   }
   void set(const std::string &parameterName, float value)
   {
      auto & a = GetAllocator();
      rapidjson::Value name(parameterName, a);
      AddMember(name, value, a);
   }
   void set(const std::string &parameterName, double value)
   {
      auto & a = GetAllocator();
      rapidjson::Value name(parameterName, a);
      AddMember(name, value, a);
   }
   void set(const std::string &parameterName, const std::string & value)
   {
      auto & a = GetAllocator();
      rapidjson::Value v(rapidjson::kStringType);
      v.SetString(value, a);
      rapidjson::Value name(parameterName, a);
      AddMember(name, v, a);
   }  
   void set(const std::string &parameterName, const char * value)
   {
      std::string s(value);
      set(parameterName, s);
   }

   void set(const std::string &parameterName, const Json & value)
   {
      auto & a = GetAllocator();
      rapidjson::Value v(rapidjson::kObjectType);
      v.CopyFrom(value, a);
      rapidjson::Value name(parameterName, a);
      AddMember(name, v, a);
   }

   void set(const std::string &parameterName, const shared::IDataContainable & value)
   {
      // TODO
   }
   
   void set(const std::string &parameterName, boost::shared_ptr<shared::IDataContainable> & value)
   {

   }

   void set(const std::string &parameterName, std::vector<bool> & values)
   {
      auto& allocator = GetAllocator();
      rapidjson::Value name(parameterName, allocator);
      rapidjson::Value & v = AddMember(name, rapidjson::Value(rapidjson::kArrayType), allocator);
      for (auto i = values.begin(); i != values.end(); ++i)
      {
         v.PushBack(*i, allocator);
      }
   }

   void set(const std::string &parameterName, std::vector<int> & values)
   {
      auto& allocator = GetAllocator();
      rapidjson::Value * v = nullptr;

      if (parameterName.empty())
      {
         SetArray();
         v = this;
      }
      else
      {
         rapidjson::Value name(parameterName, allocator);
         v = &AddMember(name, rapidjson::Value(rapidjson::kArrayType), allocator);
      }

      for (auto i = values.begin(); i != values.end(); ++i)
      {
         v->PushBack(*i, allocator);
      }
   }

   void set(const std::string &parameterName, std::vector<int64_t> & values)
   {
      auto& allocator = GetAllocator();
      rapidjson::Value * v = nullptr;

      if (parameterName.empty())
      {
         SetArray();
         v = this;
      }
      else
      {
         rapidjson::Value name(parameterName, allocator);
         v = &AddMember(name, rapidjson::Value(rapidjson::kArrayType), allocator);
      }

      for (auto i = values.begin(); i != values.end(); ++i)
      {
         v->PushBack(*i, allocator);
      }
   }

   void set(const std::string &parameterName, std::vector<float> & values)
   {
      auto& allocator = GetAllocator();
      rapidjson::Value * v = nullptr;

      if (parameterName.empty())
      {
         SetArray();
         v = this;
      }
      else
      {
         rapidjson::Value name(parameterName, allocator);
         v = &AddMember(name, rapidjson::Value(rapidjson::kArrayType), allocator);
      }

      for (auto i = values.begin(); i != values.end(); ++i)
      {
         v->PushBack(*i, allocator);
      }
   }

   void set(const std::string &parameterName, std::vector<double> & values)
   {
      auto& allocator = GetAllocator();
      rapidjson::Value * v = nullptr;

      if (parameterName.empty())
      {
         SetArray();
         v = this;
      }
      else
      {
         rapidjson::Value name(parameterName, allocator);
         v = &AddMember(name, rapidjson::Value(rapidjson::kArrayType), allocator);
      }

      for (auto i = values.begin(); i != values.end(); ++i)
      {
         v->PushBack(*i, allocator);
      }
   }
   void set(const std::string &parameterName, std::vector<std::string> & values)
   {
      auto& allocator = GetAllocator();
      rapidjson::Value * v = nullptr;

      if (parameterName.empty())
      {
         SetArray();
         v = this;
      }
      else
      {
         rapidjson::Value name(parameterName, allocator);
         v = &AddMember(name, rapidjson::Value(rapidjson::kArrayType), allocator);
      }

      for (auto i = values.begin(); i != values.end(); ++i)
      {
         rapidjson::Value val(i->c_str(), i->size());
         v->PushBack(val, allocator);
      }
   }
   void set(const std::string &parameterName, std::vector<Json> & values)
   {
      auto& allocator = GetAllocator();
      rapidjson::Value * v = nullptr;

      if (parameterName.empty())
      {
         SetArray();
         v = this;
      }
      else
      {
         rapidjson::Value name(parameterName, allocator);
         v = &AddMember(name, rapidjson::Value(rapidjson::kArrayType), allocator);
      }

      for (auto i = values.begin(); i != values.end(); ++i)
      {
         v->PushBack(*i, allocator);
      }
   }

   void set(const std::string &parameterName, std::vector<shared::IDataContainable> & values)
   {

   }
   void set(const std::string &parameterName, std::vector<boost::shared_ptr<shared::IDataContainable>> & values)
   {

   }

   void printToLog(std::ostream& os = std::cout) const
   {
      os << std::endl;
      rapidjson::OStreamWrapper osw(os);
      rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
      Accept(writer);
   }



   std::ostream& operator << (std::ostream& os)
   {
      os << serialize();
      return os;
   }

   std::istream& operator >> (std::istream& is)
   {
      RemoveAllMembers();

      rapidjson::IStreamWrapper isw(is);

      ParseStream(isw);
      return is;
   }


   std::string serialize(bool prettyOutput = false) const
   {
      rapidjson::StringBuffer sb;
      if (prettyOutput)
      {
         rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
         Accept(writer);
      }
      else
      {
         rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
         Accept(writer);
      }
      return sb.GetString();
   }

   void deserialize(const std::string & data)
   {
      RemoveAllMembers();

      if (Parse(data.c_str()).HasParseError())
         throw shared::exception::CJSONParse("Fail to parse Json", GetErrorOffset());
   }

   void serializeToFile(const std::string & filename) const
   {
      std::ofstream ofs(filename);
      rapidjson::OStreamWrapper osw(ofs);
      rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
      Accept(writer);
   }

   void deserializeFromFile(const std::string & file)
   {
      RemoveAllMembers();
      std::ifstream ifs(file);
      rapidjson::IStreamWrapper isw(ifs);
      ParseStream(isw);
   }



   bool operator ==(const Json &rhs) const
   {
      return serialize() == rhs.serialize();
   }

   bool operator !=(const Json &rhs) const
   {
      return serialize() != rhs.serialize();
   }

   void initializeWith(const Json &rhs)
   {
      CopyFrom(rhs, GetAllocator());
   }

   void initializeWith(const rapidjson::Value &rhs)
   {
      CopyFrom(rhs, GetAllocator());
   }
};


BOOST_AUTO_TEST_SUITE(TestDataContainer)


   enum EEnumType
   {
      kEnumValue1 = 7,
      kEnumValue2 = 12,
      kEnumValue3 = 14,
      kEnumValue4,
      kEnumValue5,
      kEnumValue6,
      kEnumValue7,
   };

   static const shared::CDataContainer::EnumValuesNames EEnumTypeNames = boost::assign::map_list_of
   ("EnumValue1", kEnumValue1)
   ("EnumValue2", kEnumValue2)
   ("EnumValue3", kEnumValue3);


   /*
   BOOST_AUTO_TEST_CASE(_RapidJsonTest)
   {
      const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] , \"o\" : { \"p\" : \"q\", \"r\" : 42 } }";

      rapidjson::Document document;  // Default template parameter uses UTF8 and MemoryPoolAllocator.
      document.Parse(json);
      if (document.Parse(json).HasParseError())
      {
         std::cout << "fail to parse JSON : " << json << std::endl;
         BOOST_TEST_FAIL("Invalid json input");
      }
      else
      {
         if (document.HasMember("hello"))
         {
            std::cout << "Hello=" << document["hello"].GetString() << std::endl;
         }

         rapidjson::Value* stars = rapidjson::GetValueByPointer(document, "/o/p");
         if (stars)
         {
            std::cout << "/o/p=" << stars->GetString() << std::endl;

         }
         stars = rapidjson::GetValueByPointer(document, "/o/r");
         if (stars)
         {
            std::cout << "/o/r=" << stars->Get<int>() << std::endl;

         }
      }
   }*/


   BOOST_AUTO_TEST_CASE(JsonTest1)
   {
      Json a;
      a.set("BoolParameter", true);
      a.set("DecimalParameter", 18.4);
      a.set("EnumParameter", kEnumValue2);
      a.set("EnumAsStringParameter", "EnumValue1");
      a.set("IntParameter", 42);
      a.set("Serial port", "tty0");
      a.set("StringParameter", "Yadoms is so powerful !");

      
      Json b;
      b.set("SubIntParameter", 123);
      b.set("SubStringParameter", "Just a string parameter in the sub-section");

      a.set("MySection", b);
      
      a.printToLog();
      //a.set<boost::posix_time::ptime>("DateTimeParameter", actualDatetime);

      //check data are correctly retreived
      BOOST_CHECK_EQUAL(a.getBool("BoolParameter"), true);
      BOOST_CHECK_EQUAL(a.getDouble("DecimalParameter"), 18.4);
      BOOST_CHECK_EQUAL(a.getInt("IntParameter"), 42);
      BOOST_CHECK_EQUAL(a.getString("Serial port"), "tty0");
      BOOST_CHECK_EQUAL(a.getString("StringParameter"), "Yadoms is so powerful !");

      auto c = a.getChild("MySection");
      BOOST_CHECK_EQUAL(c.getInt("SubIntParameter"), 123);
      BOOST_CHECK_EQUAL(c.getString("SubStringParameter"), "Just a string parameter in the sub-section");

      BOOST_CHECK_EQUAL(a.getInt("MySection.SubIntParameter"), 123);
      BOOST_CHECK_EQUAL(a.getString("MySection.SubStringParameter"), "Just a string parameter in the sub-section");

   }

   BOOST_AUTO_TEST_CASE(SimpleContainer)
   {
      shared::CDataContainer dc;

      auto actualDatetime = boost::posix_time::second_clock::universal_time();

      //ensure en new container is empty
      BOOST_CHECK_EQUAL(dc.empty(), true) ;

      //insert all simple data

      shared::CDataContainer dc;

      dc.set<bool>("BoolParameter", true);
      dc.set<double>("DecimalParameter", 18.4);
      dc.set<EEnumType>("EnumParameter", kEnumValue2);
      dc.set<std::string>("EnumAsStringParameter", "EnumValue1");
      dc.set<int>("IntParameter", 42);
      dc.set<std::string>("Serial port", "tty0");
      dc.set<std::string>("StringParameter", "Yadoms is so powerful !");

      dc.set<int>("MySection.SubIntParameter", 123);
      dc.set<std::string>("MySection.SubStringParameter", "Just a string parameter in the sub-section");

      dc.set<boost::posix_time::ptime>("DateTimeParameter", actualDatetime);

      dc.printToLog(std::cout);

      //check data are correctly retreived
      BOOST_CHECK_EQUAL(dc.get<bool>("BoolParameter"), true) ;
      BOOST_CHECK_EQUAL(dc.get<double>("DecimalParameter"), 18.4) ;
      BOOST_CHECK_EQUAL(dc.get<EEnumType>("EnumParameter"), kEnumValue2) ;
      BOOST_CHECK_EQUAL(dc.get<int>("IntParameter"), 42) ;
      BOOST_CHECK_EQUAL(dc.get<std::string>("Serial port"), "tty0") ;
      BOOST_CHECK_EQUAL(dc.get<std::string>("StringParameter"), "Yadoms is so powerful !") ;
      BOOST_CHECK_EQUAL(dc.get<int>("MySection.SubIntParameter"), 123) ;
      BOOST_CHECK_EQUAL(dc.getEnumValue<EEnumType>("EnumAsStringParameter", EEnumTypeNames), kEnumValue1);
      BOOST_CHECK_EQUAL(dc.get<std::string>("MySection.SubStringParameter"), "Just a string parameter in the sub-section") ;


      BOOST_CHECK_EQUAL(dc.get<boost::posix_time::ptime>("DateTimeParameter"), actualDatetime) ;

      //another test for a sub container
      shared::CDataContainer test;
      test.set<int>("int", 5);
      test.set<double>("double", 4.0);
      test.set<std::string>("string", "plouf");

      BOOST_CHECK_EQUAL(test.get<int>("int"), 5) ;
      BOOST_CHECK_EQUAL(test.get<double>("double"),4.0) ;
      BOOST_CHECK_EQUAL(test.get<std::string>("string"), "plouf") ;

      shared::CDataContainer subContainer;
      subContainer.set<int>("int4", 6);
      subContainer.set<double>("double1", 8.0);
      subContainer.set<std::string>("string2", "plouf2");

      test.set("config1", subContainer);

      BOOST_CHECK_EQUAL(test.get<double>("config1.double1"), 8.0) ;


      //check for shared_ptr
      auto shs(boost::make_shared<std::string>("un shared ptr"));
      dc.set("StringSharedPtr", shs);

      auto shs2 = dc.get<boost::shared_ptr<std::string>>("StringSharedPtr");
      BOOST_CHECK_EQUAL(*(shs.get()) == *(shs2.get()), true) ;
   }


   void benchmarkJson(int maxcount, bool withStringify)
   {
      Poco::Stopwatch sw;

      Poco::Int64 boost_perf, poco_perf, rjs_perf, js_perf;

      shared::CDataContainer dcBool, dcInt, dcString, dcLongString, dcSubItems;

      rapidjson::Document rjsBool, rjsInt, rjsString, rjsLongString, rjsSubItems;
      rjsBool.SetObject();
      rjsInt.SetObject();
      rjsString.SetObject();
      rjsLongString.SetObject();
      rjsSubItems.SetObject();

      Poco::JSON::Object::Ptr menuObjBool = new Poco::JSON::Object();
      Poco::JSON::Object::Ptr menuObjInt = new Poco::JSON::Object();
      Poco::JSON::Object::Ptr menuObjString = new Poco::JSON::Object();
      Poco::JSON::Object::Ptr menuObjLongString = new Poco::JSON::Object();
      Poco::JSON::Object::Ptr menuObjSubItems = new Poco::JSON::Object();

      rapidjson::Value name(rapidjson::kStringType);
      std::string s;


      Json jsBool, jsInt, jsString, jsLongstring, jsSubItems;

      //Bool
      {
         sw.restart();
         for (int i = 0; i < maxcount; ++i)
            dcBool.set((boost::format("Test_%1%") % i).str(), true);
         sw.stop();

         boost_perf = sw.elapsed();

         sw.restart();
         for (int i = 0; i < maxcount; ++i)
            menuObjBool->set((boost::format("Test_%1%") % i).str(), true);
         sw.stop();
         poco_perf = sw.elapsed();    
         
         sw.restart();
         for (int i = 0; i < maxcount; ++i)
            jsBool.set((boost::format("Test_%1%") % i).str(), true);
         sw.stop();
         js_perf = sw.elapsed();


         // must pass an allocator when the object may need to allocate memory
         rapidjson::Document::AllocatorType& allocator = rjsBool.GetAllocator();

         sw.restart();
         for (int i = 0; i < maxcount; ++i)
         {
            s = (boost::format("Test_%1%") % i).str();
            name.SetString(s.c_str(), s.size(), allocator);
            rjsBool.AddMember(name, true, allocator);
         }
         sw.stop();
         rjs_perf = sw.elapsed();


         std::cout << maxcount << ";" << "bool" << ";" << "ajout" << ";" << poco_perf << ";" << boost_perf << ";" << rjs_perf << ";" << js_perf << std::endl;

         if (withStringify)
         {
            sw.restart();
            dcBool.serialize();
            sw.stop();
            boost_perf = sw.elapsed();

            std::ostringstream oss;
            sw.restart();
            menuObjBool->stringify(oss);
            sw.stop();
            poco_perf = sw.elapsed();
            
            sw.restart();
            jsBool.serialize();
            sw.stop();
            js_perf = sw.elapsed();

            sw.restart();
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            rjsBool.Accept(writer);
            rjs_perf = sw.elapsed();

            std::cout << maxcount << ";" << "bool" << ";" << "stringify" << ";" << poco_perf << ";" << boost_perf << ";" << rjs_perf << ";" << js_perf << std::endl;
         }

      }

      //int
      {
         sw.restart();
         for (int i = 0; i < maxcount; ++i)
            dcInt.set((boost::format("Test_%1%") % i).str(), i);
         sw.stop();

         boost_perf = sw.elapsed();

         sw.restart();
         for (int i = 0; i < maxcount; ++i)
            menuObjInt->set((boost::format("Test_%1%") % i).str(), i);
         sw.stop();
         poco_perf = sw.elapsed();

         sw.restart();
         for (int i = 0; i < maxcount; ++i)
            jsInt.set((boost::format("Test_%1%") % i).str(), i);
         sw.stop();
         js_perf = sw.elapsed();

         // must pass an allocator when the object may need to allocate memory
         rapidjson::Document::AllocatorType& allocator = rjsInt.GetAllocator();


         sw.restart();
         for (int i = 0; i < maxcount; ++i)
         {
            s = (boost::format("Test_%1%") % i).str();
            name.SetString(s.c_str(), s.size(), allocator);
            rjsInt.AddMember(name, i, allocator);
         }
         sw.stop();
         rjs_perf = sw.elapsed();

         std::cout << maxcount << ";" << "int" << ";" << "ajout" << ";" << poco_perf << ";" << boost_perf << ";" << rjs_perf << ";" << js_perf << std::endl;

         if (withStringify)
         {
            sw.restart();
            dcInt.serialize();
            sw.stop();
            boost_perf = sw.elapsed();

            std::ostringstream oss;
            sw.restart();
            menuObjInt->stringify(oss);
            sw.stop();
            poco_perf = sw.elapsed();

            sw.restart();
            jsInt.serialize();
            sw.stop();
            js_perf = sw.elapsed();

            sw.restart();
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            rjsInt.Accept(writer);
            rjs_perf = sw.elapsed();

            std::cout << maxcount << ";" << "int" << ";" << "stringify" << ";" << poco_perf << ";" << boost_perf << ";" << rjs_perf << ";" << js_perf << std::endl;
         }
      }

      //string
      {
         sw.restart();
         for (int i = 0; i < maxcount; ++i)
            dcString.set((boost::format("Test_%1%") % i).str(), (boost::format("Test_%1%") % i).str());
         sw.stop();

         boost_perf = sw.elapsed();

         sw.restart();
         for (int i = 0; i < maxcount; ++i)
            menuObjString->set((boost::format("Test_%1%") % i).str(), (boost::format("Test_%1%") % i).str());
         sw.stop();
         poco_perf = sw.elapsed();

         sw.restart();
         for (int i = 0; i < maxcount; ++i)
            jsString.set((boost::format("Test_%1%") % i).str(), (boost::format("Test_%1%") % i).str());
         sw.stop();
         js_perf = sw.elapsed();

         // must pass an allocator when the object may need to allocate memory
         rapidjson::Document::AllocatorType& allocator = rjsString.GetAllocator();

         sw.restart();
         for (int i = 0; i < maxcount; ++i)
         {
            s = (boost::format("Test_%1%") % i).str();
            std::string s2 = (boost::format("Test_%1%") % i).str();
            name.SetString(s.c_str(), s.size(), allocator);

            rapidjson::Value val(rapidjson::kStringType);
            val.SetString(s2.c_str(), s2.size(), allocator);
            rjsString.AddMember(name, val, allocator);
         }

         sw.stop();
         rjs_perf = sw.elapsed();

         std::cout << maxcount << ";" << "string" << ";" << "ajout" << ";" << poco_perf << ";" << boost_perf << ";" << rjs_perf << ";" << js_perf << std::endl;

         if (withStringify)
         {
            sw.restart();
            dcString.serialize();
            sw.stop();
            boost_perf = sw.elapsed();

            std::ostringstream oss;
            sw.restart();
            menuObjString->stringify(oss);
            sw.stop();
            poco_perf = sw.elapsed();

            sw.restart();
            jsString.serialize();
            sw.stop();
            js_perf = sw.elapsed();

            sw.restart();
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            rjsString.Accept(writer);
            rjs_perf = sw.elapsed();

            std::cout << maxcount << ";" << "string" << ";" << "stringify" << ";" << poco_perf << ";" << boost_perf << ";" << rjs_perf << ";" << js_perf << std::endl;
         }
      }

      //long string
      std::string loremIpsum = "Lorem ipsum dolor sit amet, consectetur adipiscing elit.Morbi pellentesque eu felis rhoncus mattis.Vivamus laoreet semper lacus, ut commodo metus dapibus quis.Sed dapibus ultricies nisl.Vivamus orci erat, semper non neque eu, fringilla consequat magna.Cras sed rutrum libero, eu volutpat urna.Cras viverra, nunc quis volutpat molestie, ante lacus luctus urna, quis pretium est est vel eros.Etiam pharetra, felis at auctor imperdiet, augue est pellentesque mi, sed malesuada tellus ligula non eros.Maecenas ac diam eu erat faucibus efficitur.Phasellus efficitur a sem sed bibendum.Phasellus ac quam eget nisl malesuada fringilla.Etiam rhoncus, nisi in semper placerat, libero elit iaculis nibh, mattis sagittis sem justo non erat. Suspendisse est ligula, cursus non aliquet sit amet, tincidunt a nibh.Etiam eleifend dolor vel ante vulputate eleifend.Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas.Maecenas pulvinar magna odio, et condimentum mi vulputate in.Vivamus euismod fringilla convallis.Vestibulum dictum nibh at nunc porttitor varius.Cras enim metus, ullamcorper in ante et, sagittis pellentesque nibh.Mauris placerat dapibus posuere.  Proin commodo augue ut sapien congue vulputate.Duis mollis ipsum eget elit volutpat, a fringilla dolor fringilla.Sed sit amet neque id nulla malesuada cursus.Quisque lobortis sollicitudin nunc, ac fermentum libero tincidunt sit amet.Suspendisse potenti.Sed vitae pellentesque elit, vitae tincidunt dolor.In pretium, justo eget pretium dignissim, mi elit consequat nulla, at tempus velit metus sit amet felis.Curabitur euismod lacus felis, non lacinia nulla sodales eu.Nunc et tempor tellus.Aliquam hendrerit eros libero, at sollicitudin nulla egestas vitae.Suspendisse eu sapien sed nibh suscipit viverra ac a lacus.  Morbi pulvinar commodo ligula, et dignissim risus elementum a.Sed a neque sed augue fringilla vehicula nec vitae sapien.Nullam interdum ornare arcu vitae rutrum.Duis luctus lacus sed faucibus posuere.Suspendisse mattis aliquam ipsum, at pulvinar arcu ultricies eu.Curabitur sagittis sollicitudin nibh.Phasellus sed justo ac tortor finibus fermentum in sed tortor.Etiam fermentum mi vitae odio porttitor, sit amet ornare velit mattis.Quisque vehicula iaculis nisl, quis posuere dui.  Phasellus non justo sapien.Curabitur ut mi iaculis, bibendum quam vitae, feugiat ligula.Cras lobortis magna eros, in pharetra turpis tristique interdum.Aenean feugiat, elit sit amet lacinia commodo, risus diam aliquam justo, et accumsan nisi ex at est.Phasellus id justo dolor.Sed lorem diam, gravida efficitur purus id, hendrerit ornare mauris.Nulla vel turpis scelerisque, aliquet sapien eu, varius nibh.Suspendisse erat nisl, auctor a aliquet a, ultrices at arcu.Mauris leo ante, eleifend non nisi sed, aliquam posuere velit.Phasellus lacinia enim vel augue tempor euismod.Etiam ut porta nulla.Integer in risus in ex semper ornare eget a justo.Fusce a mollis nibh.Aenean sed sagittis massa.Donec non ligula ipsum.";
      {
         sw.restart();
         for (int i = 0; i < maxcount; ++i)
            dcLongString.set((boost::format("Test_%1%") % i).str(), loremIpsum);
         sw.stop();

         boost_perf = sw.elapsed();

         sw.restart();
         for (int i = 0; i < maxcount; ++i)
            menuObjLongString->set((boost::format("Test_%1%") % i).str(), loremIpsum);
         sw.stop();
         poco_perf = sw.elapsed();

         sw.restart();
         for (int i = 0; i < maxcount; ++i)
            jsLongstring.set((boost::format("Test_%1%") % i).str(), loremIpsum);
         sw.stop();
         js_perf = sw.elapsed();


         // must pass an allocator when the object may need to allocate memory
         rapidjson::Document::AllocatorType& allocator = rjsLongString.GetAllocator();

         sw.restart();
         for (int i = 0; i < maxcount; ++i)
         {
            s = (boost::format("Test_%1%") % i).str();
            name.SetString(s.c_str(), s.size(), allocator);

            rapidjson::Value val(rapidjson::kStringType);
            val.SetString(loremIpsum.c_str(), loremIpsum.size(), allocator);
            rjsLongString.AddMember(name, val, allocator);
         }

         sw.stop();

         std::cout << maxcount << ";" << "longstring" << ";" << "ajout" << ";" << poco_perf << ";" << boost_perf << ";" << rjs_perf << std::endl;

         if (withStringify)
         {
            sw.restart();
            dcLongString.serialize();
            sw.stop();
            boost_perf = sw.elapsed();

            std::ostringstream oss;
            sw.restart();
            menuObjLongString->stringify(oss);
            sw.stop();
            poco_perf = sw.elapsed();

            sw.restart();
            jsLongstring.serialize();
            sw.stop();
            js_perf = sw.elapsed();


            sw.restart();
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            rjsLongString.Accept(writer);
            rjs_perf = sw.elapsed();

            std::cout << maxcount << ";" << "longstring" << ";" << "stringify" << ";" << poco_perf << ";" << boost_perf << ";" << rjs_perf << ";" << js_perf << std::endl;

         }
      }

      //subitems
      {
         Poco::JSON::Object item;
         item.set("bool", true);
         item.set("int", 42);
         item.set("string", "Test42");
         item.set("longstring", loremIpsum);

         shared::CDataContainer dcItem;
         dcItem.set("bool", true);
         dcItem.set("int", 42);
         dcItem.set("string", "Test42");
         dcItem.set("longstring", loremIpsum);

         Json jsSubItem;
         jsSubItem.set("bool", true);
         jsSubItem.set("int", 42);
         jsSubItem.set("string", "Test42");
         jsSubItem.set("longstring", loremIpsum);

         rapidjson::Document rjsDoc;
         rjsDoc.SetObject();
         // must pass an allocator when the object may need to allocate memory
         rapidjson::Document::AllocatorType& allocator = rjsDoc.GetAllocator();

         rjsDoc.AddMember("bool", true, allocator);
         rjsDoc.AddMember("int", 42, allocator);

         rapidjson::Value val(rapidjson::kStringType);
         val.SetString("Test42", allocator);
         rjsDoc.AddMember("string", val, allocator);

         rapidjson::Value val2(rapidjson::kStringType);
         val2.SetString(loremIpsum.c_str(), loremIpsum.size());
         rjsDoc.AddMember("longstring", val2, allocator);

         sw.restart();
         for (int i = 0; i < maxcount; ++i)
         {
            dcSubItems.set((boost::format("Test_%1%") % i).str(), dcItem);
         }
         sw.stop();

         boost_perf = sw.elapsed();

         sw.restart();
         for (int i = 0; i < maxcount; ++i)
         {
            menuObjSubItems->set((boost::format("Test_%1%") % i).str(), item);
         }
         sw.stop();
         poco_perf = sw.elapsed();


         sw.restart();
         for (int i = 0; i < maxcount; ++i)
            jsSubItems.set((boost::format("Test_%1%") % i).str(), jsSubItem);
         sw.stop();
         js_perf = sw.elapsed();


         // must pass an allocator when the object may need to allocate memory
         rapidjson::Document::AllocatorType& allocator2 = rjsSubItems.GetAllocator();

         sw.restart();
         for (int i = 0; i < maxcount; ++i)
         {
            name.SetString(s.c_str(), s.size(), allocator2);
            rjsSubItems.AddMember(name, rjsDoc, allocator2);
         }
         sw.stop();
         rjs_perf = sw.elapsed();

         std::cout << maxcount << ";" << "subitems" << ";" << "ajout" << ";" << poco_perf << ";" << boost_perf << ";" << rjs_perf << ";" << js_perf << std::endl;

         if (withStringify)
         {
            sw.restart();
            dcSubItems.serialize();
            sw.stop();
            boost_perf = sw.elapsed();

            std::ostringstream oss;
            sw.restart();
            menuObjSubItems->stringify(oss);
            sw.stop();
            poco_perf = sw.elapsed();

            sw.restart();
            jsSubItems.serialize();
            sw.stop();
            js_perf = sw.elapsed();

            sw.restart();
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            rjsLongString.Accept(writer);
            rjs_perf = sw.elapsed();

            std::cout << maxcount << ";" << "subitems" << ";" << "stringify" << ";" << poco_perf << ";" << boost_perf << ";" << rjs_perf << ";" << js_perf << std::endl;
         }
      }
   }

   void benchmarkJsonSerialisation(int maxcount)
   {
      Poco::Stopwatch sw;
      //manual serialization
      sw.restart();
      std::string m_internalValue = "{\"result\":\"true\",\"message\":\"\",\"data\":{\"data\":[";
      int i;
      for (i = 0; i < maxcount; ++i)
      {
         m_internalValue += "{\"type\":\"";
         m_internalValue += "alphonse";
         m_internalValue += "\",\"keywordId\":\"";
         m_internalValue += "42";
         m_internalValue += "\",\"avg\":\"42.0\",\"min\":\"24.0\",\"date\":\"";
         m_internalValue += "20180305T101242";
         m_internalValue += "\",\"max\":\"84.0\"},";
      }
      if (m_internalValue.size() > 1)
      {
         if (i > 0)
            m_internalValue[m_internalValue.size() - 1] = ']'; //replace the last ,
         else
            m_internalValue += "]";
      }
      m_internalValue += "}}";
      sw.stop();
      Poco::Int64 manual = sw.elapsed();


      rapidjson::Document rjsManSer;
      rjsManSer.SetObject();
      // must pass an allocator when the object may need to allocate memory
      rapidjson::Document::AllocatorType& allocator3 = rjsManSer.GetAllocator();

      sw.restart();
      rjsManSer.AddMember("result", true, allocator3);
      rjsManSer.AddMember("message", "", allocator3);

      // create a rapidjson object type
      rapidjson::Value array(rapidjson::kArrayType);
      for (i = 0; i < maxcount; ++i)
      {
         rapidjson::Value object(rapidjson::kObjectType);
         object.AddMember("type", "alphonse", allocator3);
         object.AddMember("keywordId", 42, allocator3);
         object.AddMember("avg", 42.0, allocator3);
         object.AddMember("min", 24.0, allocator3);
         object.AddMember("date", "20180305T101242", allocator3);
         object.AddMember("max", 84.0, allocator3);
         array.PushBack(object, allocator3);
      }
      rjsManSer.AddMember("data", array, allocator3);
      rapidjson::StringBuffer buffer;
      rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
      rjsManSer.Accept(writer);
      sw.stop();
      Poco::Int64 rsjString = sw.elapsed();

      std::cout << maxcount << ";" << manual << ";" << rsjString << std::endl;

   }

   BOOST_AUTO_TEST_CASE(Benchmark)
   {
      std::cout << "Nombre d'itérations;Type;Mode;Poco;Boost;RapidJSON" << std::endl;
      benchmarkJson(10, true);
      benchmarkJson(100, true);
      benchmarkJson(1000, true);
      benchmarkJson(10000, true);

      benchmarkJsonSerialisation(10);
      benchmarkJsonSerialisation(100);
      benchmarkJsonSerialisation(1000);
      benchmarkJsonSerialisation(10000);
   }

   BOOST_AUTO_TEST_CASE(Benchmark2)
   {/*
      std::cout << "Nombre d'itérations;Manual;RapidJSON" << std::endl;
      benchmarkJsonSerialisation(10);
      benchmarkJsonSerialisation(100);
      benchmarkJsonSerialisation(1000);
      benchmarkJsonSerialisation(10000);*/
   }

   BOOST_AUTO_TEST_CASE(CollectionContainer)
   {
      shared::CDataContainer test;

      //check vector of int
      std::vector<int> vi;
      for (auto i = 0; i < 10; ++i)
         vi.push_back(i);
      test.set<std::vector<int>>("vectorint", vi);
      test.printToLog(std::cout);

      auto vi2 = test.get<std::vector<int>>("vectorint");
      BOOST_CHECK_EQUAL_COLLECTIONS(vi.begin(), vi.end(), vi2.begin(), vi2.end()) ;

      //check vector of double
      std::vector<double> vd;
      for (auto i = 0; i < 10; ++i)
         vd.push_back(i * 3.0);
      test.set<std::vector<double>>("vectordouble", vd);
      auto vd2 = test.get<std::vector<double>>("vectordouble");
      BOOST_CHECK_EQUAL_COLLECTIONS(vd.begin(), vd.end(), vd2.begin(), vd2.end()) ;

      //check vector of EEnumType
      std::vector<EEnumType> ve;
      ve.push_back(kEnumValue2);
      ve.push_back(kEnumValue4);
      ve.push_back(kEnumValue5);
      ve.push_back(kEnumValue7);
      test.set("vectorenum", ve);
      std::vector<EEnumType> ve2 = test.get<std::vector<EEnumType>>("vectorenum");
      BOOST_CHECK_EQUAL_COLLECTIONS(ve.begin(), ve.end(), ve2.begin(), ve2.end()) ;

      //check vector of shared_ptr<int>
      std::vector<boost::shared_ptr<int>> vish;
      for (auto i = 0; i < 10; ++i)
         vish.push_back(boost::make_shared<int>(i));
      test.set("vectorintsh", vish);
      auto vish2 = test.get<std::vector<boost::shared_ptr<int>>>("vectorintsh");
      auto vish2bis = test.get<std::vector<int>>("vectorintsh");
      BOOST_CHECK_EQUAL(vish.size(), vish2.size()) ;
      for (unsigned int i = 0; i < vish.size(); ++i)
      BOOST_CHECK_EQUAL(*(vish[i].get()) == *(vish2[i].get()), true) ;

      BOOST_CHECK_EQUAL(vish.size(), vish2bis.size()) ;
      for (unsigned int i = 0; i < vish.size(); ++i)
      BOOST_CHECK_EQUAL(*(vish[i].get()) == vish2bis[i], true) ;

      //check vector of shared_ptr<double>
      std::vector<boost::shared_ptr<double>> vdsh;
      for (unsigned int i = 0; i < 10; ++i)
         vdsh.push_back(boost::make_shared<double>(i * 42.0));
      test.set("vectordoublesh", vdsh);
      auto vdsh2 = test.get<std::vector<boost::shared_ptr<double>>>("vectordoublesh");
      auto vdsh2bis = test.get<std::vector<double>>("vectordoublesh");
      BOOST_CHECK_EQUAL(vdsh.size(), vdsh2.size()) ;
      for (unsigned int i = 0; i < vdsh.size(); ++i)
      BOOST_CHECK_EQUAL(*(vdsh[i].get()) == *(vdsh2[i].get()), true) ;

      BOOST_CHECK_EQUAL(vdsh.size(), vdsh2bis.size()) ;
      for (unsigned int i = 0; i < vdsh.size(); ++i)
      BOOST_CHECK_EQUAL(*(vdsh[i].get()) == vdsh2bis[i], true) ;

      //check vector of CDataContainer

      shared::CDataContainer cond1;
      cond1.set("is.keyword", 8);
      cond1.set("is.expectedValue", "32");

      shared::CDataContainer cond2;
      cond2.set("is.keyword", 9);
      cond2.set("is.expectedValue", 34);

      shared::CDataContainer cond3;
      cond3.set("is.keyword", 10);
      cond3.set("is.expectedValue", ve);

      shared::CDataContainer conditions;

      std::vector<shared::CDataContainer> allconditions;
      allconditions.push_back(cond1);
      allconditions.push_back(cond2);
      allconditions.push_back(cond3);

      conditions.set("and", allconditions);

      //do checks
      auto getAllCond = conditions.get<std::vector<shared::CDataContainer>>("and");

      conditions.printToLog(std::cout);
      BOOST_CHECK_EQUAL(allconditions.size(), getAllCond.size());

      auto getCond1 = getAllCond[0];
      auto getCond2 = getAllCond[1];
      auto getCond3 = getAllCond[2];

      BOOST_CHECK_EQUAL(cond1, getCond1) ;
      BOOST_CHECK_EQUAL(cond2, getCond2) ;
      BOOST_CHECK_EQUAL(cond3, getCond3) ;

      BOOST_CHECK_EQUAL_COLLECTIONS(allconditions.begin(), allconditions.end(), getAllCond.begin(), getAllCond.end()) ;
   }

   BOOST_AUTO_TEST_CASE(Serialization)
   {
      const std::string defaultConf("{"
         "\"BoolParameter\": true,"
         "\"DecimalParameter\": 18.4,"
         "\"EnumParameter\": 12,"
         "\"EnumAsStringParameter\": \"EnumValue1\","
         "\"IntParameter\": 42,"
         "\"Serial port\": \"tty0\","
         "\"StringParameter\": \"Yadoms is so powerful !\","
         "\"DateTimeParameter\": \"20140702T113500\","
         "\"MySection\": {"
         "\"SubIntParameter\": 123,"
         "\"SubStringParameter\": \"Just a string parameter in the sub-section\""
         "}"
         "}");

      shared::CDataContainer cfg(defaultConf);

      BOOST_CHECK_EQUAL(cfg.get<bool>("BoolParameter"), true) ;
      BOOST_CHECK_EQUAL(cfg.get<double>("DecimalParameter"), 18.4) ;
      BOOST_CHECK_EQUAL(cfg.get<int>("IntParameter"), 42) ;
      BOOST_CHECK_EQUAL(cfg.get<EEnumType>("EnumParameter"), kEnumValue2) ;
      BOOST_CHECK_EQUAL(cfg.getEnumValue<EEnumType>("EnumAsStringParameter", EEnumTypeNames), kEnumValue1) ;
      BOOST_CHECK_EQUAL(cfg.get<std::string>("Serial port"), "tty0") ;
      BOOST_CHECK_EQUAL(cfg.get<std::string>("StringParameter"), "Yadoms is so powerful !") ;
      BOOST_CHECK_EQUAL(cfg.get<int>("MySection.SubIntParameter"), 123) ;
      BOOST_CHECK_EQUAL(cfg.get<std::string>("MySection.SubStringParameter"), "Just a string parameter in the sub-section") ;

      boost::posix_time::ptime expected(boost::gregorian::date(2014, 7, 2), boost::posix_time::hours(11) + boost::posix_time::minutes(35) + boost::posix_time::seconds(0));
      BOOST_CHECK_EQUAL(cfg.get<boost::posix_time::ptime>("DateTimeParameter"), expected) ;

      //check that serialization match original values
      //just remove space, \n, \t and \r from strings
      auto str = cfg.serialize();
      auto str2 = defaultConf;

      str.erase(std::remove_if(str.begin(), str.end(), boost::is_any_of(" \t\n\r")), str.end());
      str2.erase(std::remove_if(str2.begin(), str2.end(), boost::is_any_of(" \t\n\r")), str2.end());

      BOOST_CHECK_EQUAL(str, str2) ;


      std::stringstream ss;
      ss << cfg;

      shared::CDataContainer cfgOut;
      ss >> cfgOut;

      BOOST_CHECK_EQUAL(cfg.serialize(), cfgOut.serialize()) ;

      //YADOMS_LOG(information) << cfg;
   }


   BOOST_AUTO_TEST_CASE(CheckExistance)
   {
      const std::string defaultConf("{"
         "\"BoolParameter\": \"true\","
         "\"DecimalParameter\": \"18.4\","
         "\"EnumParameter\": \"12\","
         "\"EnumAsStringParameter\": \"EnumValue1\","
         "\"IntParameter\": \"42\","
         "\"Serial port\": \"tty0\","
         "\"StringParameter\": \"Yadoms is so powerful !\","
         "\"DateTimeParameter\": \"20140702T113500\","
         "\"MySection\": {"
         "\"SubIntParameter\": \"123\","
         "\"SubStringParameter\": \"Just a string parameter in the sub-section\""
         "}"
         "}");

      shared::CDataContainer cfg(defaultConf);

      //check path existance
      BOOST_CHECK_EQUAL(cfg.exists("BoolParameter"), true) ;
      BOOST_CHECK_EQUAL(cfg.exists("MySection"), true) ;
      BOOST_CHECK_EQUAL(cfg.exists("MySection.SubIntParameter"), true) ;

      BOOST_CHECK_EQUAL(cfg.exists("SubIntParameter"), false) ;
      BOOST_CHECK_EQUAL(cfg.exists("MySection2"), false) ;
      BOOST_CHECK_EQUAL(cfg.exists("String Parameter"), false) ;

      //check child existance
      BOOST_CHECK_EQUAL(cfg.containsChild("MySection"), true) ;
      BOOST_CHECK_EQUAL(cfg.containsChild("BoolParameter"), false) ;
      BOOST_CHECK_EQUAL(cfg.containsChild("MySection.SubIntParameter"), false) ;

      //check value existance
      BOOST_CHECK_EQUAL(cfg.containsValue("MySection"), false) ;
      BOOST_CHECK_EQUAL(cfg.containsValue("BoolParameter"), true) ;
      BOOST_CHECK_EQUAL(cfg.containsValue("MySection.SubIntParameter"), true) ;
   }

   BOOST_AUTO_TEST_CASE(CurrentNodeTests)
   {
      const std::string testPlatformContent("{"
         "\"supportedPlatforms\": {"
         "\"mac\" : \"none\","
         "\"windows\" : { \"from\":\"3.1\" },"
         "\"raspberry\" : \"all\","
         "\"others\" : \"supported\""
         "}, "
         "\"supportedPlatforms2\":\"all\""
         "}");

      shared::CDataContainer testPf(testPlatformContent);

      //subnode test
      BOOST_CHECK_EQUAL(testPf.exists("supportedPlatforms"), true) ;
      auto supportedPf = testPf.get<shared::CDataContainer>("supportedPlatforms");
      BOOST_CHECK_EQUAL(supportedPf.containsChild(), true) ;
      BOOST_CHECK_EQUAL(supportedPf.containsValue(), false) ;
      BOOST_CHECK_EQUAL(supportedPf.get<std::string>("mac"), "none") ;
      BOOST_CHECK_EQUAL(supportedPf.get<std::string>("raspberry"), "all") ;

      //value test
      BOOST_CHECK_EQUAL(testPf.exists("supportedPlatforms2"), true) ;
      auto supportedPf2 = testPf.get<shared::CDataContainer>("supportedPlatforms2");
      BOOST_CHECK_EQUAL(supportedPf2.containsChild(), false) ;
      BOOST_CHECK_EQUAL(supportedPf2.containsValue(), true) ;
      BOOST_CHECK_EQUAL(supportedPf2.get<std::string>(), "all") ;
   }

   class CTestClass : public shared::IDataContainable
   {
   public:
      CTestClass()
         : m_aIntValue(0), m_dValue(0), m_sValue("")
      {
      }

      CTestClass(int i, double d, std::string & s)
         : m_aIntValue(i), m_dValue(d), m_sValue(s)
      {
      }

      void printToLog(std::ostream &cout) const
      {
         cout << "Value1=" << m_aIntValue << std::endl;
         cout << "Value2=" << m_dValue << std::endl;
         cout << "Value3=" << m_sValue << std::endl;
      }

      void extractContent(shared::CDataContainer& cont) const override
      {
         cont.set("Value1", m_aIntValue);
         cont.set("Value2", m_dValue);
         cont.set("Value3", m_sValue);
      }

      void fillFromContent(const shared::CDataContainer& initialData) override
      {
         m_aIntValue = initialData.get<int>("Value1");
         m_dValue = initialData.get<double>("Value2");
         m_sValue = initialData.get<std::string>("Value3");
      }

      void fillFromSerializedString(const std::string& serializedData) override
      {
         shared::CDataContainer deserializeData(serializedData);
         fillFromContent(deserializeData);
      }

      bool equals(const CTestClass& rhs) const
      {
         return m_aIntValue == rhs.m_aIntValue && m_dValue == rhs.m_dValue && m_sValue == rhs.m_sValue;
      }

      bool operator!=(const CTestClass& rhs) const
      {
         return !equals(rhs);
      }

      friend std::ostream& operator <<(std::ostream& ostr, CTestClass const& str)
      {
         return ostr;
      }

   private:
      int m_aIntValue;
      double m_dValue;
      std::string m_sValue;
   };

   BOOST_AUTO_TEST_CASE(DataContainable)
   {
      //containeur simple de IDataContainable
      std::string p = "test of datacontainable";
      CTestClass obj(1, 42.0, p);
      shared::CDataContainer cont;
      cont.set("myobject", obj);
      auto result = cont.get<CTestClass>("myobject");
      BOOST_CHECK_EQUAL(obj.equals(result), true) ;

      //containeur de boost::shared_ptr<IDataContainable>
      std::string q = "string1";
      auto sp(boost::make_shared<CTestClass>(2, 43.0, q));
      shared::CDataContainer cont2;
      cont2.set("myobject", sp);
      auto result2 = cont2.get<boost::shared_ptr<CTestClass>>("myobject");
      auto result2bis = cont2.get<CTestClass>("myobject");
      BOOST_CHECK_EQUAL(result2->equals(*sp.get()), true) ;
      BOOST_CHECK_EQUAL(result2bis.equals(*sp.get()), true) ;

      //containeur simple de std::vector<IDataContainable>
      std::string s = "test of std::vector<IDataContainable>";
      std::vector<CTestClass> vc;
      for (auto i = 0; i < 10; ++i)
         vc.push_back(CTestClass(i, 42.0 * i, s));
      shared::CDataContainer contvec;
      contvec.set("mycollection", vc);
      auto vc2 = contvec.get<std::vector<CTestClass>>("mycollection");
      BOOST_CHECK_EQUAL_COLLECTIONS(vc.begin(), vc.end(), vc2.begin(), vc2.end()) ;

      //containeur simple de std::vector< boost::shared_ptr<IDataContainable> >
      std::string s2 = "test of std::vector<IDataContainable>";
      std::vector<boost::shared_ptr<CTestClass>> vcsh;
      for (auto i = 0; i < 10; ++i)
         vcsh.push_back(boost::make_shared<CTestClass>(i, 42.0 * i, s2));
      shared::CDataContainer contvecsh;
      contvecsh.set("mycollectionofshared", vcsh);
      auto vcsh2 = contvecsh.get<std::vector<boost::shared_ptr<CTestClass>>>("mycollectionofshared");
      auto vc2bis = contvecsh.get<std::vector<CTestClass>>("mycollectionofshared");
      BOOST_CHECK_EQUAL(vcsh.size(), vcsh2.size()) ;
      for (unsigned int i = 0; i < vcsh.size(); ++i)
      BOOST_CHECK_EQUAL(vcsh[i]->equals(*vcsh2[i].get()), true) ;

      BOOST_CHECK_EQUAL(vcsh.size(), vc2bis.size()) ;
      for (unsigned int i = 0; i < vcsh.size(); ++i)
      BOOST_CHECK_EQUAL(vcsh[i]->equals(vc2bis[i]), true) ;
   }


   BOOST_AUTO_TEST_CASE(Field)
   {
      shared::CField<int> fi(10);
      shared::CField<double> fd(12.3);
      shared::CField<std::string> fs("this is a test");
      shared::CField<EEnumType> fe(kEnumValue2);
      std::string s = "test of datacontainble";
      shared::CField<CTestClass> fdc(CTestClass(5, 42.0, s));


      shared::CDataContainer dc;

      dc.set("FieldInt", fi);
      dc.set("FieldDouble", fd);
      dc.set("FieldString", fs);
      dc.set("FieldEnum", fe);
      dc.set("FieldDataContainable", fdc);

      dc.printToLog(std::cout);
      //check data are correctly retreived
      BOOST_CHECK_EQUAL(dc.get<int>("FieldInt"), fi()) ;
      BOOST_CHECK_EQUAL(dc.get<double>("FieldDouble"), fd()) ;
      BOOST_CHECK_EQUAL(dc.get<std::string>("FieldString"), fs()) ;
      BOOST_CHECK_EQUAL(dc.get<EEnumType>("FieldEnum"), fe()) ;
      BOOST_CHECK_EQUAL(dc.get<CTestClass>("FieldDataContainable").equals(fdc()), true) ;


      //vector de field
      std::vector<shared::CField<int>> vfi;
      for (auto i = 0; i < 10; ++i)
         vfi.push_back(shared::CField<int>(i));

      dc.set("VectorFieldInt", vfi);
      auto vi2 = dc.get<std::vector<int>>("VectorFieldInt");
      BOOST_CHECK_EQUAL_COLLECTIONS(vfi.begin(), vfi.end(), vi2.begin(), vi2.end()) ;
   }


   BOOST_AUTO_TEST_CASE(Path)
   {
      shared::CField<int> fi(10);

      //standard path separator using '.'
      shared::CDataContainer dc;
      dc.set("secA.secB.valC", fi);
      BOOST_CHECK_EQUAL(dc.get<int>("secA.secB.valC"), fi()) ;
      BOOST_CHECK_EQUAL(dc.get<shared::CDataContainer>("secA").get<shared::CDataContainer>("secB").get<int>("valC"), fi()) ;

      //no path using separator 0x00
      dc.set("secD.secE.valC", fi, 0x00);

      dc.printToLog(std::cout);

      BOOST_CHECK_EQUAL(dc.get<int>("secD.secE.valC", 0x00), fi()) ;
      BOOST_CHECK_EQUAL(dc.exists("secD.secE.valC"), false) ;
      BOOST_CHECK_THROW(dc.get<int>("secD.secE.valC"), std::exception) ;
      BOOST_CHECK_THROW(dc.get<shared::CDataContainer>("secD"), std::exception) ;
   }


   // TOFIX : This test is disabled because the default it revealed is not so serious and
   // it makes fail to full campaign. We have for the moment no good solution to fix it.
   // A good solution would probably to change our JSON parser (from Boost) by a real JSON parser.
   //BOOST_AUTO_TEST_CASE(SimpleConstruction)
   //{
   //   BOOST_CHECK_THROW(shared::CDataContainer dc("1"), std::exception) ;
   //}


#define BOOST_CHECK_MAPS(input, output) \
   { \
      BOOST_CHECK_EQUAL(input.size(), output.size());\
      std::map<std::string, std::string>::iterator ii, io;\
      io = output.begin();\
      for (ii = input.begin(); ii != input.end(); ++ii)\
      {\
         BOOST_CHECK_EQUAL(ii->first, io->first);\
         BOOST_CHECK_EQUAL(ii->second, io->second);\
         ++io;\
      }\
   }

   BOOST_AUTO_TEST_CASE(Map)
   {
      std::map<std::string, std::string> input = {{"key1", "value1"},{"key2", "value2"},{"key3", "value3"},{"key4", "value4"}};
      shared::CDataContainer dc(input);
      auto output = dc.getAsMap();

      //dont use BOOST_CHECK_EQUAL_COLLECTIONS because it do not builds with std::map
      BOOST_CHECK_MAPS(input, output);

      auto output2 = dc.get<std::map<std::string, std::string>>();
      BOOST_CHECK_MAPS(input, output2);
   }

   BOOST_AUTO_TEST_SUITE_END()

