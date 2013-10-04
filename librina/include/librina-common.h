/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/**
 * This library provides definitions common to all the other RINA
 * related libraries presented in this document. Common
 * functionalities shared among framework components (i.e. applications,
 * daemons and libraries) might be made available from this library
 * as well.
 */

#ifndef LIBRINA_COMMON_H
#define LIBRINA_COMMON_H

#ifdef __cplusplus

#include <string>
#include <vector>
#include <list>

#include "exceptions.h"
#include "patterns.h"

namespace rina {

/**
 * Returns the version number of librina
 */
std::string getVersion();

/**
 * A class that can be printed as a String
 */
class StringConvertable {
public:
	virtual std::string toString() = 0;
	virtual ~StringConvertable() {
	}
};

/**
 * Contains application naming information
 */
class ApplicationProcessNamingInformation: public StringConvertable {
	/**
	 * The process_name identifies an application process within the
	 * application process namespace. This value is required, it
	 * cannot be NULL. This name has global scope (it is defined by
	 * the chain of IDD databases that are linked together), and is
	 * assigned by an authority that manages the namespace that
	 * particular application name belongs to.
	 */
	std::string processName;

	/**
	 * The process_instance identifies a particular instance of the
	 * process. This value is optional, it may be NULL.
	 */
	std::string processInstance;

	/**
	 * The entity_name identifies an application entity within the
	 * application process. This value is optional, it may be NULL.
	 */
	std::string entityName;

	/**
	 * The entity_name identifies a particular instance of an entity
	 * within the application process. This value is optional, it
	 * may be NULL
	 */
	std::string entityInstance;

public:
	ApplicationProcessNamingInformation();
	ApplicationProcessNamingInformation(const std::string& processName,
			const std::string& processInstance);
	ApplicationProcessNamingInformation & operator=(
			const ApplicationProcessNamingInformation & other);
	bool operator==(const ApplicationProcessNamingInformation &other) const;
	bool operator!=(const ApplicationProcessNamingInformation &other) const;
	bool operator>(const ApplicationProcessNamingInformation &other) const;
	bool operator<=(const ApplicationProcessNamingInformation &other) const;
	bool operator<(const ApplicationProcessNamingInformation &other) const;
	bool operator>=(const ApplicationProcessNamingInformation &other) const;
	const std::string& getEntityInstance() const;
	void setEntityInstance(const std::string& entityInstance);
	const std::string& getEntityName() const;
	void setEntityName(const std::string& entityName);
	const std::string& getProcessInstance() const;
	void setProcessInstance(const std::string& processInstance);
	const std::string& getProcessName() const;
	void setProcessName(const std::string& processName);
	std::string getProcessNamePlusInstance();
    std::string toString();
};

/**
 * This class defines the characteristics of a flow
 */
class FlowSpecification {
	/** Average bandwidth in bytes/s. A value of 0 means don't care. */
	unsigned int averageBandwidth;

	/** Average bandwidth in SDUs/s. A value of 0 means don't care */
	unsigned int averageSDUBandwidth;

	/** In milliseconds. A value of 0 means don't care*/
	unsigned int peakBandwidthDuration;

	/** In milliseconds. A value of 0 means don't care */
	unsigned int peakSDUBandwidthDuration;

	/** A value of 0 indicates 'do not care' */
	double undetectedBitErrorRate;

	/** Indicates if partial delivery of SDUs is allowed or not */
	bool partialDelivery;

	/** Indicates if SDUs have to be delivered in order */
	bool orderedDelivery;

	/**
	 * Indicates the maximum gap allowed among SDUs, a gap of N SDUs
	 * is considered the same as all SDUs delivered. A value of -1
	 * indicates 'Any'
	 */
	int maxAllowableGap;

	/**
	 * In milliseconds, indicates the maximum delay allowed in this
	 * flow. A value of 0 indicates 'do not care'
	 */
	unsigned int delay;

	/**
	 * In milliseconds, indicates the maximum jitter allowed in this
	 * flow. A value of 0 indicates 'do not care'
	 */
	unsigned int jitter;

