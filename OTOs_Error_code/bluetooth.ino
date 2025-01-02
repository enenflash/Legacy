SoftwareSerial bluetooth(0, 1);  // RX, TX pins


class Bluetooth {
public:

  bool valid;

  void Send_Data(int direction, int strength, int x, int y) {
    String full_data = String(direction) + "," + String(strength) + "," + String(x) + "," + String(y) + ";";
    bluetooth.println(full_data);
  }

  String Get_Data() {
    if (bluetooth.available()) {
      String data = bluetooth.readStringUntil(';');
      return data;
    }

    else {
      //Serial.println("THERE IS NO BLUETOOTH AVAILABLE");
      return "NONE";
    }
  }
};