# Kotlin in Action

## Kapitel 5: Programmierung mit Lambdas

Lambda Ausdrücke oder einfacher Lambdas sind grundsätzlich kleine Codeschnipsel/-stücke welche Funktionen als Argumente übergeben werden können.
Besonders hilfreich sind Lambdas im Zusammenhang mit Datenbeständen/Collections.
Zusätzlich zu den normalen Lambdas gibt es spezielle Lambdas die sogenannten Receiver Lambdas.
Receiver Lambdas haben die Besondere Eigenschaft das diese in einem anderen Kontext ausgeführt werden können, als der, indem diese erstellt wurden.

### 5.1 Lambda Ausdrücke und Member Referenzen

#### 5.1.1 Einführung

Funktionale Programmierung ermöglicht es uns Funktionen wie Variablenwerte zu behandeln und zu verwenden

* Lambdas können als Funktions-Argumente übergeben werden
* Statt Funktionen erst zu erstellen ermöglichen uns Lambdas direkt Codeblöcke zu übergeben.
* In Java (vor Version 8) musste man für ähnliches Verhalten anonyme inneren Klassen verwenden
  * Sehr verboser Code
  * Schnell aufgebläht und unübersichtlich
* Bei Lambdas muss man nur das Verhalten implementieren der Ganze Rest herum fällt fast vollständig weg.
  * Code ist lesbarer

#### 5.1.2 Lambdas und Datenbestände/Collections

Bei Datenbeständen muss man öfters den gleichen Code auf jedes Element ausführen dies manuell zu tun ist mühsam und fehleranfällig.
(Man kann zum Beispiel schnell ein Element vergessen, die falsche)
Weiterhin folgen die Operationen öfters die gleichen Muster.
Lambdas eignen sich hierfür besonders gut, sie ermöglichen uns Operationen auf Datenbeständen einfach und effizient zu erstellen und diese auszuführen.
Zusätzlich bring Kotlin bereits Collection-Funktionen mit sich, die Lambdas als Parameter nehmen.

#### 5.13 Lambda Syntax

* Lambda Code kann unabhängig definiert werden
* Lambdas können in Variablen gespeichert/zugewiesen werden
* Lambdas können direkt in der Übergabe deklariert werden.
* Lambda Code wird immer von geschweiften Klammern `{...}` umgeben
* Argumente werden vom Code mit einem Pfeil `->` , von der eigentlichen Logik getrennt
* Falls in Lambdas in einer Variablen gespeichert werden, kann man diese Variable wie eine Normale Funktion verwendet werden.
  Beispiel:

``` kotlin
val sum = {x: Int, y: Int -> x + y}
println(sum(1,2))
```

* Lambdas können, solange diese, das Letzte Argument einer Funktion sind, hinter (und somit außerhalt) den Klammern gesetzt werden.
* Wenn ein Lambda, das einzige Argument einer Methode ist so können die vorgehenden Klammern ganz wegfallen gelassen werden.
* Parametertypen müssen auch für die Lambda-Parameter nicht explizit angegeben werden, denn auch hier funktioniert Typinferenz
  * Achtung: Typinferenz funktioniert jedoch nicht immer  
    Ein guter Leitfaden ist es den Typ nicht anzugeben, und diesen erst zu spezifizieren falls der Compiler sich beschwert
* Genau wie in Groovy braucht man keine Parameterangabe in Lambdas zu verwenden, falls das Lambda nur einen Parameter hat.  
  In diesem Fall kann man einfach den Standardparameter `ìt` verwenden.  
  `it` ist der Name des Standardparameters.
* Falls ein lambda in einer Variablen gespeichert wird so ist man jedoch gezwungen alle Parametertypen explizit anzugeben, da es keinen Kontext gibt,aus dem das Lambdas den Parametertypen schlussfolgern kann.
* der letzte Ausdruck eines Lambdas wird auch als rückgabewert/Ergebnis des Lambdas genommen/angesehen.

#### 5.1.4 Auf Scope Variablen zugreifen

