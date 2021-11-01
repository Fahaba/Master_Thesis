# Kotlin In Action

## Kapitel 6: Das Kotlin Typ-System

### 6.1 Nullbarkeit

Nullbarkeit ist eine weitere Besonderheit von Kotlin gegenüber Java und hilft besonders um mit NullPointerExceptions umzugehen und diese vorzubeugen.

Hierbei wird unter anderem Versucht ein Teil dieser Fehler zur Kompilierungszeit und nicht erst zur Laufzeit abzufangen

#### 6.1.1 Nulltypen

Im vergleich zu Java unterstützt Kotlin NullTypen.

Kotlin bietet den Programmierer die Fähigkeit selbst zu bestimmen welche Variablen  und Methodenaufrufe null sein bzw. null zurückliefen können.

Z.B. wenn eine Variable als Nullbar markiert wurde, verbietet kotlin Methoden auf diese Variable auszuführen, da dies zu einer NullPointerException führen könnten

Normal definierte Variablen werden immer als nicht nullbar angesehen. Man muss bei der Deklaration explizit sagen, das die Variable null sein kann. Dies macht man in dem ein `?` nach dem TypNamen setzt. (Beispiel: `val s: String?`)

Fall eine Variable als nullbar markiert, so werden die Operationen, welche man auf dies ausführen kann stark eingeschränkt.
So kann man keine nullbare variable einer Funktion übergeben welche eine normale  Variable erwartet.

Um als nullbar markierte Methoden normal verwenden zu können muss erstmal eine überprüfen auf null stattfinden, erst danach sind alle Operationen zugänglich.

#### 6.1.3 Safe call Operator `?.`

Dieser Operator kombiniert null Überprüfung und Methodenaufruf in einem Operator.
Falls wir eine nullbares Objekt/Variable haben einen Methoden auf dieser aufrufen wollen, dann müssen wir zunächst sichergehen, das diese nicht null ist, da in diesem Falls keine Methode aufrufen können, erst wenn wir garantieren garantieren können das die Variable nicht null ist können wir die gewünschte Methode aufrufen. Der Safe-call Operator reduziert den aufwand nur ein ? vor dem Methodenaufruf setzen zu müssen.

Safecalls können auch im zusammenhangen mit Eigenschaften verwendet werden.

#### 6.1.4 Elvis Operator `?:`

Der Elvis Operator erlaubt es Standardwerte für nullfälle zu definieren.

Beispiel

``` kotlin
fun foo(s: String?) {
  val t: String = s ?: ""
}
```

In diesem Beispiel erhält `t` den wert von `s`, fall `s` nicht null ist und ein leeres String falls s null ist.

Der Elvis Operator kann auch im Zusammenhang mit throw und return verwendet werden. Um bestimme Rückgabewerte zurückzugeben und Kontrollstrukturen frühzeitig zu verlassen oder um einfach Exceptions zu werfen.

#### 6.1.5 Safe casts `as?`

Dieser Operator versucht ein Wert in ein angegebenen Typ um zu konvertieren falls dies gültig ist wird der konvertierte Wert zurückgegeben, falls nicht wird null zurückgegeben

Man kann diesen auch in Kombination mit dem Elvis Operator verwenden um zu garantieren das kein null erhalten wird

#### 6.1.6 Not-Null Assertion `!!`

Dieser Operator stellt die Brute Force Methode dar um Nullbare Typen in nicht Nullbaren Typen umzuwandeln.

Sollte die Variable jedoch den Wert null haben so wird eine NullPointerException geworfen (deshalb sollte die Verwendung diese Operators so weit wie möglich vermieden werden)

#### 6.1.7 die `let` Funktion

Ist eine Funktion die den Umgang mit nullbaren ausrücken vereinfacht.

Ein Einsatzbereich ist nullbare Werte an Funktion zu übergeben die nicht nullbare werte verwendet.

Die `let` Funktion wird nur ausgeführt wenn der Wert nicht null ist.

Grundsätzlich nimmt eine Lambda als Parameter.

#### 6.1.8 spät-initialisierte Eigenschaften

In vielen Frameworks initialisieren Objekte in dedizierten Methoden, nachdem die Instanz erstellt wurde.

In Kotlin können jedoch nicht nullbare Eigenschaften ohne Initialisierer im Konstruktor gelassen werden.
Kotlin verlangt das alle Eigenschaft im Konstruktor initialisiert werden. Und falls es sich um einen nicht nullbare Eigenschaft handelt wird noch ein Initialisierungswert verlangt.

Um diese Einschränkung zu umgehen kann man in Kotlin solche Eigenschaften mit `lateinit` markieren.

Falls man versucht, auf solche Eigenschaften, bevor diese Initialisiert wurden zuzugreifen, erhält man eine `lateinit property has not been initialized` Exception.

#### 6.1.9 Extensions für nullbare Typen

Statt zu überprüfen, dass eine Variable nicht null ist, kann man die Überprüfung in den Extension-Funktionen überlassen. Hierfür muss der Receiver nullbar sein.

In Java sind solche Funktionen nicht realisierbar, da die Instanz einer Klasse nie null sein kann.

Weiterhin ist zu bemerken, das diese nur für Extension-Methoden funktioniert und nicht für normale Methoden.

#### 6.1.10 Nullbarkeit von Parametertypen (`T`)

Standardmäßig sind Parametertypen von Funktionen und Klassen nullbar.
Damit diese nicht null sein muss immer eine Obere Schranke angeben werden.

#### 6.1.11 Nullbarkeit und Java

