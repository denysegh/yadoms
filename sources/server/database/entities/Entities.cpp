#include "stdafx.h"
#include "Entities.h"
#include <shared/currentTime/Provider.h>

namespace database
{
   namespace entities
   {
      DECLARE_ENUM_IMPLEMENTATION(ESecurityAccess,
         ((None))
         ((Admin))
         ((User))
      )

      DECLARE_ENUM_IMPLEMENTATION(EEventType,
         ((Unload))
         ((Load))
         ((Crash))
         ((Info))
         ((Error))
      )

      DECLARE_ENUM_IMPLEMENTATION(EPluginCategory,
         ((System))
         ((User))
      )

      DECLARE_ENUM_IMPLEMENTATION(EAcquisitionSummaryType,
         ((Hour))
         ((Day))
         ((Month))
         ((Year))
      )


      DECLARE_ENUM_IMPLEMENTATION(ESystemEventCode,
         ((RuleFailed))
         ((ThreadFailed))
         ((TaskFailed))
         ((PluginDisabled))
         ((PluginCrash))
         ((YadomsCrash))
         ((DefaultCode))
         ((Started))
         ((Stopped))
         ((Updated))
         ((UpdateAvailable))
      )

      DECLARE_ENUM_IMPLEMENTATION(ERuleState,
         ((Stopped))
         ((Running))
         ((Error))
      )

      CPlugin::CPlugin() 
         : Id(0), DisplayName(""), Type(""), Configuration(shared::CDataContainer()), AutoStart(true), Category(database::entities::EPluginCategory::kSystem) {
      }
      CPlugin::~CPlugin() {
      }
      void CPlugin::extractContent(shared::CDataContainer & containerToFill) const {
         containerToFill.set("id", Id());
         containerToFill.set("displayName", DisplayName());
         containerToFill.set("type", Type());
         containerToFill.set("configuration", Configuration());
         containerToFill.set("autoStart", AutoStart());
         containerToFill.set("category", Category().toString());
      }
      void CPlugin::fillFromContent(const shared::CDataContainer & initialData) {
         if (initialData.exists("id")) Id = initialData.getInt("id");
         if (initialData.exists("displayName")) DisplayName = initialData.getString("displayName");
         if (initialData.exists("type")) Type = initialData.getString("type");
         if (initialData.exists("configuration")) Configuration = initialData.getChild("configuration");
         if (initialData.exists("autoStart")) AutoStart = initialData.getBool("autoStart");
         if (initialData.exists("category")) Category().fromString(initialData.getString("category"));
      }
      void CPlugin::fillFromSerializedString(const std::string & serializedData) {
         shared::CDataContainer deserializeData(serializedData);
         fillFromContent(deserializeData);
      }
         

      CConfiguration::CConfiguration() : Section(""), Value(""), LastModificationDate(shared::currentTime::Provider().now()) {
      }
      CConfiguration::~CConfiguration() {
      }
      void CConfiguration::extractContent(shared::CDataContainer & containerToFill) const {
         containerToFill.set("section", Section());
         containerToFill.set("value", Value());
         containerToFill.set("lastModificationDate", LastModificationDate());
      }
      void CConfiguration::fillFromContent(const shared::CDataContainer & initialData) {
         if (initialData.exists("section")) Section = initialData.getString("section");
         if (initialData.exists("value")) Value = initialData.getString("value");
         if (initialData.exists("lastModificationDate")) LastModificationDate = initialData.getBoostPTime("lastModificationDate");
      }
      void CConfiguration::fillFromSerializedString(const std::string & serializedData) {
         shared::CDataContainer deserializeData(serializedData);
         fillFromContent(deserializeData);
      };


      CPage::CPage() 
         : Id(0), Name(""), PageOrder(0) 
      {
      }
      CPage::~CPage() 
      {
      }
      void CPage::extractContent(shared::CDataContainer & containerToFill) const 
      {
         containerToFill.set("id", Id());
         containerToFill.set("name", Name());
         containerToFill.set("pageOrder", PageOrder());
      }
      void CPage::fillFromContent(const shared::CDataContainer & initialData) 
      {
         if (initialData.exists("id")) Id = initialData.getInt("id");
         if (initialData.exists("name")) Name = initialData.getString("name");
         if (initialData.exists("pageOrder")) PageOrder = initialData.getInt("pageOrder");
      }
      void CPage::fillFromSerializedString(const std::string & serializedData) 
      {
         shared::CDataContainer deserializeData(serializedData);
         fillFromContent(deserializeData);
      }
      

