# 🏛️ Auction House — Joc de Licitații în C++

> Întreci-te cu bot-ul **"Gigel"** într-o casă de licitații virtuală. Cumpără mai inteligent, cheltuie mai strategic!

---

## 📖 Conceptul Principal

Jocul te pune în pielea unui cumpărător cu un **buget inițial de 50.000 $**. În fiecare rundă, casa de licitații îți prezintă **4 obiecte** extrase aleatoriu dintr-o bază de date (un fișier text). Dacă îți dorești un obiect, va trebui să licitezi pentru el împotriva unui oponent controlat de calculator — bot-ul **"Gigel"**. Cel care oferă cel mai mult câștigă produsul.

---

## 🏗️ Arhitectura Claselor

Proiectul este structurat în **4 clase principale**:

### 📦 `item` — Obiectul licitat
- Păstrează detaliile unui produs: `name`, `price` (prețul de bază) și `stock` (stocul disponibil).
- Supraîncarcă operatorul `<<` pentru afișare rapidă:
  ```
  Item: Telefon -> Price: 1000 $ (Stoc: 5)
  ```

### 🧑 `player` — Jucătorul
- Gestionează profilul tău: `name`, `balance` și `inv[99]` (inventar sub formă de array).
- Metoda cheie **`bought()`** scade banii din balanță și adaugă obiectul în inventar.

### 🤖 `bot` — Adversarul ("Gigel")
- La fiecare licitație, calculează un **preț maxim** (`max_price`) folosind un multiplicator aleatoriu între `0.5×` și `2.1×` față de prețul de bază (metoda `call_mpr`).
- În `new_bidding`: dacă ai licitat sub pragul lui, plusează o sumă aleatorie; dacă ai depășit pragul — **se retrage** și tu câștigi.

### 📋 `lista` — Casa de licitații
- Gestionează toate produsele disponibile.
- **`citire_lista()`** — citește produsele din `obiecte.txt` folosind `@` ca delimitator:
  ```
  Ceas de aur@2000@3   →   Nume @ Pret @ Stoc
  ```
- **`item_select()`** — alege aleatoriu 4 obiecte unice pentru runda curentă.

---

## 🎮 Fluxul Jocului

```
1. Inițializare       →   Se încarcă produsele din fișierul text
2. Tutorial           →   Demonstrație automată cu pauze de 2s (Sleep() din <windows.h>)
3. Înregistrare       →   Îți ceri numele → primești 50.000 $ → se creează bot-ul "Gigel"
4. Runde repetate     →   Licitezi, câștigi sau pierzi obiecte
5. Continuare?        →   La final de rundă poți ieși din joc
```

### 🔄 O rundă pas cu pas

| Etapă | Descriere |
|-------|-----------|
| 💰 Balanță | Se afișează banii rămaşi |
| 🛒 Oferte | Apar cele 4 produse ale rundei |
| ✋ Selecție | Alegi pentru câte obiecte vrei să licitezi |
| ⚔️ Licitație | Mini-joc ping-pong cu "Gigel": tu oferi, el plusează (sau se lasă bătut) |
| ✅ Validare | Nu poți licita mai mult decât ai în balanță |
| 🧹 Reset | Ecranul se curăță cu `system("cls")` și începe o nouă rundă |

---

## 📁 Format fișier `obiecte.txt`

```
Telefon@1500@10
Ceas de aur@2000@3
Laptop@3500@5
```

---

## ⚙️ Compilare & Rulare

> ⚠️ Proiectul folosește `<windows.h>` — rulează **doar pe Windows**.

```bash
g++ -o auction main.cpp -o auction.exe
./auction.exe
```

---

## SFML UI (Windows)

### Build with CMake

1. Install SFML 2.5+ and set `SFML_DIR` to the folder that contains `SFMLConfig.cmake`.
2. Generate and build:

```bash
cmake -S . -B build
cmake --build build --config Release
```

### Run

- Copy `obiecte.txt` next to the executable or run the executable from this folder.
- The UI loads a system font from `C:/Windows/Fonts/consola.ttf` (fallback: `arial.ttf`).