Java unterstützt keine nullbaren Variablen.
Kotlin und Java jedoch in Projekten kombiniert werden.
Was passiert wenn man diese Kombiniert ist eine gute frage:

* Wenn Javacode `@Nullable` besitzt dann dann wird Variable zu einem Nullbaren Kotlin Variable umgewandelt fall `@NotNull` wird es zu einem Normalen nicht nullbaren Typ umgewandelt
* Kotlin erkennt mehre Annotation welche auf Nullbarkeit deuten automatisch
* Leider ist es öfter der Fall das keine Annotationen vorhanden sind

Im Fall wo es keine Informationen mittel Annotationen gibt spricht man von *Platform Typen* in Kotlin

Platform Typen sind Typen über die Kotlin keine Nullbarkeitsinformationen besitzt.
Man kann diese Typen beliebig behandeln:

* Man kann es als nullbaren Typen Interpretieren
* Man kann es als nicht nullbarer Typ interpretieren

Der Entwickler ist hier verantwortlich, dafür was verwendet werden soll.
Wenn man es als nicht null Typ angibt, gibt es keine Überprüfung Seitens des Compiler, demensprechend können NullPointerException auftreten. Trotzdem erhält man hier detaillierte Exception Nachricht als in Java.

Man kann in Kotlin keine eigenen Platform Typ definieren, diese können nur vom Java Code abgeleitet werden.

Vererbung:

* Bei der Implementierung von Java Interfaces in Kotlin, hat man die Wahl die Parameter und Rückgabewerte null oder nicht nullbar zu machen
* Es ist wichtig hier die richtige Entscheidung zu treffen, besonders wenn diese von nicht Kotlin Code verwendet werden.

### 6.2 Primitive und Andere Grund-Typen

Kotlin unterscheidet nicht zischen primitiven und Wrapper Typen

#### 6.2.1 Primitive: Int, Boolean und mehr

Java Unterscheidet Primitive von Referenztypen:

* Primitive beinhalten ihre Werte direkt
  * Sind effizienter (hin und her verschieben)
  * Man kann jedoch keine Funktionen direkt auf diesen Aufrufen
  * Wrapper werden deshalb zur Verfügung gestellt, welche die Primitive um kapseln
* Referenztypen haben lediglich eine Adresse welche auf den Speicher verweist wo die Werte hinterlegt werden

Kotlin unterscheidet nicht zwischen den wrappen und den Primitiven

* Man kann immer den gleichen Typen verwenden (Int statt java int und Integer)
* Sämtliche Methoden stehen zur Verfügung
* Zur Laufzeit werden die "primitiven" so effizient wie möglich dargestellt,
falls int ausreicht wird int verwenden falls nicht Integer

#### 6.2.2 Nullbarkeit von Primitiven Typen

Nullbare Typen aus Kotlin können nicht als Java Primitiven dargestellt werden
-> Alse Nullbare Primitiven in Kotlin werden immer in die Wrapper Typen umgewandelt.

#### 6.2.3 Zahlenkonvertierung

In Java werden Zahlen (Z.B float zu int) automatisch konvertiert implizit konvertiert.

In Kotlin werden die Zahlen nie impliziert konvertiert, der Entwickler muss die Konvertierung immer explizit durchführen.
Hierfür wird eine Reihe an Funktion von Kotlin Bereitgestellt.

Die explizite Konvertierungen soll Überraschungen verhindern durch Autokonvertierung vermeiden. Bei Vergleichen wird demensprechend nicht nur der Wert sondern auch der Typ Vergleichen.

Standardoperatoren sind jedoch schon überladen (+, -, ...), sodass man für diese keine explizite Konvertierung benötigt.

#### 6.2.4 `Any` und `Any?`

`Any` in Kotlin ist das gleiche wie `Object` in Java.
Unterschied ist das `Any` auch ein SuperTyp von Primitiven ist im vergleich zu Java.

`Anay?` ist das gleiche wie `Any` mit dem Zusatz, dass dieser jetzt null enthalten kann.

#### 6.2.5 `Unit`

`Unit` besitzt die gleiche Funktion wie `void` in Java:
Es wird verwendet um Funktionen ohne Rückgabewerte zu definieren.

Es unterscheidet sich jedoch in Kombination mit Generischen Funktionen kombiniert:

* Im vergleich zu Javas `void` ist `Unit` ein Typ und kann demensprechend als Argument-Typ übergeben werden
* Jede Funktionen die `Unit` als Rückgabewert hat, gibt auch implizit einen Wert Typ Unit wert immer Unit zurück.
* Funktionen mit generischen Rückgabewerte, können diese Wert vom Typ `Unit` zurückgeben.

#### 6.2.6 `Nothing`, diese Funktion wird nie was zurückgeben

Wir für Funktionen verwendet bei denen es sinnlos ist einen Wert zurück liefern, oder nie Normal Terminieren werden (z.B. Endlosschleifen, Exceptions, ...)

### 6.3 Collections und Arrays

#### 6.3.1 Nullbarkeit und Collections

Bei der Typangabe von Collections kann auch Nullbarkeit verwendet werden.
Bei Collections wird jedoch zwischen Nullbarkeit der Collection und Nullbarkeit der einzelnen Werte unterschieden.

#### 6.3.2 Read-Only und Mutable Collections

Unterschied zu Java ist das Interfaces  welche auf den Inhalt zugreifen, von Interfaces welche den Inhalt von Collections voneinander getrennt werden.

* `kotlin.collections.Collection` haben nur Methoden welche lesenden zugriff besitzt
* `kotlin.collections.MutableCollection` haben nur Methoden welche lesenden zugriff besitzt