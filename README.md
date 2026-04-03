# ky038-sound-level-indicator
KY-038 microphone sensor + Arduino — real-time sound level indicator with 5-LED bar display. Wokwi simulation included.
# 🎙 Sound Level Indicator — KY-038 + Arduino

> Proyecto ganador del hackathon introductorio de Arduino organizado por el capítulo RAS de la Universidad Tecnológica de Panamá.  
> **Restricción:** sensor KY-038, mínimo 3 LEDs, 1 hora de desarrollo.

---

## ¿Qué hace?

Lee el nivel de sonido ambiente con el sensor KY-038 y lo visualiza en tiempo real encendiendo entre 1 y 5 LEDs según la intensidad — como un VU meter analógico.

---

## Demo

https://github.com/user-attachments/assets/c28fbbd1-8e48-4480-9d3f-ef8e2fd97900

---

## Componentes

| Componente         | Cantidad |
|--------------------|----------|
| Arduino Uno/Nano   | 1        |
| Sensor KY-038      | 1        |
| LED (cualquier color) | 5     |
| Resistencia 220Ω   | 10       |
| Protoboard + jumpers | —      |

---

## Diagrama de conexiones

| KY-038 | Arduino |
|--------|---------|
| VCC    | 5V      |
| GND    | GND     |
| A0     | A0      |
| D0     | (no usado) |

| LED   | Pin Arduino |
|-------|-------------|
| LED 1 | D9          |
| LED 2 | D10         |
| LED 3 | D11         |
| LED 4 | D12         |
| LED 5 | D13         |

> 💡 Conecta dos resistencia de 220Ω en serie con cada LED (entre el pin y el ánodo).

---

## Simulación en Cirkit Design

<img width="3000" height="2345" alt="circuit_image" src="https://github.com/user-attachments/assets/3de87ff2-98a8-493c-a6cc-c11fd6daad08" />

---

## Código original (hackathon)

Versión funcional entregada durante la competencia.

```cpp
#define red   9
#define blue  10
#define green 11
#define yell  12
#define whi   13

const int mic = A0;
int lectura;

void setup() {
  Serial.begin(9600);
  pinMode(green, OUTPUT);
  pinMode(yell,  OUTPUT);
  pinMode(red,   OUTPUT);
  pinMode(blue,  OUTPUT);
  pinMode(whi,   OUTPUT);

  digitalWrite(green, LOW);
  digitalWrite(yell,  LOW);
  digitalWrite(red,   LOW);
  digitalWrite(blue,  LOW);
  digitalWrite(whi,   LOW);
}

void loop() {
  int lectura = analogRead(mic);

  if (lectura >= 980) {
    digitalWrite(green, HIGH);
    digitalWrite(yell,  HIGH);
    digitalWrite(red,   HIGH);
    digitalWrite(blue,  HIGH);
    digitalWrite(whi,   HIGH);
  } else if (lectura >= 823) {
    digitalWrite(green, HIGH);
    digitalWrite(yell,  HIGH);
    digitalWrite(whi,   HIGH);
    digitalWrite(blue,  HIGH);
    digitalWrite(red,   LOW);
  } else if (lectura >= 623) {
    digitalWrite(green, HIGH);
    digitalWrite(yell,  HIGH);
    digitalWrite(whi,   HIGH);
    digitalWrite(blue,  LOW);
    digitalWrite(red,   LOW);
  } else if (lectura >= 423) {
    digitalWrite(green, LOW);
    digitalWrite(yell,  HIGH);
    digitalWrite(red,   LOW);
    digitalWrite(blue,  LOW);
    digitalWrite(whi,   HIGH);
  } else if (lectura >= 223) {
    digitalWrite(green, LOW);
    digitalWrite(yell,  LOW);
    digitalWrite(red,   LOW);
    digitalWrite(blue,  LOW);
    digitalWrite(whi,   HIGH);
  } else {
    digitalWrite(green, LOW);
    digitalWrite(yell,  LOW);
    digitalWrite(red,   LOW);
    digitalWrite(blue,  LOW);
    digitalWrite(whi,   LOW);
  }

  Serial.println(lectura);
}
```

---

## Código mejorado

Refactorización con nombres descriptivos, arreglo de pines, bucle de control y eliminación de la variable global redundante.

```cpp
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
```

---

## Diferencias entre versiones

| Aspecto | Original | Mejorado |
|---|---|---|
| Nombres de variables | `whi`, `yell`, `red` | `PIN_MIC`, `PINES_LED`, `nivelSonido` |
| Variable global redundante | `int lectura` declarada dos veces | Una sola variable local en `loop()` |
| Control de LEDs | 6 bloques `if-else` repetitivos | Bucle + dos funciones con responsabilidad única |
| Escalabilidad | Agregar un LED = reescribir todo | Agregar un LED = un número en el arreglo |
| Legibilidad | Funcional pero denso | Autodocumentado |

---

## Lo que aprendí

- Usar la salida **analógica (A0)** del KY-038 en lugar de la digital (D0) permite múltiples niveles de respuesta, no solo encendido/apagado.
- `Serial.println()` fue clave para calibrar los umbrales en tiempo real durante la competencia.
- Un proyecto simple bien ejecutado y bien explicado supera a uno complejo a medias.

---

## Autor

**Dominik** — Estudiante de Ingeniería Eléctrica y Electrónica, Universidad Tecnológica de Panamá  
Hackathon Arduino — RAS UTP, abril 2026

---

## Licencia

MIT License — libre para usar, modificar y distribuir.  
Ver [LICENSE](LICENSE) para más detalles.
