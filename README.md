# CcspPandM (P&M) Documentation

CcspPandM is the RDK-B middleware component that serves as the central provisioning and management engine for broadband residential gateway devices. It acts as the primary TR-181 data model provider, exposing device information, network configuration, firewall settings, and system management capabilities. The component implements the Device.DeviceInfo, Device.Time, Device.UserInterface, Device.Bridging, Device.Ethernet, Device.IP, Device.DNS, Device.Firewall, and other core TR-181 objects required for remote management via TR-069 and other protocols.

P&M functions as the backbone of device management operations, handling configuration persistence through PSM (Persistent Storage Manager), and managing system-wide parameters. It integrates with various RDK-B middleware components to maintain device state, enforce security policies, and coordinate network operations across the broadband stack.

The component operates as a system service that must be initialized early in the boot sequence to provide essential TR-181 parameters to other middleware components, and external management systems. It serves as the source for device identity, capabilities, network configuration, and operational status information required for proper device operation and remote management.

```mermaid
graph LR
    subgraph "External Systems"
        RemoteMgmt["Remote Management"]
        LocalUI["Local Web UI"]
    end

    subgraph "RDK-B Platform"
        subgraph "Management Agents"
            ProtocolAgents["Protocol Agents<br/>(TR-069, USP, WebPA, webconfig)"]
        end

        PandM["CcspPandM"]
        OtherComponents["Other RDK-B Components<br/>(WanManager, OneWiFi, etc.)"]

        subgraph "System Layer"
            PlatformHAL["Platform HAL"]
            Linux["Linux System"]
        end
    end

    %% External connections
    RemoteMgmt -->|TR-069/WebPA/USP| ProtocolAgents
    LocalUI -->|HTTP/HTTPS| ProtocolAgents

    %% Upper layer to P&M
    ProtocolAgents -->|IPC| PandM

    %% P&M to services
    PandM -->|IPC| OtherComponents

    %% System integration
    PandM <-->|HAL APIs| PlatformHAL
    PlatformHAL <-->|Driver Interfaces| Linux

    classDef external fill:#fff3e0,stroke:#ef6c00,stroke-width:2px;
    classDef pandm fill:#e1f5fe,stroke:#0277bd,stroke-width:3px;
    classDef rdkbComponent fill:#e8f5e8,stroke:#2e7d32,stroke-width:2px;
    classDef system fill:#fce4ec,stroke:#c2185b,stroke-width:2px;

    class RemoteMgmt,LocalUI external;
    class PandM pandm;
    class ProtocolAgents,OtherComponents rdkbComponent;
    class PlatformHAL,Linux system;
```

**Key Features & Responsibilities**: 

- **TR-181 Data Model Provider**: Implements and exposes core TR-181 objects including Device.DeviceInfo, Device.Time, Device.UserInterface, Device.Bridging, Device.Ethernet, Device.IP, Device.DNS, and Device.Firewall for remote management and configuration
- **Device Identity Management**: Manages critical device information including manufacturer details, model information, serial numbers, hardware/software versions, and device capabilities required for provisioning and support
- **Network Configuration Management**: Provides centralized management of network interfaces, bridging configurations, DHCP settings, DNS configurations, and routing parameters across the broadband stack
- **Security and Firewall Control**: Implements firewall configuration management, security policies, and access control mechanisms to protect the device and home network from threats
- **System Configuration Persistence**: Interfaces with PSM and SysCfg to ensure configuration persistence across reboots and firmware upgrades, maintaining device state consistency
- **Time and NTP Management**: Manages system time synchronization, NTP server configurations, and timezone settings required for accurate logging and scheduled operations
- **User Interface Configuration**: Controls web UI access parameters, authentication settings, and remote access capabilities for device management
- **Status Monitoring and Reporting**: Provides real-time device status, performance metrics, and diagnostic information to management systems and other middleware components
- **Component Lifecycle Management**: Coordinates initialization, configuration updates, and shutdown procedures with other RDK-B middleware components through event notifications and state management

## Design

The CcspPandM component follows a modular, service-oriented architecture designed to serve as the central TR-181 data model provider and configuration management hub in the RDK-B middleware stack. The design emphasizes high availability, configuration persistence, and seamless integration with both northbound management protocols (TR-069, USP, WebPA) and southbound system interfaces (HAL, kernel, device drivers). The component operates as a system service with early initialization requirements to ensure other middleware components can access essential device parameters during boot.

