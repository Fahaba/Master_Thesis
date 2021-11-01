# Kotlin in Action Notes

## Kapitel 1

## Kapitel 2 - Kotlin Basics

### Functions And Variables
*   Typdeklarationen können oft weggelassen werden
*   Fördert die Verwendung von Immutable statt Mutable Variablen
*   Funktionen werden mit `fun` deklariert
*   Parametertypen werden hinter den Namen hinzugefügt
*   Funktionen müssen sich nicht innerhalb von Klassen befinden, können sich außerhalb von Klassen befinden
*   Arrays sind Klassen und haben keine spezielle Syntax wie in Java
*   Semicolon am Ende einer Zeile sind optional und können weggelassen werden

Funktionen
*   Deklariert mit `fun` Schlüsselwort
*   Rückgabetyp wird hinter der Parameterliste und nach einem `:`
*   `if` ist ein Statement in Kotlin, und nicht eine Expression wie in Java
    *   In Kotlin sind die Meisten Kontrollstrukturen Statements
    *   Ausnahmen sind Schleifen (`do`, `for`, `do/while`), diese sind wie in Java Expressions
*   Fall eine Funktion aus einer Expression besteht
    *   Kann man den `return` Statement und `{` `}` weglassen 
    *   (Man muss nach Typdefinition der Funktion `=`)
    *   Hierbei Spricht man von einem *expression body*
    *   Andernfalls spricht man vom *block body*
    *   Hier kann man den Rückgabe Typ der Funktion (evtl.) weglassen

Variablen
*   Deklaration
    *   Schlüsselwort `val` für immutable
        *   Kann nach Initialisierung nicht neu zugeordnet werden (= final in Java)
        *   Zustand des Objekt auf dem die Variable zeigt kann sich natürlich ändern
    *   Schlüsselwort `var` für mutable
        *   Wert kann neu zugeordnet werden (= nicht final in Java)
        *   Trotz möglicher neu Zuordnung ist der Variablentyp Fest 
*   Typangabe
    *   Angabe nach Nach Variablenname
    *   Kann wegfallen wenn Variable einen Initializer hat
        *   Compiler kann den Typen aus der Initialisierungswert ableiten
    *   Muss vorhanden sein wenn Variable keinen Initializer hat
*   Mutable/Immutable

String Templates
*   Einfache Erstellung von Strings mit `$` und `${}` innerhalb der String
    *   Variablen nach`$` werden ausgewertet 
    *   Expression in `${}` werden ausgewertet

### Classes and Properties (p. 23)
*   Classen die nur Daten (keine Funktionen usw.)
    *   werden *value object* genannt
    *   einfache Syntax

Eigenschaften/Properties
*   In Java werden Felder und ihre  Accessoren  Properties genannt
*   Properties sind Teil von Kotlin
*   wenn eine Variable/Property deklariert wird, werden die entsprechenden Accessor automatisch und transparent mit erstellt
    *   Bei `val` wir nur ein Getter erstellt
    *   Bei `var` werden Getter und Setter erstellt
    *   Wenn Eigenschaft mit `is` beginnt 
        *   wird kein zusätzlicher Präfix für die Getter Methode hinzugefügt
        *   beim Setter wird `is` durch `set` ersetzt wenn es sich um ein `val` Eigenschaft handelt
*   man kann die Standard Accessoren durch eigene überschreiben

Benutzerdefinierte Accessoren
*   man kann eigene Accessoren für Eigenschaften definieren
*   Nicht alle Eigenschaften brauchen ein Feld

Code Source Layout
*   Man Klassen  **und Funktionen** via `import` importieren
*   mit `import package.*` werden nicht nur sämtlichen Klassen im Paket importiert sondern auch alle Top-Level Funktionen in diesem Paket
*   Man Kann mehrere Klassen in der Gleichen Datei haben
*   Name der `.kt` Dateien darf beliebig gewählt werden, es muss nicht der Name einer Klasse sein
*   Java Paket-Dateistruktur muss nicht eingehalten werden, man kann z.B. ein ganzes Paket in einer Datei haben.

### Representing and handling choices `enum` and `when`
*   `when` kann als ersatz für Java's `switch` angesehen werden

