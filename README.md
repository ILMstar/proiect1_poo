1. Conceptul Principal
Jocul te pune în pielea unui cumpărător cu un buget inițial generos (50.000$). În fiecare rundă, casa de licitații îți prezintă 4 obiecte extrase la întâmplare dintr-o bază de date (un fișier text). Dacă îți dorești un obiect, va trebui să licitezi pentru el împotriva unui oponent controlat de calculator (bot-ul "Gigel"). Cel care oferă cel mai mult câștigă produsul.

2. Arhitectura Claselor (Componentele codului)
Proiectul este împărțit în 4 clase principale:

Clasa item (Obiectul licitat):

Păstrează detaliile unui produs: name (numele), price (prețul de bază) și stock (stocul disponibil).

Are metode pentru a returna aceste detalii și supraîncarcă operatorul << pentru a putea afișa ușor produsul pe ecran (ex: Item: Telefon -> Price: 1000 $ (Stoc: 5)).

Clasa player (Jucătorul):

Gestionează profilul tău: name (numele tău), balance (banii de care dispui) și inv[99] (un inventar rudimentar sub forma unui array, unde reține câte obiecte de un anumit tip ai cumpărat).

Metoda cheie este bought(), care îți scade banii din balanță și adaugă obiectul în inventar.

Clasa bot (Adversarul / "Gigel"):

Când începe licitația pentru un obiect, bot-ul își calculează un "preț maxim" pe care este dispus să îl dea (max_price), folosind o înmulțire aleatorie între 0.5 și 2.1 față de prețul de bază (metoda call_mpr).

În metoda new_bidding, bot-ul decide: dacă prețul pe care l-ai oferit tu este mai mic decât pragul lui maxim, va plusa o sumă aleatorie peste tine. Dacă ai licitat mai mult decât e el dispus să dea, se retrage și tu câștigi obiectul.

Clasa lista (Casa de licitații):

Se ocupă de managementul produselor.

citire_lista(): Citește produsele dintr-un fișier numit obiecte.txt. Face o parsare manuală foarte interesantă folosind caracterul @ ca delimitator. Asta înseamnă că fișierul text trebuie să arate cam așa: Ceas de aur@2000@3 (Nume@Pret@Stoc).

item_select(): Alege aleatoriu 4 obiecte din lista completă pentru a le afișa în runda curentă, asigurându-se că nu sunt duplicate (folosind array-ul display).

3. Fluxul Jocului (Cum decurge execuția în main)
Inițializarea: Se încarcă produsele din fișierul text.

Tutorialul (runTutorial): Jocul rulează o demonstrație automată folosind obiecte "dummy" (tut_player și tut_bot) și folosește funcția Sleep() din <windows.h> pentru a face pauze de 2 secunde, dând senzația unei licitații reale. Îți explică pas cu pas ce urmează să faci.

Înregistrarea: Te întreabă dacă vrei să joci, îți cere un nume și îți creează profilul cu 50.000$. Tot acum se naște și bot-ul "Gigel".

Bucla principală (Rundele):

Afișează balanța ta.

Afișează cele 4 oferte ale rundei.

Te întreabă dacă și pentru câte vrei să licitezi.

Pentru fiecare obiect ales, începe un mini-joc de ping-pong între tine și "Gigel". Tu introduci o sumă, Gigel plusează (dacă vrea), tu decizi dacă mai plusezi sau te lași bătut. Există și o validare utilă: nu poți licita mai mulți bani decât ai în balanță.

La finalul rundei, ecranul este curățat cu system("cls") și ești întrebat dacă vrei să continui la runda următoare.