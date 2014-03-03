/* Autogenerated with Kurento Idl */

#include <memory>

#include "MediaPipeline.hpp"
#include "FaceOverlayFilter.hpp"
#include <JsonSerializer.hpp>

namespace kurento {

std::shared_ptr<MediaObject> FaceOverlayFilter::Factory::createObject (const Json::Value &params)
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

FaceOverlayFilter::Factory::StaticConstructor FaceOverlayFilter::Factory::staticConstructor;

FaceOverlayFilter::Factory::StaticConstructor::StaticConstructor()
{
  if (objectRegistrar) {
    std::shared_ptr <Factory> factory (new FaceOverlayFilter::Factory());

    objectRegistrar->registerFactory(factory);
  }
}

void
FaceOverlayFilter::Invoker::invoke (std::shared_ptr<MediaObject> obj,
    const std::string &methodName, const Json::Value &params,
    Json::Value &response)
{
  if (methodName == "unsetOverlayedImage" && params.size() == 0) {
    Json::Value aux;

    // TODO: Implement method unsetOverlayedImage
    std::shared_ptr<FaceOverlayFilter> finalObj;
    finalObj = std::dynamic_pointer_cast<FaceOverlayFilter> (obj);
    if (!finalObj) {
      JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                                "Object not found or has incorrect type");
      throw e;
    }

    finalObj->unsetOverlayedImage ();
    return;
  }

  if (methodName == "setOverlayedImage" && params.size() == 5) {
    Json::Value aux;
    std::string uri;
    float offsetXPercent;
    float offsetYPercent;
    float widthPercent;
    float heightPercent;

    if (!params.isMember ("uri")) {
      /* param 'uri' not present, raise exception */
      JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                                "'uri' parameter is requiered");
      throw e;
    } else {
      aux = params["uri"];

      if (!aux.isString ()) {
        /* param 'uri' has invalid type value, raise exception */
        JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                                "'uri' parameter should be a string");
        throw e;
      }

      uri = aux.asString ();
    }

    if (!params.isMember ("offsetXPercent")) {
      /* param 'offsetXPercent' not present, raise exception */
      JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                                "'offsetXPercent' parameter is requiered");
      throw e;
    } else {
      aux = params["offsetXPercent"];

      if (!aux.isDouble ()) {
        /* param 'offsetXPercent' has invalid type value, raise exception */
        JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                                "'offsetXPercent' parameter should be a double");
        throw e;
      }

      offsetXPercent = aux.asDouble ();
    }

    if (!params.isMember ("offsetYPercent")) {
      /* param 'offsetYPercent' not present, raise exception */
      JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                                "'offsetYPercent' parameter is requiered");
      throw e;
    } else {
      aux = params["offsetYPercent"];

      if (!aux.isDouble ()) {
        /* param 'offsetYPercent' has invalid type value, raise exception */
        JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                                "'offsetYPercent' parameter should be a double");
        throw e;
      }

      offsetYPercent = aux.asDouble ();
    }

    if (!params.isMember ("widthPercent")) {
      /* param 'widthPercent' not present, raise exception */
      JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                                "'widthPercent' parameter is requiered");
      throw e;
    } else {
      aux = params["widthPercent"];

      if (!aux.isDouble ()) {
        /* param 'widthPercent' has invalid type value, raise exception */
        JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                                "'widthPercent' parameter should be a double");
        throw e;
      }

      widthPercent = aux.asDouble ();
    }

    if (!params.isMember ("heightPercent")) {
      /* param 'heightPercent' not present, raise exception */
      JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                                "'heightPercent' parameter is requiered");
      throw e;
    } else {
      aux = params["heightPercent"];

      if (!aux.isDouble ()) {
        /* param 'heightPercent' has invalid type value, raise exception */
        JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                                "'heightPercent' parameter should be a double");
        throw e;
      }

      heightPercent = aux.asDouble ();
    }

    // TODO: Implement method setOverlayedImage
    std::shared_ptr<FaceOverlayFilter> finalObj;
    finalObj = std::dynamic_pointer_cast<FaceOverlayFilter> (obj);
    if (!finalObj) {
      JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                                "Object not found or has incorrect type");
      throw e;
    }

    finalObj->setOverlayedImage (uri, offsetXPercent, offsetYPercent, widthPercent, heightPercent);
    return;
  }

  Filter::Invoker::invoke(obj, methodName, params, response);
}

std::string
FaceOverlayFilter::connect(const std::string &eventType, std::shared_ptr<EventHandler> handler)
{
  return Filter::connect (eventType, handler);
}

} /* kurento */

void
Serialize(std::shared_ptr<kurento::FaceOverlayFilter> &object, JsonSerializer &serializer)
{
  if (!serializer.IsWriter) {
    try {
      std::shared_ptr<kurento::MediaObject> aux;
      aux = kurento::FaceOverlayFilter::Factory::getObject (serializer.JsonValue.asString ());
      object = std::dynamic_pointer_cast<kurento::FaceOverlayFilter> (aux);
      return;
    } catch (kurento::JsonRpc::CallException &ex) {
      kurento::JsonRpc::CallException e (kurento::JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'FaceOverlayFilter' object not found: "+ ex.getMessage());
      throw e;
    }
  }
  std::shared_ptr<kurento::Filter> aux = std::dynamic_pointer_cast<kurento::Filter> (object);

  void Serialize(std::shared_ptr<kurento::Filter> &object, JsonSerializer &serializer);
  Serialize(aux, serializer);
}

void
Serialize(kurento::FaceOverlayFilter &object, JsonSerializer &serializer)
{
  void Serialize(kurento::Filter &object, JsonSerializer &serializer);
  try {
    Serialize(dynamic_cast<kurento::Filter &> (object), serializer);
  } catch (std::bad_cast) {
  }
}
