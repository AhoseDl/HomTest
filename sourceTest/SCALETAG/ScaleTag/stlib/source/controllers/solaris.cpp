#include "solaris.h"

namespace scaletag {
namespace controllers {

Solaris::Solaris(/*scaletag::frames::ObjectFactory* _objectFactory, */QQmlApplicationEngine* _engine)
    : QObject(_engine)
{
 //objectFactory.reset(_objectFactory);
    engine.reset(_engine);
}


scaletag::frames::IObjectFactory* Solaris::getObjectFactory() const{
    return objectFactory.data();
}

} // namespace controllers
} // namespace scaletag
