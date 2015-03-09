#include <WiFi.h>
#include <WiFiUdp.h>
#include <Network.h>

int status = WL_IDLE_STATUS;

unsigned int localport = 10000;

WiFiUDP Udp;


void setup() {

	Serial.begin(9600);

	while (status != WL_CONNECTED) {
		Serial.println("Coming online..");
		status = WiFi.begin(NET_SSID,NET_AUTH);

		delay(10000);
	}

	if(status == WL_CONNECTED) {
		Serial.println("Connected.");
	}

	Udp.begin(localport);
}

enum Direction {
	FORWARD, LEFT, RIGHT, REVERSE, NONE
};


Direction currentDir = NONE;

void reply(const char * reply) {
	Udp.beginPacket(Udp.remoteIP(),Udp.remotePort());
	Udp.write(reply);
	Udp.endPacket();
}
char packetBuffer[255];

void loop() {

	int packetSize = Udp.parsePacket();

	if (packetSize) {

		int len = Udp.read(packetBuffer,255);
		if (len > 0)
			packetBuffer[len] = 0;

		String msg = packetBuffer;

		if(msg.equals("forward")) {
			reply("Moving forward.");
		} else if (msg.equals("reverse")) {
			reply("Backing up.");
		} else if (msg.equals("left")) {
			reply("Turning left.");
		} else if (msg.equals("right")) {
			reply("Turning right.");
		} else if (msg.equals("stop")) {
			reply("Stopped.");
		} else if(msg.equals("test")){
			reply("Test confirmed.");
		}
		delay(15);
	}
}
