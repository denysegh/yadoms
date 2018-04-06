#include "stdafx.h"
#include "File.h"
#include <shared/encryption/Base64.h>
#include <shared/DataContainer.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace configuration
         {
            CFile::CFile()
            {
            }

            CFile::~CFile()
            {
            }

            void CFile::extractContent(CDataContainer & containerToFill) const
            {
               containerToFill.set("name", getFileName());
               containerToFill.set("size", getSize());
               containerToFill.set("type", getMimeType());
               containerToFill.set("lastModified", getLastModificationDate().getBoostDateTime());
               containerToFill.set("content", m_base64content);
            }

            void CFile::fillFromContent(const CDataContainer & initialData)
            {
               m_filename = initialData.getString("name");
               m_filesize = initialData.getUInt("size");
               m_mimeType = initialData.getString("type");
               std::string dateIso = initialData.getString("lastModified");
               boost::posix_time::ptime a = boost::posix_time::from_iso_string(dateIso);
               m_lastModified = dateTime::CDateTimeContainer(a);
               m_base64content = initialData.getString("content");
            }

            void CFile::fillFromSerializedString(const std::string & serializedData)
            {
               CDataContainer deserializeData(serializedData);
               fillFromContent(deserializeData);
            }

            const std::string & CFile::getFileName() const
            {
               return m_filename;
            }

            unsigned int CFile::getSize() const
            {
               return m_filesize;
            }

            const std::string & CFile::getMimeType() const
            {
               return m_mimeType;
            }

            const dateTime::CDateTimeContainer & CFile::getLastModificationDate() const
            {
               return m_lastModified;
            }

            const std::string & CFile::getContentAsBase64() const
            {
               return m_base64content;
            }

            std::string CFile::getContent() const
            {
               return encryption::CBase64::decode(m_base64content);
            }
         }
      }
   }
} // namespace shared::plugin::yPluginApi::configuration