The architecture implements a multi-layered approach with clear separation between the TR-181 parameter handling layer, internal business logic modules, configuration persistence layer, and hardware abstraction interfaces. This design enables scalable parameter management, efficient configuration updates, and reliable state synchronization across distributed RDK-B components. The component integrates RBus-based IPC mechanisms for high-performance inter-component communication while maintaining backward compatibility with legacy D-Bus interfaces where required.

The design incorporates robust error handling, configuration validation, and rollback mechanisms to ensure system stability during configuration changes. Event-driven notifications are used to propagate configuration updates to dependent components, while lazy initialization patterns optimize boot performance. The modular design allows for platform-specific customizations through well-defined HAL interfaces and configuration files, enabling deployment across diverse hardware platforms while maintaining consistent TR-181 compliance.

Data persistence is achieved through integration with PSM (Persistent Storage Manager) for TR-181 parameters and SysCfg for system-level configurations. The design includes comprehensive logging, telemetry integration, and diagnostic capabilities to support field troubleshooting and performance monitoring. Thread-safe parameter access patterns and atomic configuration updates ensure data consistency in multi-threaded environments typical of RDK-B deployments.

```mermaid
flowchart LR
    subgraph CcspPandM
        direction LR
        SSPMain["SSP Main<br/>Component Lifecycle"]
        RBusIf["RBus Interface<br/>Message Handling"]
        
        subgraph "TR-181 Implementation"
            DeviceInfoMod["Device Info Module<br/>Hardware & System"]
            TimeMod["Time Module<br/>NTP & Timezone"]
            NetworkMod["Network Module<br/>Ethernet & Bridging"]
            SecurityMod["Security Module<br/>Firewall & UI"]
            DNSMod["DNS Module<br/>Client Configuration"]
        end
        
        subgraph "Support Modules"
            ConfigMgr["Config Manager<br/>Parameter Persistence"]
            StatusMon["Status Monitor<br/>Health Tracking"]
            EventPub["Event Publisher<br/>Notifications"]
        end
    end
    
    SSPMain --> RBusIf
    RBusIf --> DeviceInfoMod
    RBusIf --> TimeMod
    RBusIf --> NetworkMod
    RBusIf --> SecurityMod
    RBusIf --> DNSMod
    
    DeviceInfoMod --> ConfigMgr
    TimeMod --> ConfigMgr
    NetworkMod --> ConfigMgr
    SecurityMod --> ConfigMgr
    DNSMod --> ConfigMgr
    
    StatusMon --> EventPub
    ConfigMgr --> EventPub
```

### Prerequisites and Dependencies

**Build-Time Flags and Configuration:**

| Configure Option | DISTRO Feature | Build Flag | Purpose | Default |
|------------------|----------------|------------|---------|---------|
| `--enable-maptsupport` | N/A | `FEATURE_SUPPORT_MAPT_NAT46` | Enable MAP-T (Mapping of Address and Port with Translation) IPv6 transition | Disabled |
| `--enable-core_net_lib_feature_support` | N/A | `CORE_NET_LIB_FEATURE_SUPPORT` | Enable advanced core networking library support | Disabled |
| `--enable-hotspotsupport` | N/A | `FEATURE_HOTSPOT_SUPPORT` | Enable WiFi hotspot and GRE tunnel configuration support | Disabled |

**RDK-B Platform and Integration Requirements:**

- **RDK-B Components**: CcspCommonLibrary, CcspPsm, CcspLMLite, systemd services for proper ordering
- **HAL Dependencies**: Platform HAL APIs, WiFi HAL, Ethernet HAL, MoCA HAL interfaces
- **Systemd Services**: CcspCrSsp.service, CcspPsmSsp.service, and platform HAL services must be active before CcspPandM
- **Message Bus**: RBus daemon with component registration under "com.cisco.spvtg.ccsp.pam" namespace
- **TR-181 Data Model**: Core TR-181 Issue 2 compliance with Device.DeviceInfo, Device.Time, Device.Bridging, Device.Ethernet object hierarchies
- **Configuration Files**: CcspPam.cfg, CcspDmLib.cfg, TR181-USGv2.XML data model definitions in /usr/ccsp/pam/
- **Startup Order**: PSM → SysCfg → HAL services → CcspPandM → dependent CCSP components

