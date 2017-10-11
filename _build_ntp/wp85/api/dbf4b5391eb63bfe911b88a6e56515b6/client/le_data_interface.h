/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_le_data Data Connection
 *
 * @ref le_data_interface.h "API Reference"
 *
 * <HR>
 *
 * A data connection is needed for applications that exchange data with
 * devices where SMS messages are insufficient or not possible.  The data connection can
 * be over a mobile network, over Wi-Fi, or over a fixed link (e.g., Ethernet).
 *
 * The data connection service provides a basic API for requesting and releasing a data connection.
 *
 * @note This interface does not support yet data connection over a fixed link.
 *
 * @section le_data_binding IPC interfaces binding
 *
 * All the functions of this API are provided by the @b dataConnectionService application service.
 *
 * Here's a code sample binding to Data Connection services:
 * @verbatim
   bindings:
   {
      clientExe.clientComponent.le_data -> dataConnectionService.le_data
   }
   @endverbatim
 *
 * @section c_le_data_default Default Data Connection
 *
 * Default data connection is obtained using le_data_Request().  Before the data
 * connection is requested, an application registers a connection state handler using
 * le_data_AddConnectionStateHandler().  Once the data connection is established, the
 * handler will be called indicating it's now connected.  If the state of the data
 * connection changes, then the handler will be called with the new state. To release a data
 * connection, an application can use le_data_Release().
 *
 * The technology of the established connection can be retrieved with le_data_GetTechnology().
 *
 * If the default data connection is not currently available when le_data_Request() is called,
 * the data connection service first ensures all pre-conditions are satisfied (e.g.,
 * modem is registered on the network), before trying to start the data connection.
 *
 * If the default data connection is already available when le_data_Request() is called, a
 * new connection will not be started.  Instead, the existing data connection will be used.  This
 * happens if another application also requested the default data connection.  This
 * is how multiple applications can share the same data connection.
 *
 * Once an application makes a data connection request, it should monitor the connection state
 * reported to the registered connection state handler.  The application
 * should only try transmitting data when the state is connected, and should stop transmitting
 * data when the state is not connected.  If the state is not connected, the data connection
 * service will try to re-establish the connection. There's no need for an application to
 * issue a new connection request.
 *
 * The default data connection will not necessarily be released when an application calls
 * le_data_Release().  The data connection will be released only after le_data_Release()
 * is called by all applications that previously called le_data_Request().
 *
 * @section c_le_data_rank Technology rank
 *
 * The technology to use for the default data connection can be chosen by the applications
 * with an ordered list. If the connection becomes unavailable through a technology, the next
 * one in the list is used for the default data connection. If the connection is also unavailable
 * through the last technology of the list, the first technology will be used again.
 * The default sequence is @ref LE_DATA_WIFI at rank #1 and @ref LE_DATA_CELLULAR at rank #2.
 *
 * @note
 * - Only one list is available and therefore only one application should set the technology
 * preferences for the default data connection.
 * - The list should not be modified while the default data connection is established.
 * .
 *
 *
 * - le_data_SetTechnologyRank() sets the rank of the technology to use for the data connection
 * service.
 * le_data_SetTechnologyRank() inserts a technology into a list, so all the technologies
 * previously set with ranks @c r and @c r+n are automatically shifted to ranks @c r+1
 * and @c r+n+1. Technologies with ranks under @c r are not impacted. If the technology is already
 * in the list, it is removed from its current rank and added to the new rank.
 *
 * - le_data_GetFirstUsedTechnology() and le_data_GetNextUsedTechnology() let you retrieve
 * the different technologies of the ordered list to use for the default connection data.
 *
 * @section c_le_data_configdb Configuration tree
 * @copydoc c_le_data_configdbPage_Hide
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */
/**
 * @interface c_le_data_configdbPage_Hide
 *
 * The configuration of the Wi-Fi access point is stored in the configuration database of the
 * @c dataConnectionService under the following path:
 * @verbatim
    dataConnectionService:/
        wifi/
            SSID<string> == TestSsid
            secProtocol<int> == 3
            passphrase<string> == Passw0rd
   @endverbatim
 *
 * @note The security protocol is one of the supported protocols defined in the
 * @ref le_wifiClient_SecurityProtocol_t enumerator.
 *
 * @todo This solution is temporary, as the list of access points to connect to should be
 * managed by the Wi-Fi client. Until its API is modified, the config tree is used to configure
 * the only access point to use for the default data connection.
 *
 */
/**
 * @file le_data_interface.h
 *
 * Legato @ref c_le_data include file.
 *
 * Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */

#ifndef LE_DATA_INTERFACE_H_INCLUDE_GUARD
#define LE_DATA_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

//--------------------------------------------------------------------------------------------------
/**
 *
 * Connect the current client thread to the service providing this API. Block until the service is
 * available.
 *
 * For each thread that wants to use this API, either ConnectService or TryConnectService must be
 * called before any other functions in this API.  Normally, ConnectService is automatically called
 * for the main thread, but not for any other thread. For details, see @ref apiFilesC_client.
 *
 * This function is created automatically.
 */
