/*****************************************************************************
Copyright(c) 2015 LORD Corporation. All rights reserved.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the included
LICENSE.txt file for a copy of the full GNU General Public License.
*****************************************************************************/
#include "mscl/MicroStrain/Wireless/SyncSamplingNetwork.h"
#include "mscl/MicroStrain/Wireless/WirelessNode.h"
#include "mscl/MicroStrain/Wireless/Configuration/WirelessNodeConfig.h"

#include <boost/test/unit_test.hpp>
#include "mock_WirelessNode.h"
#include "mock_BaseStation.h"
using namespace mscl;

struct Sampling_Continuous
{
	uint16 retx;
	uint16 chs;
	uint16 rate;
	uint16 syncMode;
	uint16 dataFormat;
	uint16 collectionMode;
	uint16 samplingDelay;
	uint16 samplingMode;
};

struct Sampling_Burst
{
	uint16 retx;
	uint16 chs;
	uint16 rate;
	uint16 syncMode;
	uint16 dataFormat;
	uint16 collectionMode;
	uint16 samplingDelay;
	uint16 samplingMode;
	uint16 sweeps;
	uint16 timebetwburst;
};

static void expectSampling_Continuous(std::shared_ptr<mock_WirelessNodeImpl> impl, Sampling_Continuous settings)
{
	expectRead(impl, NodeEepromMap::NODE_RETRANSMIT, Value::UINT16(settings.retx));
	expectRead(impl, NodeEepromMap::ACTIVE_CHANNEL_MASK, Value::UINT16(settings.chs));
	expectRead(impl, NodeEepromMap::SAMPLE_RATE, Value::UINT16(settings.rate));
	expectRead(impl, NodeEepromMap::SYNC_SAMPLE_SETTING, Value::UINT16(settings.syncMode));
	expectRead(impl, NodeEepromMap::DATA_FORMAT, Value::UINT16(settings.dataFormat));
	expectRead(impl, NodeEepromMap::COLLECTION_MODE, Value::UINT16(settings.collectionMode));
	expectRead(impl, NodeEepromMap::SAMPLING_DELAY, Value::UINT16(settings.samplingDelay));
	expectRead(impl, NodeEepromMap::SAMPLING_MODE, Value::UINT16(settings.samplingMode));
}

static void expectSampling_Burst(std::shared_ptr<mock_WirelessNodeImpl> impl, Sampling_Burst settings)
{
	expectRead(impl, NodeEepromMap::NODE_RETRANSMIT, Value::UINT16(settings.retx));
	expectRead(impl, NodeEepromMap::ACTIVE_CHANNEL_MASK, Value::UINT16(settings.chs));
	expectRead(impl, NodeEepromMap::SAMPLE_RATE, Value::UINT16(settings.rate));
	expectRead(impl, NodeEepromMap::SYNC_SAMPLE_SETTING, Value::UINT16(settings.syncMode));
	expectRead(impl, NodeEepromMap::DATA_FORMAT, Value::UINT16(settings.dataFormat));
	expectRead(impl, NodeEepromMap::NUM_SWEEPS, Value::UINT16(settings.sweeps));
	expectRead(impl, NodeEepromMap::TIME_BETW_SESSIONS, Value::UINT16(settings.timebetwburst));
	expectRead(impl, NodeEepromMap::COLLECTION_MODE, Value::UINT16(settings.collectionMode));
	expectRead(impl, NodeEepromMap::SAMPLING_DELAY, Value::UINT16(settings.samplingDelay));
	expectRead(impl, NodeEepromMap::SAMPLING_MODE, Value::UINT16(settings.samplingMode));
}