**Threading Model** 

CcspPandM implements a hybrid threading model combining event-driven message processing with dedicated worker threads for specific subsystem management. The component operates with a main event loop thread for RBus message handling and parameter requests, complemented by specialized threads for time synchronization, configuration persistence, and status monitoring operations.

- **Threading Architecture**: Multi-threaded with event-driven message processing and dedicated worker threads for subsystem management
- **Main Thread**: Handles RBus registration, message bus event processing, TR-181 parameter get/set operations, and component lifecycle management 
- **Main worker Threads**:
  - **Time Sync Thread**: Manages NTP synchronization, system clock updates, and timezone configuration changes   
  - **Config Persistence Thread**: Handles PSM/SysCfg write operations, configuration backup, and atomic parameter updates
  - **Status Monitor Thread**: Performs periodic status collection, health monitoring, and telemetry data gathering
  - **Event Publisher Thread**: Manages asynchronous event publishing to subscriber components and external systems
- **Synchronization**: Uses mutex locks for parameter access protection, atomic operations for configuration updates, and event queues for thread-safe communication

### Component State Flow

**Initialization to Active State**

CcspPandM follows a structured initialization sequence from system startup through active operational state, with careful dependency management and configuration validation to ensure reliable service provision to other RDK-B components.

```mermaid
sequenceDiagram
    autonumber
    participant Systemd
    participant PandM
    participant ConfigLoader
    participant PSM
    participant RBus
    participant TR181
    participant HAL

    Systemd->>PandM: Start Service → Initializing
    Note right of PandM: Initialize logging system<br/>Setup signal handlers<br/>Create component context<br/>Initialize memory pools

    PandM->>ConfigLoader: Load Configuration Files → LoadingConfig
    Note right of ConfigLoader: Load ccsp_msg.cfg<br/>Parse TR-181 XML definitions<br/>Read component configuration<br/>Validate configuration syntax

    ConfigLoader->>PSM: PSM Connection → ConnectingPSM
    PSM-->>PandM: PSM Connection Established

    PandM->>RBus: RBus Registration → RegisteringRBus
    RBus-->>PandM: RBus Registration Success

    PandM->>TR181: Load TR-181 Data Model → LoadingTR181
    TR181-->>PandM: TR-181 Data Model Loaded

    PandM->>HAL: Initialize HAL Interfaces → InitializingHAL
    HAL-->>PandM: HAL Interfaces Initialized

    PandM->>PandM: Validate Parameters → ValidatingConfig
    PandM->>PandM: Publish Ready Events → PublishingEvents
    PandM->>Systemd: All Systems Ready → Active

    Note right of PandM: Process TR-181 requests<br/>Handle configuration updates<br/>Monitor system status<br/>Publish telemetry events

    Systemd->>PandM: Parameter Change → ConfigUpdate
    PandM->>Systemd: Update Complete → Active

    Systemd->>PandM: Status Request → StatusUpdate
    PandM->>Systemd: Status Provided → Active

    Systemd->>PandM: Stop Request → Shutdown
    PandM->>Systemd: Shutdown Complete
```

**Runtime State Changes and Context Switching**

CcspPandM handles several runtime state transitions during normal operation, including configuration updates from management systems, system events requiring parameter refresh, and failover scenarios for configuration persistence.

**State Change Triggers:**

- TR-181 parameter set operations from management protocols trigger configuration validation and persistence workflows
- System time changes trigger NTP synchronization state updates and dependent parameter refresh operations
- Network interface status changes trigger Device.Ethernet and Device.IP parameter updates and event notifications
- PSM service availability changes trigger configuration persistence mode switching between active and cached operations
- HAL interface errors trigger fallback mechanisms and error state reporting to dependent components

**Context Switching Scenarios:**

- Configuration update mode switches the component from normal operation to atomic update processing with transaction rollback capability
- Time synchronization context switching occurs during NTP server changes, requiring temporary suspension of time-dependent operations
- Emergency configuration mode activates during PSM unavailability, using local caching and deferred persistence mechanisms
- Diagnostic mode switching enables enhanced logging and parameter tracing for troubleshooting without affecting normal operations

### Call Flow

**Initialization Call Flow:**