      CWidget::CWidget() : Id(0), IdPage(0), Type(""), SizeX(1), SizeY(1), Position(0), Title(""), Configuration(shared::CDataContainer()) {
      }
      CWidget::~CWidget() {
      }
      void CWidget::extractContent(shared::CDataContainer & containerToFill) const {
         containerToFill.set("id", Id());
         containerToFill.set("idPage", IdPage());
         containerToFill.set("type", Type());
         containerToFill.set("sizeX", SizeX());
         containerToFill.set("sizeY", SizeY());
         containerToFill.set("position", Position());
         containerToFill.set("title", Title());
         containerToFill.set("configuration", Configuration());
      }
      void CWidget::fillFromContent(const shared::CDataContainer & initialData) {
         if (initialData.exists("id")) Id = initialData.getInt("id");
         if (initialData.exists("idPage")) IdPage = initialData.getInt("idPage");
         if (initialData.exists("type")) Type = initialData.getString("type");
         if (initialData.exists("sizeX")) SizeX = initialData.getInt("sizeX");
         if (initialData.exists("sizeY")) SizeY = initialData.getInt("sizeY");
         if (initialData.exists("position")) Position = initialData.getInt("position");
         if (initialData.exists("title")) Title = initialData.getString("title");
         if (initialData.exists("configuration")) Configuration = initialData.getChild("configuration");
      }
      void CWidget::fillFromSerializedString(const std::string & serializedData) {
         shared::CDataContainer deserializeData(serializedData);
         fillFromContent(deserializeData);
      }
      
      CPluginEventLogger::CPluginEventLogger() : Id(0), EventDate(shared::currentTime::Provider().now()), PluginName(""), PluginVersion(""), EventType(database::entities::EEventType::kUnload), Message("") {
      }
      CPluginEventLogger::~CPluginEventLogger() {
      }
      void CPluginEventLogger::extractContent(shared::CDataContainer & containerToFill) const {
         containerToFill.set("id", Id());
         containerToFill.set("eventDate", EventDate());
         containerToFill.set("pluginName", PluginName());
         containerToFill.set("pluginVersion", PluginVersion());
         containerToFill.set("eventType", EventType().toString());
         containerToFill.set("message", Message());
      }
      void CPluginEventLogger::fillFromContent(const shared::CDataContainer & initialData) {
         if (initialData.exists("id")) Id = initialData.getInt("id");
         if (initialData.exists("eventDate")) EventDate = initialData.getBoostPTime("eventDate");
         if (initialData.exists("pluginName")) PluginName = initialData.getString("pluginName");
         if (initialData.exists("pluginVersion")) PluginVersion = initialData.getString("pluginVersion");
         if (initialData.exists("eventType")) EventType().fromString(initialData.getString("eventType"));
         if (initialData.exists("message")) Message = initialData.getString("message");
      }
      void CPluginEventLogger::fillFromSerializedString(const std::string & serializedData) {
         shared::CDataContainer deserializeData(serializedData);
         fillFromContent(deserializeData);
      }


      CEventLogger::CEventLogger() : Id(0), Date(shared::currentTime::Provider().now()), Code(database::entities::ESystemEventCode::kDefaultCode), Who(""), What("") {
      }
      CEventLogger::~CEventLogger() {
      }
      void CEventLogger::extractContent(shared::CDataContainer & containerToFill) const {
         containerToFill.set("id", Id());
         containerToFill.set("date", Date());
         containerToFill.set("code", Code().toString());
         containerToFill.set("who", Who());
         containerToFill.set("what", What());
      }
      void CEventLogger::fillFromContent(const shared::CDataContainer & initialData) {
         if (initialData.exists("id")) Id = initialData.getInt("id");
         if (initialData.exists("date")) Date = initialData.getBoostPTime("date");
         if (initialData.exists("code")) Code().fromString(initialData.getString("code"));
         if (initialData.exists("who")) Who = initialData.getString("who");
         if (initialData.exists("what")) What = initialData.getString("what");
      }
      void CEventLogger::fillFromSerializedString(const std::string & serializedData) {
         shared::CDataContainer deserializeData(serializedData);
         fillFromContent(deserializeData);
      }
      ;


