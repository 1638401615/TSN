#ifndef _GATE_CONTROL_LIST_H
#define _GATE_CONTROL_LIST_H

#include <tinyxml2/tinyxml2.h>

#include <bitset>
#include <exception>
#include <memory>
#include <sstream>
#include <vector>

#include "../../timer/ITimer.h"
#include "../../timer/TimeContext.h"
#include "../../utils/Log.h"
#include "../../utils/config/ConfigSetting.h"
// #include "../../utils/reflector/Reflector.h"
#include "../../utils/reflector/DynamicCreate.h"
#include "TransmissionGate.h"

using namespace tinyxml2;

namespace faker_tsn {

class GateControlListItem {
   public:
    Time::TimeInterval m_timeInterval; /* time interval */
    std::bitset<8> m_gateStates;       /* gate states */

    GateControlListItem(Time::TimeInterval timeInterval, std::bitset<8> gateStates = std::bitset<8>("00000000")) : m_timeInterval(timeInterval), m_gateStates(gateStates) {}

    std::string toString() {
        std::stringstream ss;
        ss << "\n";
        ss << "TimeInterval: " << this->m_timeInterval.toString();
        ss << "\n";
        ss << "GateStates: " << this->m_gateStates.to_string();
        return ss.str();
    }
};

class GateControlList : public REFLECT_OBJECT, public DynamicCreator<GateControlList, unsigned int> {
    unsigned int m_portId;
    unsigned int m_gateSize;
    std::vector<std::shared_ptr<TransmissionGate>> m_gates; /* gate container */
    std::vector<GateControlListItem> m_gcl;                 /* gate control list (GCL) */
    Time::TimeInterval m_period;                            /* scheduling period */

    /* load GCL XML file */
    virtual void loadScheduleXML(std::string filename);

   public:
    GateControlList(unsigned int portId) : m_portId(portId) {
        /* load config file */
        std::string filename = ConfigSetting::getInstance().get<const char*>("gclRoute");
        this->loadScheduleXML(filename);
        /* register ticker to timer */
        Time::TimePoint start(0, 0);
        for (GateControlListItem item : this->m_gcl) {
            INFO(item.toString());
            std::shared_ptr<Ticker> ticker = std::make_shared<Ticker>(
                start,
                item.m_timeInterval,
                this->m_period);
            TimeContext::getInstance().getTimer()->addTicker(ticker);
            start += item.m_timeInterval;
        }
    }

    virtual ~GateControlList() = default;

    inline unsigned int getPortId() {
        return this->m_portId;
    }

    /*** Obeserver Pattern ***/

    /* append gate */
    inline void appendGate(std::shared_ptr<TransmissionGate> gate) {
        this->m_gates.push_back(gate);
    }

    /* update all gates */
    inline void updateGates() {
        static int i = 0;//the index of the gate_control_list_item
        for (auto gate : this->m_gates) {
            int j = 0;//the index of the bitset
            gate->onUpdate(this->m_gcl[i].m_gateStates[j++]);
        }
        i++;
        i %= this->m_gates.size();
    }
};  // namespace faker_tsn

}  // namespace faker_tsn

#endif  // _GATE_CONTROL_LIST_H