```mermaid
sequenceDiagram
    participant Init as Systemd
    participant PandM as CcspPandM
    participant RBus as RBus Daemon
    participant PSM as PSM Service
    participant HAL as HAL Layer

    Init->>PandM: Start Service
    PandM->>PandM: Initialize Logging & Config
    PandM->>RBus: Register Component
    RBus-->>PandM: Registration Success
    PandM->>PSM: Connect & Load Parameters
    PSM-->>PandM: Parameter Data
    PandM->>HAL: Initialize Device Interfaces
    HAL-->>PandM: Interface Status
    PandM->>RBus: Publish Ready Events
    PandM->>Init: Service Active (systemd notify)
```

**Request Processing Call Flow:**

```mermaid
sequenceDiagram
    participant Client as TR-069 PA
    participant PandM as CcspPandM
    participant Module as Internal Module
    participant PSM as PSM Service
    participant HAL as HAL Interface

    Client->>PandM: Get Device.DeviceInfo.SerialNumber
    PandM->>Module: Route Parameter Request
    Module->>HAL: Get Hardware Serial Number
    HAL-->>Module: Serial Number Data
    Module-->>PandM: Parameter Value
    PandM-->>Client: Response (Serial Number)
    
    Note over Client,HAL: Parameter Set Operation
    Client->>PandM: Set Device.Time.NTPServer1
    PandM->>Module: Validate & Process
    Module->>PSM: Persist Configuration
    PSM-->>Module: Persistence Confirmed
    Module->>HAL: Update NTP Configuration
    HAL-->>Module: Configuration Applied
    Module-->>PandM: Success Response
    PandM-->>Client: Set Operation Complete
```

## TR‑181 Data Models

### Supported TR-181 Parameters

CcspPandM serves as the primary implementation provider for core TR-181 data model objects essential for device management, network configuration, and system administration in RDK-B broadband gateways. The component implements standard BBF TR-181 Issue 2 parameters along with RDK-specific extensions for enhanced functionality and platform integration.

### Object Hierarchy

```
Device.
├── DeviceInfo.
│   ├── Manufacturer (string, R)
│   ├── ManufacturerOUI (string, R)
│   ├── ModelName (string, R)
│   ├── Description (string, R)
│   ├── ProductClass (string, R)
│   ├── SerialNumber (string, R)
│   ├── HardwareVersion (string, R)
│   ├── SoftwareVersion (string, R)
│   ├── UpTime (unsignedInt, R)
│   ├── FirstUseDate (dateTime, R)
│   ├── FactoryResetCount (unsignedInt, R)
│   ├── MemoryStatus.
│   │   ├── Total (unsignedInt, R)
│   │   ├── Used (unsignedInt, R)
│   │   └── Free (unsignedInt, R)
│   └── X_RDKCENTRAL-COM_RFC.
│       └── Feature.{i}.
├── Time.
│   ├── Enable (boolean, R/W)
│   ├── Status (string, R)
│   ├── NTPServer1 (string, R/W)
│   ├── NTPServer2 (string, R/W)
│   ├── NTPServer3 (string, R/W)
│   ├── NTPServer4 (string, R/W)
│   ├── NTPServer5 (string, R/W)
│   ├── CurrentLocalTime (dateTime, R)
│   ├── LocalTimeZone (string, R/W)
│   └── TimeOffset (string, R/W)
├── UserInterface.
│   ├── PasswordReset (boolean, R/W)
│   ├── PasswordLockoutEnable (boolean, R/W)
│   ├── PasswordLockoutAttempts (unsignedInt, R/W)
│   ├── PasswordLockoutTime (unsignedInt, R/W)
│   └── X_CISCO_COM_RemoteAccess.
│       ├── HttpEnable (boolean, R/W)
│       ├── HttpPort (unsignedInt, R/W)
│       ├── HttpsEnable (boolean, R/W)
│       └── HttpsPort (unsignedInt, R/W)
├── Bridging.
│   ├── MaxBridgeEntries (unsignedInt, R)
│   ├── BridgeNumberOfEntries (unsignedInt, R)
│   └── Bridge.{i}.
│       ├── Enable (boolean, R/W)
│       ├── Status (string, R)
│       ├── Name (string, R/W)
│       ├── Standard (string, R/W)
│       └── Port.{i}.
├── Ethernet.
│   ├── InterfaceNumberOfEntries (unsignedInt, R)
│   ├── LinkNumberOfEntries (unsignedInt, R)
│   ├── Interface.{i}.
│   │   ├── Enable (boolean, R/W)
│   │   ├── Status (string, R)
│   │   ├── Name (string, R)
│   │   ├── MACAddress (string, R)
│   │   └── Stats.
└── DNS.
    └── Client.
        ├── Enable (boolean, R/W)
        ├── Status (string, R)
        ├── ServerNumberOfEntries (unsignedInt, R)
        └── Server.{i}.
            ├── Enable (boolean, R/W)
            ├── DNSServer (string, R/W)
            └── Interface (string, R/W)
```

