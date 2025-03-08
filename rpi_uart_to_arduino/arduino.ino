#define BUFFER_SIZE 64  // Adjust as needed

void setup() {
    Serial.begin(115200);  // USB Serial Monitor
    Serial1.begin(115200); // UART
}

void loop() {
    uint8_t receivedNumber = 0;
    
    if (Serial1.available() >= sizeof(receivedNumber)) {  // Ensure 4 bytes are available
        Serial1.readBytes((char*)&receivedNumber, sizeof(receivedNumber));
        Serial.print("Received binary number: ");
        Serial.println(receivedNumber);
    }
}

