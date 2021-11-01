Buildsysteme  
*	Build Systeme ist allgegenwertig in der modernen Softwareprojekte
*	Sind kern von Automatisierungsprozesse.
*	Steigerung der Softwarequaliät durch bestimmte Praktiken/Prozesse wie CD CI
*	Gradle ist ein solches Buildprozess der im Rahmen dieser Arbeit betrachtet werden soll
*	Zielsetzung war es einen tieferen Einblick in die Interne Funktionsweise und Prozesse von Gradle erhalten
*	Jedoch ohne sich vorher in Gradle einzuarbeiten (Blind rein)
*	Da angenommen wurde das Gradle hauptsächlich in Groovy implementiert wurde, sollte dessen Besonderheiten ausgenutzt werden um den Einblick zu erhalten.
	* Durch Metaprogrammierung (was eine Besonderheit von Groovy darstellt)
*	Hinzu kommt das JPM-System
*	Sollte als Anwendungsbeispiel in Zusammenhang mit Gradle untersucht werden.
*	Anwendungsbeispiel wird verwendet für Performanceuntersuchung von Gradle
*	Zunächt wird jedoch das neue JPMS untersucht.
*	JPMS führt Module ein
*	Diese kommen im Anwendungsbeispiel zum Einsatz.
*	Dienen um das Verhalten von Gradle im umgang mit vielen Projeten zu beobachten
*	Dabei entspricht ein Modul einem Projekt
*	Gleichzeit wird beobachtet ob eine grosse Modulanzahl in einem JPMS Projekt, unerwarteten Effekt auslöst