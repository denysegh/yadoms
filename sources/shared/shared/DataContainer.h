#pragma once

#include <shared/Export.h>
#include <shared/exception/OutOfRange.hpp>
#include <shared/exception/InvalidParameter.hpp>
#include "serialization/IDataSerializable.h"
#include "serialization/IDataFileSerializable.h"
#include "IDataContainable.h"
#include "enumeration/IExtendedEnum.h"
#include "Field.hpp"

#define RAPIDJSON_HAS_STDSTRING 1
#define RAPIDJSON_ASSERT(x) {if(!(x)) throw new shared::exception::CException("Assert");}
#include <shared/rapidjson/document.h>

namespace shared
{

   class YADOMS_SHARED_EXPORT CDataContainer /*: public serialization::IDataSerializable, public serialization::IDataFileSerializable /*, public IDataContainable*/
   {
   public:
      //--------------------------------------------------------------
      //
      //
      //
      // Constructors & destructor
      //
      //
      //
      //--------------------------------------------------------------

      //--------------------------------------------------------------
      /// \brief		Constructor. Empty data
      //--------------------------------------------------------------
      CDataContainer();

      //--------------------------------------------------------------
      /// \brief		Constructor. 
      /// \param [in] initialData    Initial data for this container (will be deserialized)
      //--------------------------------------------------------------
      explicit CDataContainer(const std::string & initialData);     
      
      //--------------------------------------------------------------
      /// \brief		Constructor. 
      /// \param [in] initialData    Initial map data
      //--------------------------------------------------------------
      explicit CDataContainer(const std::map<std::string, std::string> & initialData);


      //--------------------------------------------------------------
      /// \brief		Constructor. 
      /// \param [in] initialData    Initial data for this container (will be deserialized)
      //--------------------------------------------------------------
      CDataContainer(const CDataContainer & initialData);

      //--------------------------------------------------------------
      /// \brief		Constructor. 
      /// \param [in] initialData    Initial data for this container (will be deserialized)
      //--------------------------------------------------------------
      CDataContainer(boost::shared_ptr<rapidjson::Value> initialData);

   private:
      //--------------------------------------------------------------
      /// \brief		Constructor. 
      /// \param [in] initialData    Initial data for this container (will be deserialized)
      //--------------------------------------------------------------
      CDataContainer(rapidjson::Value &initialData);

   public:
      //--------------------------------------------------------------
      /// \brief			Destructor
      /// \return    	void
      //--------------------------------------------------------------
      virtual ~CDataContainer();


      //--------------------------------------------------------------
      /// \brief	    Type representing authorized enum values list
      //--------------------------------------------------------------
      typedef std::map<std::string, unsigned int> EnumValuesNames;

      bool getBool(const std::string &parameterName, const char pathCar = '.') const;
      int getInt(const std::string &parameterName, const char pathCar = '.') const;
      int64_t getInt64(const std::string &parameterName, const char pathCar = '.') const;
      unsigned int getUInt(const std::string &parameterName, const char pathCar = '.') const;
      uint64_t getUInt64(const std::string &parameterName, const char pathCar = '.') const;
      float getFloat(const std::string &parameterName, const char pathCar = '.') const;
      double getDouble(const std::string &parameterName, const char pathCar = '.') const;
      std::string getString(const std::string &parameterName, const char pathCar = '.') const;
      boost::posix_time::ptime getBoostPTime(const std::string &parameterName, const char pathCar = '.') const;

      int getEnumValue(const std::string& parameterName, const EnumValuesNames& valuesNames, const char pathCar = '.') const;
      void get(const std::string &parameterName, shared::IDataContainable & value, const char pathCar = '.');
      void get(const std::string &parameterName, boost::shared_ptr<shared::IDataContainable> & value, const char pathCar = '.');

      
      bool getBoolWithDefault(const std::string &parameterName, bool defaultValue, const char pathCar = '.') const;
      int getIntWithDefault(const std::string &parameterName, int defaultValue, const char pathCar = '.') const;
      int64_t getInt64WithDefault(const std::string &parameterName, int64_t defaultValue, const char pathCar = '.') const;
      unsigned int getUIntWithDefault(const std::string &parameterName, unsigned int defaultValue, const char pathCar = '.') const;
      uint64_t getUInt64WithDefault(const std::string &parameterName, uint64_t defaultValue, const char pathCar = '.') const;
      float getFloatWithDefault(const std::string &parameterName, float defaultValue, const char pathCar = '.') const;
      double getDoubleWithDefault(const std::string &parameterName, double defaultValue, const char pathCar = '.') const;
      std::string getStringWithDefault(const std::string &parameterName, const std::string & defaultValue, const char pathCar = '.') const;
      boost::posix_time::ptime getBoostPTimeWithDefault(const std::string &parameterName, const boost::posix_time::ptime & defaultValue, const char pathCar = '.') const;
      int getEnumValueWithDefault(const std::string& parameterName, const EnumValuesNames& valuesNames, int defaultValue, const char pathCar = '.') const;

