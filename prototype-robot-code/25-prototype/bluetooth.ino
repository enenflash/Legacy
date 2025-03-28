/* ##### BLUETOOTH ##### */
SoftwareSerial bluetooth(0, 1);

// Bluetooth Class
class Bluetooth {
public:

  // Sending Data
  void sendData(int direction, int strength, int x, int y, bool inPosition) {
    String full_data = String(direction) + "," + String(strength) + "," + String(x) + "," + String(y) + "," + String(inPosition) + ";";
    bluetooth.println(full_data);
  };

  // Recieving Data
  String getData() {
    if (bluetooth.available()) {
      String data = bluetooth.readStringUntil(';');
      return data;
    }

    return "NONE";
  };
};