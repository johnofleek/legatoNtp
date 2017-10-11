/**
 ** fires off ntp client if the cellular network is up and when current time is < TIME_IN_THE_PAST
 ** 

 **/
#include "legato.h"
#include "interfaces.h"


// Temperature
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>



#define MAIN_TIMER_NAME                 "NTP_APP_TIMER"
#define MAIN_TEMPERATURE_UPDATE_RATE (10)        // seconds

#define TIME_IN_THE_PAST            946684800    // 1 January 2000 00:00:00


// for testing set the date in the TIME_IN_THE_PAST
// date 100616441999


void main_setSystemTimeFromNtp(void)
{
    system("/usr/sbin/ntpd -p time.nist.gov");
}



// IP data connection
// static le_mdc_ProfileRef_t profileRef;

static bool DataConnected = false;
//--------------------------------------------------------------------------------------------------
/**
 * Callback for the connection state
 */
//--------------------------------------------------------------------------------------------------
static void ConnectionStateHandler
(
    const char* intfName,
    bool isConnected,
    void* contextPtr
)
{
    if (isConnected)
    {
        DataConnected = isConnected;
        LE_INFO("ntp detected data connect '%s'", intfName);
    }
    else
    {
        LE_INFO("ntp detected data disconnect '%s'", intfName);
        
        DataConnected = false;
    }
}


static le_timer_Ref_t updateTimerRef;


static void updateTimer_cbh(le_timer_Ref_t timerRef)
{
    le_clk_Time_t currentTime= le_clk_GetAbsoluteTime();
    le_mdc_ProfileRef_t profileRef;
    le_mdc_ConState_t mdcConnectionState;
    
    if(currentTime.sec > TIME_IN_THE_PAST)
    {
        LE_INFO("Time OK");
    }

    else if(DataConnected)
    {
        LE_INFO("Trying to set the time");
        main_setSystemTimeFromNtp();
    }
    else
    {
        LE_INFO("No Network - can't run ntp client");
        // There is no Legato way I can find to query le_data for the state of the connection
        // The following bodge looks at the cellular interface to see if it's connected
        
        // warning this doesn't handle profiles - just uses the default set with cm
        profileRef = le_mdc_GetProfile(LE_MDC_DEFAULT_PROFILE);
        if(profileRef != NULL)
        {
            le_mdc_GetSessionState(profileRef, &mdcConnectionState);
            if(mdcConnectionState==LE_MDC_CONNECTED )
            {
                DataConnected = true;
            }
            else
            {
                DataConnected = false;
            }
        }
    }
            
}

static void main_timer_init(void)
{
    updateTimerRef = le_timer_Create (MAIN_TIMER_NAME);
    
    le_timer_SetHandler ( updateTimerRef, updateTimer_cbh);
    le_timer_SetRepeat( updateTimerRef, 0 );
    le_timer_SetMsInterval( updateTimerRef, MAIN_TEMPERATURE_UPDATE_RATE * 1000);        
    le_timer_Start( updateTimerRef); 
}


COMPONENT_INIT
{   
    LE_INFO("started\n");
        
    // manually start the service
    le_data_ConnectService();
    LE_INFO("data service connected\n");
    
    le_data_AddConnectionStateHandler(&ConnectionStateHandler, NULL);
    
    main_timer_init();
}