* Lambdas können auf die lokalen Variablen der Funktion zugreifen in der das Lambda aufgerufen werden.
* Natürlich kann es nur auf Variablen zugreifen welche vor dem Aufruf der Funktion deklariert wurde.
* Unterschied zwischen Kotlin und Java:
  * In Java kann man nur auf finale Variablen zugreifen (bei der Verwendung von inneren Klassen)
  * Kotlin erlaubt es den Lambdas auch nicht Finale Variablen (`var`) zu modifizieren
    * nicht `var` Variablen von der umgebenden Funktion werden vom Lambda eingefangen, man spricht von *captured* Variablen
    * Standardmäßig existiert eine lokale Variable nur solange wie die Funktion existiert.
    * Für eingefangene Variablen wird ein spezieller Wrapper um die Variable erzeugt, das Lambda erhält anschließend eine Referenz auf diesen Wrapper und kann mit dessen Hilfe den Wert ändern.
  * Man muss jedoch bei asynchrone events jedoch aufpassen da es kann das die lambda erst nach dem die Funktion verlassen wird ausgeführt, aufgerufen wird.

#### 5.1.5 Member Referenzen

Member Referenzen erlauben es uns einfach bereits vorhandene Funktionen und Felder wie Lambdas zu verwenden.

* Um eine Funktion oder oder einen Wert in ein Lambda umzuwandeln muss man lediglich `::` verwenden
* Beispiel:

``` kotlin
1. val getAge = Person::age
2. val getAge = { person: Person -> person.age}
```

* Im Beispiel sind Zeile 1 und 2 äquivalent
* Bei Top-Level Funktion, welche keiner Klasse angehören kann man einfach den Klassennamen weglassen (z.B. `::topLevelFunction`) um diese ein Lambda zu konvertieren.
* Man kann auch die Erstellung von Klasseninstanzen herauszögern in dem *Konstruktor Referenzen* verwendet (`createPerson = ::Person`).
* Man kann auch *Extension Funktionen* auf dieser Weise referenzieren

### 5.2 Funktionale API für Datenbestände (Collections)

Funktionales programmieren bringt besonders viele Vorteile bzw. vereinfacht es den Umgang mit Datenbeständen.
Im Folgenden werden wir uns einige in der Kotlin Standardbibliothek mitgelieferten Funktionen für Datenbestände anschauen.

#### 5.2.1 filter und map

filter:

* Verwendet um Datenbestände zu filtern
* liefert die liste Elemente eines Datenbestandes zurück, welche für den mitgegebenen Lambda true ergeben, zurück.

map:

* Map erlaubt es Elemente eines Datenbestandes zu transformieren, filter erlaubt das nicht
* Liefert uns ein neues Datenbestand, welche die Transformierten Elemente, des ursprünglichen Datenbestandes hat zurück

Man kann sämtliche funktionsaufrufe auf Datenbestände hintereinander verketten.

#### 5.2.2 all, any, count, find

all

* all wird verwendet um zu überprüfen ob alle Elemente eines Datenbestandes ein Kriterium erfüllen
* falls negiert (!all) sollte man stattdessen any verwenden

any

* wird zu Überprüfung ob wenigstens ein element das Kriterium nicht erfüllt verwendet
* falls negiert (!any) sollte man all verwenden

count

* Count liefert uns den Wert, wie viele Elemente des Datenbestandes ein Kriterium erfüllen zurück
* überall wo man count verwendet könnte man auch filter und size verwenden:
  * Hierbei würde ein Zusätzlicher Datenbestand, vor dem Aufruf von size, erzeugt werden.
  * Ist also nicht so effizient wie count, besonders bei großen Datenbeständen.

find

* liefert uns das Erste Element zurück das unser Kriterium erfüllt.
* kann durch firstOrNull ersetzt werden, beide machen genau das gleiche.

#### 5.2.3 groupBy

* Gruppiert Elemente eines Datenbestandes
* Jede Groupe wird in einer Liste abgespeichert
* Das Ergebnis ist eine Map die folgendermaßen aufgebaut ist (falls die Elemente einem Int nach, z.B. alter, gruppiert werden):