//--------------------------------------------------------------------------------------------------
void le_data_ConnectService
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Try to connect the current client thread to the service providing this API. Return with an error
 * if the service is not available.
 *
 * For each thread that wants to use this API, either ConnectService or TryConnectService must be
 * called before any other functions in this API.  Normally, ConnectService is automatically called
 * for the main thread, but not for any other thread. For details, see @ref apiFilesC_client.
 *
 * This function is created automatically.
 *
 * @return
 *  - LE_OK if the client connected successfully to the service.
 *  - LE_UNAVAILABLE if the server is not currently offering the service to which the client is bound.
 *  - LE_NOT_PERMITTED if the client interface is not bound to any service (doesn't have a binding).
 *  - LE_COMM_ERROR if the Service Directory cannot be reached.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_data_TryConnectService
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Disconnect the current client thread from the service providing this API.
 *
 * Normally, this function doesn't need to be called. After this function is called, there's no
 * longer a connection to the service, and the functions in this API can't be used. For details, see
 * @ref apiFilesC_client.
 *
 * This function is created automatically.
 */
//--------------------------------------------------------------------------------------------------
void le_data_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Reference returned by Request function and used by Release function
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_data_RequestObj* le_data_RequestObjRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Interface name string length.
 */
//--------------------------------------------------------------------------------------------------
#define LE_DATA_INTERFACE_NAME_MAX_LEN 100


//--------------------------------------------------------------------------------------------------
/**
 * Interface name string length. One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_DATA_INTERFACE_NAME_MAX_BYTES 101


//--------------------------------------------------------------------------------------------------
/**
 * Technologies.
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    LE_DATA_WIFI = 0,
        ///< Wi-Fi

    LE_DATA_CELLULAR = 1,
        ///< Cellular

    LE_DATA_MAX = 2
        ///< Unknown state.
}
le_data_Technology_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'le_data_ConnectionState'
 */
//--------------------------------------------------------------------------------------------------
typedef struct le_data_ConnectionStateHandler* le_data_ConnectionStateHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for connection state changes
 *
 * @param intfName
 *        Interface name for the data connection
 * @param isConnected
 *        State: connected or disconnected
 * @param contextPtr
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_data_ConnectionStateHandlerFunc_t)
(
    const char* intfName,
    bool isConnected,
    void* contextPtr
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'le_data_ConnectionState'
 *
 * This event provides information on connection state changes
 */
//--------------------------------------------------------------------------------------------------
le_data_ConnectionStateHandlerRef_t le_data_AddConnectionStateHandler
(
    le_data_ConnectionStateHandlerFunc_t handlerPtr,
        ///< [IN]

    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'le_data_ConnectionState'
 */
//--------------------------------------------------------------------------------------------------
void le_data_RemoveConnectionStateHandler
(
    le_data_ConnectionStateHandlerRef_t addHandlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Request the default data connection
 *
 * @return
 *      - Reference to the data connection (to be used later for releasing the connection)
 *      - NULL if the data connection requested could not be processed
 */
//--------------------------------------------------------------------------------------------------
le_data_RequestObjRef_t le_data_Request
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Release a previously requested data connection
 */
//--------------------------------------------------------------------------------------------------
void le_data_Release
(
    le_data_RequestObjRef_t requestRef
        ///< [IN] Reference to a previously requested data connection
);

//--------------------------------------------------------------------------------------------------
/**
 * Set the rank of the technology used for the data connection service
 *
 * @return
 *      - @ref LE_OK if the technology is added to the list
 *      - @ref LE_BAD_PARAMETER if the technology is unknown
 *      - @ref LE_UNSUPPORTED if the technology is not available
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_data_SetTechnologyRank
(
    uint32_t rank,
        ///< [IN] Rank of the used technology

    le_data_Technology_t technology
        ///< [IN] Technology
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the first technology to use
 * @return
 *      - One of the technologies from @ref le_data_Technology_t enumerator if the list is not empty
 *      - @ref LE_DATA_MAX if the list is empty
 */
//--------------------------------------------------------------------------------------------------
le_data_Technology_t le_data_GetFirstUsedTechnology
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the next technology to use
 * @return
 *      - One of the technologies from @ref le_data_Technology_t enumerator if the list is not empty
 *      - @ref LE_DATA_MAX if the list is empty or the end of the list is reached
 */
//--------------------------------------------------------------------------------------------------
le_data_Technology_t le_data_GetNextUsedTechnology
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Get the technology currently used for the default data connection
 *
 * @return
 *      - One of the technologies from @ref le_data_Technology_t enumerator
 *      - @ref LE_DATA_MAX if the current technology is not set
 *
 * @note The supported technologies are @ref LE_DATA_WIFI and @ref LE_DATA_CELLULAR
 */
//--------------------------------------------------------------------------------------------------
le_data_Technology_t le_data_GetTechnology
(
    void
);


#endif // LE_DATA_INTERFACE_H_INCLUDE_GUARD

