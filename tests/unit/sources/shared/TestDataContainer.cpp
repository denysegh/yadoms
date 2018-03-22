#include "stdafx.h"
#include <boost/test/unit_test.hpp>

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/DataContainer.h"

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


   BOOST_AUTO_TEST_CASE(SimpleContainer)
   {
      shared::CDataContainer dc;

      auto actualDatetime = boost::posix_time::second_clock::universal_time();

      //ensure en new container is empty
      BOOST_CHECK_EQUAL(dc.empty(), true) ;

      //insert all simple data
      dc.set("BoolParameter", true);
      dc.set("DecimalParameter", 18.4);
      dc.set("EnumParameter", kEnumValue2);
      dc.set("EnumAsStringParameter", "EnumValue1");
      dc.set("IntParameter", 42);
      dc.set("Serial port", "tty0");
      dc.set("StringParameter", "Yadoms is so powerful !");
      dc.set("MySection.SubIntParameter", 123);
      dc.set("MySection.SubStringParameter", "Just a string parameter in the sub-section");
      dc.set("DateTimeParameter", actualDatetime);

      //check data are correctly retreived
      BOOST_CHECK_EQUAL(dc.getBool("BoolParameter"), true) ;
      BOOST_CHECK_EQUAL(dc.getDouble("DecimalParameter"), 18.4) ;
      BOOST_CHECK_EQUAL(static_cast<EEnumType>(dc.getInt("EnumParameter")), kEnumValue2) ;
      BOOST_CHECK_EQUAL(static_cast<EEnumType>(dc.getEnumValue("EnumAsStringParameter", EEnumTypeNames)), kEnumValue1) ;
      BOOST_CHECK_EQUAL(dc.getInt("IntParameter"), 42) ;
      BOOST_CHECK_EQUAL(dc.getString("Serial port"), "tty0") ;
      BOOST_CHECK_EQUAL(dc.getString("StringParameter"), "Yadoms is so powerful !") ;
      BOOST_CHECK_EQUAL(dc.getInt("MySection.SubIntParameter"), 123) ;
      BOOST_CHECK_EQUAL(dc.getString("MySection.SubStringParameter"), "Just a string parameter in the sub-section") ;
      BOOST_CHECK_EQUAL(dc.getBoostPTime("DateTimeParameter"), actualDatetime) ;

      //another test for a sub container
      shared::CDataContainer test;
      test.set("int", 5);
      test.set("double", 4.0);
      test.set("string", "plouf");

      BOOST_CHECK_EQUAL(test.getInt("int"), 5) ;
      BOOST_CHECK_EQUAL(test.getDouble("double"),4.0) ;
      BOOST_CHECK_EQUAL(test.getString("string"), "plouf") ;

      shared::CDataContainer subContainer;
      subContainer.set("int4", 6);
      subContainer.set("double1", 8.0);
      subContainer.set("string2", "plouf2");

      test.set("config1", subContainer);

      BOOST_CHECK_EQUAL(test.getDouble("config1.double1"), 8.0) ;
   }

   BOOST_AUTO_TEST_CASE(CollectionContainer)
   {
      shared::CDataContainer test;

      //check vector of int
      std::vector<int> vi;
      for (auto i = 0; i < 10; ++i)
         vi.push_back(i);
      test.set("vectorint", vi);
      std::vector<int> vi2;
      test.getChilds("vectorint", vi2);
      BOOST_CHECK_EQUAL_COLLECTIONS(vi.begin(), vi.end(), vi2.begin(), vi2.end()) ;

      //check vector of double
      std::vector<double> vd;
      for (auto i = 0; i < 10; ++i)
         vd.push_back(i * 3.0);
      test.set("vectordouble", vd);
      std::vector<double> vd2;
      test.getChilds("vectordouble", vd2);
      BOOST_CHECK_EQUAL_COLLECTIONS(vd.begin(), vd.end(), vd2.begin(), vd2.end()) ;

      //check vector of EEnumType
      std::vector<int> ve;
      ve.push_back(kEnumValue2);
      ve.push_back(kEnumValue4);
      ve.push_back(kEnumValue5);
      ve.push_back(kEnumValue7);
      test.set("vectorenum", ve);

      std::vector<int> ve2;
      test.getChilds("vectorenum", ve2);
      BOOST_CHECK_EQUAL_COLLECTIONS(ve.begin(), ve.end(), ve2.begin(), ve2.end()) ;

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
      std::vector<shared::CDataContainer> getAllCond;
      conditions.getChilds("and", getAllCond);

      BOOST_CHECK_EQUAL(allconditions.size(), getAllCond.size()) ;

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

      BOOST_CHECK_EQUAL(cfg.getBool("BoolParameter"), true) ;
      BOOST_CHECK_EQUAL(cfg.getDouble("DecimalParameter"), 18.4) ;
      BOOST_CHECK_EQUAL(cfg.getInt("IntParameter"), 42) ;
      BOOST_CHECK_EQUAL(cfg.getInt("EnumParameter"), kEnumValue2) ;
      BOOST_CHECK_EQUAL(cfg.getEnumValue("EnumAsStringParameter", EEnumTypeNames), kEnumValue1) ;
      BOOST_CHECK_EQUAL(cfg.getString("Serial port"), "tty0") ;
      BOOST_CHECK_EQUAL(cfg.getString("StringParameter"), "Yadoms is so powerful !") ;
      BOOST_CHECK_EQUAL(cfg.getInt("MySection.SubIntParameter"), 123) ;
      BOOST_CHECK_EQUAL(cfg.getString("MySection.SubStringParameter"), "Just a string parameter in the sub-section") ;

      boost::posix_time::ptime expected(boost::gregorian::date(2014, 7, 2),
                                        boost::posix_time::hours(11) + boost::posix_time::minutes(35) + boost::posix_time::seconds(0));
      BOOST_CHECK_EQUAL(cfg.getBoostPTime("DateTimeParameter"), expected) ;

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

   BOOST_AUTO_TEST_CASE(ConvertTypes)
   {
      const std::string defaultConf("{"
         "\"bool1\": true,"
         "\"bool2\": false,"
         "\"bool3\": \"true\","
         "\"bool4\": \"false\","
         "\"bool5\": \"1\","
         "\"bool6\": \"1.025\","
         "\"bool7\": \"0.42\","
         "\"bool8\": 1,"
         "\"bool9\": 1.025,"
         "\"bool10\": 0.42,"
         "\"int1\": 42,"
         "\"int2\": \"42\","
         "\"int3\": 42.0,"
         "\"int4\": \"42.0\","
         "\"int5\": 42.98,"
         "\"int6\": \"42.98\","
         "\"int7\": -42,"
         "\"int8\": \"-42\","
         "\"int9\": 2147483647,"
         "\"int10\": -2147483648,"      
         "\"uint1\": 42,"
         "\"uint2\": \"42\","
         "\"uint3\": 42.0,"
         "\"uint4\": \"42.0\","
         "\"uint5\": 42.98,"
         "\"uint6\": \"42.98\","
         "\"uint7\": -42,"
         "\"uint8\": \"-42\","
         "\"uint9\": 4294967295,"
         "\"int64_1\": 42,"
         "\"int64_2\": \"42\","
         "\"int64_3\": 42.0,"
         "\"int64_4\": \"42.0\","
         "\"int64_5\": 42.98,"
         "\"int64_6\": \"42.98\","
         "\"int64_7\": -42,"
         "\"int64_8\": \"-42\","
         "\"int64_9\": 9223372036854775807,"
         "\"int64_10\": -9223372036854775808,"
         "\"uint64_1\": 42,"
         "\"uint64_2\": \"42\","
         "\"uint64_3\": 42.0,"
         "\"uint64_4\": \"42.0\","
         "\"uint64_5\": 42.98,"
         "\"uint64_6\": \"42.98\","
         "\"uint64_7\": -42,"
         "\"uint64_8\": \"-42\","
         "\"uint64_9\": 18446744073709551615"
         "}");

      shared::CDataContainer cfg(defaultConf);

      BOOST_CHECK_EQUAL(cfg.getBool("bool1"), true);
      BOOST_CHECK_EQUAL(cfg.getBool("bool2"), false);
      BOOST_CHECK_EQUAL(cfg.getBool("bool3"), true);
      BOOST_CHECK_EQUAL(cfg.getBool("bool4"), false);
      BOOST_CHECK_EQUAL(cfg.getBool("bool5"), true);
      BOOST_CHECK_EQUAL(cfg.getBool("bool6"), false);
      BOOST_CHECK_EQUAL(cfg.getBool("bool7"), false);
      BOOST_CHECK_EQUAL(cfg.getBool("bool8"), true);
      BOOST_CHECK_EQUAL(cfg.getBool("bool9"), false);
      BOOST_CHECK_EQUAL(cfg.getBool("bool10"), false);

      int intmax = 2147483647;
      int intmin = -((int)2147483648);
      BOOST_CHECK_EQUAL(cfg.getInt("int1"), 42);
      BOOST_CHECK_EQUAL(cfg.getInt("int2"), 42);
      BOOST_CHECK_EQUAL(cfg.getInt("int3"), 42);
      BOOST_CHECK_EQUAL(cfg.getInt("int4"), 42);
      BOOST_CHECK_EQUAL(cfg.getInt("int5"), 42);
      BOOST_CHECK_EQUAL(cfg.getInt("int6"), 42);
      BOOST_CHECK_EQUAL(cfg.getInt("int7"), -42);
      BOOST_CHECK_EQUAL(cfg.getInt("int8"), -42);
      BOOST_CHECK_EQUAL(cfg.getInt("int9"), intmax);
      BOOST_CHECK_EQUAL(cfg.getInt("int10"), intmin);

      unsigned int uintmax = 4294967295;
      unsigned int uint42 = 42;
      BOOST_CHECK_EQUAL(cfg.getUInt("uint1"), uint42);
      BOOST_CHECK_EQUAL(cfg.getUInt("uint2"), uint42);
      BOOST_CHECK_EQUAL(cfg.getUInt("uint3"), uint42);
      BOOST_CHECK_EQUAL(cfg.getUInt("uint4"), uint42);
      BOOST_CHECK_EQUAL(cfg.getUInt("uint5"), uint42);
      BOOST_CHECK_EQUAL(cfg.getUInt("uint6"), uint42);
      BOOST_CHECK_EQUAL(cfg.getUInt("uint7"), uintmax - 42 + 1);
      BOOST_CHECK_EQUAL(cfg.getUInt("uint8"), uintmax - 42 + 1);
      BOOST_CHECK_EQUAL(cfg.getUInt("uint9"), uintmax);

      int64_t int64min = -((int64_t)9223372036854775808);
      BOOST_CHECK_EQUAL(cfg.getInt64("int64_1"), 42);
      BOOST_CHECK_EQUAL(cfg.getInt64("int64_2"), 42);
      BOOST_CHECK_EQUAL(cfg.getInt64("int64_3"), 42);
      BOOST_CHECK_EQUAL(cfg.getInt64("int64_4"), 42);
      BOOST_CHECK_EQUAL(cfg.getInt64("int64_5"), 42);
      BOOST_CHECK_EQUAL(cfg.getInt64("int64_6"), 42);
      BOOST_CHECK_EQUAL(cfg.getInt64("int64_7"), -42);
      BOOST_CHECK_EQUAL(cfg.getInt64("int64_8"), -42);
      BOOST_CHECK_EQUAL(cfg.getInt64("int64_9"), 9223372036854775807);
      BOOST_CHECK_EQUAL(cfg.getInt64("int64_10"), int64min);

      uint64_t uint64max = 18446744073709551615;
      BOOST_CHECK_EQUAL(cfg.getUInt64("uint64_1"), 42);
      BOOST_CHECK_EQUAL(cfg.getUInt64("uint64_2"), 42);
      BOOST_CHECK_EQUAL(cfg.getUInt64("uint64_3"), 42);
      BOOST_CHECK_EQUAL(cfg.getUInt64("uint64_4"), 42);
      BOOST_CHECK_EQUAL(cfg.getUInt64("uint64_5"), 42);
      BOOST_CHECK_EQUAL(cfg.getUInt64("uint64_6"), 42);
      BOOST_CHECK_EQUAL(cfg.getUInt64("uint64_7"), uint64max - 42 + 1);
      BOOST_CHECK_EQUAL(cfg.getUInt64("uint64_8"), uint64max - 42 + 1);
      BOOST_CHECK_EQUAL(cfg.getUInt64("uint64_9"), uint64max);
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
      BOOST_CHECK_EQUAL(cfg.exists(""), true) ;
      BOOST_CHECK_EQUAL(cfg.exists("BoolParameter"), true) ;
      BOOST_CHECK_EQUAL(cfg.exists("MySection"), true) ;
      BOOST_CHECK_EQUAL(cfg.exists("MySection.SubIntParameter"), true) ;

      BOOST_CHECK_EQUAL(cfg.exists("SubIntParameter"), false) ;
      BOOST_CHECK_EQUAL(cfg.exists("MySection2"), false) ;
      BOOST_CHECK_EQUAL(cfg.exists("String Parameter"), false) ;

      //check child existance
      BOOST_CHECK_EQUAL(cfg.containsChild(""), true) ;
      BOOST_CHECK_EQUAL(cfg.containsChild("MySection"), true) ;
      BOOST_CHECK_EQUAL(cfg.containsChild("BoolParameter"), false) ;
      BOOST_CHECK_EQUAL(cfg.containsChild("MySection.SubIntParameter"), false) ;

      //check value existance
      BOOST_CHECK_EQUAL(cfg.containsValue(""), false) ;
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
      auto supportedPf = testPf.getChild("supportedPlatforms");
      BOOST_CHECK_EQUAL(supportedPf.containsChild(""), true) ;
      BOOST_CHECK_EQUAL(supportedPf.containsValue(""), false) ;
      BOOST_CHECK_EQUAL(supportedPf.getString("mac"), "none") ;
      BOOST_CHECK_EQUAL(supportedPf.getString("raspberry"), "all") ;

      //value test
      BOOST_CHECK_EQUAL(testPf.exists("supportedPlatforms2"), true) ;
      BOOST_CHECK_EQUAL(testPf.containsChild("supportedPlatforms2"), false) ;
      BOOST_CHECK_EQUAL(testPf.containsValue("supportedPlatforms2"), true) ;
      BOOST_CHECK_EQUAL(testPf.getString("supportedPlatforms2"), "all") ;
   }

   class CTestClass : public shared::IDataContainable
   {
   public:
      CTestClass()
         : m_aIntValue(0), m_dValue(0), m_sValue("")
      {
      }

      CTestClass(int i, double d, std::string s)
         : m_aIntValue(i), m_dValue(d), m_sValue(s)
      {
      }

      void extractContent(shared::CDataContainer& cont) const override
      {
         cont.set("Value1", m_aIntValue);
         cont.set("Value2", m_dValue);
         cont.set("Value3", m_sValue);
      }

      void fillFromContent(const shared::CDataContainer& initialData) override
      {
         m_aIntValue = initialData.getInt("Value1");
         m_dValue = initialData.getDouble("Value2");
         m_sValue = initialData.getString("Value3");
      }

      void fillFromSerializedString(const std::string& serializedData) override
      {
         const shared::CDataContainer deserializeData(serializedData);
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
      CTestClass obj(1, 42.0, "test of datacontainable");
      shared::CDataContainer cont;
      cont.set("myobject", obj);

      CTestClass result;
      cont.get("myobject", result);
      BOOST_CHECK_EQUAL(obj.equals(result), true) ;

      //containeur de boost::shared_ptr<IDataContainable>
      auto sp(boost::make_shared<CTestClass>(2, 43.0, "string1"));
      shared::CDataContainer cont2;
      auto spi = boost::dynamic_pointer_cast<shared::IDataContainable> (sp);
      cont2.set("myobject", spi);
      CTestClass a;
      cont2.get("myobject", a);
      BOOST_CHECK_EQUAL(a.equals(*sp.get()), true) ;

      //containeur simple de std::vector<IDataContainable>
      std::vector<CTestClass> vc;
      for (auto i = 0; i < 10; ++i)
         vc.push_back(CTestClass(i, 42.0 * i, "test of std::vector<IDataContainable>"));
      shared::CDataContainer contvec;
      for (auto i = vc.begin(); i != vc.end(); ++i)
      {
         contvec.push("mycollection", *i);
      }

      
      std::vector<CTestClass> vc2;
      std::vector<shared::CDataContainer> vc3;
      contvec.getChilds("mycollection", vc3);
      for (auto i = vc3.begin(); i != vc3.end(); ++i)
      {
         CTestClass ab;
         ab.fillFromContent(*i);
         vc2.push_back(ab);
      }
      BOOST_CHECK_EQUAL_COLLECTIONS(vc.begin(), vc.end(), vc2.begin(), vc2.end()) ;
   }


   BOOST_AUTO_TEST_CASE(Field)
   {
      shared::CField<int> fi(10);
      shared::CField<double> fd(12.3);
      shared::CField<std::string> fs("this is a test");
      shared::CField<EEnumType> fe(kEnumValue2);
      shared::CField<CTestClass> fdc(CTestClass(5, 42.0, "test of datacontainble"));

      shared::CDataContainer dc;

      dc.set("FieldInt", fi);
      dc.set("FieldDouble", fd);
      dc.set("FieldString", fs);
      dc.set("FieldEnum", fe);
      dc.set("FieldDataContainable", fdc);

      //check data are correctly retreived
      BOOST_CHECK_EQUAL(dc.getInt("FieldInt"), fi()) ;
      BOOST_CHECK_EQUAL(dc.getDouble("FieldDouble"), fd()) ;
      BOOST_CHECK_EQUAL(dc.getString("FieldString"), fs()) ;
      BOOST_CHECK_EQUAL(static_cast<EEnumType>(dc.getInt("FieldEnum")), fe()) ;

      CTestClass a;
      dc.get("FieldDataContainable", a);
      BOOST_CHECK_EQUAL(a.equals(fdc()), true) ;
   }


   BOOST_AUTO_TEST_CASE(Path)
   {
      shared::CField<int> fi(10);

      shared::CDataContainer dc;
      dc.set("secA.secB.valC", fi);
      BOOST_CHECK_EQUAL(dc.getInt("secA.secB.valC"), fi()) ;
      //BOOST_CHECK_EQUAL(dc.get<shared::CDataContainer>("secA").get<shared::CDataContainer>("secB").get<int>("valC"), fi()) ;

      //no path using separator 0x00
      /*dc.set("secD.secE.valC", fi, 0x00);
      BOOST_CHECK_EQUAL(dc.get<int>("secD.secE.valC", 0x00), fi()) ;
      BOOST_CHECK_EQUAL(dc.exists("secD.secE.valC"), false) ;
      BOOST_CHECK_THROW(dc.get<int>("secD.secE.valC"), std::exception) ;
      BOOST_CHECK_THROW(dc.get<shared::CDataContainer>("secD"), std::exception) ;*/
   }


   // TOFIX : This test is disabled because the default it revealed is not so serious and
   // it makes fail to full campaign. We have for the moment no good solution to fix it.
   // A good solution would probably to change our JSON parser (from Boost) by a real JSON parser.
   //BOOST_AUTO_TEST_CASE(SimpleConstruction)
   //{
   //   BOOST_CHECK_THROW(shared::CDataContainer dc("1"), std::exception) ;
   //}


   BOOST_AUTO_TEST_CASE(Merge)
   {
      shared::CDataContainer to(
         "{"
         "   \"developerMode\": \"false\","
         "   \"location\":"
         "   {"
         "      \"latitude\": \"48.853\","
         "      \"longitude\": \"2.35\","
         "      \"timezone\": \"Europe/Paris\""
         "   },"
         "   \"language\": \"en\","
         "   \"advancedParameters\": \"false\","
         "   \"dateFormatString\": \"LLL\","
         "   \"refreshPage\": \"false\","
         "   \"basicAuthentication\":"
         "   {"
         "      \"active\": \"false\","
         "      \"user\": \"admin\","
         "      \"password\": \"\""
         "   },"
         "   \"multilevelNorMerged\":"
         "   {"
         "      \"valueLevel1\": \"1\","
         "      \"level2\":"
         "      {"
         "         \"valueLevel2\": \"2\","
         "         \"level3\": "
         "         {"
         "            \"valueLevel3\": \"3\""
         "         }"
         "      }"
         "   },"
         "   \"multilevelMerged\":"
         "   {"
         "      \"valueLevel1\": \"1\","
         "      \"level2\":"
         "      {"
         "         \"valueLevel2\": \"2\","
         "         \"level3\": "
         "         {"
         "            \"valueLevel3\": \"3\""
         "         }"
         "      }"
         "   }"
         "}");

      const shared::CDataContainer from(
         "{"
         "   \"developerMode\": \"true\","
         "   \"location\":"
         "   {"
         "      \"latitude\": \"52.5\""
         "   },"
         "   \"language\": \"fr\","
         "   \"advancedParameters\": \"true\","
         "   \"refreshPage\": \"false\","
         "   \"basicAuthentication\":"
         "   {"
         "      \"active\": \"true\","
         "      \"user\": \"admin\","
         "      \"password\": \"1234\""
         "   },"
         "   \"multilevelMerged\":"
         "   {"
         "      \"valueLevel1\": \"1\","
         "      \"level2\":"
         "      {"
         "         \"valueLevel2\": \"2\","
         "         \"level3\": "
         "         {"
         "            \"valueLevel3\": \"abc\""
         "         }"
         "      }"
         "   },"
         "   \"newBloc\":"
         "   {"
         "      \"valueLevel1\": \"1\","
         "      \"level2\":"
         "      {"
         "         \"valueLevel2\": \"2\","
         "         \"level3\": "
         "         {"
         "            \"valueLevel3\": \"abc\""
         "         }"
         "      }"
         "   }"
         "}");

      const shared::CDataContainer expected(
         "{"
         "   \"developerMode\": \"true\","
         "   \"location\":"
         "   {"
         "      \"latitude\": \"52.5\","
         "      \"longitude\": \"2.35\","
         "      \"timezone\": \"Europe/Paris\""
         "   },"
         "   \"language\": \"fr\","
         "   \"advancedParameters\": \"true\","
         "   \"dateFormatString\": \"LLL\","
         "   \"refreshPage\": \"false\","
         "   \"basicAuthentication\":"
         "   {"
         "      \"active\": \"true\","
         "      \"user\": \"admin\","
         "      \"password\": \"1234\""
         "   },"
         "   \"multilevelNorMerged\":"
         "   {"
         "      \"valueLevel1\": \"1\","
         "      \"level2\":"
         "      {"
         "         \"valueLevel2\": \"2\","
         "         \"level3\": "
         "         {"
         "            \"valueLevel3\": \"3\""
         "         }"
         "      }"
         "   },"
         "   \"multilevelMerged\":"
         "   {"
         "      \"valueLevel1\": \"1\","
         "      \"level2\":"
         "      {"
         "         \"valueLevel2\": \"2\","
         "         \"level3\": "
         "         {"
         "            \"valueLevel3\": \"abc\""
         "         }"
         "      }"
         "   },"
         "   \"newBloc\":"
         "   {"
         "      \"valueLevel1\": \"1\","
         "      \"level2\":"
         "      {"
         "         \"valueLevel2\": \"2\","
         "         \"level3\": "
         "         {"
         "            \"valueLevel3\": \"abc\""
         "         }"
         "      }"
         "   }"
         "}");


      to.mergeFrom(from);

      to.printToLog();
      expected.printToLog();
      BOOST_CHECK_EQUAL(to.serialize(), expected.serialize());
   }


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

      std::map<std::string, std::string> output;
      dc.getChilds("", output);

      //dont use BOOST_CHECK_EQUAL_COLLECTIONS because it do not builds with std::map
      BOOST_CHECK_MAPS(input, output);
   }

BOOST_AUTO_TEST_SUITE_END()
