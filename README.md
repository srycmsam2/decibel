# Décibelmètre numérique

Mini-projet d'électroacoustique (USTHB, 2023-2024), réalisé en équipe : un microphone, un préamplificateur et une carte Arduino Uno mesurent le niveau sonore, l'affichent en décibels et le traduisent en couleur.

## Fonctionnement

1. Le microphone transforme le son en signal électrique, très faible.
2. Le préamplificateur renforce ce signal.
3. L'Arduino Uno mesure l'amplitude du signal sur une fenêtre de 50 ms et la convertit en niveau sonore (dB).
4. L'écran LCD 16×2 affiche le niveau et sa catégorie.
5. Trois LED indiquent la zone :

| LED    | Niveau        | Zone    |
|--------|---------------|---------|
| Verte  | < 60 dB       | Faible  |
| Orange | 60 à 85 dB    | Modérée |
| Rouge  | > 85 dB       | Élevée  |

Le seuil de 85 dB correspond à la limite d'exposition considérée comme sûre pour 8 heures par jour.

## Matériel

- Arduino Uno (ATmega328)
- Module capteur de son réalisé sur plaque perforée : microphone et préamplificateur, d'après le module KY-038 (sortie analogique amplifiée)
- Écran LCD 16×2
- 3 LED (verte, orange, rouge) avec leurs résistances

## Brochage utilisé dans le code

| Élément                | Broche Arduino |
|------------------------|----------------|
| Sortie du module micro | A0             |
| LED verte              | 8              |
| LED orange             | 9              |
| LED rouge              | 10             |
| LCD (RS, E, D4 à D7)   | 12, 11, 5, 4, 3, 2 |

Si ton câblage est différent, modifie les constantes au début de `decibelmetre.ino`.

## Calibration

Le niveau en dB est une estimation obtenue par **calibration à un point** : ce n'est pas un sonomètre certifié, et il n'y a pas de pondération en fréquence (A, B ou C).

1. Ouvre le moniteur série (9600 bauds).
2. Place le micro devant un son stable, avec un sonomètre de référence à côté.
3. Reporte dans le code le niveau lu (`DB_REF`) et l'amplitude crête-à-crête affichée (`P2P_REF`).

## Fichiers

- `decibelmetre/decibelmetre.ino` : programme Arduino (bibliothèque `LiquidCrystal`, incluse dans l'IDE).