```kotlin
Map<Int, List<CollectionName>>
```

#### 5.2.4 flatMap und flatten

flatMap

* Eine FlatMap macht zwei Sachen:
  * Es Transformiert den Datenbestand, anhand des übergebenen Lambdas (Map)
  * Anschließend kombiniert es die Ergebnislisten zu einer einzigen Liste (Flatten)

flatten

* Falls wir keine Transformation machen dann können wir einfach mehrere Listen mit flatten zu einer einzelnen Liste kombinieren.

toSet

* toSet entfernt Duplikate von Datenbestände

### 5.3 Lazy Collections Operatoren: Sequenzen

Jedes mal wenn eine der Obengenannten Methoden, oder Sonstige Kotlin API Methoden für Datenbestände auf Datenbestände ausgeführt werden wird ein neues Datenbestand erzeugt.
Wie bereits erwähnt kann man mehrere dieser API aufrufen hintereinander reihen.
Für jedes der Aufrufe wird dementsprechend zwischendurch einen neues Datenbestand erzeugt da kotlin diese API Aufrufe eifrig Auswertet (eager evaluation).

Obwohl Kotlins eifrige Auswertung an sich ziemlich effizient ist so wird es schnell problematisch wenn wir mit großen Datenbestände arbeiten.
Um mit diesen effizienter umgehen zu können hat Kotlin eine besondere Fähigkeit in der Form von Sequenzen (Sequences) die faule Auswertung (lazy evaluation) ermöglicht.

Es ist möglich eine Kette von Operationen in eine Sequenz umzuwandeln indem man `asSequence` Methode Aufruft. Weiterhin unterstützen Sequenzen sämtlich Kotlin API Aufrufe für Datenbestände sodass nichts weiteres geändert werden muss. Lediglich sollte die Sequenz nach der letzten Operation wieder zurück konvertier werden (z.B. mit der toList() Methode), damit man anschließend mit dem resultierenden Datenbestand weiterarbeiten kann.

Bei Sequenzen werden keine Datenbestände bei jedem Operationsaufruf erstellt, somit hat man eine geringeren Speicherprofil im vergleich mit zur Eifrigen Auswertung.

#### 5.3.1 Sequenz Operationen: Zwischen- und Terminaloperationen

Operationen auf Sequenzen fallen immer in einer von zwei Kategorien:

1. Zwischenoperationen: Jede Zwischenoperation liefert immer eine neue Sequenz als Ergebnis.  
   Jede Auswertung ist immer faul (lazy). Dementsprechend werden auf jedes Element des Ausgehenden Datenbestandes alle Operationen der Sequenz ausgeführt, bevor man mit dem nächsten Element weiter weitermacht. Bei eager evaluation wird erstmals die erste Operationen auf alle Elemente des Datenbestandes ausgeführt werden, anschließend auf die Elemente des resultierenden Datenbestandes die nächste Operationen durchgeführt, usw. bis man alle Operationen durch hat. Dies ist Bei Sequenzen nicht der Fall
2. Terminaloperation: Diese liefert uns eine festes Ergebnis zurück hierbei kann es sich um ein neues Datenbestand, ein Element oder was auch immer man nach der letzten Operation in der Sequenz zurückgegeben wird. Weiterhin, da Sequenzoperationen faul sind, werden diese erst nur dann ausgeführt wenn die Werte Benötigt werden. Die Terminaloperation dient demensprechend als Auslöser der für die Auswertung der Zwischenoperationen. Hieraus kann man auch schließen das die Terminaloperation nicht faul sondern eifrig ist.

Weiterhin ist hier ist gut zu wissen das Sequenzen sehr ähnlich bzw. fast das gleiche sind wie die in Java 8 eingeführten Streams sind.

### 5.4 Verwendung mit Javas funktionalen Interfaces

> Als *Funktionale Interfaces* oder auch *SAM Interfaces* (SAM: Single Abstract Method) bezeichnete Interfaces in Java, sind Interfaces die nur eine abstrakte Methode besitzen.

