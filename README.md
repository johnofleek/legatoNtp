## Legato ntpClient app

Waits for IP to appear on the cellular interface and when current date/time is < TIME_IN_THE_PAST then requests time via system ntpd

There are some issues the with detection of the cellular IP interface the current implementation is a hack as I can't see a way to using le_data for this

A timer is used to drive the detection as there are no events i can find to hook into. The timer update rate is fast on app start then slow once the clock is set

