// ─── Pines ────────────────────────────────────────────────────────────────────
const int PIN_MIC         = A0;
const int NUM_LEDS        = 5;
const int PINES_LED[NUM_LEDS] = { 9, 10, 11, 12, 13 };

// ─── Umbrales de sonido (valor analógico 0–1023) ──────────────────────────────
// Cada umbral representa el mínimo para encender N LEDs.
const int UMBRALES[NUM_LEDS] = { 223, 423, 623, 823, 980 };

// ─── Setup ───────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(PINES_LED[i], OUTPUT);
    digitalWrite(PINES_LED[i], LOW);
  }
}

// ─── Loop ────────────────────────────────────────────────────────────────────
void loop() {
  int nivelSonido = analogRead(PIN_MIC);
  int ledsActivos = contarLedsActivos(nivelSonido);
  actualizarLeds(ledsActivos);
  Serial.println(nivelSonido);
}

// ─── Funciones ───────────────────────────────────────────────────────────────

// Devuelve cuántos LEDs deben encenderse según el nivel de sonido.
int contarLedsActivos(int nivelSonido) {
  int cantidad = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    if (nivelSonido >= UMBRALES[i]) cantidad++;
  }
  return cantidad;
}

// Enciende los primeros N LEDs y apaga el resto.
void actualizarLeds(int cantidad) {
  for (int i = 0; i < NUM_LEDS; i++) {
    digitalWrite(PINES_LED[i], i < cantidad ? HIGH : LOW);
  }
}