Die Java API ist voll mit den oben genannten Funktionalen Interfaces.
Sämtliche Methoden in Kotlin, die ein solches Interface als Parameter besitzen, können, statt einer anonymen Klasse, wie es in Java üblich ist, einfach ein Lambda als Argument erhalten.
Das übergebene Lambda wird dann für die Implementierung der Abstrakten Methode des Interfaces verwendet.
Der Code bleibt durch die Verwendung von Lambdas kompakt, wird nicht zusätzlich aufgebläht und ist somit übersichtlicher.

#### 5.4.1 Lambdas an Java Methoden übergeben

Überall wo in Java ein funktionales Interface als Argument erwartet können man stattdessen ein Lambda verwenden. Wie dies Funktioniert wird im folgenden beschrieben:

Falls ein Lambda übergeben wird, dann erstellt der Compiler im Hintergrund ein anonymes Objekt das dieses Interface implementiert, und verwendet das übergebene Lambda für die Implementierung der abstrakten Methode.

Ein Vorteil Lambdas zu verwenden statt explizit anonymen Objekten zu verwenden ist, dass nicht bei jedem Aufruf einen neue Instanz des Objekts erstellt wird. Bei expliziten Objekten wird bei jedem Aufruf eine Instanz erstellt. Falls wir Lambdas verwenden, welche keinen Zugriff auf Variablen der umgebenden Funktion hat, dann wird die gleiche Instanz zwischen den verschiedenen Aufrufen verwendet.

#### 5.4.2 SAM Konstruktor explizit verwenden

> Ein *SAM Konstruktor* ist eine vom Compiler generierte Funktion, mit der man aus einem Lambda explizit eine Instanz, eines Funktionalen Interfaces generieren kann.

SAM Konstruktoren sind hilfreich in Kontexten wo der Compiler die Lambdas nicht automatisch in Funktionale Interfaces umwandelt. 
Ein Beispiel wäre eine Methoden die uns eine Instanz eines funktionalen Interfaces liefert.
Ein Lambda als Resultat einer Methode zurückzugeben ist hier nicht möglich als müssen wir das Lambda vor der Rückgabe in eine Instanz konvertieren. Diese wird anschließend zurückgeben.

Für Verwendung des SAM Konstruktors muss man lediglich den Namen der zu generierende Funktionalen Interface angeben. (z.B. `Runnable { // this is the lambda}` würde aus dem Lambda eine Instanz von `Runnable` erstellen)

### 5.5 Lambda mit Receiver: `apply` und `with`

Die Fähigkeit Methodenaufrufe von Objekte in Lambdas, ohne Qualifier, durchzuführen, ist eine Besonderheit von Kotlin (Java hat diese Fähigkeit nicht)
Lambdas welche diese Fähigkeit besitzen werden auch *Lambda mit Receiver* genannt.
Hierbei sind die Receiver die Objekte, dessen Methoden ohen Qualifier aufgerufen werden können.

#### 5.5.1 `with` Funktion

Diese Funktion ist besonders hilfreich um mehrere nacheinander folgende Operationen auf das gleiche Objekt durchzuführen. Bei der Verwendung von `with` muss man den Objektnamen nicht angeben.

Beispiel:

``` kotlin
val stringBuilder = StringBuilder()
with(stringBuilder){
  for (letter in 'A'..'Z') {
    this.append(letter)
  }
}
```

With ist eine Funktion welche ein ReceiverObjekt (hier: stringBuilder) und ein Lambda als Argumente erwarten

* Um auf den receiver zuzugreifen gibt es verschieden Möglichkeiten
  * Man kann `this` verwenden.
  * Man kann Methoden vom Receiver Aufrufen ohne diesen explizit anzugeben.
  * Gleiches gilt für die Eigenschaften des Receivers

#### 5.5.2 `apply` Funktion

Sehr ähnlich zur `with` Funktion. Unterschied ist, dass das ReceiverObjekt zurückgegeben wird.
Diese Funktion ist eine *Extension function* auch hier wird das erste Argument als Receiver vom Lambda (zweites Argument) verwendet