      CDevice::CDevice() : Id(0), PluginId(0), Name(""), FriendlyName(""), Model(""), Details(shared::CDataContainer()), Configuration(shared::CDataContainer()), Type(""), Blacklist(false) {
      }
      CDevice::~CDevice() {
      }
      void CDevice::extractContent(shared::CDataContainer & containerToFill) const {
         containerToFill.set("id", Id());
         containerToFill.set("pluginId", PluginId());
         containerToFill.set("name", Name());
         containerToFill.set("friendlyName", FriendlyName());
         containerToFill.set("model", Model());
         containerToFill.set("details", Details());
         containerToFill.set("configuration", Configuration());
         containerToFill.set("type", Type());
         containerToFill.set("blacklist", Blacklist());
      }
      void CDevice::fillFromContent(const shared::CDataContainer & initialData) {
         if (initialData.exists("id")) Id = initialData.getInt("id");
         if (initialData.exists("pluginId")) PluginId = initialData.getInt("pluginId");
         if (initialData.exists("name")) Name = initialData.getString("name");
         if (initialData.exists("friendlyName")) FriendlyName = initialData.getString("friendlyName");
         if (initialData.exists("model")) Model = initialData.getString("model");
         if (initialData.exists("details")) Details = initialData.getChild("details");
         if (initialData.exists("configuration")) Configuration = initialData.getChild("configuration");
         if (initialData.exists("type")) Type = initialData.getString("type");
         if (initialData.exists("blacklist")) Blacklist = initialData.getBool("blacklist");
      }
      void CDevice::fillFromSerializedString(const std::string & serializedData) {
         shared::CDataContainer deserializeData(serializedData);
         fillFromContent(deserializeData);
      }
      CKeyword::CKeyword() : Id(0), DeviceId(0), CapacityName(""), AccessMode(shared::plugin::yPluginApi::EKeywordAccessMode::kNoAccess), Name(""), FriendlyName(""), Type(shared::plugin::yPluginApi::EKeywordDataType::kString), Units(""), TypeInfo(shared::CDataContainer()), Measure(shared::plugin::yPluginApi::historization::EMeasureType::kAbsolute), Details(shared::CDataContainer()), Blacklist(false), LastAcquisitionValue(""), LastAcquisitionDate(shared::currentTime::Provider().now()) {
      }
      CKeyword::~CKeyword() {
      }
      void CKeyword::extractContent(shared::CDataContainer & containerToFill) const {
         containerToFill.set("id", Id());
         containerToFill.set("deviceId", DeviceId());
         containerToFill.set("capacityName", CapacityName());
         containerToFill.set("accessMode", AccessMode().toString());
         containerToFill.set("name", Name());
         containerToFill.set("friendlyName", FriendlyName());
         containerToFill.set("type", Type().toString());
         containerToFill.set("units", Units());
         containerToFill.set("typeInfo", TypeInfo());
         containerToFill.set("measure", Measure().toString());
         containerToFill.set("details", Details());
         containerToFill.set("blacklist", Blacklist());
         containerToFill.set("lastAcquisitionValue", LastAcquisitionValue());
         containerToFill.set("lastAcquisitionDate", LastAcquisitionDate());
      }
      void CKeyword::fillFromContent(const shared::CDataContainer & initialData) {
         if (initialData.exists("id")) Id = initialData.getInt("id");
         if (initialData.exists("deviceId")) DeviceId = initialData.getInt("deviceId");
         if (initialData.exists("capacityName")) CapacityName = initialData.getString("capacityName");
         if (initialData.exists("accessMode")) AccessMode().fromString(initialData.getString("accessMode"));
         if (initialData.exists("name")) Name = initialData.getString("name");
         if (initialData.exists("friendlyName")) FriendlyName = initialData.getString("friendlyName");
         if (initialData.exists("type")) Type().fromString(initialData.getString("type"));
         if (initialData.exists("units")) Units = initialData.getString("units");
         if (initialData.exists("typeInfo")) TypeInfo = initialData.getChild("typeInfo");
         if (initialData.exists("measure")) Measure().fromString(initialData.getString("measure"));
         if (initialData.exists("details")) Details = initialData.getChild("details");
         if (initialData.exists("blacklist")) Blacklist = initialData.getBool("blacklist");
         if (initialData.exists("lastAcquisitionValue")) LastAcquisitionValue = initialData.getString("lastAcquisitionValue");
         if (initialData.exists("lastAcquisitionDate")) LastAcquisitionDate = initialData.getBoostPTime("lastAcquisitionDate");
      }
      void CKeyword::fillFromSerializedString(const std::string & serializedData) {
         shared::CDataContainer deserializeData(serializedData);
         fillFromContent(deserializeData);
      }
      
