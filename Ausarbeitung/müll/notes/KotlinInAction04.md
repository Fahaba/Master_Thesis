# Kotlin in Action

## Kapitel 4: Classes, Objects and Interfaces

Klassen und Interfaces unterscheiden sich etwas von Java

* Interfaces können Eigenschaft-Deklarationen haben
* In Kotlin sind Deklarationen standardmäßig `final` und `public`
* Genestete Klassen sind nicht sofort inner Klassen wie in Java
  * Haben keine implizite Referenz auf die äußere Klasse
Es gibt in Kotlin *Daten Klassen*
* Der Compiler erstellt automatisch bestimmte Methoden für diese Art von Klassen
* Der delegation Pattern wird von Kotlin unterstützt
  * Man muss sich nicht manuell darum kümmern

### Klassenhierarchien

Interfaces:

* Werden mit `interface` deklariert
* Können beinhalten:
  * Abstrakte Methoden
  * Implementierung von nicht abstrakten Methoden (ähnlich zu den `default` Methoden in Java 8, aber ohne zusätzlichen Schlüsselwort)
  * Besitzen keinen Zustand
* `:` ersetzt sowohl `extends` als auch `implements`
* Eine Klassen kann (wie in Java):
  * Viele Interfaces implementieren
  * Aber nur eine Abstrakte Klasse erweitern
* Statt der `@Override` Annotation setzt man in Kotlin den `override` Modifikator vor der Implementierung der abstrakten Methoden.
  * Der `override` Modifikator ist nicht optional sondern verpflichtend
* Wir haben eine Klasse die zwei oder mehr Interfaces implementiert, wobei mehrere der Interfaces eine gleichnamige Methode mit Standardimplementierung besitzen.
  * Frage: Welche der beiden Implementierungen wird genommen/gewinnt?
  * Antwort: Keine, Fall man keine neue Implementierung angibt müssen alle ausgeführt werden werden.
  * Beispiel: `Clickable` und `Focusable` besitzen beide eine Methode `showOff` mit Standardimplementierung

    ```Kotlin
    class Button : Clickable, Focusable {

        override fun click() = println("I was clicked")

        override fun showOff() {
            super<Clickable>.showOff()
            super<Focusable>.showOff()
        }
    }
    ```

`open` `final` und `abstract` Modifikatoren

> Im folgenden bezeichnen wir als offen eine Klasse, Methode, Eigenschaft die überschrieben werden kann und als geschlossen solche die nicht überschrieben werden können.

* Kotlin Klassen sind standardmäßig `final` (Java Klassen sind offen)
* Um Subklassen zu öffnen verwendet man den `open` Modifikator verwenden
  * Für Eigenschaften die überschreiben werden können muss man auch den `open` Modifikator verwenden
  * Gleiches gilt auch für Methoden die überschrieben werden können
* Methoden die mit `override` Überschrieben werden werden als offen angenommen
  * Um diese schließen muss man `final` verwenden
  * Beispiel:

    ```Kotlin
    override fun click() // ist offen
    final override fun click() // is geschloßen
    ```

* Abstrakte Klassen werden mit `abstract` gekennzeichnet
  * Abstrakte Klassen können nicht instanziiert werden
  * Beinhalten abstrakte Methoden, die keine Implementierung besitzen
    * Diese Methoden sind immer implizit `open`
  * Beinhalten konkrete Methoden mit Implementierung
    * Diese sind implizit geschlossen
    * Können mit `open` geöffnet werden

Sichtbarkeit: Standardmäßig offen

* Sichtbarkeit funktioniert ähnlich wie in Java:
  * Wir haben: `public`, `protected` und `private`
* In Kotlin sind alle Deklarationen (ohne Modifikator) standardmäßig `public`
  * In Java standardmäßig `package-private`
  * In Kotlin dienen Pakete jedoch nur für organization von Code nicht um die Sichtbarkeit zu steuern
  * Kotlin besitzt jedoch den `internal` Modifikator
    * Dieser gibt Sichtbarkeit in einem *Modul* an
      > Ein Kotlin Modul sind Kotlin Dateien die gemeinsam kompiliert wurden (z.B. Maven-/Gradle-Projekt oder IntelliJ Modul)