### Parameter Definitions

**Core Parameters:**

| Parameter Path | Data Type | Access | Default Value | Description | BBF Compliance |
|----------------|-----------|--------|---------------|-------------|----------------|
| `Device.DeviceInfo.Manufacturer` | string | R | `"RDK Management"` | Device manufacturer name as configured during manufacturing process | TR-181 Issue 2 |
| `Device.DeviceInfo.ManufacturerOUI` | string | R | `"001234"` | Organizationally Unique Identifier assigned to manufacturer by IEEE Registration Authority | TR-181 Issue 2 |
| `Device.DeviceInfo.ModelName` | string | R | `"RDK Reference"` | Model designation assigned by manufacturer to identify specific product variant and capabilities | TR-181 Issue 2 |
| `Device.DeviceInfo.SerialNumber` | string | R | `"000000000000"` | Unique identifier assigned by manufacturer for device identification and support operations | TR-181 Issue 2 |
| `Device.DeviceInfo.HardwareVersion` | string | R | `"1.0"` | Hardware revision identifier indicating major and minor hardware design changes affecting functionality | TR-181 Issue 2 |
| `Device.DeviceInfo.SoftwareVersion` | string | R | `"RDKB-1.0"` | Software version string identifying current firmware release including major, minor, and patch levels | TR-181 Issue 2 |
| `Device.DeviceInfo.UpTime` | unsignedInt | R | `0` | Time in seconds since device last reboot, reset automatically upon system restart | TR-181 Issue 2 |
| `Device.Time.Enable` | boolean | R/W | `true` | Enable or disable NTP time synchronization functionality with configurable server endpoints | TR-181 Issue 2 |
| `Device.Time.Status` | string | R | `"Disabled"` | Current time synchronization status: Disabled, Unsynchronized, Synchronized, Error_FailedToSynchronize | TR-181 Issue 2 |
| `Device.Time.NTPServer1` | string | R/W | `"pool.ntp.org"` | Primary NTP server hostname or IP address for time synchronization with fallback support | TR-181 Issue 2 |
| `Device.Bridging.Bridge.{i}.Enable` | boolean | R/W | `true` | Enable or disable bridge instance with automatic port configuration and VLAN support | TR-181 Issue 2 |
| `Device.Ethernet.Interface.{i}.Enable` | boolean | R/W | `true` | Enable or disable Ethernet interface with automatic link detection and speed negotiation | TR-181 Issue 2 |
| `Device.DNS.Client.Server.{i}.DNSServer` | string | R/W | `"8.8.8.8"` | DNS server IP address with automatic validation and reachability testing | TR-181 Issue 2 |

### Parameter Registration and Access

- **Implemented Parameters**: CcspPandM implements over 500 TR-181 parameters across Device.DeviceInfo, Device.Time, Device.UserInterface, Device.Bridging, Device.Ethernet, Device.IP, Device.DNS, Device.Firewall, and Device.GatewayInfo objects with full read/write capability and validation
- **Parameter Registration**: Parameters are registered with RBus message bus using namespace "com.cisco.spvtg.ccsp.pam" with automatic discovery and capability advertisement to management protocols
- **Access Mechanism**: Other RDK-B components access parameters via RBus IPC using standard get/set operations with transaction support and atomic updates for related parameter groups
- **Validation Rules**: Comprehensive parameter validation including data type checking, range validation, dependency verification, and cross-parameter consistency enforcement with rollback capability

## Internal Modules

CcspPandM is structured around several key internal modules that handle specific aspects of device management and TR-181 parameter implementation. Each module is responsible for a defined set of functionality and interfaces with other modules through well-defined APIs.

