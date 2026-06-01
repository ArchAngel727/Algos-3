# Beschreibung des verwendeten Algorithmus.
Zur Suche des kürzesten Pfades wurde Dijkstras Algorithmus verwendet. Dijkstra findet in einem Graphen mit nicht-negativen Kantengewichten den kürzesten Weg von einem Startknoten zu allen anderen Knoten. Da die Kantengewichte hier Fahrzeiten zwischen Stationen sind und damit nie negativ, ist Dijkstra korrekt einsetzbar.

Grundidee: Der Algorithmus arbeitet greedy. Für jeden Knoten wird eine vorläufige Distanz vom Start gepflegt (initial `INT_MAX`, für den Start 0). In jedem Schritt wird der noch nicht abgeschlossene Knoten mit der kleinsten Distanz aus einer Prioritätswarteschlange extrahiert und über seine ausgehenden Kanten werden die Distanzen der Nachbarn ggf. verbessert (Relaxation). Sobald ein Knoten so extrahiert wurde, steht seine endgültige Distanz fest. Sobald das Ziel extrahiert wird, kann abgebrochen werden. Der Pfad wird anschließend über die Vorgänger-Map vom Ziel rückwärts zum Start rekonstruiert.

Implementierungsdetail: Da `std::priority_queue` keine Decrease-Key-Operation hat, wird beim Verbessern einer Distanz einfach ein neuer Eintrag in die PQ eingefügt. Veraltete Einträge werden beim Extrahieren durch den Vergleich `dist > distance[station]` erkannt und übersprungen (Lazy Deletion). Als Datenstrukturen kommen eine Adjazenzliste (`unordered_map<string, vector<Edge>>`) sowie zwei `unordered_map` für Distanzen und Vorgänger zum Einsatz.
In jeder Kante wird zusätzlich die Linie gespeichert, damit beim Ausgeben des Pfades Umstiege (Linienwechsel zwischen aufeinanderfolgenden Kanten) erkannt werden können.

# Aufwandsanalyse
Sei `V` die Anzahl der Stationen (Knoten) und `E` die Anzahl der Verbindungen (Kanten).

**Verwendete Datenstrukturen:**

- Graph als Adjazenzliste (`unordered_map<string, vector<Edge>>`): Zugriff auf die Nachbarn eines Knotens in O(deg(v)) amortisiert
- Prioritätswarteschlange als binärer Min-Heap (`std::priority_queue`): Push und Pop jeweils in O(log n)

**Gesamtaufwand: O((V + E) * log V)**

**Begründung:** Jeder Knoten wird höchstens einmal endgültig abgeschlossen, also gibt es maximal V relevante Extract-Min-Operationen zu O(log V). Über alle Knoten hinweg wird jede Kante höchstens einmal relaxiert, das ergibt bis zu E `push`-Operationen in die PQ zu jeweils O(log V) (genauer O(log E), aber log E <= log V^2 = 2 * log V, also gleiche Größenordnung).

Durch das Lazy-Deletion-Verfahren können in der PQ zwischenzeitlich bis zu E Einträge liegen, da jede Distanzverbesserung einen neuen Eintrag hinzufügt statt den alten zu aktualisieren. Veraltete Einträge werden beim Pop durch `if (dist > distance[station]) continue;` in O(1) übersprungen -- die asymptotische Schranke ändert sich dadurch nicht.

Zusammengezählt: O(V log V) für die Extract-Min-Operationen plus O(E log V) für die Relaxationen, also **O((V + E) * log V)**. Bei einem zusammenhängenden Graphen gilt E >= V-1, weshalb das oft kurz als **O(E log V)** geschrieben wird.

**Speicherbedarf:** O(V + E) für Adjazenzliste, Distanz- und Vorgänger-Map sowie PQ.

## Laufzeitmessung

Zur Überprüfung der theoretischen Schranke wurde die Laufzeit für verschiedene Start-Ziel-Paare mit unterschiedlich langen Routen gemessen. Die Messung erfolgte über ein Python-Script, das das kompilierte Programm (mit `-O2`) je 20 Mal pro Paar als Subprozess startet und die Gesamtlaufzeit per `time.perf_counter()` misst.

| Start | Ziel | min | median | avg |
|---|---|---|---|---|
| Praterstern | Stephansplatz | 1.24 ms | 1.45 ms | 1.45 ms |
| Praterstern | Volkstheater | 1.33 ms | 1.47 ms | 1.47 ms |
| Heiligenstadt | Reumannplatz | 1.38 ms | 1.62 ms | 1.59 ms |
| Hütteldorf | Aspernstrasse | 1.51 ms | 1.68 ms | 1.69 ms |

**Interpretation:** Die gemessenen Zeiten liegen alle in derselben Größenordnung (1,2 - 1,7 ms) und steigen leicht mit zunehmender Pfadlänge an. Das passt qualitativ zur Schranke O((V + E) * log V): längere Pfade bedeuten, dass Dijkstra mehr Knoten aus der Prioritätswarteschlange extrahiert, bevor der Early-Exit beim Zielknoten greift.

**Einschränkung:** Da nur die Gesamtlaufzeit des Programms gemessen wurde, enthalten die Werte auch Overhead durch Prozessstart, Einlesen der Eingabedatei, Graphaufbau und Ausgabe. Bei einem so kleinen Graphen (Wiener U-Bahn: ca. 100 Stationen) dominiert dieser konstante Overhead den eigentlichen Dijkstra-Aufruf, der vermutlich im Mikrosekunden-Bereich liegt. Eine saubere empirische Bestätigung der O((V + E) * log V)-Kurve würde deutlich größere Graphen erfordern. Die Beobachtung, dass die Laufzeit mit der Routenlänge wächst und nicht etwa abnimmt oder konstant bleibt, ist mit der theoretischen Schranke aber konsistent.
