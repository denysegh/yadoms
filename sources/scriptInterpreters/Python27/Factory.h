#pragma once
#include "IFactory.h"
#include "IScriptFile.h"
#include <shared/process/IProcessObserver.h>
#include <shared/process/IExternalProcessLogger.h>

//--------------------------------------------------------------
/// \brief	Python interpreter
//--------------------------------------------------------------
class CFactory : public IFactory
{
public:
   CFactory();
   virtual ~CFactory();

   // IFactory implementation
   boost::shared_ptr<IPythonExecutable> createPythonExecutable() const override;
   boost::shared_ptr<shared::process::IProcess> createScriptProcess(int scriptInstanceId,
                                                                    const std::string& scriptPath,
                                                                    boost::shared_ptr<IPythonExecutable> pythonExecutable,
                                                                    const boost::filesystem::path& interpreterPath,
                                                                    const std::string& scriptApiId,
                                                                    boost::function2<void, bool, int> onInstanceStateChangedFct) const override;
   // [END] IFactory implementation

protected:
   boost::shared_ptr<IScriptFile> createScriptFile(const std::string& scriptPath) const;
   boost::shared_ptr<shared::process::IProcessObserver> createScriptProcessObserver(int scriptInstanceId,
                                                                                    boost::function2<void, bool, int> onInstanceStateChangedFct) const;
   boost::shared_ptr<shared::process::IExternalProcessLogger> createScriptLogger(int scriptInstanceId) const;
};