BOOST_AUTO_TEST_SUITE(SyncSamplingNetwork_Test)

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_constructor)
{
	BaseStation b = makeBaseStationWithMockImpl();

	SyncSamplingNetwork nwk(b);

	BOOST_CHECK_EQUAL(nwk.lossless(), true);
	BOOST_CHECK_EQUAL(nwk.highCapacity(), false);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.0, 0.1);
	BOOST_CHECK_EQUAL(nwk.ok(), true);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNode_burst)
{
	//this test adds a single node in burst mode

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	SyncSamplingNetwork nwk(b);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Burst s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 5;
	s.rate = 105;
	s.syncMode = WirelessTypes::syncMode_burst;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.sweeps = 256;
	s.timebetwburst = 350;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;

	expectSampling_Burst(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 12.512, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 12.512, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 16);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 56);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 1);

	SyncNetworkInfo infoCopy(info);

	BOOST_CHECK_EQUAL(infoCopy.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(infoCopy.configurationApplied(), false);
	BOOST_CHECK_CLOSE(infoCopy.percentBandwidth(), 12.512, 0.1);
	BOOST_CHECK_EQUAL(infoCopy.groupSize(), 1);
	BOOST_CHECK_EQUAL(infoCopy.transmissionPerGroup(), 16);
	BOOST_CHECK_EQUAL(infoCopy.maxTdmaAddress(), 56);
	BOOST_CHECK_EQUAL(infoCopy.tdmaAddress(), 1);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNode_continuous)
{
	//this tests adds a single node in continuous mode

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	SyncSamplingNetwork nwk(b);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);

	//eeprom reads performed by the SyncSamplingNetwork
	uint16 retx = WirelessTypes::retransmission_on,
		chs = 7,
		rate = 112,
		syncMode = WirelessTypes::syncMode_continuous,
		dataFormat = WirelessTypes::dataFormat_2byte_uint,
		collectionMode = WirelessTypes::collectionMethod_logAndTransmit,
		samplingDelay = 5,
		samplingMode = WirelessTypes::samplingMode_sync;

	expectRead(impl, NodeEepromMap::NODE_RETRANSMIT, Value::UINT16(retx));				//retransmission		(on)
	expectRead(impl, NodeEepromMap::ACTIVE_CHANNEL_MASK, Value::UINT16(chs));			//active channels mask	(channels 1, 2, and 3)
	expectRead(impl, NodeEepromMap::SAMPLE_RATE, Value::UINT16(rate));					//sample rate			(2Hz)
	expectRead(impl, NodeEepromMap::SYNC_SAMPLE_SETTING, Value::UINT16(syncMode));		//sync sampling mode	(continuous)
	expectRead(impl, NodeEepromMap::DATA_FORMAT, Value::UINT16(dataFormat));			//data format			(2 byte uint)
	expectRead(impl, NodeEepromMap::COLLECTION_MODE, Value::UINT16(collectionMode));	//collection mode		(log and transmit)
	expectRead(impl, NodeEepromMap::SAMPLING_DELAY, Value::UINT16(samplingDelay));		//sampling delay		(5)
	expectRead(impl, NodeEepromMap::SAMPLING_MODE, Value::UINT16(samplingMode));
	
	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNode_noChannelMask)
{
	//this tests adds a single node with no active channels

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	uint16 retx = WirelessTypes::retransmission_on,
		chs = 0,
		rate = 112,
		syncMode = WirelessTypes::syncMode_continuous,
		dataFormat = WirelessTypes::dataFormat_2byte_uint,
		collectionMode = WirelessTypes::collectionMethod_logAndTransmit,
		samplingDelay = 5,
		samplingMode = WirelessTypes::samplingMode_sync;

	expectRead(impl, NodeEepromMap::NODE_RETRANSMIT, Value::UINT16(retx));				//retransmission		(on)
	expectRead(impl, NodeEepromMap::ACTIVE_CHANNEL_MASK, Value::UINT16(chs));			//active channels mask	(no channels active)
	expectRead(impl, NodeEepromMap::SAMPLE_RATE, Value::UINT16(rate));					//sample rate			(2Hz)
	expectRead(impl, NodeEepromMap::SYNC_SAMPLE_SETTING, Value::UINT16(syncMode));		//sync sampling mode	(continuous)
	expectRead(impl, NodeEepromMap::DATA_FORMAT, Value::UINT16(dataFormat));			//data format			(2 byte uint)
	expectRead(impl, NodeEepromMap::COLLECTION_MODE, Value::UINT16(collectionMode));	//collection mode		(log and transmit)
	expectRead(impl, NodeEepromMap::SAMPLING_DELAY, Value::UINT16(samplingDelay));		//sampling delay		(5)
	expectRead(impl, NodeEepromMap::SAMPLING_MODE, Value::UINT16(samplingMode));

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.0, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.0, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 0);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 0);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNode_burst_noChannelMask)
{
	//this test adds a single node in burst mode, with no active channels

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	uint16 retx = WirelessTypes::retransmission_on,
		chs = 0,
		rate = 105,
		syncMode = WirelessTypes::syncMode_burst,
		dataFormat = WirelessTypes::dataFormat_2byte_uint,
		sweeps = 256,
		timebetwburst = 350,
		collectionMode = WirelessTypes::collectionMethod_logAndTransmit,
		samplingDelay = 5,
		samplingMode = WirelessTypes::samplingMode_sync;

	expectRead(impl, NodeEepromMap::NODE_RETRANSMIT, Value::UINT16(retx));				//retransmission		(on)
	expectRead(impl, NodeEepromMap::ACTIVE_CHANNEL_MASK, Value::UINT16(chs));			//active channels mask	(channels 1 and 3)
	expectRead(impl, NodeEepromMap::SAMPLE_RATE, Value::UINT16(rate));					//sample rate			(256Hz)
	expectRead(impl, NodeEepromMap::SYNC_SAMPLE_SETTING, Value::UINT16(syncMode));		//sync sampling mode	(burst)
	expectRead(impl, NodeEepromMap::DATA_FORMAT, Value::UINT16(dataFormat));			//data format			(2 byte uint)
	expectRead(impl, NodeEepromMap::NUM_SWEEPS, Value::UINT16(sweeps));					//sweeps per session	(256)
	expectRead(impl, NodeEepromMap::TIME_BETW_SESSIONS, Value::UINT16(timebetwburst));	//time between sessions	(350)
	expectRead(impl, NodeEepromMap::COLLECTION_MODE, Value::UINT16(collectionMode));	//collection mode		(log and transmit)
	expectRead(impl, NodeEepromMap::SAMPLING_DELAY, Value::UINT16(samplingDelay));		//sampling delay		(5)
	expectRead(impl, NodeEepromMap::SAMPLING_MODE, Value::UINT16(samplingMode));

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.0, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.0, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 0);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 0);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNode_logOnly)
{
	//this tests adds a single node with log only

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	uint16 retx = WirelessTypes::retransmission_on,
		chs = 7,
		rate = 112,
		syncMode = WirelessTypes::syncMode_continuous,
		dataFormat = WirelessTypes::dataFormat_2byte_uint,
		collectionMode = WirelessTypes::collectionMethod_logOnly,
		samplingDelay = 5,
		samplingMode = WirelessTypes::samplingMode_sync;

	expectRead(impl, NodeEepromMap::NODE_RETRANSMIT, Value::UINT16(retx));				//retransmission		(on)
	expectRead(impl, NodeEepromMap::ACTIVE_CHANNEL_MASK, Value::UINT16(chs));			//active channels mask	(channels 1, 2, and 3)
	expectRead(impl, NodeEepromMap::SAMPLE_RATE, Value::UINT16(rate));					//sample rate			(2Hz)
	expectRead(impl, NodeEepromMap::SYNC_SAMPLE_SETTING, Value::UINT16(syncMode));		//sync sampling mode	(continuous)
	expectRead(impl, NodeEepromMap::DATA_FORMAT, Value::UINT16(dataFormat));			//data format			(2 byte uint)
	expectRead(impl, NodeEepromMap::COLLECTION_MODE, Value::UINT16(collectionMode));	//collection mode		(log and transmit)
	expectRead(impl, NodeEepromMap::SAMPLING_DELAY, Value::UINT16(samplingDelay));		//sampling delay		(5)
	expectRead(impl, NodeEepromMap::SAMPLING_MODE, Value::UINT16(samplingMode));

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.0, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.0, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 0);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 0);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 0);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNode_losslessDisabled_burst)
{
	//this test adds a single node in burst mode, with lossless disabled

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	SyncSamplingNetwork nwk(b);
	nwk.lossless(false);	//disabled lossless

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);

	//eeprom reads performed by the SyncSamplingNetwork
	uint16 retx = WirelessTypes::retransmission_on,
		chs = 5,
		rate = 105,
		syncMode = WirelessTypes::syncMode_burst,
		dataFormat = WirelessTypes::dataFormat_2byte_uint,
		sweeps = 256,
		timebetwburst = 350,
		collectionMode = WirelessTypes::collectionMethod_logAndTransmit,
		samplingDelay = 5,
		samplingMode = WirelessTypes::samplingMode_sync;

	expectRead(impl, NodeEepromMap::NODE_RETRANSMIT, Value::UINT16(retx));				//retransmission		(on)
	expectRead(impl, NodeEepromMap::ACTIVE_CHANNEL_MASK, Value::UINT16(chs));			//active channels mask	(channels 1 and 3)
	expectRead(impl, NodeEepromMap::SAMPLE_RATE, Value::UINT16(rate));					//sample rate			(256Hz)
	expectRead(impl, NodeEepromMap::SYNC_SAMPLE_SETTING, Value::UINT16(syncMode));		//sync sampling mode	(burst)
	expectRead(impl, NodeEepromMap::DATA_FORMAT, Value::UINT16(dataFormat));			//data format			(2 byte uint)
	expectRead(impl, NodeEepromMap::NUM_SWEEPS, Value::UINT16(sweeps));					//sweeps per session	(256)
	expectRead(impl, NodeEepromMap::TIME_BETW_SESSIONS, Value::UINT16(timebetwburst));	//time between sessions	(350)
	expectRead(impl, NodeEepromMap::COLLECTION_MODE, Value::UINT16(collectionMode));	//collection mode		(log and transmit)
	expectRead(impl, NodeEepromMap::SAMPLING_DELAY, Value::UINT16(samplingDelay));		//sampling delay		(5)
	expectRead(impl, NodeEepromMap::SAMPLING_MODE, Value::UINT16(samplingMode));
	
	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 6.256, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 6.256, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 8);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 120);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 1);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNode_losslessDisabled_continuous)
{
	//this tests adds a single node in continuous mode, with lossless disabled

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);
	nwk.lossless(false);	//disable lossless

	//eeprom reads performed by the SyncSamplingNetwork
	uint16 retx = WirelessTypes::retransmission_on,
		chs = 7,
		rate = 105,
		syncMode = WirelessTypes::syncMode_continuous,
		dataFormat = WirelessTypes::dataFormat_2byte_uint,
		collectionMode = WirelessTypes::collectionMethod_transmitOnly,
		samplingDelay = 5,
		samplingMode = WirelessTypes::samplingMode_sync;

	expectRead(impl, NodeEepromMap::NODE_RETRANSMIT, Value::UINT16(retx));				//retransmission		(on)
	expectRead(impl, NodeEepromMap::ACTIVE_CHANNEL_MASK, Value::UINT16(chs));			//active channels mask	(channels 1 2 and 3)
	expectRead(impl, NodeEepromMap::SAMPLE_RATE, Value::UINT16(rate));					//sample rate			(256Hz)
	expectRead(impl, NodeEepromMap::SYNC_SAMPLE_SETTING, Value::UINT16(syncMode));		//sync sampling mode	(burst)
	expectRead(impl, NodeEepromMap::DATA_FORMAT, Value::UINT16(dataFormat));			//data format			(2 byte uint)
	expectRead(impl, NodeEepromMap::COLLECTION_MODE, Value::UINT16(collectionMode));	//collection mode		(log and transmit)
	expectRead(impl, NodeEepromMap::SAMPLING_DELAY, Value::UINT16(samplingDelay));		//sampling delay		(5)
	expectRead(impl, NodeEepromMap::SAMPLING_MODE, Value::UINT16(samplingMode));

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 12.512, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 12.512, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 16);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 56);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 1);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNode_highCapacity)
{
	//this tests adds a single node in continuous mode, with high capacity enabled

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);
	nwk.lossless(false);	//disable lossless
	nwk.highCapacity(true);	//enable high capacity

	//eeprom reads performed by the SyncSamplingNetwork
	uint16 retx = WirelessTypes::retransmission_on,
		chs = 7,
		rate = 110,
		syncMode = WirelessTypes::syncMode_continuous,
		dataFormat = WirelessTypes::dataFormat_2byte_uint,
		collectionMode = WirelessTypes::collectionMethod_transmitOnly,
		samplingDelay = 5,
		samplingMode = WirelessTypes::samplingMode_sync;

	expectRead(impl, NodeEepromMap::NODE_RETRANSMIT, Value::UINT16(retx));				//retransmission		(on)
	expectRead(impl, NodeEepromMap::ACTIVE_CHANNEL_MASK, Value::UINT16(chs));			//active channels mask	(channels 1 2 and 3)
	expectRead(impl, NodeEepromMap::SAMPLE_RATE, Value::UINT16(rate));					//sample rate			(256Hz)
	expectRead(impl, NodeEepromMap::SYNC_SAMPLE_SETTING, Value::UINT16(syncMode));		//sync sampling mode	(burst)
	expectRead(impl, NodeEepromMap::DATA_FORMAT, Value::UINT16(dataFormat));			//data format			(2 byte uint)
	expectRead(impl, NodeEepromMap::COLLECTION_MODE, Value::UINT16(collectionMode));	//collection mode		(log and transmit)
	expectRead(impl, NodeEepromMap::SAMPLING_DELAY, Value::UINT16(samplingDelay));		//sampling delay		(5)
	expectRead(impl, NodeEepromMap::SAMPLING_MODE, Value::UINT16(samplingMode));

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_EQUAL(nwk.highCapacity(), true);
	BOOST_CHECK_EQUAL(nwk.lossless(), false);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.391, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.391, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 2);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 2040);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNode_highCapacity2)
{
	//this tests adds a single node in continuous mode, with high capacity enabled, with 16hz 1ch

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);
	nwk.lossless(false);	//disable lossless
	nwk.highCapacity(true);	//enable high capacity

	//eeprom reads performed by the SyncSamplingNetwork
	uint16 retx = WirelessTypes::retransmission_on,
		chs = 1,
		rate = 109,
		syncMode = WirelessTypes::syncMode_continuous,
		dataFormat = WirelessTypes::dataFormat_2byte_uint,
		collectionMode = WirelessTypes::collectionMethod_transmitOnly,
		samplingDelay = 5,
		samplingMode = WirelessTypes::samplingMode_sync;

	expectRead(impl, NodeEepromMap::NODE_RETRANSMIT, Value::UINT16(retx));				//retransmission		(on)
	expectRead(impl, NodeEepromMap::ACTIVE_CHANNEL_MASK, Value::UINT16(chs));			//active channels mask	(channel 1)
	expectRead(impl, NodeEepromMap::SAMPLE_RATE, Value::UINT16(rate));					//sample rate			(16Hz)
	expectRead(impl, NodeEepromMap::SYNC_SAMPLE_SETTING, Value::UINT16(syncMode));		//sync sampling mode	(burst)
	expectRead(impl, NodeEepromMap::DATA_FORMAT, Value::UINT16(dataFormat));			//data format			(2 byte uint)
	expectRead(impl, NodeEepromMap::COLLECTION_MODE, Value::UINT16(collectionMode));	//collection mode		(log and transmit)
	expectRead(impl, NodeEepromMap::SAMPLING_DELAY, Value::UINT16(samplingDelay));		//sampling delay		(5)
	expectRead(impl, NodeEepromMap::SAMPLING_MODE, Value::UINT16(samplingMode));

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_EQUAL(nwk.highCapacity(), true);
	BOOST_CHECK_EQUAL(nwk.lossless(), false);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.391, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.391, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 2);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 2040);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNode_multiple)
{
	//this tests adds a single node in continuous mode

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	uint16 retx = WirelessTypes::retransmission_on,
		chs = 7,
		rate = 112,
		syncMode = WirelessTypes::syncMode_continuous,
		dataFormat = WirelessTypes::dataFormat_2byte_uint,
		collectionMode = WirelessTypes::collectionMethod_logAndTransmit,
		samplingDelay = 5,
		samplingMode = WirelessTypes::samplingMode_sync;

	expectRead(impl, NodeEepromMap::NODE_RETRANSMIT, Value::UINT16(retx));				//retransmission		(on)
	expectRead(impl, NodeEepromMap::ACTIVE_CHANNEL_MASK, Value::UINT16(chs));			//active channels mask	(channel 1 2 3)
	expectRead(impl, NodeEepromMap::SAMPLE_RATE, Value::UINT16(rate));					//sample rate			(16Hz)
	expectRead(impl, NodeEepromMap::SYNC_SAMPLE_SETTING, Value::UINT16(syncMode));		//sync sampling mode	(burst)
	expectRead(impl, NodeEepromMap::DATA_FORMAT, Value::UINT16(dataFormat));			//data format			(2 byte uint)
	expectRead(impl, NodeEepromMap::COLLECTION_MODE, Value::UINT16(collectionMode));	//collection mode		(log and transmit)
	expectRead(impl, NodeEepromMap::SAMPLING_DELAY, Value::UINT16(samplingDelay));		//sampling delay		(5)
	expectRead(impl, NodeEepromMap::SAMPLING_MODE, Value::UINT16(samplingMode));

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);

	//try to add the node again (nothing happens)
	nwk.addNode(node100);

	//verify the network information hasn't changed
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo again for the nodes in the network
	const SyncNetworkInfo& info2 = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info2.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info2.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info2.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info2.groupSize(), 1);
	BOOST_CHECK_EQUAL(info2.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info2.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info2.tdmaAddress(), 9);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNode_failCommunication)
{
	//this tests adding a node that fails to communicate

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	expectReadModel(impl, WirelessModels::node_gLink_2g);

	SyncSamplingNetwork nwk(b);

	expectGoodPing(impl);
	MOCK_EXPECT(impl->readEeprom).throws(Error_NodeCommunication(100));

	//add the node to the network
	BOOST_CHECK_THROW(nwk.addNode(node100), Error_NodeCommunication);

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.0, 0.1);

	//the node is not in the network, should throw an exception
	BOOST_CHECK_THROW(nwk.getNodeNetworkInfo(100), Error);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNode_failCommunication_thenSucceed)
{
	//this tests adding a node that fails to communicate, and then adding it again and succeeding

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	expectReadModel(impl, WirelessModels::node_gLink_2g);

	SyncSamplingNetwork nwk(b);

	MOCK_EXPECT(impl->readEeprom).once().throws(Error_NodeCommunication(100));

	//add the node to the network
	BOOST_CHECK_THROW(nwk.addNode(node100), Error_NodeCommunication);

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.0, 0.1);

	//the node is not in the network, should throw an exception
	BOOST_CHECK_THROW(nwk.getNodeNetworkInfo(100), Error);

	//eeprom reads performed by the SyncSamplingNetwork
	uint16 retx = WirelessTypes::retransmission_on,
		chs = 7,
		rate = 112,
		syncMode = WirelessTypes::syncMode_continuous,
		dataFormat = WirelessTypes::dataFormat_2byte_uint,
		collectionMode = WirelessTypes::collectionMethod_logAndTransmit,
		samplingDelay = 5,
		samplingMode = WirelessTypes::samplingMode_sync;

	expectRead(impl, NodeEepromMap::NODE_RETRANSMIT, Value::UINT16(retx));				//retransmission		(on)
	expectRead(impl, NodeEepromMap::ACTIVE_CHANNEL_MASK, Value::UINT16(chs));			//active channels mask	(channel 1 2 3)
	expectRead(impl, NodeEepromMap::SAMPLE_RATE, Value::UINT16(rate));					//sample rate			(16Hz)
	expectRead(impl, NodeEepromMap::SYNC_SAMPLE_SETTING, Value::UINT16(syncMode));		//sync sampling mode	(burst)
	expectRead(impl, NodeEepromMap::DATA_FORMAT, Value::UINT16(dataFormat));			//data format			(2 byte uint)
	expectRead(impl, NodeEepromMap::COLLECTION_MODE, Value::UINT16(collectionMode));	//collection mode		(log and transmit)
	expectRead(impl, NodeEepromMap::SAMPLING_DELAY, Value::UINT16(samplingDelay));		//sampling delay		(5)
	expectRead(impl, NodeEepromMap::SAMPLING_MODE, Value::UINT16(samplingMode));

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network again
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo again for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_removeNode)
{
	//this tests removing a single node

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	uint16 retx = WirelessTypes::retransmission_on,
		chs = 7,
		rate = 112,
		syncMode = WirelessTypes::syncMode_continuous,
		dataFormat = WirelessTypes::dataFormat_2byte_uint,
		collectionMode = WirelessTypes::collectionMethod_logAndTransmit,
		samplingDelay = 5,
		samplingMode = WirelessTypes::samplingMode_sync;

	expectRead(impl, NodeEepromMap::NODE_RETRANSMIT, Value::UINT16(retx));				//retransmission		(on)
	expectRead(impl, NodeEepromMap::ACTIVE_CHANNEL_MASK, Value::UINT16(chs));			//active channels mask	(channel 1 2 3)
	expectRead(impl, NodeEepromMap::SAMPLE_RATE, Value::UINT16(rate));					//sample rate			(16Hz)
	expectRead(impl, NodeEepromMap::SYNC_SAMPLE_SETTING, Value::UINT16(syncMode));		//sync sampling mode	(burst)
	expectRead(impl, NodeEepromMap::DATA_FORMAT, Value::UINT16(dataFormat));			//data format			(2 byte uint)
	expectRead(impl, NodeEepromMap::COLLECTION_MODE, Value::UINT16(collectionMode));	//collection mode		(log and transmit)
	expectRead(impl, NodeEepromMap::SAMPLING_DELAY, Value::UINT16(samplingDelay));		//sampling delay		(5)
	expectRead(impl, NodeEepromMap::SAMPLING_MODE, Value::UINT16(samplingMode));

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo again for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);

	//trying to remove a node that isn't in the network doesn't cause an error
	nwk.removeNode(456);

	//remove the node from the network
	BOOST_CHECK_NO_THROW(nwk.removeNode(node100.nodeAddress()));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.0, 0.1);

	//the node is not in the network, should throw an exception
	BOOST_CHECK_THROW(nwk.getNodeNetworkInfo(100), Error);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_refresh)
{
	//this tests adding a node, changing its configuration, and then calling refresh

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);

	
	//NODE CHANGES TO BURST MODE

	MOCK_RESET(impl->readEeprom);//reset all expections

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Burst sb;
	sb.retx = WirelessTypes::retransmission_on;
	sb.chs = 5;
	sb.rate = 105;
	sb.syncMode = WirelessTypes::syncMode_burst;
	sb.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	sb.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	sb.samplingDelay = 5;
	sb.samplingMode = WirelessTypes::samplingMode_sync;
	sb.sweeps = 256;
	sb.timebetwburst = 350;
	expectSampling_Burst(impl, sb);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);


	//refresh the network
	BOOST_CHECK_NO_THROW(nwk.refresh());

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 12.512, 0.1);

	//verify the networkInfo for the nodes in the network
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 12.512, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 16);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 56);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 1);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNode_disableLosslessAfterwards)
{
	//this tests adds a single node in continuous mode, with lossless disabled

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 105;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the new network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 25.02, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 25.02, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 64);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 8);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 1);

	//now disable lossless
	BOOST_CHECK_NO_THROW(nwk.lossless(false));

	//verify the new network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 12.512, 0.1);

	//verify the networkInfo again for the nodes in the network
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 12.512, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 16);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 56);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 1);

	MOCK_RESET(impl->readEeprom_uint16);//reset all expections

	//disable lossless again, and with no expections set, this will verify that nothing gets changed in the network
	BOOST_CHECK_NO_THROW(nwk.lossless(false));
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addMultipleNodes)
{
	//this test adds multiple nodes to the network

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);


	//create another node to add
	BaseStation b2 = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl2(new mock_WirelessNodeImpl(b2, 200));
	WirelessNode node200(200, b2);
	node200.setImpl(impl2);

	expectNodeFeatures(features, impl2);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Burst sb;
	sb.retx = WirelessTypes::retransmission_on;
	sb.chs = 5;
	sb.rate = 105;
	sb.syncMode = WirelessTypes::syncMode_burst;
	sb.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	sb.sweeps = 256;
	sb.timebetwburst = 350;
	sb.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	sb.samplingDelay = 5;
	sb.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Burst(impl2, sb);

	expectReadModel(impl2, WirelessModels::node_gLink_2g);
	expectGoodPing(impl2);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node200));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 13.294, 0.1);

	const SyncNetworkInfo& info2 = nwk.getNodeNetworkInfo(200);
	BOOST_CHECK_EQUAL(info2.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info2.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info2.percentBandwidth(), 12.512, 0.1);
	BOOST_CHECK_EQUAL(info2.groupSize(), 1);
	BOOST_CHECK_EQUAL(info2.transmissionPerGroup(), 16);
	BOOST_CHECK_EQUAL(info2.maxTdmaAddress(), 56);
	BOOST_CHECK_EQUAL(info2.tdmaAddress(), 1);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNode_enablehighCapacityAfterwards)
{
	//this tests enabling high capacity after a node was added

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 110;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_transmitOnly;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_EQUAL(nwk.highCapacity(), false);
	BOOST_CHECK_EQUAL(nwk.lossless(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 2);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 504);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);

	//now turn off lossless and enable high capacity
	nwk.lossless(false);	//disable lossless
	nwk.highCapacity(true);	//enable high capacity

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_EQUAL(nwk.highCapacity(), true);
	BOOST_CHECK_EQUAL(nwk.lossless(), false);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.391, 0.1);

	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.391, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 2);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 2040);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);

	MOCK_RESET(impl->readEeprom_uint16);//reset all expections

	//enable high capacity again, and with no expections set, this will verify that nothing gets changed in the network
	BOOST_CHECK_NO_THROW(nwk.highCapacity(true));
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_applyConfiguration_continuous)
{
	//this tests applying the configuration with a single continuous node

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::TX_PER_GROUP, Value::UINT16((uint16)1)); //tx per group (should be 1)
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::GROUP_SIZE, Value::UINT16((uint16)1)); //group size (should be 1)
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::TDMA_ADDRESS, Value::UINT16((uint16)9)); //TDMA address (should be 9)

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);
	
	//apply the configuration
	BOOST_CHECK_NO_THROW(nwk.applyConfiguration());

	BOOST_CHECK_EQUAL(info.configurationApplied(), true);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_applyConfiguration_burst)
{
	//this tests applying the configuration with a single continuous node

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Burst sb;
	sb.retx = WirelessTypes::retransmission_off;
	sb.chs = 5;
	sb.rate = 105;
	sb.syncMode = WirelessTypes::syncMode_burst;
	sb.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	sb.sweeps = 256;
	sb.timebetwburst = 350;
	sb.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	sb.samplingDelay = 5;
	sb.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Burst(impl, sb);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 12.512, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 16);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 56);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 1);

	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::TX_PER_GROUP, Value::UINT16(static_cast<uint16>(16))); //tx per group
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::GROUP_SIZE, Value::UINT16(static_cast<uint16>(1))); //group size
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::TDMA_ADDRESS, Value::UINT16(static_cast<uint16>(1))); //TDMA address
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::NODE_RETRANSMIT, Value::UINT16(static_cast<uint16>(1))); //retransmission
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::MAX_RETRANS_BURST, Value::UINT16(static_cast<uint16>(1280))); //max retransmissions per burst


	//apply the configuration
	BOOST_CHECK_NO_THROW(nwk.applyConfiguration());

	BOOST_CHECK_EQUAL(info.configurationApplied(), true);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_applyConfiguration_again)
{
	//this tests applying the configuration twice, which should do nothing if nothing changed

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);

	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::TX_PER_GROUP, Value::UINT16(static_cast<uint16>(1))); //tx per group
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::GROUP_SIZE, Value::UINT16(static_cast<uint16>(1))); //group size
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::TDMA_ADDRESS, Value::UINT16(static_cast<uint16>(9))); //TDMA address

	//apply the configuration
	BOOST_CHECK_NO_THROW(nwk.applyConfiguration());

	BOOST_CHECK_EQUAL(info.configurationApplied(), true);

	MOCK_RESET(impl->readEeprom_uint16);//reset all expections
	MOCK_RESET(impl->writeEeprom_uint16);//reset all expections

	//this should do nothing as the node already has been applied and nothing changed
	BOOST_CHECK_NO_THROW(nwk.applyConfiguration());

	BOOST_CHECK_EQUAL(info.configurationApplied(), true);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_apply_networkNotOk)
{
	//this tests calling applyConfiguration() when the network is not "ok"

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);


	MOCK_RESET(impl->readEeprom);//reset all expections so refresh will fail
	MOCK_EXPECT(impl->readEeprom).throws(Error_NodeCommunication(100));

	//refresh the network
	BOOST_CHECK_THROW(nwk.refresh(), Error_NodeCommunication);

	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_PoorCommunication);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), false);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.0, 0.1);	//network bandwidth does not include nodes that failed to communicate

	BOOST_CHECK_THROW(nwk.applyConfiguration(), Error);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_startSampling_withBase_pcTime)
{
	std::shared_ptr<mock_baseStationImpl> baseImpl(new mock_baseStationImpl);
	BaseStation baseForNode(baseImpl);

	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(baseForNode));
	WirelessNode node(123, baseForNode);
	node.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);

	SyncSamplingNetwork nwk(baseForNode);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node));

	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::TX_PER_GROUP, Value::UINT16(static_cast<uint16>(1))); //tx per group
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::GROUP_SIZE, Value::UINT16(static_cast<uint16>(1))); //group size
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::TDMA_ADDRESS, Value::UINT16(static_cast<uint16>(9))); //TDMA address

	//apply the configuration
	BOOST_CHECK_NO_THROW(nwk.applyConfiguration());

	MOCK_EXPECT(baseImpl->disableBeacon).once();									//disable the beacon on the master base
	MOCK_EXPECT(baseImpl->node_startSyncSampling).with(123).returns(true);			//send the start sync sampling command to node 123
	MOCK_EXPECT(baseImpl->enableBeacon_pctime).returns(Timestamp(0));				//enable the beacon on the base station

	BOOST_CHECK_NO_THROW(nwk.startSampling());
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_startSampling_withBase_pcTime_configNotApplied)
{
	BaseStation baseForNode = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(baseForNode));
	WirelessNode node(123, baseForNode);
	node.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);

	SyncSamplingNetwork nwk(baseForNode);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node));

	BOOST_CHECK_THROW(nwk.startSampling(), Error);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_startSampling_withBase_timestamp)
{
	std::shared_ptr<mock_baseStationImpl> baseImpl(new mock_baseStationImpl);
	BaseStation baseForNode(baseImpl);

	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(baseForNode));
	WirelessNode node(123, baseForNode);
	node.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);

	SyncSamplingNetwork nwk(baseForNode);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node));

	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::TX_PER_GROUP, Value::UINT16(static_cast<uint16>(1))); //tx per group
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::GROUP_SIZE, Value::UINT16(static_cast<uint16>(1))); //group size
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::TDMA_ADDRESS, Value::UINT16(static_cast<uint16>(9))); //TDMA address

	//apply the configuration
	BOOST_CHECK_NO_THROW(nwk.applyConfiguration());

	MOCK_EXPECT(baseImpl->disableBeacon).once();										//disable the beacon on the master base
	MOCK_EXPECT(baseImpl->node_startSyncSampling).with(123).returns(true);				//send the start sync sampling command to node 123
	MOCK_EXPECT(baseImpl->enableBeacon_withTime).returns(Timestamp(123456));			//enable the beacon on the base station

	BOOST_CHECK_NO_THROW(nwk.startSampling(Timestamp(123456)));
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_startSampling_withBase_timestamp_configNotApplied)
{
	BaseStation baseForNode = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(baseForNode));
	WirelessNode node(123, baseForNode);
	node.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);

	SyncSamplingNetwork nwk(baseForNode);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node));

	BOOST_CHECK_THROW(nwk.startSampling(Timestamp(0)), Error);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_startSampling_noBase)
{
	std::shared_ptr<mock_baseStationImpl> baseImpl(new mock_baseStationImpl);
	BaseStation baseForNode(baseImpl);

	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(baseForNode));
	WirelessNode node(123, baseForNode);
	node.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);

	SyncSamplingNetwork nwk(baseForNode);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node));

	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::TX_PER_GROUP, Value::UINT16(static_cast<uint16>(1))); //tx per group
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::GROUP_SIZE, Value::UINT16(static_cast<uint16>(1))); //group size
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::TDMA_ADDRESS, Value::UINT16(static_cast<uint16>(9))); //TDMA address

	//apply the configuration
	BOOST_CHECK_NO_THROW(nwk.applyConfiguration());

	MOCK_EXPECT(baseImpl->node_startSyncSampling).with(123).returns(true);	//send the start sync sampling command to node 123

	BOOST_CHECK_NO_THROW(nwk.startSampling_noBeacon());
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_startSampling_noBase_configNotApplied)
{
	BaseStation baseForNode = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(baseForNode));
	WirelessNode node(123, baseForNode);
	node.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);

	SyncSamplingNetwork nwk(baseForNode);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node));

	BOOST_CHECK_THROW(nwk.startSampling_noBeacon(), Error);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_startSampling_failSendStart)
{
	std::shared_ptr<mock_baseStationImpl> baseImpl(new mock_baseStationImpl);
	BaseStation baseForNode(baseImpl);

	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(baseForNode));
	WirelessNode node(123, baseForNode);
	node.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);

	SyncSamplingNetwork nwk(baseForNode);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node));

	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::TX_PER_GROUP, Value::UINT16(static_cast<uint16>(1))); //tx per group
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::GROUP_SIZE, Value::UINT16(static_cast<uint16>(1))); //group size
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::TDMA_ADDRESS, Value::UINT16(static_cast<uint16>(9))); //TDMA address

	//apply the configuration
	BOOST_CHECK_NO_THROW(nwk.applyConfiguration());

	MOCK_EXPECT(baseImpl->node_startSyncSampling).with(123).returns(false);	//send the start sync sampling command to node 123

	BOOST_CHECK_THROW(nwk.startSampling_noBeacon(), Error_NodeCommunication);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_startSampling_retry)
{
	std::shared_ptr<mock_baseStationImpl> baseImpl(new mock_baseStationImpl);
	BaseStation baseForNode(baseImpl);

	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(baseForNode));
	WirelessNode node(123, baseForNode);
	node.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);

	SyncSamplingNetwork nwk(baseForNode);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node));

	std::shared_ptr<mock_WirelessNodeImpl> impl2(new mock_WirelessNodeImpl(baseForNode, 2));
	WirelessNode node2(2, baseForNode);
	node2.setImpl(impl2);

	std::unique_ptr<NodeFeatures> features2;
	expectNodeFeatures(features2, impl2, WirelessModels::node_vLink);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s2;
	s2.retx = WirelessTypes::retransmission_on;
	s2.chs = 255;
	s2.rate = 106;
	s2.syncMode = WirelessTypes::syncMode_continuous;
	s2.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s2.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s2.samplingDelay = 10;
	s2.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl2, s2);

	expectReadModel(impl2, WirelessModels::node_vLink);
	expectGoodPing(impl2);

	//add another node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node2));

	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::TX_PER_GROUP, Value::UINT16(static_cast<uint16>(1))); //tx per group
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::GROUP_SIZE, Value::UINT16(static_cast<uint16>(1))); //group size
	MOCK_EXPECT(impl->writeEeprom).with(NodeEepromMap::TDMA_ADDRESS, Value::UINT16(static_cast<uint16>(9))); //TDMA address

	MOCK_EXPECT(impl2->writeEeprom).with(NodeEepromMap::TX_PER_GROUP, mock::any); //tx per group
	MOCK_EXPECT(impl2->writeEeprom).with(NodeEepromMap::GROUP_SIZE, mock::any); //group size
	MOCK_EXPECT(impl2->writeEeprom).with(NodeEepromMap::TDMA_ADDRESS, mock::any); //TDMA address

	//apply the configuration
	BOOST_CHECK_NO_THROW(nwk.applyConfiguration());

	MOCK_EXPECT(baseImpl->node_startSyncSampling).with(123).returns(false);	//FAIL SENDING START TO NODE 123
	MOCK_EXPECT(baseImpl->node_startSyncSampling).with(2).returns(true);	//send the start sync sampling command to node 2

	BOOST_CHECK_THROW(nwk.startSampling_noBeacon(), Error_NodeCommunication);

	MOCK_RESET(baseImpl->node_startSyncSampling);//reset all expections
	MOCK_EXPECT(baseImpl->node_startSyncSampling).with(123).returns(true);	//NOW SUCCEED SENDING START TO NODE 2
	MOCK_EXPECT(baseImpl->node_startSyncSampling).with(2).returns(false);	//THIS SHOULDN'T MATTER BC IT SHOULDNT BE CALLED AS IT WAS ALREADY STARTED PREVIOUSLY

	BOOST_CHECK_NO_THROW(nwk.startSampling_noBeacon());
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNode_continuous_tooMuchBandwidth)
{
	//this tests adds a single node in continuous mode

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 12;
	s.rate = 104;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 50.049, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 50.049, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 64);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 8);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 1);

	BaseStation b2 = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl2(new mock_WirelessNodeImpl(b2, 200));
	WirelessNode node200(200, b2);
	node200.setImpl(impl2);

	expectNodeFeatures(features, impl2);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s2;
	s2.retx = WirelessTypes::retransmission_on;
	s2.chs = 7;
	s2.rate = 104;
	s2.syncMode = WirelessTypes::syncMode_continuous;
	s2.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s2.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s2.samplingDelay = 5;
	s2.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl2, s2);

	expectReadModel(impl2, WirelessModels::node_gLink_2g);
	expectGoodPing(impl2);

	//add another node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node200));

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info2 = nwk.getNodeNetworkInfo(200);
	BOOST_CHECK_EQUAL(info2.status(), SyncNetworkInfo::status_DoesNotFit);
	BOOST_CHECK_EQUAL(info2.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info2.percentBandwidth(), 50.049, 0.1);
	BOOST_CHECK_EQUAL(info2.groupSize(), 1);
	BOOST_CHECK_EQUAL(info2.transmissionPerGroup(), 64);
	BOOST_CHECK_EQUAL(info2.maxTdmaAddress(), 8);
	BOOST_CHECK_EQUAL(info2.tdmaAddress(), 0);

	BOOST_CHECK_EQUAL(nwk.ok(), false);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 100.098, 0.1);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNodes_networkContention)
{
	//this tests adding multiple nodes until a node fails to be added due to not
	//finding a slot even though the percent of bandwidth looks like it should be ok. (network contention)

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 1));
	WirelessNode node1(1, b);
	node1.setImpl(impl); 

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 255;
	s.rate = 105;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_transmitOnly;
	s.samplingDelay = 10;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_vLink);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node1));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 50.049, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(1);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 50.049, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 64);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 8);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 1);

	BaseStation b2 = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl2(new mock_WirelessNodeImpl(b2, 2));
	WirelessNode node2(2, b2);
	node2.setImpl(impl2);

	std::unique_ptr<NodeFeatures> features2;
	expectNodeFeatures(features2, impl2, WirelessModels::node_vLink);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s2;
	s2.retx = WirelessTypes::retransmission_on;
	s2.chs = 255;
	s2.rate = 106;
	s2.syncMode = WirelessTypes::syncMode_continuous;
	s2.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s2.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s2.samplingDelay = 10;
	s2.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl2, s2);

	expectReadModel(impl2, WirelessModels::node_vLink);
	expectGoodPing(impl2);

	//add another node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node2));

	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 75.073, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info2 = nwk.getNodeNetworkInfo(2);
	BOOST_CHECK_EQUAL(info2.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info2.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info2.percentBandwidth(), 25.024, 0.1);
	BOOST_CHECK_EQUAL(info2.groupSize(), 1);
	BOOST_CHECK_EQUAL(info2.transmissionPerGroup(), 32);
	BOOST_CHECK_EQUAL(info2.maxTdmaAddress(), 24);
	BOOST_CHECK_EQUAL(info2.tdmaAddress(), 9);


	BaseStation b3 = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl3(new mock_WirelessNodeImpl(b3, 3));
	WirelessNode node3(3, b3);
	node3.setImpl(impl3);

	std::unique_ptr<NodeFeatures> features3;
	expectNodeFeatures(features3, impl3, WirelessModels::node_vLink);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s3;
	s3.retx = WirelessTypes::retransmission_on;
	s3.chs = 255;
	s3.rate = 107;
	s3.syncMode = WirelessTypes::syncMode_continuous;
	s3.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s3.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s3.samplingDelay = 10;
	s3.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl3, s3);

	expectReadModel(impl3, WirelessModels::node_vLink);
	expectGoodPing(impl3);

	//add another node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node3));

	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 87.585, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info3 = nwk.getNodeNetworkInfo(3);
	BOOST_CHECK_EQUAL(info3.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info3.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info3.percentBandwidth(), 12.512, 0.1);
	BOOST_CHECK_EQUAL(info3.groupSize(), 1);
	BOOST_CHECK_EQUAL(info3.transmissionPerGroup(), 16);
	BOOST_CHECK_EQUAL(info3.maxTdmaAddress(), 56);
	BOOST_CHECK_EQUAL(info3.tdmaAddress(), 25);



	BaseStation b4 = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl4(new mock_WirelessNodeImpl(b4, 4));
	WirelessNode node4(4, b4);
	node4.setImpl(impl4);

	std::unique_ptr<NodeFeatures> features4;
	expectNodeFeatures(features4, impl4);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s4;
	s4.retx = WirelessTypes::retransmission_on;
	s4.chs = 1;
	s4.rate = 113;
	s4.syncMode = WirelessTypes::syncMode_continuous;
	s4.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s4.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s4.samplingDelay = 5;
	s4.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl4, s4);

	expectReadModel(impl4, WirelessModels::node_gLink_2g);
	expectGoodPing(impl4);

	//add another node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node4));

	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 88.367, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info4 = nwk.getNodeNetworkInfo(4);
	BOOST_CHECK_EQUAL(info4.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info4.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info4.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info4.groupSize(), 1);
	BOOST_CHECK_EQUAL(info4.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info4.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info4.tdmaAddress(), 57);


	BaseStation b5 = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl5(new mock_WirelessNodeImpl(b5, 5));
	WirelessNode node5(5, b5);
	node5.setImpl(impl5);

	std::unique_ptr<NodeFeatures> features5;
	expectNodeFeatures(features5, impl5, WirelessModels::node_tcLink_6ch);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s5;
	s5.retx = WirelessTypes::retransmission_on;
	s5.chs = 127;
	s5.rate = 110;
	s5.syncMode = WirelessTypes::syncMode_continuous;
	s5.dataFormat = WirelessTypes::dataFormat_4byte_float;
	s5.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s5.samplingDelay = 10;
	s5.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl5, s5);
	expectRead(impl5, NodeEepromMap::FILTER_1, Value::UINT16(static_cast<uint16>(1)));	//filter type

	expectReadModel(impl5, WirelessModels::node_tcLink_6ch);
	expectGoodPing(impl5);

	//add another node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node5));

	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 91.495, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info5 = nwk.getNodeNetworkInfo(5);
	BOOST_CHECK_EQUAL(info5.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info5.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info5.percentBandwidth(), 3.128, 0.1);
	BOOST_CHECK_EQUAL(info5.groupSize(), 1);
	BOOST_CHECK_EQUAL(info5.transmissionPerGroup(), 4);
	BOOST_CHECK_EQUAL(info5.maxTdmaAddress(), 248);
	BOOST_CHECK_EQUAL(info5.tdmaAddress(), 121);



	//ADDING THIS LAST NODE SHOULD CAUSE NETWORK CONTENTION	
	BaseStation b6 = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl6(new mock_WirelessNodeImpl(b6, 6));
	WirelessNode node6(6, b6);
	node6.setImpl(impl6);

	std::unique_ptr<NodeFeatures> features6;
	expectNodeFeatures(features6, impl6, WirelessModels::node_tcLink_6ch_ip67);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s6;
	s6.retx = WirelessTypes::retransmission_on;
	s6.chs = 127;
	s6.rate = 110;
	s6.syncMode = WirelessTypes::syncMode_continuous;
	s6.dataFormat = WirelessTypes::dataFormat_4byte_float;
	s6.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s6.samplingDelay = 10;
	s6.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl6, s6);
	expectRead(impl6, NodeEepromMap::FILTER_1, Value::UINT16(static_cast<uint16>(1)));	//filter type

	expectReadModel(impl6, WirelessModels::node_tcLink_6ch_ip67);
	expectGoodPing(impl6);

	//add another node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node6));

	BOOST_CHECK_EQUAL(nwk.ok(), false);	//network contention
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 94.623, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info6 = nwk.getNodeNetworkInfo(6);
	BOOST_CHECK_EQUAL(info6.status(), SyncNetworkInfo::status_Contention);
	BOOST_CHECK_EQUAL(info6.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info6.percentBandwidth(), 3.128, 0.1);
	BOOST_CHECK_EQUAL(info6.groupSize(), 1);
	BOOST_CHECK_EQUAL(info6.transmissionPerGroup(), 4);
	BOOST_CHECK_EQUAL(info6.maxTdmaAddress(), 248);
	BOOST_CHECK_EQUAL(info6.tdmaAddress(), 0);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNodes_tcLink6ch)
{
	//this tests adding a tcLink 6ch to the network

	BaseStation b6 = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b6, 6));
	WirelessNode node6(6, b6);
	node6.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl, WirelessModels::node_tcLink_6ch);

	SyncSamplingNetwork nwk(b6);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 255;
	s.rate = 113;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_4byte_float;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 10;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectRead(impl, NodeEepromMap::FILTER_1, Value::UINT16(static_cast<uint16>(1)));	//filter type
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_tcLink_6ch);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node6));

	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info6 = nwk.getNodeNetworkInfo(6);
	BOOST_CHECK_EQUAL(info6.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info6.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info6.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info6.groupSize(), 1);
	BOOST_CHECK_EQUAL(info6.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info6.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info6.tdmaAddress(), 161);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNodes_tcLink1ch)
{
	//this tests adding a tcLink 6ch to the network

	BaseStation b6 = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b6, 6));
	WirelessNode node6(6, b6);
	node6.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl, WirelessModels::node_tcLink_1ch);

	SyncSamplingNetwork nwk(b6);	

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 1;
	s.rate = 107;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_4byte_float;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 10;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectRead(impl, NodeEepromMap::FILTER_1, Value::UINT16(static_cast<uint16>(1)));	//filter type
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_tcLink_1ch);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node6));

	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 3.128, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info6 = nwk.getNodeNetworkInfo(6);
	BOOST_CHECK_EQUAL(info6.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info6.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info6.percentBandwidth(), 3.128, 0.1);
	BOOST_CHECK_EQUAL(info6.groupSize(), 1);
	BOOST_CHECK_EQUAL(info6.transmissionPerGroup(), 8);
	BOOST_CHECK_EQUAL(info6.maxTdmaAddress(), 120);
	BOOST_CHECK_EQUAL(info6.tdmaAddress(), 9);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNodes_envLinkPro)
{
	//this tests adding an env-link pro

	BaseStation b6 = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b6, 6));
	WirelessNode node6(6, b6);
	node6.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl, WirelessModels::node_envLink_pro);

	SyncSamplingNetwork nwk(b6);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 255;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_4byte_float;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 10;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectRead(impl, NodeEepromMap::FILTER_1, Value::UINT16(static_cast<uint16>(1)));	//filter 1
	expectRead(impl, NodeEepromMap::FILTER_2, Value::UINT16(static_cast<uint16>(1)));	//filter 1
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_envLink_pro);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node6));

	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info6 = nwk.getNodeNetworkInfo(6);
	BOOST_CHECK_EQUAL(info6.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info6.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info6.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info6.groupSize(), 1);
	BOOST_CHECK_EQUAL(info6.transmissionPerGroup(), 2);
	BOOST_CHECK_EQUAL(info6.maxTdmaAddress(), 504);
	BOOST_CHECK_EQUAL(info6.tdmaAddress(), 393);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNodes_envLinkMini)
{
	//this tests adding an envLink mini

	BaseStation b6 = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b6, 6));
	WirelessNode node6(6, b6);
	node6.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl, WirelessModels::node_envLink_mini);

	SyncSamplingNetwork nwk(b6);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 199;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_4byte_float;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 10;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectRead(impl, NodeEepromMap::FILTER_1, Value::UINT16(static_cast<uint16>(1)));	//filter type
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_envLink_mini);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node6));

	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info6 = nwk.getNodeNetworkInfo(6);
	BOOST_CHECK_EQUAL(info6.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info6.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info6.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info6.groupSize(), 1);
	BOOST_CHECK_EQUAL(info6.transmissionPerGroup(), 2);
	BOOST_CHECK_EQUAL(info6.maxTdmaAddress(), 504);
	BOOST_CHECK_EQUAL(info6.tdmaAddress(), 353);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNodes_wattLink)
{
	//this tests adding a watt-link

	BaseStation b6 = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b6, 6));
	WirelessNode node6(6, b6);
	node6.setImpl(impl);

	SyncSamplingNetwork nwk(b6);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 1;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_4byte_float;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 10;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_wattLink_3D400);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node6));

	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNodes_sgLinkHermetic)
{
	//this tests adding an sg-link hermetic

	BaseStation b6 = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b6, 6));
	WirelessNode node6(6, b6);
	node6.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl, WirelessModels::node_sgLink_herm_2700);

	SyncSamplingNetwork nwk(b6);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 1;
	s.rate = 105;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 10;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_sgLink_herm_2700);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node6));

	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 6.256, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info6 = nwk.getNodeNetworkInfo(6);
	BOOST_CHECK_EQUAL(info6.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info6.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info6.percentBandwidth(), 6.256, 0.1);
	BOOST_CHECK_EQUAL(info6.groupSize(), 1);
	BOOST_CHECK_EQUAL(info6.transmissionPerGroup(), 16);
	BOOST_CHECK_EQUAL(info6.maxTdmaAddress(), 56);
	BOOST_CHECK_EQUAL(info6.tdmaAddress(), 1);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_setPendingConfig)
{
	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl, WirelessModels::node_gLink_2g);

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);
	

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);


	//build the sampling config that we want to set as pending
	WirelessNodeConfig config;
	config.activeChannels(ChannelMask(5));
	config.sampleRate(WirelessTypes::sampleRate_256Hz);
	config.samplingMode(WirelessTypes::samplingMode_syncBurst);
	config.numSweeps(25600);
	config.timeBetweenBursts(TimeSpan::Seconds(350));

	//set the pending config for the node
	nwk.setPendingConfig(100, config);


	//verify the network information again, should now be changed
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 12.512, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info2 = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info2.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info2.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info2.percentBandwidth(), 12.512, 0.1);
	BOOST_CHECK_EQUAL(info2.groupSize(), 1);
	BOOST_CHECK_EQUAL(info2.transmissionPerGroup(), 16);
	BOOST_CHECK_EQUAL(info2.maxTdmaAddress(), 56);
	BOOST_CHECK_EQUAL(info2.tdmaAddress(), 1);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_setPendingConfig_InvalidNode)
{
	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);
	

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);


	//build the sampling config that we want to set as pending
	WirelessNodeConfig config;

	//set the pending config for a node NOT IN THE NETWORK
	BOOST_CHECK_THROW(nwk.setPendingConfig(200, config), Error);


	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_clearPendingConfig)
{
	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);


	//build the sampling config that we want to set as pending
	WirelessNodeConfig config;
	config.activeChannels(ChannelMask(5));
	config.sampleRate(WirelessTypes::sampleRate_256Hz);
	config.samplingMode(WirelessTypes::samplingMode_syncBurst);
	config.numSweeps(25600);
	config.timeBetweenBursts(TimeSpan::Seconds(350));

	//set the pending config for the node
	nwk.setPendingConfig(100, config);


	//verify the network information again, should now be changed
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 12.512, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info2 = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info2.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info2.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info2.percentBandwidth(), 12.512, 0.1);
	BOOST_CHECK_EQUAL(info2.groupSize(), 1);
	BOOST_CHECK_EQUAL(info2.transmissionPerGroup(), 16);
	BOOST_CHECK_EQUAL(info2.maxTdmaAddress(), 56);
	BOOST_CHECK_EQUAL(info2.tdmaAddress(), 1);

	//clear the pending config for the node
	nwk.clearPendingConfig(100);

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info3 = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info3.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info3.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info3.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info3.groupSize(), 1);
	BOOST_CHECK_EQUAL(info3.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info3.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info3.tdmaAddress(), 9);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_clearAllPendingConfigs)
{
	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);
	

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);


	//build the sampling config that we want to set as pending
	WirelessNodeConfig config;
	config.activeChannels(ChannelMask(5));
	config.sampleRate(WirelessTypes::sampleRate_256Hz);
	config.samplingMode(WirelessTypes::samplingMode_syncBurst);
	config.numSweeps(25600);
	config.timeBetweenBursts(TimeSpan::Seconds(350));

	//set the pending config for the node
	nwk.setPendingConfig(100, config);


	//verify the network information again, should now be changed
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 12.512, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info2 = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info2.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info2.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info2.percentBandwidth(), 12.512, 0.1);
	BOOST_CHECK_EQUAL(info2.groupSize(), 1);
	BOOST_CHECK_EQUAL(info2.transmissionPerGroup(), 16);
	BOOST_CHECK_EQUAL(info2.maxTdmaAddress(), 56);
	BOOST_CHECK_EQUAL(info2.tdmaAddress(), 1);

	//clear the pending config for the node
	nwk.clearAllPendingConfigs();

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info3 = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info3.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info3.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info3.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info3.groupSize(), 1);
	BOOST_CHECK_EQUAL(info3.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info3.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info3.tdmaAddress(), 9);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_addNode_invalidConfig)
{
	//this tests adding a node that has a config set to something other than sync sampling

	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_nonSync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_THROW(nwk.addNode(node100), Error_InvalidNodeConfig);
}