* Sichtbarkeit:

  | Modifikator | Sichtbarkeit: Klasse | Sichtbarkeit: Top Level|
  | :-:         | :-:                  |:-: |
  | public      | überall              | überall|
  | internal    | Innerhalb des Moduls | Innerhalb des Moduls|
  | protected   | Innerhalb Subklassen | - |
  | private     | Innerhalb der Klasse | Innerhalb der Datei |

* Kotlin erlaubt es nicht weniger sichtbare Klassen zu referenzieren
  > Für zu Probleme bei extension Methods
* `protected` Member einer Klasse können von der Klasse selbst oder Unter-/Subklassen aufgerufen werden
  * In Java können diese von allen Klassen im gleichen Paket zugegriffen werden
* In Kotlin sehen die äußeren Klassen auch nicht die `private` Member der inneren oder verschachtelten (nested) Klassen

Innere und genestete Klassen: Standardmäßig nested

* Wie in Java kann man Klassen in Klassen deklarieren
* Ein unterschied jedoch: innere Klassen können nicht auf die äußere Klasse zugreifen
* In Java wird eine Klasse die in einer anderen Klasse deklariert wird automatisch zu einer inneren Klasse (=inner class)
  * Möchte man eine innere Klasse als statisch so wird die Referenz zu äußeren Klasse entfernt (=nested class)
  * Eine Innere Klasse ohne Modifikator in Kotlin ist, das gleiche wie eine statische innere Klasse in Java (=nested class)
* Um eine Innere Klasse wie in Java zu erhalten muss man den `inner` Modifikator verwenden
* Um von eine Inneren Klasse auf die äußere zuzugreifen in Kotlin muss  diese Syntax verwenden: `this@outer`

Sealed Klassen: Restriktive Klassenhierarchien

* Bei der Auswertung von `when` Ausdrücken muss man immer einen `default` Zweig haben
* Ist nicht immer ideal
  * Wenn man neue Subklassen erstellt, erkennt der Compiler nicht unbedingt das sich was geändert hat
  * Wenn man für die neue Subklasse keine neuen Zweig hinzufügt wird der Default-Branch genommen
* Mit dem `sealed` Modifikator kann meine eine Klasse *abschließen*
  * Es schränkt die Fähigkeit ein neue Subklassen zu erstellen
  * Alle Subklassen müssen in der Superklasse verschachtelt sein.
  * Wenn alle Subklassen im `when` betrachte werden braucht man keinen Default-Zweig
  * Eine `sealed` Klasse ist auch immer implizit offen
* Sealed Klassen können nicht als Interfaces deklariert werden.

### Nichttrivial Klassendeklarationen

Einfache Klassendeklaration:

``` Kotlin
class User(val nickname: String)
```

* Ein solcher Konstruktor (ohne `{}` Codeblock) wird auch *primary constructor* genannt
* Hier werden Konstruktor-Parameter und Klasseneigenschaften spezifiziert

Der obere Code ist äquivalent zu:

``` Kotlin
class User constructor(_nickname: String) {
  val nickname: String

  init {
    nickname = _nickname
  }
}
```

* `constructor`-Schlüsselwort leitet den Anfang eines primären oder sekundären Konstruktors an
  * Kann weggelassen werden wenn wenn keine Sichtbarkeitsmodifikatoren oder Annotationen vorhanden sind (bei Primären Konstruktoren)
* `init` leitet den *initializer block* ein
  * Dieser Codeblock wird aufgerufen wenn eine Klasse erstellt wird
  * Man kann mehrere Initialisierungsblocks haben
* Um Objekte einer Klasse zu erzeugen braucht man in Kotlin kein `new` Schlüsselwort
* Wenn eine Klasse eine Superklasse besitzt so muss auch der Konstruktor der Superklasse initialisiert werden
  * Falls eine Klasse von einer anderen Klasse ohne Konstruktor erbt, so muss man trotzdem einen Konstruktor in Form von `()` angeben
  * Ausnahme sind Interfaces, da diese nie einen Konstruktor besitzen müssen keine Klammern angegeben werden
* Mit `private constructor()` werden Primäre Konstruktoren privat gemacht
  * Der Konstruktor kann nur von der Klasse selbst aus aufgerufen werden.

Sekundär-Konstruktoren: Initialisierung von Superklassen

