//------------------------------------------------------------------------------
//  consoleapplication.cc
//  (C) 2006 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "app/consoleapplication.h"
#include "io/logfileconsolehandler.h"

namespace App
{
using namespace Util;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
ConsoleApplication::ConsoleApplication()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
ConsoleApplication::~ConsoleApplication()
{
    n_assert(!this->IsOpen());
}

//------------------------------------------------------------------------------
/**
*/
bool
ConsoleApplication::Open()
{
    n_assert(!this->IsOpen());
    if (Application::Open())
    {
        // initialize core subsystem
        this->coreServer = Core::CoreServer::Create();
        this->coreServer->SetCompanyName(this->companyName);
        this->coreServer->SetAppName(this->appName);
        this->coreServer->Open();

        // initialize io subsystem
        this->ioServer = IoServer::Create();

        // attach a log file console handler
        #if __WIN32__
        Ptr<LogFileConsoleHandler> logFileHandler = LogFileConsoleHandler::Create();
        Console::Instance()->AttachHandler(logFileHandler.upcast<ConsoleHandler>());
        #endif

        return true;   
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
ConsoleApplication::Close()
{
    n_assert(this->IsOpen());

    // shutdown io subsystem
    this->ioServer = 0;

    // shutdown core subsystem
    this->coreServer = 0;

    Application::Close();
}

} // namespace App