BOOST_AUTO_TEST_CASE(SyncSamplingNetwork_setPendingConfig_invalidConfig)
{
	BaseStation b = makeBaseStationWithMockImpl();
	std::shared_ptr<mock_WirelessNodeImpl> impl(new mock_WirelessNodeImpl(b, 100));
	WirelessNode node100(100, b);
	node100.setImpl(impl);

	std::unique_ptr<NodeFeatures> features;
	expectNodeFeatures(features, impl);
	

	SyncSamplingNetwork nwk(b);

	//eeprom reads performed by the SyncSamplingNetwork
	Sampling_Continuous s;
	s.retx = WirelessTypes::retransmission_on;
	s.chs = 7;
	s.rate = 112;
	s.syncMode = WirelessTypes::syncMode_continuous;
	s.dataFormat = WirelessTypes::dataFormat_2byte_uint;
	s.collectionMode = WirelessTypes::collectionMethod_logAndTransmit;
	s.samplingDelay = 5;
	s.samplingMode = WirelessTypes::samplingMode_sync;
	expectSampling_Continuous(impl, s);

	expectReadModel(impl, WirelessModels::node_gLink_2g);
	expectGoodPing(impl);

	//add the node to the network
	BOOST_CHECK_NO_THROW(nwk.addNode(node100));

	//verify the network information
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network
	const SyncNetworkInfo& info = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info.groupSize(), 1);
	BOOST_CHECK_EQUAL(info.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info.tdmaAddress(), 9);


	//build the sampling config that we want to set as pending
	WirelessNodeConfig config;
	config.activeChannels(ChannelMask(5));
	config.sampleRate(WirelessTypes::sampleRate_256Hz);
	config.samplingMode(WirelessTypes::samplingMode_nonSync);		//INVALID CONFIG
	config.numSweeps(25600);
	config.timeBetweenBursts(TimeSpan::Seconds(350));

	//set the pending config for the node
	BOOST_CHECK_THROW(nwk.setPendingConfig(100, config), Error_InvalidNodeConfig);


	//verify the network information again, nothing should have changed
	BOOST_CHECK_EQUAL(nwk.ok(), true);
	BOOST_CHECK_CLOSE(nwk.percentBandwidth(), 0.782, 0.1);

	//verify the networkInfo for the nodes in the network (nothing should have changed)
	const SyncNetworkInfo& info2 = nwk.getNodeNetworkInfo(100);
	BOOST_CHECK_EQUAL(info2.status(), SyncNetworkInfo::status_OK);
	BOOST_CHECK_EQUAL(info2.configurationApplied(), false);
	BOOST_CHECK_CLOSE(info2.percentBandwidth(), 0.782, 0.1);
	BOOST_CHECK_EQUAL(info2.groupSize(), 1);
	BOOST_CHECK_EQUAL(info2.transmissionPerGroup(), 1);
	BOOST_CHECK_EQUAL(info2.maxTdmaAddress(), 1016);
	BOOST_CHECK_EQUAL(info2.tdmaAddress(), 9);
}

BOOST_AUTO_TEST_SUITE_END()