	/**
	 * The maximum SDU size for the flow. May influence the choice
	 * of the DIF where the flow will be created.
	 */
	unsigned int maxSDUsize;

public:
	FlowSpecification();
	bool operator==(const FlowSpecification &other) const;
	bool operator!=(const FlowSpecification &other) const;
	unsigned int getAverageBandwidth() const;
	void setAverageBandwidth(unsigned int averageBandwidth);
	unsigned int getAverageSduBandwidth() const;
	void setAverageSduBandwidth(unsigned int averageSduBandwidth);
	unsigned int getDelay() const;
	void setDelay(unsigned int delay);
	unsigned int getJitter() const;
	void setJitter(unsigned int jitter);
	int getMaxAllowableGap() const;
	void setMaxAllowableGap(int maxAllowableGap);
	unsigned int getMaxSDUSize() const;
	void setMaxSDUSize(unsigned int maxSduSize);
	bool isOrderedDelivery() const;
	void setOrderedDelivery(bool orderedDelivery);
	bool isPartialDelivery() const;
	void setPartialDelivery(bool partialDelivery);
	unsigned int getPeakBandwidthDuration() const;
	void setPeakBandwidthDuration(unsigned int peakBandwidthDuration);
	unsigned int getPeakSduBandwidthDuration() const;
	void setPeakSduBandwidthDuration(unsigned int peakSduBandwidthDuration);
	double getUndetectedBitErrorRate() const;
	void setUndetectedBitErrorRate(double undetectedBitErrorRate);
};

/**
 * Defines the properties that a QoSCube is able to provide
 */
class QoSCube {

	/** The name of the QoS cube*/
	std::string name;

	/** The id of the QoS cube */
	int id;

	/** Average bandwidth in bytes/s. A value of 0 means don't care. */
	unsigned int averageBandwidth;

	/** Average bandwidth in SDUs/s. A value of 0 means don't care */
	unsigned int averageSDUBandwidth;

	/** In milliseconds. A value of 0 means don't care*/
	unsigned int peakBandwidthDuration;

	/** In milliseconds. A value of 0 means don't care */
	unsigned int peakSDUBandwidthDuration;

	/** A value of 0 indicates 'do not care' */
	double undetectedBitErrorRate;

	/** Indicates if partial delivery of SDUs is allowed or not */
	bool partialDelivery;

	/** Indicates if SDUs have to be delivered in order */
	bool orderedDelivery;

	/**
	 * Indicates the maximum gap allowed among SDUs, a gap of N SDUs
	 * is considered the same as all SDUs delivered. A value of -1
	 * indicates 'Any'
	 */
	int maxAllowableGap;

	/**
	 * In milliseconds, indicates the maximum delay allowed in this
	 * flow. A value of 0 indicates 'do not care'
	 */
	unsigned int delay;

	/**
	 * In milliseconds, indicates the maximum jitter allowed in this
	 * flow. A value of 0 indicates 'do not care'
	 */
	unsigned int jitter;
public:
	QoSCube();
	QoSCube(const std::string& name, int id);
	bool operator==(const QoSCube &other) const;
	bool operator!=(const QoSCube &other) const;
	int getId() const;
	const std::string& getName() const;
	unsigned int getAverageBandwidth() const;
	void setAverageBandwidth(unsigned int averageBandwidth);
	unsigned int getAverageSduBandwidth() const;
	void setAverageSduBandwidth(unsigned int averageSduBandwidth);
	unsigned int getDelay() const;
	void setDelay(unsigned int delay);
	unsigned int getJitter() const;
	void setJitter(unsigned int jitter);
	int getMaxAllowableGap() const;
	void setMaxAllowableGap(int maxAllowableGap);
	bool isOrderedDelivery() const;
	void setOrderedDelivery(bool orderedDelivery);
	bool isPartialDelivery() const;
	void setPartialDelivery(bool partialDelivery);
	unsigned int getPeakBandwidthDuration() const;
	void setPeakBandwidthDuration(unsigned int peakBandwidthDuration);
	unsigned int getPeakSduBandwidthDuration() const;
	void setPeakSduBandwidthDuration(unsigned int peakSduBandwidthDuration);
	double getUndetectedBitErrorRate() const;
	void setUndetectedBitErrorRate(double undetectedBitErrorRate);
};

/**
 * This class contains the properties of a single DIF
 */
class DIFProperties {
	/** The name of the DIF */
	ApplicationProcessNamingInformation DIFName;