      CAcquisition::CAcquisition() : Date(shared::currentTime::Provider().now()), KeywordId(0), Value("") {
      }
      CAcquisition::~CAcquisition() {
      }
      void CAcquisition::extractContent(shared::CDataContainer & containerToFill) const {
         containerToFill.set("date", Date());
         containerToFill.set("keywordId", KeywordId());
         containerToFill.set("value", Value());
      }
      void CAcquisition::fillFromContent(const shared::CDataContainer & initialData) {
         if (initialData.exists("date")) Date = initialData.getBoostPTime("date");
         if (initialData.exists("keywordId")) KeywordId = initialData.getInt("keywordId");
         if (initialData.exists("value")) Value = initialData.getString("value");
      }
      void CAcquisition::fillFromSerializedString(const std::string & serializedData) {
         shared::CDataContainer deserializeData(serializedData);
         fillFromContent(deserializeData);
      }
      ;
      CAcquisitionSummary::CAcquisitionSummary() : Type(database::entities::EAcquisitionSummaryType::kHour), Date(shared::currentTime::Provider().now()), KeywordId(0), Avg(""), Min(""), Max("") {
      }
      CAcquisitionSummary::~CAcquisitionSummary() {
      }
      void CAcquisitionSummary::extractContent(shared::CDataContainer & containerToFill) const {
         containerToFill.set("type", Type().toString());
         containerToFill.set("date", Date());
         containerToFill.set("keywordId", KeywordId());
         containerToFill.set("avg", Avg());
         containerToFill.set("min", Min());
         containerToFill.set("max", Max());
      }
      void CAcquisitionSummary::fillFromContent(const shared::CDataContainer & initialData) {
         if (initialData.exists("type")) Type().fromString(initialData.getString("type"));
         if (initialData.exists("date")) Date = initialData.getBoostPTime("date");
         if (initialData.exists("keywordId")) KeywordId = initialData.getInt("keywordId");
         if (initialData.exists("avg")) Avg = initialData.getString("avg");
         if (initialData.exists("min")) Min = initialData.getString("min");
         if (initialData.exists("max")) Max = initialData.getString("max");
      }
      void CAcquisitionSummary::fillFromSerializedString(const std::string & serializedData) {
         shared::CDataContainer deserializeData(serializedData);
         fillFromContent(deserializeData);
      }
      ;
      CRule::CRule() : Id(0), Name(""), Description(""), Interpreter(""), Editor(""), Model(""), Content(""), Configuration(shared::CDataContainer()), AutoStart(true), State(database::entities::ERuleState::kStopped), ErrorMessage(""), StartDate(shared::currentTime::Provider().now()), StopDate(shared::currentTime::Provider().now()) {
      }
      CRule::~CRule() {
      }
      void CRule::extractContent(shared::CDataContainer & containerToFill) const {
         containerToFill.set("id", Id());
         containerToFill.set("name", Name());
         containerToFill.set("description", Description());
         containerToFill.set("interpreter", Interpreter());
         containerToFill.set("editor", Editor());
         containerToFill.set("model", Model());
         containerToFill.set("content", Content());
         containerToFill.set("configuration", Configuration());
         containerToFill.set("autoStart", AutoStart());
         containerToFill.set("state", State().toString());
         containerToFill.set("errorMessage", ErrorMessage());
         containerToFill.set("startDate", StartDate());
         containerToFill.set("stopDate", StopDate());
      }
      void CRule::fillFromContent(const shared::CDataContainer & initialData) {
         if (initialData.exists("id")) Id = initialData.getInt("id");
         if (initialData.exists("name")) Name = initialData.getString("name");
         if (initialData.exists("description")) Description = initialData.getString("description");
         if (initialData.exists("interpreter")) Interpreter = initialData.getString("interpreter");
         if (initialData.exists("editor")) Editor = initialData.getString("editor");
         if (initialData.exists("model")) Model = initialData.getString("model");
         if (initialData.exists("content")) Content = initialData.getString("content");
         if (initialData.exists("configuration")) Configuration = initialData.getChild("configuration");
         if (initialData.exists("autoStart")) AutoStart = initialData.getBool("autoStart");
         if (initialData.exists("state")) State().fromString(initialData.getString("state"));
         if (initialData.exists("errorMessage")) ErrorMessage = initialData.getString("errorMessage");
         if (initialData.exists("startDate")) StartDate = initialData.getBoostPTime("startDate");
         if (initialData.exists("stopDate")) StopDate = initialData.getBoostPTime("stopDate");
      }
      void CRule::fillFromSerializedString(const std::string & serializedData) {
         shared::CDataContainer deserializeData(serializedData);
         fillFromContent(deserializeData);
      }
      ;

