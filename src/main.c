#include <glib.h>
#include <libnotify/notify.h>
#include <stdio.h>
#include <stdlib.h>

#define CAPACITY_FILE "/sys/class/power_supply/BAT1/capacity"
#define STATUS_FILE "/sys/class/power_supply/BAT1/status"

#define BATTERY_LEVEL_RESERVE 10
#define BATTERY_LEVEL_CRITICAL 15
#define BATTERY_LEVEL_LOW 100

enum ChargingStatus { Charging, Discharging };

void suspend_system() { system("systemctl suspend"); }

void trim_string(char *string) { string[strcspn(string, "\n\r")] = '\0'; }

int show_notification(char *title, char *message, NotifyUrgency urgency) {
	notify_init("Battery Monitor");
	NotifyNotification *n = notify_notification_new(title, message, 0);
	notify_notification_set_timeout(n, 5000);
	notify_notification_set_urgency(n, urgency);
	if (!notify_notification_show(n, 0)) {
		fprintf(stderr, "Error\n");
		return 1;
	}
	g_object_unref(G_OBJECT(n));
	notify_uninit();
	return 0;
}

int make_notification(int battery_level, int charging_status) {
	char title[32], message[128];
	int retval = -1;
	NotifyUrgency urgency;

	if (charging_status == Charging) {
		return -2;
	} else if (battery_level < BATTERY_LEVEL_RESERVE) {
		suspend_system();
	} else if (battery_level < BATTERY_LEVEL_CRITICAL) {
		strcpy(title, "Battery Critical");
		sprintf(message, "Battery charge less than %d%%",
				BATTERY_LEVEL_CRITICAL);
		urgency = NOTIFY_URGENCY_CRITICAL;
		retval = show_notification(title, message, urgency);
	} else if (battery_level < BATTERY_LEVEL_LOW) {
		strcpy(title, "Battery Low");
		sprintf(message, "Battery charge less than  %d%%", BATTERY_LEVEL_LOW);
		urgency = NOTIFY_URGENCY_NORMAL;
		retval = show_notification(title, message, urgency);
	}
	return retval;
}

int get_battery_level() {
	char buff[4];
	FILE *fp = fopen(CAPACITY_FILE, "r");
	fgets(buff, sizeof(buff), fp);
	fclose(fp);
	return atoi(buff);
}

int get_charging_status() {
	char buff[16];
	FILE *fp = fopen(STATUS_FILE, "r");
	fgets(buff, sizeof(buff), fp);
	trim_string(buff);
	fclose(fp);
	if (strcmp("Charging", buff) == 0)
		return Charging;
	else
		return Discharging;
}

int main(int argc, char *argv[]) {
	int battery_level = get_battery_level();
	int charging_status = get_charging_status();
	make_notification(battery_level, charging_status);
}
