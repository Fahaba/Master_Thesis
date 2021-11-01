class GroovyMaster {
    final name
    def art = []

    GroovyMaster(String name) {
        this.name = name
    }

    def createPainting(artworkName, year) {...}
    def createSculpture(artworkName, year, sculptureHeight) {...}
    
    def printArt() {
        art.each {
            println it.info()
        }
    }
	
    static String greetings() {...}
}