      CDataContainer getChild(const std::string &parameterName, const char pathCar = '.') const;
      void getChilds(const std::string &parameterName, std::vector<CDataContainer> & result, const char pathCar = '.') const;
      void getChilds(const std::string &parameterName, std::vector<bool> & result, const char pathCar = '.') const;
      void getChilds(const std::string &parameterName, std::vector<int> & result, const char pathCar = '.') const;
      void getChilds(const std::string &parameterName, std::vector<int64_t> & result, const char pathCar = '.') const;
      void getChilds(const std::string &parameterName, std::vector<unsigned int> & result, const char pathCar = '.') const;
      void getChilds(const std::string &parameterName, std::vector<uint64_t> & result, const char pathCar = '.') const;
      void getChilds(const std::string &parameterName, std::vector<float> & result, const char pathCar = '.') const;
      void getChilds(const std::string &parameterName, std::vector<double> & result, const char pathCar = '.') const;
      void getChilds(const std::string &parameterName, std::vector<std::string> & result, const char pathCar = '.') const;
      void getChilds(const std::string &parameterName, std::vector<boost::posix_time::ptime> & result, const char pathCar = '.') const;
      void getChilds(const std::string &parameterName, std::map<std::string, std::string> & result, const char pathCar = '.') const;

      std::map<std::string, std::string> getAsMap(const std::string &parameterName, const char pathCar = '.') const;

      std::vector<std::string> getKeys() const;
      //--------------------------------------------------------------
      /// \brief			Set a null object
      //--------------------------------------------------------------
      void setNull(const std::string &parameterName, const char pathCar = '.');

      //--------------------------------------------------------------
      /// \brief			Set value (replace or create)
      //--------------------------------------------------------------
      void set(const std::string &parameterName, bool value, const char pathCar = '.');
      void set(const std::string &parameterName, int value, const char pathCar = '.');
      void set(const std::string &parameterName, unsigned int value, const char pathCar = '.');
      void set(const std::string &parameterName, int64_t value, const char pathCar = '.');
      void set(const std::string &parameterName, uint64_t value, const char pathCar = '.');
      void set(const std::string &parameterName, float value, const char pathCar = '.');
      void set(const std::string &parameterName, double value, const char pathCar = '.');
      void set(const std::string &parameterName, const std::string & value, const char pathCar = '.');
      void set(const std::string &parameterName, const char * value, const char pathCar = '.');
      void set(const std::string &parameterName, const CDataContainer & value, const char pathCar = '.');
      void set(const std::string &parameterName, const shared::IDataContainable & value, const char pathCar = '.');
      void set(const std::string &parameterName, boost::shared_ptr<shared::IDataContainable> & value, const char pathCar = '.');
      void set(const std::string &parameterName, boost::posix_time::ptime value, const char pathCar = '.');
      void set(const std::string &parameterName, const std::vector<bool> & values, const char pathCar = '.');
      void set(const std::string &parameterName, const std::vector<int> & values, const char pathCar = '.');
      void set(const std::string &parameterName, const std::vector<unsigned int> & values, const char pathCar = '.');
      void set(const std::string &parameterName, const std::vector<int64_t> & values, const char pathCar = '.');
      void set(const std::string &parameterName, const std::vector<uint64_t> & values, const char pathCar = '.');
      void set(const std::string &parameterName, const std::vector<float> & values, const char pathCar = '.');
      void set(const std::string &parameterName, const std::vector<double> & values, const char pathCar = '.');
      void set(const std::string &parameterName, const std::vector<std::string> & values, const char pathCar = '.');
      void set(const std::string &parameterName, const std::vector<CDataContainer> & values, const char pathCar = '.');
      void set(const std::string &parameterName, const std::vector<shared::IDataContainable> & values, const char pathCar = '.');
      void set(const std::string &parameterName, const std::vector<boost::shared_ptr<shared::IDataContainable>> & values, const char pathCar = '.');
      void set(const std::string &parameterName, const std::vector<boost::posix_time::ptime> & values, const char pathCar = '.');
      
      //--------------------------------------------------------------
      /// \brief			Push value to an array (append or create)
      //--------------------------------------------------------------
      void push(const std::string &parameterName, bool value, const char pathCar = '.');
      void push(const std::string &parameterName, int value, const char pathCar = '.');
      void push(const std::string &parameterName, unsigned int value, const char pathCar = '.');
      void push(const std::string &parameterName, int64_t value, const char pathCar = '.');
      void push(const std::string &parameterName, uint64_t value, const char pathCar = '.');
      void push(const std::string &parameterName, float value, const char pathCar = '.');
      void push(const std::string &parameterName, double value, const char pathCar = '.');
      void push(const std::string &parameterName, const std::string & value, const char pathCar = '.');
      void push(const std::string &parameterName, const char * value, const char pathCar = '.');
      void push(const std::string &parameterName, const CDataContainer & value, const char pathCar = '.');
      void push(const std::string &parameterName, const shared::IDataContainable & value, const char pathCar = '.');
      void push(const std::string &parameterName, boost::shared_ptr<shared::IDataContainable> & value, const char pathCar = '.');
      void push(const std::string &parameterName, std::vector<bool> & values, const char pathCar = '.');
      void push(const std::string &parameterName, std::vector<int> & values, const char pathCar = '.');
      void push(const std::string &parameterName, std::vector<unsigned int> & values, const char pathCar = '.');
      void push(const std::string &parameterName, std::vector<int64_t> & values, const char pathCar = '.');
      void push(const std::string &parameterName, std::vector<uint64_t> & values, const char pathCar = '.');
      void push(const std::string &parameterName, std::vector<float> & values, const char pathCar = '.');
      void push(const std::string &parameterName, std::vector<double> & values, const char pathCar = '.');
      void push(const std::string &parameterName, std::vector<std::string> & values, const char pathCar = '.');
      void push(const std::string &parameterName, std::vector<CDataContainer> & values, const char pathCar = '.');
      void push(const std::string &parameterName, std::vector<shared::IDataContainable> & values, const char pathCar = '.');
      void push(const std::string &parameterName, std::vector<boost::shared_ptr<shared::IDataContainable>> & values, const char pathCar = '.');

