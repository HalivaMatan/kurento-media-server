/* Autogenerated with Kurento Idl */

#ifndef __POINTER_DETECTOR_ADV_FILTER_HPP__
#define __POINTER_DETECTOR_ADV_FILTER_HPP__

#include <jsoncpp/json/json.h>
#include <JsonRpcException.hpp>
#include <ObjectRegistrar.hpp>
#include <memory>
#include <vector>
#include "Filter.hpp"
#include "WindowIn.hpp"
#include "WindowOut.hpp"
#include <sigc++/sigc++.h>
#include <EventHandler.hpp>

namespace kurento {

class MediaPipeline;
class WindowParam;
class PointerDetectorWindowMediaParam;
class PointerDetectorWindowMediaParam;

class PointerDetectorAdvFilter : public virtual Filter {

public:

  PointerDetectorAdvFilter () {};
  virtual ~PointerDetectorAdvFilter () {};

  virtual void addWindow (std::shared_ptr<PointerDetectorWindowMediaParam> window) {throw "Not implemented";};
  virtual void clearWindows () {throw "Not implemented";};
  virtual void trackcolourFromCalibrationRegion () {throw "Not implemented";};
  virtual void removeWindow (const std::string& windowId) {throw "Not implemented";};

  virtual std::string connect(const std::string &eventType, std::shared_ptr<EventHandler> handler);

  sigc::signal<void, WindowIn> signalWindowIn;
  sigc::signal<void, WindowOut> signalWindowOut;

  class Factory : public virtual kurento::Factory
  {
  public:
    Factory () {};

    virtual std::shared_ptr<MediaObject> createObject (const Json::Value
        &params);

    virtual std::string getName () {
      return "PointerDetectorAdvFilter";
    };

  private:

    std::shared_ptr<MediaObject> createObject (std::shared_ptr<MediaPipeline> mediaPipeline, std::shared_ptr<WindowParam> calibrationRegion, const std::vector<std::shared_ptr<PointerDetectorWindowMediaParam>>& windows, int garbagePeriod);

    class StaticConstructor
    {
    public:
      StaticConstructor();
    };

    static StaticConstructor staticConstructor;

  };

  class Invoker : public virtual Filter::Invoker
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

#endif /*  __POINTER_DETECTOR_ADV_FILTER_HPP__ */