Klassen mit mehreren Konstruktoren sind in Kotlin vergleichsweise selten.
Man muss Konstruktoren oft nicht überladen da wie die *Default Parameter Values* und *Named Syntax* haben

* Klassen ohne Primär-Konstruktoren haben keine Klammern mit Parameter-Definition im Klassen-Header (/Kopf am Anfang der Klassendefinition)
* Sekundär-Konstruktoren können mit `constructor(par1: Type1, ..., parN: TypeN) { ... }` Blöcken definiert werden

Interface-Eigenschaften implementieren

In Kotlin können Interfaces abstrakte Eigenschaften besitzen
Klassen die ein Solches Interface, mit Eigenschaften, implementiert, müssen eine Möglichkeit bereitstellen auf diese Eigenschaften zuzugreifen.
Das Interface selbst definiert nicht ob die Eigenschaft in einem Feld vorliegen muss oder durch einen getter erhalten wird (da das Interface ja keinen Zustand besitzen darf)
Hier kann man:

* Die Eigenschaft direkt im Klassen-Konstruktor mit `override` überschreiben (somit erhält diese ein Feld, auf dem dieser abgespeichert werden kann)
* Man kann in der Klasse selbst eine Getter-Methode Definieren die verwendet werden soll, ohne das ein Feld deklariert werden muss um einen Wert abzuspeichern
* Wir können der Eigenschaft in der Klasse sofort einen Wert zuordnen

Interfaces können auch bereits vordefinierte Getter und Setter Methode haben solange diese keinen Zustand besitzen.

#### Auf Felder mit Getter und Setter zugreifen (Guter deutscher Name für *backing field*)

Man kann wie bei C# die Feldzugriffslogik modifizieren, in dem man eigene Getter und Setter erstellt.
Auch die Syntax ist ähnlich wie in C#:

``` Kotlin
var prop: Type = value
  set(value: Type) {
    // custom logic here
  }
  get() {
    // custom logic here
  }
```

* um auf den eigentlichen Wert des Feldes im Setter und Getter zuzugreifen verwenden wir `field`
* ?? Man Kann nur einen eigenen Accessor für eine *mutable* Eigenschaft definieren ??

#### Visibilität der Accessoren ändern

Standardmäßig haben die Accessoren die Gleiche Sichtbarkeit wie die Eigenschaften.
Die Sichtbarkeit kann jedoch geändert werden.

* Sichtbarkeit kann geändert werden ohne das man eine konkrete Implementierung angibt

### Daten Klassen und Klassen Delegierung

* Für manche Klassen muss man in Java bestimmte Methoden wie `equals`, `hashCode`, `toString` erstellen.
  * Viele IDE s können diesen Code öfters automatisch erstellen so dass Entwickler keine zusätzliche Fleißarbeit erbringen muss.
  * Jedoch leidet die Lesbarkeit der Dateien durch den zusätzlich generiertem Code.
  * Kotlin hilft hierbei:
    * Der Compiler kümmert sich im Hintergrund um die Generierung des Codes
    * Dadurch bleibt der Code lesbarer das es nicht zusätzlich aufgebläht wird

> Kotlin sorgt dafür das der so übersichtlich wie möglich bleibt dies kann man anhand der default Konstruktoren, der default Accessors usw.

#### Universale Objekt Methoden

Wie bereits oben Erwähnt gibt es viele Standard-Methoden, welche man oft überschreiben will (wie `toString`, ... )

> Unterschied zischen Java und Kotlin: `==` vergleicht ob die Objekte gleich sind und nicht wie in Java ob Ihre Referenz gleich ist. `==` ruft immer die `equals` Funktion eines Objekts. Um Referenzen zu vergleichen verwendet man in Kotlin `===`  
Weiterer Unterschied `Object` in Java ist `Any` in Kotlin und `?` hinter einem Objekt gibt an das dieser `null` sein kann.

### 4.4 Object Schlüsselwort: Deklaration und Erstellung von Objekten in einem

Das `object` Schlüsselwort dient zur Definition und gleichzeitigen Instanziierung einer Klasse (Erzeugung eines Objektes)
Es wird verwendet um:

