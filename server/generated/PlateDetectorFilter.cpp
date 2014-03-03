/* Autogenerated with Kurento Idl */

#include <memory>

#include "MediaPipeline.hpp"
#include "PlateDetectorFilter.hpp"
#include <JsonSerializer.hpp>

namespace kurento {

std::shared_ptr<MediaObject> PlateDetectorFilter::Factory::createObject (const Json::Value &params)
{
  std::shared_ptr<MediaPipeline> mediaPipeline;
  int garbagePeriod = 0;

  if (!params.isMember ("mediaPipeline")) {
    /* param 'mediaPipeline' not present, raise exception */
    JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'mediaPipeline' parameter is requiered");
    throw e;
  } else {
    JsonSerializer s(false);
    s.JsonValue = params;
    s.SerializeNVP(mediaPipeline);
  }

  if (!params.isMember ("garbagePeriod")) {
    /* param 'garbagePeriod' not present, using default */
    garbagePeriod = 120;
  } else {
    JsonSerializer s(false);
    s.JsonValue = params;
    s.SerializeNVP(garbagePeriod);
  }

  return createObject (mediaPipeline, garbagePeriod);
}

PlateDetectorFilter::Factory::StaticConstructor PlateDetectorFilter::Factory::staticConstructor;

PlateDetectorFilter::Factory::StaticConstructor::StaticConstructor()
{
  if (objectRegistrar) {
    std::shared_ptr <Factory> factory (new PlateDetectorFilter::Factory());

    objectRegistrar->registerFactory(factory);
  }
}

void
PlateDetectorFilter::Invoker::invoke (std::shared_ptr<MediaObject> obj,
    const std::string &methodName, const Json::Value &params,
    Json::Value &response)
{
  Filter::Invoker::invoke(obj, methodName, params, response);
}

std::string
PlateDetectorFilter::connect(const std::string &eventType, std::shared_ptr<EventHandler> handler)
{
  if ("PlateDetected" == eventType) {
    sigc::connection conn = signalPlateDetected.connect ([&, handler] (PlateDetected event) {
      JsonSerializer s (true);

      s.Serialize ("data", event);
      s.Serialize ("object", this);
      s.JsonValue["type"] = "PlateDetected";
      s.JsonValue["subscription"] = handler->getId();
      handler->sendEvent(s.JsonValue);
    });
    handler->setConnection (conn);
    return handler->getId();
  }
  return Filter::connect (eventType, handler);
}

} /* kurento */

void
Serialize(std::shared_ptr<kurento::PlateDetectorFilter> &object, JsonSerializer &serializer)
{
  if (!serializer.IsWriter) {
    try {
      std::shared_ptr<kurento::MediaObject> aux;
      aux = kurento::PlateDetectorFilter::Factory::getObject (serializer.JsonValue.asString ());
      object = std::dynamic_pointer_cast<kurento::PlateDetectorFilter> (aux);
      return;
    } catch (kurento::JsonRpc::CallException &ex) {
      kurento::JsonRpc::CallException e (kurento::JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'PlateDetectorFilter' object not found: "+ ex.getMessage());
      throw e;
    }
  }
  std::shared_ptr<kurento::Filter> aux = std::dynamic_pointer_cast<kurento::Filter> (object);

  void Serialize(std::shared_ptr<kurento::Filter> &object, JsonSerializer &serializer);
  Serialize(aux, serializer);
}

void
Serialize(kurento::PlateDetectorFilter &object, JsonSerializer &serializer)
{
  void Serialize(kurento::Filter &object, JsonSerializer &serializer);
  try {
    Serialize(dynamic_cast<kurento::Filter &> (object), serializer);
  } catch (std::bad_cast) {
  }
}