	/**
	 * The maximum SDU size this DIF can handle (writes with bigger
	 * SDUs will return an error, and read will never return an SDUs
	 * bigger than this size
	 */
	unsigned int maxSDUSize;

	/**
	 * The different QoS cubes supported by the DIF
	 */
	std::list<QoSCube> qosCubes;
public:
	DIFProperties();
	DIFProperties(const ApplicationProcessNamingInformation& DIFName,
			int maxSDUSize);
	const ApplicationProcessNamingInformation& getDifName() const;
	unsigned int getMaxSduSize() const;
	const std::list<QoSCube>& getQoSCubes() const;
	void addQoSCube(const QoSCube& qosCube);
	void removeQoSCube(const QoSCube& qosCube);
};

/**
 * Enum type that identifies the different types of events
 */
enum IPCEventType {
	FLOW_ALLOCATION_REQUESTED_EVENT,
	FLOW_DEALLOCATION_REQUESTED_EVENT,
	APPLICATION_UNREGISTERED_EVENT,
	FLOW_DEALLOCATED_EVENT,
	APPLICATION_REGISTRATION_REQUEST_EVENT,
	APPLICATION_UNREGISTRATION_REQUEST_EVENT,
	APPLICATION_REGISTRATION_CANCELED_EVENT,
	ASSIGN_TO_DIF_REQUEST_EVENT,
	IPC_PROCESS_REGISTERED_TO_DIF,
	IPC_PROCESS_UNREGISTERED_FROM_DIF,
	IPC_PROCESS_QUERY_RIB,
	GET_DIF_PROPERTIES,
	OS_PROCESS_FINALIZED
};

/**
 * Base class for IPC Events
 */
class IPCEvent {
	/** The type of event */
	IPCEventType eventType;

	/**
	 * If the event is a request, this is the number to relate it
	 * witht the response
	 */
	unsigned int sequenceNumber;

public:
	virtual ~IPCEvent(){}

	IPCEvent(IPCEventType eventType, unsigned int sequenceNumber) {
		this->eventType = eventType;
		this->sequenceNumber = sequenceNumber;
	}

	IPCEventType getType() const {
		return eventType;
	}

	unsigned int getSequenceNumber() const{
		return sequenceNumber;
	}
};

/**
 * Event informing about an incoming flow request from a local application
 */
class FlowRequestEvent: public IPCEvent {
	/** The port-id that locally identifies the flow */
	int portId;

	/** The name of the DIF that is providing this flow */
	ApplicationProcessNamingInformation DIFName;

	/** The local application name*/
	ApplicationProcessNamingInformation localApplicationName;

	/** The remote application name*/
	ApplicationProcessNamingInformation remoteApplicationName;

	/** The characteristics of the flow */
	FlowSpecification flowSpecification;

	bool localRequest;

public:
	FlowRequestEvent(const FlowSpecification& flowSpecification,
			bool localRequest,
			const ApplicationProcessNamingInformation& localApplicationName,
			const ApplicationProcessNamingInformation& remoteApplicationName,
			unsigned int sequenceNumber);
	FlowRequestEvent(int portId,
			const FlowSpecification& flowSpecification,
			bool localRequest,
			const ApplicationProcessNamingInformation& localApplicationName,
			const ApplicationProcessNamingInformation& remoteApplicationName,
			const ApplicationProcessNamingInformation& DIFName,
			unsigned int sequenceNumber);
	int getPortId() const;
	bool isLocalRequest() const;
	const FlowSpecification& getFlowSpecification() const;
	void setPortId(int portId);
	void setDIFName(const ApplicationProcessNamingInformation& difName);
	const ApplicationProcessNamingInformation& getDIFName() const;
	const ApplicationProcessNamingInformation& getLocalApplicationName() const;
	const ApplicationProcessNamingInformation& getRemoteApplicationName() const;
};

/**
 * Event informing the IPC Process about a flow deallocation request
 */
class FlowDeallocateRequestEvent: public IPCEvent {
	/** The port-id that locally identifies the flow */
	int portId;

