/* Autogenerated with Kurento Idl */

#include "Media.hpp"
#include <JsonSerializer.hpp>
#include "MediaObject.hpp"

void
Serialize(kurento::Media& event, JsonSerializer& s)
{
  s.Serialize("source", event.source);
  s.Serialize("type", event.type);
}