| Module/Class | Description | Key Files |
|-------------|------------|-----------|
| SSP Main | System Service Provider main module handling component initialization, RBus registration, and lifecycle management | `ssp_main.c`, `ssp_global.h`, `ssp_internal.h` |
| TR-181 APIs | Implementation of TR-181 parameter handlers for all supported objects including get/set operations and validation | `cosa_deviceinfo_apis.h`, `cosa_time_apis.h`, `cosa_ethernet_apis.h` |
| Device Info Module | Manages Device.DeviceInfo parameters including hardware identification, system status, and capability reporting | `cosa_deviceinfo_apis.h`, device info implementation files |
| Network Management | Handles Device.Ethernet, Device.Bridging, Device.IP, and Device.DNS parameter implementation and network configuration | `cosa_ethernet_apis.h`, `cosa_bridging_apis.h`, `cosa_ip_apis.h` |
| Time Management | Implements Device.Time parameters, NTP synchronization, and timezone management functionality | `cosa_time_apis.h`, time management implementation files |
| Security Management | Manages Device.Firewall parameters, Device.UserInterface security settings, and access control policies | `cosa_firewall_apis.h`, `cosa_userinterface_apis.h` |
| Configuration Persistence | Interfaces with PSM and SysCfg for parameter persistence, backup, and restore operations | PSM client interfaces, SysCfg integration modules |

## Component Interactions

CcspPandM serves as a central hub for TR-181 parameter management, interacting with numerous RDK-B middleware components, system services, and external management systems. The component provides essential device information and configuration services required by other middleware components while consuming services from system-level components for hardware access and configuration persistence.

### Interaction Matrix

| Target Component/Layer | Communication Pattern | Key APIs/Endpoints |
|------------------------|---------------------|------------------|
| **RDK-B Middleware Components** |
| TR-069 PA | Request-Response/Pub-Sub | `Device.DeviceInfo.*`, `Device.Time.*`, `Device.ManagementServer.*` |
| USP PA | Request-Response/Event | `Device.LocalAgent.*`, `Device.DeviceInfo.*`, capability notifications |
| WebPA Agent | Request-Response/Async | `Device.*` parameter hierarchy, bulk operations |
| PSM Service | Synchronous/Transactional | `PSM_Set()`, `PSM_Get()`, transaction APIs |
| OneWiFi | Pub-Sub/Request-Response | `Device.Time.CurrentLocalTime`, status events |
| **System & HAL Layers** |
| HAL Layer | Synchronous Function Calls | `hal_get_device_info()`, `hal_ethernet_get_status()`, `hal_time_set()` |
| PSM Storage | Synchronous/Atomic | `/nvram/bbhm_cur_cfg.xml`, PSM database operations |
| SysCfg Service | File I/O/Event | `/etc/utopia/service.d/`, syscfg commit operations |

**Main events Published by CcspPandM:**

| Event Name | Event Topic/Path | Trigger Condition | Subscriber Components |
|------------|-----------------|-------------------|---------------------|
| Device.DeviceInfo.UpTime | `Device.DeviceInfo.UpTime` | System uptime updates every 60 seconds | TR-069 PA, Telemetry, Status Monitor |
| Device.Time.Status | `Device.Time.Status` | NTP synchronization status changes | OneWiFi, Certificate Manager, Log Manager |
| Device.Ethernet.Interface.Status | `Device.Ethernet.Interface.{i}.Status` | Ethernet interface state changes | Bridge Manager, DHCP Manager, Network Monitor |
| Device.DeviceInfo.MemoryStatus | `Device.DeviceInfo.MemoryStatus.*` | Memory usage threshold exceeded | Self-Heal, Performance Monitor, Telemetry |
| Device.Bridging.Bridge.Status | `Device.Bridging.Bridge.{i}.Status` | Bridge interface operational status changes | Network Services, QoS Manager, VLAN Manager |

### IPC Flow Patterns

**Primary IPC Flow - TR-181 Parameter Get Operation:**

