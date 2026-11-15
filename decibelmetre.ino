// Décibelmètre numérique : microphone + préamplificateur -> Arduino Uno -> écran LCD 16x2 + 3 LED
//
//   LED verte  : niveau faible   (< 60 dB)
//   LED orange : niveau modéré   (60 à 85 dB)
//   LED rouge  : niveau élevé    (> 85 dB)

#include <LiquidCrystal.h>

// ---------- Brochage (à adapter à ton câblage) ----------
const int PIN_MICRO  = A0;   // sortie analogique du module micro + préamplificateur
const int LED_VERTE  = 8;
const int LED_ORANGE = 9;
const int LED_ROUGE  = 10;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);   // RS, E, D4, D5, D6, D7

// ---------- Seuils des trois zones ----------
const float SEUIL_MODERE = 60.0;   // dB
const float SEUIL_ELEVE  = 85.0;   // dB

// ---------- Calibration à un point (à régler avec un sonomètre de référence) ----------
// Pour un son stable, on relève l'amplitude crête-à-crête affichée sur le moniteur série
// et le niveau lu sur le sonomètre de référence, puis on les reporte ici.
const float DB_REF  = 72.0;    // niveau de référence en dB
const float P2P_REF = 200.0;   // amplitude crête-à-crête (0 à 1023) mesurée à ce niveau

const unsigned long FENETRE_MS = 50;   // durée d'une mesure
float dbLisse = 0.0;                   // niveau lissé pour un affichage stable

void setup() {
  pinMode(LED_VERTE, OUTPUT);
  pinMode(LED_ORANGE, OUTPUT);
  pinMode(LED_ROUGE, OUTPUT);
  lcd.begin(16, 2);
  Serial.begin(9600);
}

// Amplitude crête-à-crête du signal sur une fenêtre de mesure
int mesureCreteACrete() {
  unsigned long debut = millis();
  int mini = 1023;
  int maxi = 0;
  while (millis() - debut < FENETRE_MS) {
    int v = analogRead(PIN_MICRO);
    if (v < mini) mini = v;
    if (v > maxi) maxi = v;
  }
  return maxi - mini;
}

// Allume la LED de la zone et écrit le niveau sur l'écran
void afficher(float db) {
  const char* zone;
  digitalWrite(LED_VERTE, LOW);
  digitalWrite(LED_ORANGE, LOW);
  digitalWrite(LED_ROUGE, LOW);

  if (db < SEUIL_MODERE) {
    zone = "Faible          ";
    digitalWrite(LED_VERTE, HIGH);
  } else if (db <= SEUIL_ELEVE) {
    zone = "Modere          ";
    digitalWrite(LED_ORANGE, HIGH);
  } else {
    zone = "ELEVE !         ";
    digitalWrite(LED_ROUGE, HIGH);
  }

  lcd.setCursor(0, 0);
  lcd.print("Niveau: ");
  lcd.print((int)round(db));
  lcd.print(" dB   ");
  lcd.setCursor(0, 1);
  lcd.print(zone);
}

void loop() {
  int p2p = mesureCreteACrete();
  if (p2p < 1) p2p = 1;   // évite log(0)

  // Amplitude x2 = +6 dB : niveau = niveau de référence + 20 * log10(amplitude / amplitude de référence)
  float db = DB_REF + 20.0 * log10((float)p2p / P2P_REF);
  db = constrain(db, 30.0, 120.0);

  dbLisse = (dbLisse == 0.0) ? db : (0.7 * dbLisse + 0.3 * db);

  afficher(dbLisse);

  Serial.print("crete-a-crete = ");
  Serial.print(p2p);
  Serial.print("   niveau = ");
  Serial.print(dbLisse, 1);
  Serial.println(" dB");
}
