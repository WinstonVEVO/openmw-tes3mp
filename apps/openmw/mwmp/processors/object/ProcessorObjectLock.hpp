#ifndef OPENMW_PROCESSOROBJECTLOCK_HPP
#define OPENMW_PROCESSOROBJECTLOCK_HPP

#include "BaseObjectProcessor.hpp"

namespace mwmp
{
    class ProcessorObjectLock final: public BaseObjectProcessor
    {
    public:
        ProcessorObjectLock()
        {
            BPP_INIT(ID_OBJECT_LOCK)
        }

        virtual void Do(ObjectPacket &packet, ObjectList &objectList)
        {
            BaseObjectProcessor::Do(packet, objectList);

            objectList.lockObjects(ptrCellStore);
        }
    };
}

#endif //OPENMW_PROCESSOROBJECTLOCK_HPP
