#include "stdafx.h"
#include "FT2xxSerialPort.h"
#include <shared/Log.h>


CFT2xxSerialPort::CFT2xxSerialPort(const std::string& name,
                                   const boost::asio::serial_port_base::baud_rate& baudrate,
                                   const boost::asio::serial_port_base::parity& parity,
                                   const boost::asio::serial_port_base::character_size& characterSize,
                                   const boost::asio::serial_port_base::stop_bits& stop_bits,
                                   const boost::asio::serial_port_base::flow_control& flowControl,
                                   const boost::posix_time::time_duration& connectRetryDelay,
                                   bool flushAtConnect)
: m_asyncReadBuffer(0)
{
}

void CFT2xxSerialPort::setPortNumber(int port)
{
}

std::vector<int> CFT2xxSerialPort::getPortComNumber()
{
   m_serialPortComNumber.push_back(20);
   return m_serialPortComNumber;
}

CFT2xxSerialPort::~CFT2xxSerialPort()
{
   CFT2xxSerialPort::stop();
}

void CFT2xxSerialPort::setReceiveBufferMaxSize(std::size_t size)
{
}

void CFT2xxSerialPort::start()
{
}

void CFT2xxSerialPort::stop()
{
}

bool CFT2xxSerialPort::connect()
{
   return true;
}

void CFT2xxSerialPort::disconnect()
{
}

bool CFT2xxSerialPort::isConnected() const
{
   return m_isConnected;
}

void CFT2xxSerialPort::subscribeForConnectionEvents(shared::event::CEventHandler& forEventHandler,
                                                    int forId)
{
}

void CFT2xxSerialPort::setReceiveBufferHandler(boost::shared_ptr<shared::communication::IReceiveBufferHandler> handler)
{
   m_receiveBufferHandler = handler;
}

void CFT2xxSerialPort::flush()
{
}

void CFT2xxSerialPort::tryConnect()
{
}

void CFT2xxSerialPort::startRead() const
{
}

void CFT2xxSerialPort::activateGPIO(const int GPIONumber) const
{
}

void CFT2xxSerialPort::desactivateGPIO() const
{
   // No GPIO activate => No access
   activateGPIO(0);
}

void CFT2xxSerialPort::send(const shared::communication::CByteBuffer& buffer)
{
   // Not used here 
}

void CFT2xxSerialPort::sendText(const std::string& content)
{
   // Not used here
}

void CFT2xxSerialPort::notifyEventHandler() const
{
}

void CFT2xxSerialPort::notifyEventHandler(const std::string& i18nErrorMessage) const
{
}

void CFT2xxSerialPort::notifyEventHandler(const std::string& i18nErrorMessage, const std::map<std::string, std::string>& i18nMessageParameters) const
{
}