      //--------------------------------------------------------------
      /// \brief		Check if the container is empty
      /// \return    true if contanier is empty
      //--------------------------------------------------------------
      bool empty() const;

      bool exists(const std::string &parameterName, const char pathCar = '.') const;

      bool containsChild(const std::string& parameterName, const char pathCar = '.') const;
      bool containsValue(const std::string& parameterName, const char pathCar = '.') const;

      void printToLog(std::ostream& os = std::cout) const;
      //--------------------------------------------------------------
      /// \brief		Output operator (write the serialized container)
      /// \param [in] os  The stream to write
      /// \return   The stream
      //--------------------------------------------------------------
      std::ostream& operator<<(std::ostream& os);

      //--------------------------------------------------------------
      /// \brief		Input operator (read a serialized container)
      /// \param [in] is  The stream to read
      /// \return   The stream
      //--------------------------------------------------------------
      std::istream& operator>>(std::istream& is);

      //--------------------------------------------------------------
      /// \brief		Output operator (write the serialized container)
      /// \param [in] os  The stream to write
      /// \param [in] dt  The input data container
      /// \return   The stream
      //--------------------------------------------------------------
      friend YADOMS_SHARED_EXPORT std::ostream& operator<<(std::ostream& os, const CDataContainer& dt);

      //--------------------------------------------------------------
      /// \brief		Input operator (read a serialized container)
      /// \param [in] is  The stream to read
      /// \param [in] dt  The output data container
      /// \return   The stream
      //--------------------------------------------------------------
      friend YADOMS_SHARED_EXPORT std::istream& operator>>(std::istream& is, CDataContainer& dt);

      std::string serialize(bool prettyOutput = false) const;
      void deserialize(const std::string & data);
      void serializeToFile(const std::string & filename) const;
      void deserializeFromFile(const std::string & file);
      //--------------------------------------------------------------
      /// \brief		Equality operator
      /// \param [in] rhs  The container to compare with
      /// \return   true if the container equals to this instance
      //--------------------------------------------------------------
      bool operator ==(const CDataContainer &rhs) const;

      //--------------------------------------------------------------
      /// \brief		Difference operator
      /// \param [in] rhs  The container to compare with
      /// \return   true if the container is different to this instance
      //--------------------------------------------------------------
      bool operator !=(const CDataContainer &rhs) const;

      //--------------------------------------------------------------
      /// \brief		Affectation operator
      /// \param [in] rhs  The container to copy
      /// \return   Reference to this instance
      //--------------------------------------------------------------
      CDataContainer & operator=(const CDataContainer &rhs);

      //--------------------------------------------------------------
      /// \brief		Affectation operator
      /// \param [in] rhs  A serialized string
      /// \return   Reference to this instance
      //--------------------------------------------------------------
      CDataContainer & operator=(const std::string &rhs);
      
      //--------------------------------------------------------------
      /// \brief	    Merge this container from another one
      /// \param [in] from source container
      /// \desc       Add non-exisiting (or replace existing) values of "from" container into this container
      ///             Values of this container not present in "from" container will be kept
      //--------------------------------------------------------------
      void mergeFrom(const CDataContainer& from);

      void initializeWith(const CDataContainer &rhs);
      void initializeWith(const rapidjson::Value &rhs);
      void initializeWith(boost::shared_ptr<rapidjson::Value> rhs);

   private:
      std::string generatePath(const std::string & parameterName, const char pathChar) const;
      rapidjson::Value* find(const std::string & parameterName, const char pathChar) const;
      rapidjson::Value* findOrCreateArray(const std::string & parameterName, const char pathChar);
      static void mergeObjects(rapidjson::Value &dstObject, rapidjson::Value &srcObject, rapidjson::Document::AllocatorType &allocator);

      
   public:
      //--------------------------------------------------------------
      /// \brief		An empty container which could be used as default method parameter
      //--------------------------------------------------------------
      const static CDataContainer EmptyContainer;

   private:
      rapidjson::Document * get() const { return (rapidjson::Document *) &m_document; }
      //--------------------------------------------------------------
      /// \brief		Internal container
      //--------------------------------------------------------------
      rapidjson::Document m_document;

   };

   


} // namespace shared
