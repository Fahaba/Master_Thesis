## Kapitel 3: Defining and calling functions

Zusätzliche Fähigkeiten der Java Bibliotheken werden wurden mit *Extension Funktionen* ermöglicht

### Collections in Kotlin

* Collections werden Hergestellt mit:
  * HashSet: `hashSetOf(1, 7, 53)`
  * ArrayList: `arrayListOf(1, 7, 53)`
  * Map: `hashMapOf(1 to "one", 7 to "seven", 53 to "fifty-three")`
* Kotlin verwendet die Standard Java Collections
* Kotlin gibt diesen Collections jedoch zusätzliche Fähigkeiten:
  * Letztes element ausgeben `.last()`
  * Maximum finden `.max()`

### Making functions easier to call

* Generische Typen sind ähnlich zu Java

Named Arguments

* Bei Funktionsaufrufen kann man den Namen eines Argumentes den man übergeben will mit angeben
* Wenn man dies tut sollte man es für alle nachfolgende Argumente auch machen

Default parameter values

* Man kann Funktionen-Parameter Standardwerte zuweisen
* Diese werden beim Methodenaufruf verwendet falls keine neuen Werte übergeben werden.
* Default Parametes und Named Arguments kombiniert reduziert den Bedarf Methoden zu überladen
  * Man kann sich jetzt bei Methodenaufrufen entscheiden bestimmte Argumente wegfallen zu lassen.
  * Durch die Verwendung von Named Arguments kann man auch manche Parameter in der Mitte wegfallen lassen.
* Kann man man in Java nicht -> man muss aufpassen wenn man solchen Kotlin code in Java verwenden will.

Top Level Functions and Properties:

* Code welcher zu keiner Bestimmten Klasse passt
  * Man kann diesen man einfach als Top Level Methode und Eigenschaften definieren
  * In Java muss man diesen in einer Zustandslosen Statischen Klassen definieren
* Solche Methoden können auch importiert werden
* Verwendung mit Java:
  * Es wird automatisch eine Klasse erstellt der diese Methoden beinhaltet
  * Name der Klasse = Name der Datei oder kann mit `@file:JvmName("Name")` festgelegt werden

Top Level Properties

* Eigenschaften können wie Methoden auf der Obersten Ebene der Datei platziert werden.
* Solche Eigenschaften werden im in einem Statischen Feld gespeichert
* Werden wie Normale Eigenschaften behandelt (haben Getter und Setter)

### Extension Functions and Properties

> Definition: Extension Function  
Es handelt sich hierbei um eine Normale Funktion, welche als Member einer Klasse aufgerufen werden kann, obwohl die Methode außerhalb der Klasse definiert wurde.

* Deklaration von Extension Functions:  
  
  ```kotlin
  fun Klasse.neueFunktion():Typ {...}
  ```

  * Bei der Deklaration von Extension Funktionen referenziert `this` die in der zu Erweiternde Klasse
  * `this` kann auch komplett weggelassen werden
* Extension Funktionen einer Klasse sind nicht überall zugreifbar
  * Man muss diese erst importieren bevor man diese verwenden kann
    Imports können auch mit `as` umbenannt werden
* Java
  * Werden als statische Methoden Interpretiert
  * Erstes Argument ist die zu erweiternde Klasse
* Extension Funktionen können nicht überschreiben werden (overriding)
  * Diese werden außerhalb der Klassen definiert
  * Werden statisch aufgelöst

Extension Properties

* Sind zustandslos
  * Es gibt keine passende Stelle eine Zustand zu speichern
  * Ermöglichen es einer Klassen Nicht zusätzlichen Zustand zu geben
* Einiger Vorteil kurze Syntax
* Getter muss immer Implementier werden da es kein Feld gibt, um einen Wert auszulesen.
* Initializer sind aus gleichem Grund Verboten (Kein Feld um Wert zu speichern vorhanden)

### working with Collections

* `vararg`: Variable Anzahl an Argumenten die man einer Funktion übergeben kann.
* *infix* Notation: Methoden aufrufen ohne Klammern und Punkt
* *Destructuring declaration*: Destrukturierung -> Zuweisen von Mehreren Werten in mehreren Variablen in einer Zeile (entpacken?)

varargs

* ähnlich zu Java
* man verwendet `varargs` Schlüsselwort statt `...`
* In Kotlin wird man explizit aufgefordert den Array zu entpacken
* Zum entpacken eines Arrays setzt man `*` vor dem array

Infix Notation

* z.B. `to` ist eine Methode
* Bei einem Infix Methodenaufruf kommt der Methodenname zwischen Objekt und Parameter gesetzt ohne zusätzliche Trennelemente (Punkt und Klammer)
* Man kann infix Aufrufe mit normalen und extension Funktionen die nur ein Parameter haben verwendet werden

### Strings und Regular Expressions

* Aufsplitten trennt man immer zwischen Regular Expression und String für Trennzeichen
* Java oft Verwirrung da Split immer eine Regular Expression als Parameter nimmt
* `""" Her some text """` Dreifach Anführungszeichen
  * Kein escaping bei regulären Ausdrücken
  * Man kann mehrzeilige Strings haben

### Local Functions and Extensions

* Man Kann Funktionen in Funktionen haben -> lokale Funktionen
* Lokale Funktionen können auf dem Scope der umschließende Funktion zugreifen