      CRecipient::CRecipient() : Id(0), FirstName(""), LastName(""), Fields(std::vector< boost::shared_ptr<database::entities::CRecipientField> >()) {
      }
      CRecipient::~CRecipient() {
      }
      void CRecipient::extractContent(shared::CDataContainer & containerToFill) const {
         containerToFill.set("id", Id());
         containerToFill.set("firstName", FirstName());
         containerToFill.set("lastName", LastName());
         shared::CDataContainer contvec;
         for (auto & i : Fields())
         {
            containerToFill.push("fields", *i);
         }
      }
      void CRecipient::fillFromContent(const shared::CDataContainer & initialData) {
         if (initialData.exists("id")) Id = initialData.getInt("id");
         if (initialData.exists("firstName")) FirstName = initialData.getString("firstName");
         if (initialData.exists("lastName")) LastName = initialData.getString("lastName");
         if (initialData.exists("fields"))
         {
            std::vector<shared::CDataContainer> a;
            initialData.getChilds("fields", a);

            Fields().clear();
            for (auto & b : a)
            {
               boost::shared_ptr<database::entities::CRecipientField> f = boost::make_shared<database::entities::CRecipientField>();
               f->fillFromContent(b);
               Fields().push_back(f);
            }
         }
      }
      void CRecipient::fillFromSerializedString(const std::string & serializedData) {
         shared::CDataContainer deserializeData(serializedData);
         fillFromContent(deserializeData);
      }
      ;
      CRecipientField::CRecipientField() : IdRecipient(0), PluginType(""), FieldName(""), Value("") {
      }
      CRecipientField::~CRecipientField() {
      }
      void CRecipientField::extractContent(shared::CDataContainer & containerToFill) const {
         containerToFill.set("idRecipient", IdRecipient());
         containerToFill.set("pluginType", PluginType());
         containerToFill.set("fieldName", FieldName());
         containerToFill.set("value", Value());
      }
      void CRecipientField::fillFromContent(const shared::CDataContainer & initialData) {
         if (initialData.exists("idRecipient")) IdRecipient = initialData.getInt("idRecipient");
         if (initialData.exists("pluginType")) PluginType = initialData.getString("pluginType");
         if (initialData.exists("fieldName")) FieldName = initialData.getString("fieldName");
         if (initialData.exists("value")) Value = initialData.getString("value");
      }
      void CRecipientField::fillFromSerializedString(const std::string & serializedData) {
         shared::CDataContainer deserializeData(serializedData);
         fillFromContent(deserializeData);
      }
      ;
   } //namespace entities
} //namespace database