	/** The application that requested the flow deallocation*/
	ApplicationProcessNamingInformation applicationName;

public:
	FlowDeallocateRequestEvent(int portId,
			const ApplicationProcessNamingInformation& appName,
			unsigned int sequenceNumber);
	FlowDeallocateRequestEvent(int portId,
				unsigned int sequenceNumber);
	int getPortId() const;
	const ApplicationProcessNamingInformation& getApplicationName() const;
};

/**
 * Event informing that a flow has been deallocated by an IPC Process, without
 * the application having requested it
 */
class FlowDeallocatedEvent: public IPCEvent {
	/** The port id of the deallocated flow */
	int portId;

	/** An error code indicating why the flow was deallocated */
	int code;

public:
	FlowDeallocatedEvent(int portId, int code);
	int getPortId() const;
	int getCode() const;
	const ApplicationProcessNamingInformation getDIFName() const;
};

/**
 * Identifies the types of application registrations
 * APPLICATION_REGISTRATION_SINGLE_DIF - registers the application in a single
 * DIF, specified by the application
 * APPLICATION_REGISTRATION_ANY_DIF - registers the application in any of the
 * DIFs available to the application, chosen by the IPC Manager
 */
enum ApplicationRegistrationType {
	APPLICATION_REGISTRATION_SINGLE_DIF,
	APPLICATION_REGISTRATION_ANY_DIF
};

/**
 * Contains information about the registration of an application
 */
class ApplicationRegistrationInformation {

	/** The type of registration requested */
	ApplicationRegistrationType applicationRegistrationType;

	/** Optional DIF name where the application wants to register */
	ApplicationProcessNamingInformation difName;

public:
	ApplicationRegistrationInformation();
	ApplicationRegistrationInformation(
			ApplicationRegistrationType applicationRegistrationType);
	ApplicationRegistrationType getRegistrationType() const;
	const ApplicationProcessNamingInformation& getDIFName() const;
	void setDIFName(const ApplicationProcessNamingInformation& difName);
};

/**
 * Event informing that an application has requested the
 * registration to a DIF
 */
class ApplicationRegistrationRequestEvent: public IPCEvent {
	/** The application that wants to register */
	ApplicationProcessNamingInformation applicationName;

	/** The application registration information*/
	ApplicationRegistrationInformation applicationRegistrationInformation;

public:
	ApplicationRegistrationRequestEvent(
		const ApplicationProcessNamingInformation& appName,
		const ApplicationRegistrationInformation&
		applicationRegistrationInformation, unsigned int sequenceNumber);
	const ApplicationProcessNamingInformation& getApplicationName() const;
	const ApplicationRegistrationInformation&
		getApplicationRegistrationInformation() const;
};

/**
 * Event informing that an application has requested the
 * unregistration from a DIF
 */
class ApplicationUnregistrationRequestEvent: public IPCEvent {
	/** The application that wants to unregister */
	ApplicationProcessNamingInformation applicationName;

	/** The DIF to which the application wants to cancel the registration */
	ApplicationProcessNamingInformation DIFName;

public:
	ApplicationUnregistrationRequestEvent(
			const ApplicationProcessNamingInformation& appName,
			const ApplicationProcessNamingInformation& DIFName,
			unsigned int sequenceNumber);
	const ApplicationProcessNamingInformation& getApplicationName() const;
	const ApplicationProcessNamingInformation& getDIFName() const;
};

/**
 * Event informing that an OS process (an application or an
 * IPC Process daemon) has finalized
 */
class OSProcessFinalizedEvent: public IPCEvent {
	/**
	 * The naming information of the application that has
	 * finalized
	 */
	ApplicationProcessNamingInformation applicationName;

