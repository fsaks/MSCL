/*******************************************************************************
Copyright(c) 2015-2016 LORD Corporation. All rights reserved.

MIT Licensed. See the included LICENSE.txt for a copy of the full MIT License.
*******************************************************************************/
#include "stdafx.h"
#include "NodeFeatures_bladeImpactLink.h"

#include "mscl/Exceptions.h"
#include "AvailableSampleRates.h"
#include "mscl/MicroStrain/Wireless/Configuration/NodeEepromMap.h"
#include "mscl/MicroStrain/Wireless/ChannelMask.h"

namespace mscl
{
    NodeFeatures_bladeImpactLink::NodeFeatures_bladeImpactLink(const NodeInfo& info):
        NodeFeatures(info)
    {
        addCalCoeffChannelGroup(1, NodeEepromMap::CH_ACTION_SLOPE_1, NodeEepromMap::CH_ACTION_ID_1);
        addCalCoeffChannelGroup(2, NodeEepromMap::CH_ACTION_SLOPE_2, NodeEepromMap::CH_ACTION_ID_2);
        addCalCoeffChannelGroup(3, NodeEepromMap::CH_ACTION_SLOPE_3, NodeEepromMap::CH_ACTION_ID_3);
        //addCalCoeffChannelGroup(4, NodeEepromMap::CH_ACTION_SLOPE_4, NodeEepromMap::CH_ACTION_ID_4);

        static const ChannelMask CH1(BOOST_BINARY(00000001)); //ch1
        static const ChannelMask CH2(BOOST_BINARY(00000010)); //ch2
        static const ChannelMask CH3(BOOST_BINARY(00000100)); //ch3
        m_channelGroups.emplace_back(CH1, "Accel Channel 1", ChannelGroup::SettingsMap{{WirelessTypes::chSetting_lowPassFilter, NodeEepromMap::LOW_PASS_FILTER_1}});
        m_channelGroups.emplace_back(CH2, "Accel Channel 2", ChannelGroup::SettingsMap{{WirelessTypes::chSetting_lowPassFilter, NodeEepromMap::LOW_PASS_FILTER_2}});
        m_channelGroups.emplace_back(CH3, "Accel Channel 3", ChannelGroup::SettingsMap{{WirelessTypes::chSetting_lowPassFilter, NodeEepromMap::LOW_PASS_FILTER_3}});

        //Channels
        m_channels.emplace_back(1, WirelessChannel::channel_1, WirelessTypes::chType_acceleration);    //accel x
        m_channels.emplace_back(2, WirelessChannel::channel_2, WirelessTypes::chType_acceleration);    //accel y
        m_channels.emplace_back(3, WirelessChannel::channel_3, WirelessTypes::chType_acceleration);    //accel z
        //m_channels.emplace_back(4, WirelessChannel::channel_4, WirelessTypes::chType_temperature);    //internal temp
    }

    const WirelessTypes::SamplingModes NodeFeatures_bladeImpactLink::samplingModes() const
    {
        //build and return the sampling modes that are supported
        WirelessTypes::SamplingModes result;

        result.push_back(WirelessTypes::samplingMode_sync);
        result.push_back(WirelessTypes::samplingMode_syncBurst);
        result.push_back(WirelessTypes::samplingMode_nonSync);
        result.push_back(WirelessTypes::samplingMode_nonSyncEvent);
        result.push_back(WirelessTypes::samplingMode_syncEvent);

        //no support for armed datalogging

        return result;
    }

    const WirelessTypes::WirelessSampleRates NodeFeatures_bladeImpactLink::sampleRates(WirelessTypes::SamplingMode samplingMode) const
    {
        //the list of sample rates varies for each sampling mode
        switch(samplingMode)
        {
        case WirelessTypes::samplingMode_nonSync:
        case WirelessTypes::samplingMode_sync:
            return AvailableSampleRates::continuous_blade;
        
        case WirelessTypes::samplingMode_syncBurst:
            return AvailableSampleRates::burst_blade;

        case WirelessTypes::samplingMode_nonSyncEvent:
        case WirelessTypes::samplingMode_syncEvent:
            return AvailableSampleRates::event_blade;

        default:
            throw Error("Invalid SamplingMode");
        }
    }

    const WirelessTypes::Filters NodeFeatures_bladeImpactLink::lowPassFilters() const
    {
        static const WirelessTypes::Filters filters = {
            {WirelessTypes::filter_4096hz},
            {WirelessTypes::filter_2048hz},
            {WirelessTypes::filter_1024hz},
            {WirelessTypes::filter_512hz},
            {WirelessTypes::filter_256hz},
            {WirelessTypes::filter_128hz}
        };
        return filters;
    }

    const WirelessTypes::StorageLimitModes NodeFeatures_bladeImpactLink::storageLimitModes() const
    {
        WirelessTypes::StorageLimitModes modes;
        modes.push_back(WirelessTypes::storageLimit_overwrite);
        return modes;
    }
}