```mermaid
sequenceDiagram
    participant Client as TR-069 PA
    participant RBus as RBus Daemon
    participant PandM as CcspPandM
    participant HAL as HAL Layer
    participant PSM as PSM Service

    Client->>RBus: Get Device.DeviceInfo.SerialNumber
    RBus->>PandM: Route Parameter Request
    PandM->>PandM: Validate Parameter Path
    alt Cached Value Available
        PandM->>PandM: Return Cached Value
    else Fresh Data Required
        PandM->>HAL: hal_get_device_info()
        HAL-->>PandM: Device Information
        PandM->>PandM: Update Cache
    end
    PandM-->>RBus: Parameter Value Response
    RBus-->>Client: Device Serial Number
```

**Configuration Update Flow:**

```mermaid
sequenceDiagram
    participant Client as WebPA Agent
    participant PandM as CcspPandM
    participant PSM as PSM Service
    participant SysCfg as SysCfg
    participant HAL as HAL Layer
    participant Subscribers as Event Subscribers

    Client->>PandM: Set Device.Time.NTPServer1
    PandM->>PandM: Validate Parameter Value
    PandM->>PSM: Persist Parameter
    PSM-->>PandM: Persistence Confirmed
    PandM->>SysCfg: Update System Config
    SysCfg-->>PandM: Config Applied
    PandM->>HAL: Update NTP Configuration
    HAL-->>PandM: Configuration Active
    PandM->>Subscribers: Publish Parameter Change Event
    PandM-->>Client: Set Operation Success
```

## Implementation Details

### Major HAL APIs Integration

CcspPandM integrates with multiple HAL interfaces to provide hardware abstraction and platform-specific functionality for TR-181 parameter implementation. The component relies on standardized HAL APIs for device information, network interface management, and time synchronization services.

**Core HAL APIs:**

| HAL API | Purpose | Implementation File |
|---------|---------|-------------------|
| `hal_get_device_info()` | Retrieve hardware device information for Device.DeviceInfo parameters | `cosa_deviceinfo_apis.c` |
| `hal_ethernet_get_interface_info()` | Get Ethernet interface status and configuration for Device.Ethernet | `cosa_ethernet_apis.c` |
| `hal_time_set_ntp_config()` | Configure NTP servers and time synchronization settings | `cosa_time_apis.c` |
| `hal_firewall_get_config()` | Retrieve firewall configuration and security settings | `cosa_firewall_apis.c` |
| `hal_bridge_get_status()` | Get bridge interface status and port information | `cosa_bridging_apis.c` |

### Key Implementation Logic

- **Parameter Request Router**: Central request routing engine that dispatches TR-181 parameter get/set operations to appropriate internal modules based on parameter namespace and access permissions. Multi-threaded request processing with priority queues for management protocol requests. Parameter validation engine with data type checking, range validation, and dependency verification. Request routing logic in `ssp_messagebus_interface.c` with namespace-based module dispatch

- **Configuration State Machine**: Comprehensive state management for configuration updates with validation, rollback, and consistency checking across related parameter groups. Atomic transaction support for multi-parameter updates with automatic rollback on validation failures. Configuration validation with cross-parameter dependency checking and constraint enforcement. State persistence and recovery mechanisms for configuration consistency across reboots

- **Event Processing**: Asynchronous event processing system for hardware status changes, configuration updates, and system notifications with reliable delivery guarantees. Hardware event monitoring with automatic parameter refresh and status synchronization. Configuration change event propagation to subscriber components with guaranteed delivery. Asynchronous event processing with queuing and retry mechanisms for reliability

- **Error Handling Strategy**: Comprehensive error detection, logging, and recovery mechanisms for robust operation in production environments. HAL interface error handling with automatic retry and fallback mechanisms for hardware communication failures. Configuration validation errors with detailed error reporting and automatic rollback to last known good state. Parameter access error handling with graceful degradation and alternative data source utilization.

- **Logging & Debugging**: Advanced logging framework with configurable verbosity levels and diagnostic capabilities for field troubleshooting. Structured logging with component identification, severity levels, and contextual information for operational monitoring. Parameter access tracing with request/response logging and performance metrics collection. Debug hooks for runtime parameter inspection and configuration state analysis.

### Key Configuration Files

| Configuration File | Purpose | Override Mechanisms |
|--------------------|---------|-------------------|
| `CcspPam.cfg` | Component registration and RBus configuration | Environment variables, command line args |
| `/etc/utopia/service.d/pam.conf` | System service configuration and dependencies | Systemd service overrides |
