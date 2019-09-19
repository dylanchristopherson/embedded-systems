/** Simple Sysfs LED control program
 *  this program can be used to easily control LED using a class
 */

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
using namespace std;
#define GPIO "/sys/class/gpio/"
#define FLASH_DELAY 50000 //50 milliseconds

class LED {
	private:
		string gpioPath;
		int gpioNumber;
		void writeSysfs(string path, string filename, string value);
	public:
		LED(int gpioNumber);
		virtual void turnOn();
		virtual void turnOff();
		virtual void displayState();
		virtual ~LED(); 	// the destructor -- called automatically
};

LED::LED(int gpioNumber) { // constructor implementation
	this->gpioNumber = gpioNumber;
	gpioPath = string(GPIO "gpio") + to_string(gpioNumber) + string("/");
	writeSysfs(string(GPIO), "export", to_string(gpioNumber));
	usleep(100000);
	writeSysfs(gpioPath, "direction", "out");
}

// This implementation function is "hidden" outside of the class
void LED::writeSysfs(string path, string filename, string value) {
	ofstream fs;
	fs.open((path+filename).c_str());
	fs << value;
	fs.close();
	fs << value;
	fs.close();
}

void LED::turnOn() {
	writeSysfs(gpioPath, "value", "1");
}

void LED::turnOff() {
	writeSysfs(gpioPath, "value", "0");
}

void LED::displayState() {
	ifstream fs;
	fs.open((gpioPath + "value").c_str());
	string line;
	cout << "The current LED state is";
	while(getline(fs,line)) cout << line << endl;
	fs.close();
}

LED::~LED() { // the destructor unexports the sysfs GPIO entries
	cout << "Destroying the LED with GPIO number " << gpioNumber << endl;
	writeSysfs(string(GPIO), "unexport", to_string(gpioNumber));
}

int main(int argc, char* argv[]) { // the main function start point
	cout << "Starting the makeLEDs program" << endl;

	LED led(17);    // create the LED object
	cout << "Flashing the LEDs for 5 seconds" << endl;
	for(int i = 0; i < 50; i++) {
		led.turnOff();
		usleep(FLASH_DELAY);
		led.turnOn();
		usleep(FLASH_DELAY);
	}
	led.displayState();    // display final GPIO17 state
	cout << "Finished the makeLEDs program" << endl;
	led.displayState();
	cout << "Finished the makeLEDs program" << endl;
	return 0;
}

