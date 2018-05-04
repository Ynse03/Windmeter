const int soundPin = A3;
const unsigned long sampleWindow = 100;

const double dBAnalogQuiet = 10;
// const double dBAnalogModerate = 12;
// const double dBAnalogLoud = 17;

void setup() {
  Serial.begin(9600);
  pinMode(soundPin, INPUT);
}

void loop() {
  unsigned long startMillis = millis();
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  unsigned int samples = 0;
  unsigned int discards = 0;

  while (millis() < startMillis + sampleWindow) {
    unsigned int sample = analogRead(soundPin);
    samples++;
    if (sample < 1024) {
      signalMax = max(sample, signalMax);
      signalMin = min(sample, signalMin);
    } else {
      discards++;
    }
  }

  int peakToPeak = signalMax - signalMin;
  int decibels = map(peakToPeak, 0, 100, 40, 90);

  Serial.print("sampled: ");
  Serial.print(samples);
  Serial.print(", discarded: ");
  Serial.print(discards);
  Serial.print(", min: ");
  Serial.print(signalMin);
  Serial.print(", max: ");
  Serial.print(signalMax);
  Serial.print(", peak2peak: ");
  Serial.print(peakToPeak);
  Serial.print(", dB: ");
  Serial.println(calculateDecibels(signalMax));
}

float calculateDecibels(int x) {
  return 20 * log10(x/dBAnalogQuiet);
}
