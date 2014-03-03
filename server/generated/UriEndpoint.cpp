/* Autogenerated with Kurento Idl */

#include <memory>

#include "UriEndpoint.hpp"
#include <JsonSerializer.hpp>

namespace kurento {


void
UriEndpoint::Invoker::invoke (std::shared_ptr<MediaObject> obj,
    const std::string &methodName, const Json::Value &params,
    Json::Value &response)
{
  if (methodName == "getUri" && params.size() == 0) {
    Json::Value aux;
    std::string ret;
    JsonSerializer serializer(true);

    // TODO: Implement method getUri
    std::shared_ptr<UriEndpoint> finalObj;
    finalObj = std::dynamic_pointer_cast<UriEndpoint> (obj);
    if (!finalObj) {
      JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                                "Object not found or has incorrect type");
      throw e;
    }

    ret =finalObj->getUri ();
    serializer.SerializeNVP (ret);
    response = serializer.JsonValue["ret"];
    return;
  }

  if (methodName == "pause" && params.size() == 0) {
    Json::Value aux;

    // TODO: Implement method pause
    std::shared_ptr<UriEndpoint> finalObj;
    finalObj = std::dynamic_pointer_cast<UriEndpoint> (obj);
    if (!finalObj) {
      JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                                "Object not found or has incorrect type");
      throw e;
    }

    finalObj->pause ();
    return;
  }

  if (methodName == "stop" && params.size() == 0) {
    Json::Value aux;

    // TODO: Implement method stop
    std::shared_ptr<UriEndpoint> finalObj;
    finalObj = std::dynamic_pointer_cast<UriEndpoint> (obj);
    if (!finalObj) {
      JsonRpc::CallException e (JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                                "Object not found or has incorrect type");
      throw e;
    }

    finalObj->stop ();
    return;
  }

  Endpoint::Invoker::invoke(obj, methodName, params, response);
}

std::string
UriEndpoint::connect(const std::string &eventType, std::shared_ptr<EventHandler> handler)
{
  return Endpoint::connect (eventType, handler);
}

} /* kurento */

void
Serialize(std::shared_ptr<kurento::UriEndpoint> &object, JsonSerializer &serializer)
{
  if (!serializer.IsWriter) {
    try {
      std::shared_ptr<kurento::MediaObject> aux;
      aux = kurento::UriEndpoint::Factory::getObject (serializer.JsonValue.asString ());
      object = std::dynamic_pointer_cast<kurento::UriEndpoint> (aux);
      return;
    } catch (kurento::JsonRpc::CallException &ex) {
      kurento::JsonRpc::CallException e (kurento::JsonRpc::ErrorCode::SERVER_ERROR_INIT,
                              "'UriEndpoint' object not found: "+ ex.getMessage());
      throw e;
    }
  }
  std::shared_ptr<kurento::Endpoint> aux = std::dynamic_pointer_cast<kurento::Endpoint> (object);

  void Serialize(std::shared_ptr<kurento::Endpoint> &object, JsonSerializer &serializer);
  Serialize(aux, serializer);
}

void
Serialize(kurento::UriEndpoint &object, JsonSerializer &serializer)
{
  void Serialize(kurento::Endpoint &object, JsonSerializer &serializer);
  try {
    Serialize(dynamic_cast<kurento::Endpoint &> (object), serializer);
  } catch (std::bad_cast) {
  }
}
