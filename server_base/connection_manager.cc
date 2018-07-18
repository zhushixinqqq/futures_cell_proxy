/*
 *  author liuxiaobing@formaxmarket.com
 *  Copyright (c) 2013-2014 formax
 *  2014-03-03
 * */
#include <boost/bind.hpp>
#include <boost/thread/locks.hpp>
#include <algorithm>

#include "futures_cell_proxy/server_base/connection_manager.h"

namespace http { namespace server_base {

ConnectionManager::ConnectionManager():m_is_stopping(false) {
  boost::function<void()> monitor = boost::bind(&ConnectionManager::MonitorThread, this);
  m_monitor_thread.reset(new boost::thread(monitor));
}

ConnectionManager::~ConnectionManager() {
  m_is_stopping = true;
  m_monitor_thread->join();
}

void ConnectionManager::Start(ConnectionPtr c) {
  boost::lock_guard<boost::mutex> lock(m_connection_mutex);
  if (c->Start()) {
    m_connections.insert(c);
  } else {
    c->Stop();
  }
}

void ConnectionManager::Stop(ConnectionPtr c) {
  boost::lock_guard<boost::mutex> lock(m_connection_mutex);
  size_t erase_num = m_connections.erase(c);
  if (erase_num > 0) {
    LOG(INFO) << "stop connection to " << c->EndPoint();
    c->Stop();
  } else {
    LOG(WARNING) << "connection with endpoint: " << c->EndPoint()
      << " may stop already";
  }
}

void ConnectionManager::Erase(ConnectionPtr c) {
  boost::lock_guard<boost::mutex> lock(m_connection_mutex);
  size_t erase_num = m_connections.erase(c);
  if (erase_num > 0) {
    LOG(INFO) << "remove connection to " << c->EndPoint() << " from connection manager";
  } else {
    LOG(WARNING) << "connection with endpoint: " << c->EndPoint()
      << " may remove already";
  }
}

void ConnectionManager::StopAll() {
  boost::lock_guard<boost::mutex> lock(m_connection_mutex);
  std::for_each(m_connections.begin(), m_connections.end(), boost::bind(&Connection::Stop, _1));
  m_connections.clear();
}


void ConnectionManager::MonitorThread() {
  while (!m_is_stopping) {
    boost::this_thread::sleep(boost::posix_time::seconds(10));
    boost::lock_guard<boost::mutex> lock(m_connection_mutex);
    LOG(INFO) <<__func__<< " start check connection size:" << m_connections.size();
    for (std::set<ConnectionPtr>::iterator it = m_connections.begin();
        it != m_connections.end(); ) {
      std::set<ConnectionPtr>::iterator cur = it;
      ++it;
      ConnectionPtr cnn = *cur;
      if (cnn->IsTimeOut()) {
        m_connections.erase(cur);
        VLOG(9) << __FUNCTION__ << " killing connection: " << cnn->EndPoint()
          << " service timeout!";
        cnn->Stop();
      }
    }
  }
}

} // namespace server_base
} // namespace http