enum Klassen deklarieren
*   deklariert mit `enum class`
*   `enum` ist ein *soft keyword*
    *   Hat spezielle Bedeutung wenn es vor `class` steht
    *   Sonst kann es beliebig genutzt werden (z.B. für Variablennamen)
*   Gleiche Syntax wie für Klassen
    *   `enum class` + Name + ( + Eigenschaften + ) + Konstanten
*   Einzige stelle in Kotlin wo `;` verwendet wird
    *   Zur Trennung der Konstanten Liste und der enum Funktionen

`when` mit enums verwenden
*   gleich wie `swtich` in Java
*   man braucht nach jedem Zweig kein `break` um abzubrechen
*   man kann mehrere Werte einer Branch zuordnen
    *   hier verwendet man `,` um die verschiedenen werte zu trennen

`when` mit beliebigen Objekte verwenden
*   `when` is besser als Java `switch`
    *   In Java muss man im `switch` Konstanten verwenden
    *   `when` erlaubt auch Objekte
*   `setOf()` erstellt eine Menge aus Elementen
    *   Bei vergleichen von Mengen ist die Reihenfolge ignoriert `setOf(x,y) == setOf(y,x)`

`when` ohne Argumente
*   When keine Argumente angegeben werden, kann man für das Branching beliebige Boolean Expression angeben

Smart Casts
*   Wenn man eine Variable mit `is` auf Einen Typ prüft muss man diese anschließend nicht auf den geprüften Typ casten, der Compiler macht dies automatisch
*   Man kann die Variable so verwenden als wäre sie vom Geprüften Typ
*   Dieses Verhalten wird als SmartCast bezeichnet
*   Wenn man Smart Casts mit Klasseneigenschaften zu verwenden:
    *   Müssen diese `val` sein
    *   Und dürfen keine Modifizierten Accessoren haben
*   Explizites Casting erfolgt mit `as` Schlüsselwort

*   `when` statt `if`
*   Kotlin besitzt keine Ternären Operatoren wie in Java
    *   Man kann `if` und `when` ähnlich wie ternäre Operatoren verwenden
    *   Beide prüfen nicht nur auf Gültigkeit sondern können auch einen return Wert zurückgeben
*   Bei rückgabewerte gilt die letzte Expression in einer Branch liefert den Rückgabewert

### Iteration: `while` und `for` Schleifen (p.35)
*   `while` Schleife ist Identisch zu Java
*   `for` Schleife gibt es in nur einer Version (Äquivalenz zu Java `for-each`)

Ranges und Progressions
*   Keine Standard `for`-Schleifen wie in Java möglich
*   Man verwendet Ranges um das gleiche Verhalten zu erhalten
*   Ranges werden mit den `..` Operator gekennzeichnet (z.B. `val oneToTen = 1..10`)
*   Linke und Rechte Grenze von Ranges sind *immer inklusiv*
*   Wenn man über alle Werte in einem Range iterieren kann, wird dieser Range auch als *Progression* bezeichnet
*   In Kombination mit `for` Schleifen
    *   Komplexere Ranges mit Schritten zwischen jedem Wert
    *   Man verwendet Schlüsselwörter `upTo, downTo, step`
        *   `upTo` und `downTo` Werte sind *exklusiv*
    *   So kann man gleiches verhalten wie Normale

Über Maps iterieren
*   `for` `in` Schleifen werden meistens verwendet um Collections zu traversieren
*   die `..` Syntax funktioniert nicht nur Zahlen, es funktioniert auch für `Chars`
*   Man kann Elemente von Collections in der for schleife entpacken (z.B. Schlüssel und Wert)

`in` zum überprüfen ob ein Element in einer Collection liegt
*   man kann `in` verwenden um zu überprüfen ob ein Element in einer Collection liegt
*   man kann `!in` verwenden um zu überprüfen ob ein Element nicht in einer Collection liegt
*   Ranges sind nicht nur auf `Char` und `Int` beschränkt
    *   Man kann Ranges für jede Klasse die `java.lang.Comparable` implementiert  erstellen

### Exceptions in Kotlin (p.39)
*   ähnlich wie in Java
*   jedoch `throw` ist hier eine Expression
    *   kann als Teil von anderen Expressions dienen
*   `try`, `catch` und `finally` können verwendet werden wie in Java
*   Kotlin unterscheidet nicht zwischen *checked* und *unchecked* Exceptions

`try` als Expression
*   