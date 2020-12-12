#define CONFIG_WIFI_SSID "Your home wifi SSID"
#define CONFIG_WIFI_PASSWORD "Your_WIFI_Password_Here"

//time of sunrise on weekdays (days that aren't sat or sun. stat holiday status ignored)
#define CONFIG_SUNRISE_WEEKDAY_START_HOUR "5" //hour to start. 24 hr clock, don't use leading 0's. 
#define CONFIG_SUNRISE_WEEKDAY_START_MINUTE "50" //minute to start. This starts the sunrise at 5:50am. 

//time of sunrise on weekends (days that are sat or sun. stat holiday status ignored)
#define CONFIG_SUNRISE_WEEKEND_START_HOUR "6" //hour to start. 24 hr clock, don't use leading 0's. 
#define CONFIG_SUNRISE_WEEKEND_START_MINUTE "30" //minute to start. This starts the sunrise at 6:40am. 

//time of sunset (same every day)
#define CONFIG_SUNSET_START_HOUR "19" //hour to start. 24 hr clock, don't use leading 0's. 
#define CONFIG_SUNSET_START_MINUTE "45" //starts the sunrise at 5:01pm. 
