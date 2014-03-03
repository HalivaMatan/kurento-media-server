/* Autogenerated with Kurento Idl */

#ifndef __SESSION_ENDPOINT_HPP__
#define __SESSION_ENDPOINT_HPP__

#include <jsoncpp/json/json.h>
#include <JsonRpcException.hpp>
#include <ObjectRegistrar.hpp>
#include <memory>
#include <vector>
#include "Endpoint.hpp"
#include "MediaSessionTerminated.hpp"
#include "MediaSessionStarted.hpp"
#include <sigc++/sigc++.h>
#include <EventHandler.hpp>

namespace kurento {


class SessionEndpoint : public virtual Endpoint {

public:

  SessionEndpoint () {};
  virtual ~SessionEndpoint () {};

  virtual std::string connect(const std::string &eventType, std::shared_ptr<EventHandler> handler);

  sigc::signal<void, MediaSessionTerminated> signalMediaSessionTerminated;
  sigc::signal<void, MediaSessionStarted> signalMediaSessionStarted;

  class Factory : public virtual kurento::Factory
  {
  public:
    Factory () {};

    virtual std::shared_ptr<MediaObject> createObject (const Json::Value
        &params);

    virtual std::string getName () {
      return "SessionEndpoint";
    };

  private:

    std::shared_ptr<MediaObject> createObject (int garbagePeriod);

    class StaticConstructor
    {
    public:
      StaticConstructor();
    };

    static StaticConstructor staticConstructor;

  };

  class Invoker : public virtual Endpoint::Invoker
  {
  public:
    Invoker() {};
    virtual void invoke (std::shared_ptr<MediaObject> obj,
        const std::string &methodName, const Json::Value &params,
        Json::Value &response);
  };

  virtual MediaObject::Invoker &getInvoker() {
    return invoker;
  }

  private:
    Invoker invoker;

};

} /* kurento */

#endif /*  __SESSION_ENDPOINT_HPP__ */