* Singleton Objekte zu erstellen
* Companion Objekte zu erstellen (beinhalten zu einer Klasse gehörigen Logik jedoch nicht Instanziiert werden müssen??)
* Objekt Expressions zu erstellen (Alternative zu Java's anonyme innere Klassen)

#### 4.4.1 Singleton Einfach gemacht

Ein Singleton ist eine Klasse von der es genau eine Instanz gibt.
Beim Singleton Pattern definiert man eine Klasse mit einem privaten Konstruktor mit einem statischen Feld welche diese Klasseninstanz beinhaltet.
Kotlin unterstützt Singleton Objekte von Haus aus:

* Hierfür verwendet man das *Object Declaration* Feature von Kotlin
  * Dies Kombiniert die Klassendeklaration und das erstellen einer einzelnen Instanz der Klasse
  * Also man definiert eine Klasse und eine Variable dieser Klasse ein einem Statement
  * Unterschied zu Normalen Klassen:
    * Einzig verboten in *Objekt Deklarationen* sind Konstruktoren (Sowohl Primär als auch Sekundär)
    * Im Vergleich zu normalen Klassen werden Objekt Deklarationen sofort erzeugt und nicht erst durch Konstruktor-Aufrufen im Code
  * Objekt Deklarationen können auch von Klassen und Interfaces erben
  * Man kann auch solche Objekte anderen Klassen definieren
    * Hierbei habe diese nur eine Instanz für alle Klasseninstanzen der übergeordneten Klasse, nicht eine Instanz pro Instanz der Übergeordneten Klasse

#### 4.4.2 Companion Objects

In Kotlin gibt es keine statischen Member (Methoden/Variablen) wie in Java

* Companion Objekte haben zugriff auf sämtliche *private* member einer Klasse
* Companion Objekte eigenen sich besonders gut für Factory Methoden zur Objektgenerierung
  * Der eigentliche Konstruktor wird *private* gehalten
  * Die CompanionObjekte können passend (ihrer Rolle) benannt werden.
  * Die Companion Objekte können zusätzliche Logik enthalten
  * Die Companion Objekte rufen den private Konstruktor auf damit man am Ende ein passendes Objekt der Klasse hat.
  * Dienen als alternative zu mehreren Konstruktoren dienen.
* Companion Objekte können jedoch in Subklassen nicht überschrieben werden.

#### 4.4.3 Companion Objekte als normale Objekte

Ein Companion Objekt ist ein reguläres Objekt, welches in einer Klasse deklariert wurde:

* Es kann benannt werden,
* Ein Interface implementieren,
* Kann Eigenschaften haben,
* Kann Extension Methoden haben.

Zum referenzieren von Companion Objekte:

* Mann kann diesen mit Namen referenzieren (z.B. `a.cobject.funct`)
* Man kann Methoden von Companion Objekten direkt aufrufen, ohne dessen Namen zu kennen (z.B. `a.funct` ist das gleiche wie oben)
* Falls man den Namen vom Companion Objekt wegfallen lässt wird immer der Name `Companion` verwendet

Interfaces mit Companion Objekte

* Companion Objekte können Interfaces Implementieren

Extension Funktionen

* Man kann extension Funktionen auf Companion Objekten definieren
* Hierfür muss vorher jedoch erst ein Companion Objekt für einen Klasse erzeugt worden sein (auch ein leeres Companion Objekt genügt)
* Fall das Companion Objekt kein Name hat so muss man `Companion` stattdessen verwenden (z.B. `fun myclass.Companion.myfunction(...): ...`

#### 4.4.4 `object` und *Anonymous Inner Classes*

Mit dem `object` Schlüsselwort kann man neben Singleton-Ähnlichen Objekten auch anonyme Objekte erstellen.
Diese ersetzen Javas *anonyme innere Klassen*.

* Syntax von innere Objekte haben ähnliche Syntax zu Java
  * Objektname fällt jedoch in Kotlin weg.
* Anonyme Innere Klasse in Java können nur eine Klasse erweitern oder ein Interface implementieren.
  * Anonyme Objekte in Kotlin können eine, mehrere oder keine Klassen Implementieren.
* Anonyme Objekte sind keine Singletons.
  * Jedes mal wenn ein Objekt-Ausdruck ausgeführt wird, wird eine neue Instanz des Objekts erzeugt.
* Anonyme Objekte können nur Variablen der Funktion auslesen, indem das Objekt erstellt wurde.
