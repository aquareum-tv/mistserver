/// \file config.h
/// Contains generic function headers for managing configuration.

#pragma once

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "unknown"
#endif

#include <sys/wait.h>
#include "json.h"
#include <string>

/// Contains utility code, not directly related to streaming media
namespace Util{
  extern uint32_t printDebugLevel;
  extern __thread char streamName[256]; ///< Used by debug messages to identify the stream name
  void setStreamName(const std::string & sn);
  extern __thread char exitReason[256];
  extern __thread char* mRExitReason;
  void logExitReason(const char* shortString, const char *format, ...);

  enum binType {
    UNSET,
    INPUT,
    OUTPUT,
    PROCESS,
    CONTROLLER
  };

  /// Deals with parsing configuration from commandline options.
  class Config{
  private:
    JSON::Value vals; ///< Holds all current config values
    int long_count;
    static void signal_handler(int signum, siginfo_t *sigInfo, void *ignore);

  public:
    static void setMutexAborter(void * mutex);
    // variables
    static bool is_active;     ///< Set to true by activate(), set to false by the signal handler.
    static bool is_restarting; ///< Set to true when restarting, set to false on boot.
    static binType binaryType;
    // functions
    Config();
    Config(std::string cmd);
    void addOption(std::string optname, JSON::Value option);
    void printHelp(std::ostream &output);
    bool parseArgs(int &argc, char **&argv);
    bool hasOption(const std::string &optname);
    JSON::Value &getOption(std::string optname, bool asArray = false);
    std::string getString(std::string optname);
    int64_t getInteger(std::string optname);
    bool getBool(std::string optname);
    void activate();
    int threadServer(Socket::Server &server_socket, int (*callback)(Socket::Connection &S));
    int forkServer(Socket::Server &server_socket, int (*callback)(Socket::Connection &S));
    int serveThreadedSocket(int (*callback)(Socket::Connection &S));
    int serveForkedSocket(int (*callback)(Socket::Connection &S));
    int servePlainSocket(int (*callback)(Socket::Connection &S));
    void addOptionsFromCapabilities(const JSON::Value &capabilities);
    void addBasicConnectorOptions(JSON::Value &capabilities);
    void addStandardPushCapabilities(JSON::Value &capabilities);
    void addConnectorOptions(int port, JSON::Value &capabilities);
  };

  inline void buildPipedPart(JSON::Value &p, std::deque<std::string> &argDeq, const JSON::Value &argset);

  /// The interface address the current serveSocket function is listening on
  extern std::string listenInterface;
  /// The port the current serveSocket function is listening on
  extern uint32_t listenPort;

  /// Will set the active user to the named username.
  void setUser(std::string user);

}// namespace Util

inline void Util::buildPipedPart(JSON::Value &p, std::deque<std::string> &argDeq, const JSON::Value &argset){
  jsonForEachConst(argset, it){
    if (it->isMember("option") && p.isMember(it.key())){
      if (!it->isMember("type")){
        if (JSON::Value(p[it.key()]).asBool()){
          argDeq.push_back((*it)["option"]);
        }
        continue;
      }
      if ((*it)["type"].asStringRef() == "str" && !p[it.key()].isString()){
        p[it.key()] = p[it.key()].asString();
      }
      if ((*it)["type"].asStringRef() == "uint" || (*it)["type"].asStringRef() == "int" ||
          (*it)["type"].asStringRef() == "debug"){
        p[it.key()] = JSON::Value(p[it.key()].asInt()).asString();
      }
      if ((*it)["type"].asStringRef() == "inputlist" && p[it.key()].isArray()){
        jsonForEach(p[it.key()], iVal){
          argDeq.push_back((*it)["option"]);
          argDeq.push_back(iVal->asString());
        }
        continue;
      }
      if (p[it.key()].asStringRef().size() > 0){
        argDeq.push_back((*it)["option"]);
        argDeq.push_back(p[it.key()].asString());
      }else{
        argDeq.push_back((*it)["option"]);
      }
    }
  }
}