	/**
	 * If this id is greater than 0, it means that the process
	 * that finalized was an IPC Process Daemon. Otherwise it is an
	 * application process.
	 */
	unsigned int ipcProcessId;

public:
	OSProcessFinalizedEvent(const ApplicationProcessNamingInformation& appName,
			unsigned int ipcProcessId, unsigned int sequenceNumber);
	const ApplicationProcessNamingInformation& getApplicationName() const;
	unsigned int getIPCProcessId() const;
};

/**
 * Stores IPC Events that have happened, ready to be consumed and
 * processed by client classes.
 */
class IPCEventProducer {
public:
	/** Retrieves the next available event, if any */
	IPCEvent * eventPoll();

	/** Blocks until there is an event available */
	IPCEvent * eventWait();

	/**
	 * Blocks until there is an event available, no more than the
	 * time specified
	 */
	IPCEvent * eventTimedWait(int seconds, int nanoseconds);
};

/**
 * Make IPCManager singleton
 */
extern Singleton<IPCEventProducer> ipcEventProducer;

/**
 * Base class for all RINA exceptions
 */
class IPCException: public Exception {
public:
	IPCException(const std::string& description);
	static const std::string operation_not_implemented_error;
};

/**
 * Represents a policy. This is a generic placeholder which should be defined
 * during the second prototype activities
 */
class Policy {

public:
	bool operator==(const Policy &other) const;
	bool operator!=(const Policy &other) const;
};

/**
 * Represents a parameter that has a name and value
 */
class Parameter {
	std::string name;
	std::string value;

public:
	Parameter();
	Parameter(const std::string & name, const std::string & value);
	bool operator==(const Parameter &other) const;
	bool operator!=(const Parameter &other) const;
	const std::string& getName() const;
	void setName(const std::string& name);
	const std::string& getValue() const;
	void setValue(const std::string& value);
};

/**
 * Contains the data about a DIF Configuration
 */
class DIFConfiguration {

	/** The type of DIF */
	std::string difType;

	/** The name of the DIF */
	ApplicationProcessNamingInformation difName;

	/** The QoS cubes supported by the DIF */
	std::list<QoSCube> qosCubes;

	/** The policies of the DIF */
	std::list<Policy> policies;

	/** Configuration parameters */
	std::list<Parameter> parameters;

public:
	const ApplicationProcessNamingInformation& getDifName() const;
	void setDifName(const ApplicationProcessNamingInformation& difName);
	const std::string& getDifType() const;
	void setDifType(const std::string& difType);
	const std::list<Policy>& getPolicies();
	void setPolicies(const std::list<Policy>& policies);
	const std::list<QoSCube>& getQosCubes() const;
	void setQosCubes(const std::list<QoSCube>& qosCubes);
	const std::list<Parameter>& getParameters() const;
	void setParameters(const std::list<Parameter>& parameters);
	void addParameter(const Parameter& parameter);
};

/**
 * Represents the value of an object stored in the RIB
 */
class RIBObjectValue{
	//TODO
};

/**
 * Represents an object in the RIB
 */
class RIBObject{

	/** The class (type) of object */
	std::string clazz;

	/** The name of the object (unique within a class)*/
	std::string name;

	/** A synonim for clazz+name (unique within the RIB) */
	long instance;

	/** The value of the object */
	RIBObjectValue value;

	/** Geneartes a unique object instance */
	long generateObjectInstance();

public:
	RIBObject();
	RIBObject(std::string clazz, std::string name, RIBObjectValue value);
	bool operator==(const RIBObject &other) const;
	bool operator!=(const RIBObject &other) const;
	const std::string& getClazz() const;
	void setClazz(const std::string& clazz);
	long getInstance() const;
	void setInstance(long instance);
	const std::string& getName() const;
	void setName(const std::string& name);
	RIBObjectValue getValue() const;
	void setValue(RIBObjectValue value);
};

/**
 * Thrown when there are problems initializing librina
 */
class InitializationException: public IPCException {
public:
	InitializationException():
		IPCException("Problems initializing librina"){
	}
	InitializationException(const std::string& description):
		IPCException(description){
	}
};

/**
 * Initialize librina providing the local Netlink port-id where this librina
 * instantiation will be bound
 * @param localPort
 */
void initialize(unsigned int localPort);

/**
 * Initialize librina letting the OS choose the Netlink port-id where this
 * librina instantiation will be bound
 */
void initialize();

}
#endif

#endif
