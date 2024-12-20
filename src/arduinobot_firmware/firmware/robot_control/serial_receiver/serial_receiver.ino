 #define LED_PIN 13

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);  // Endret fra LOW til OUTPUT
  digitalWrite(LED_PIN, LOW);  // Sett initial tilstand

  Serial.begin(115200);
  Serial.setTimeout(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()) {
    String input = Serial.readStringUntil('\n');  // Les til linjeskift
    int x = input.toInt();
    
    if(x == 0) {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED OFF");  // Send tilbakemelding
    }
    else {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED ON");   // Send tilbakemelding
    }
  }
}
