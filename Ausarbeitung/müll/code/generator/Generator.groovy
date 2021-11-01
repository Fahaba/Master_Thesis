import groovy.io.FileType

class Generator {

    def static rootDir = new File('multi-module-project')

    static void main(String[] args) {

        def children = 2
        def depth = 2
        int nodeCount = 0

        if (args.size() > 1) {
            println("Using provided parameters")
            children  = args[0].toInteger()
            depth     = args[1].toInteger()
        }
        if (args.size() > 2) {
            rootDir = new File(args[2])
        }

        // Nice to know
        println "\nGenerating Project Tree Structure"
        println "Children: $children"
        println "Depth: $depth"
        println "Modules locations: $rootDir\n"

        // to make sure that we do not delete any important files by accident
        // happened to me once :(
        def files = []
        def directories = []

        if (rootDir.exists()) {
            rootDir.eachFile(FileType.FILES) {
                if (it.name != "settings.gradle" &&
                        it.name != "build.gradle" &&
                        it.name != "gradle.properties")
                    files << it
            }
            rootDir.eachFile(FileType.DIRECTORIES) {
                if (!(it.name ==~ /^module([0-9]*)/) &&
                        it.name != "build" &&
                        it.name != "modules" &&
                        it.name != ".gradle")
                    directories << it
            }
        }

        assert files.isEmpty()
        assert directories.isEmpty()

        //create gradle project folder
        rootDir.deleteDir()
        rootDir.mkdir()
        createGradleBuild(rootDir)
        createGradleProperties(rootDir)



        (0..depth).each {
            nodeCount += Math.pow(children, it)
        }

        cleanOldModules(rootDir)
        createModules(nodeCount, children)
        modBuildGradle(rootDir, nodeCount)

        // create modules for javac
        def jpRoot = new File(rootDir.absoluteFile.parentFile, "${rootDir.name}-java")

        if (jpRoot.exists()) {
            jpRoot.eachFile(FileType.FILES) {
                files << it
            }
            jpRoot.eachFile(FileType.DIRECTORIES) {
                if (!(it.name ==~ /^module([0-9]*)/))
                    directories << it
            }
        }

        assert files.isEmpty()
        assert directories.isEmpty()

        jpRoot.deleteDir()
        jpRoot.mkdir()

        (1..nodeCount).each{
            def modFile = new File("$rootDir/module$it/src/main/java/module-info.java")
            def srcFile = new File("$rootDir/module$it/src/main/java/module$it/Module${it}.java")

            def srcDst = new File(jpRoot, "module$it/module$it/Module${it}.java")
            srcDst.parentFile.mkdirs()
            srcDst.createNewFile()
            srcDst.createNewFile()
            srcDst << srcFile.bytes

            def modDst = new File(jpRoot, "module$it/module-info.java")
            modDst.createNewFile()
            modDst << modFile.bytes
        }
    }

    /**
     * creates a single module
     * @param moduleNumber  number of the module
     * @param parent        number of the parent module
     * @param children      List children modules
     */
    def static createModule(int moduleNumber, int parent ,List children) {

        // create basic folder structure
        def moduleName = "module$moduleNumber"
        def modDir = new File(rootDir, moduleName)
        modDir.mkdir()

        // create src/main/java
        def srcDir = new File(modDir, 'src')
        srcDir.mkdir()
        def mainDir = new File(srcDir, 'main')
        mainDir.mkdir()
        def javaDir = new File(mainDir, 'java')
        javaDir.mkdir()

        // Create moduleXX
        def pacDir = new File(javaDir, "module$moduleNumber")
        pacDir.mkdir()

        // create build.gradle file
        def gradleFile = new File(modDir, 'build.gradle')
        gradleFile << "mainClassName = 'module${moduleNumber}.Module$moduleNumber'\n"

        if (children.size > 0) gradleFile << gradleDependencies(children)

        // create module-info file
        def modInfoFile = new File(javaDir, 'module-info.java')
        modInfoFile << ( "module $moduleName {\n")
        // node opens itself to parent
        if (parent != -1)
            modInfoFile << ( "    exports $moduleName to module$parent;\n")
        // requires access to children
        if (children.size > 0)
            children.each {
                modInfoFile << "    requires module$it;\n"
            }
        modInfoFile << ( "}")


        // java source code
        def srcFile = new File(pacDir, "Module${moduleNumber}.java")
        srcFile << "package $moduleName;\n"
        srcFile << "\n"

        if (children.size > 0) srcFile << importChildren(children)

        srcFile << "public class Module$moduleNumber {\n"
        srcFile << "\n"
        srcFile << "    private static final String NAME = \"$moduleName\";\n"
        srcFile << "    private static final String DAD = \"module$parent\";\n"
        srcFile << "\n"
        srcFile << "    public static void main(String[] args) { \n"
        srcFile << "        System.out.println(\"Hello World!\"); \n"

        if (children.size > 0) srcFile << callHiMethods(children)

        srcFile << "    }\n"
        srcFile << "\n"
        srcFile << "    public static void hiDad$moduleNumber() {\n"

        if (children.size > 0) srcFile << callHiMethods(children)

        srcFile << "        System.out.println(\"Hi Dad from child: [\" + NAME + \"] to father: [\" + DAD + \"]\");\n"
        srcFile << "    }\n"
        srcFile << "\n"
        srcFile << "}\n"
    }

    /**
     * creates a balanced tree module structure
     * @param nodeCount     number of modules to be created
     * @param childCount    number of children every inner module will have
     */
    def static createModules(int nodeCount, int childCount) {
        (1..nodeCount).each { node ->
            // the offset to determine correct parent nodes of left children
            int offset = childCount - 2
            int parent
            if (node == 1) parent = -1
            else parent = (node+offset).intdiv(childCount)

            def children = []
            (0..<childCount).each {
                int child =  node * childCount - offset + it
                if (child <= nodeCount)
                    children.add( child )
            }

            createModule(node,parent, children)
        }
    }

    static String importChildren(children) {
        String returnImports = ""
        children.each {
            returnImports += "import module${it}.Module${it};\n"
        }
        returnImports += "\n"
    }

    static String callHiMethods(children){
        String returnCalls = ""
        children.each {
            returnCalls += "        Module${it}.hiDad$it();\n"
        }
        returnCalls
    }

    static String gradleDependencies(children) {
        String returnDependencies = '\ndependencies {\n'
        children.each {
            returnDependencies += "    implementation project(':module$it')\n"
        }
        returnDependencies += '}\n'
    }

    /**
     * clean root directory of previously generated modules
     * @param   rootDir root Directory
     */
    static cleanOldModules(File rootDir) {
        rootDir.eachFile {
            if (it.name.startsWith('mod'))
                it.deleteDir()
        }
    }

    /**
     * modifies the settings.gradle file to include the generated modules
     * @param   rootDir root Directory (where the settings.build file should be located
     * @param   modules number modules that will be created
     */
    static modBuildGradle(File rootDir, int modules) {
        def settingsFile = new File(rootDir, 'settings.gradle')
        settingsFile.createNewFile()
        def newSettingsFile = ''
        settingsFile.eachLine {
            if (!it.startsWith("include '"))
                newSettingsFile += it + "\n"
        }
        (1..modules).each {newSettingsFile += "include 'module$it'\n"}
        settingsFile.write(newSettingsFile)
    }

    /**
     * Creates the build.gradle file
     * @param   rootDir root Directory (where the build.gradle file will be placed)
     */
    static createGradleBuild(File rootDir) {
        def buildFile = new File(rootDir, 'build.gradle')
        buildFile.createNewFile()
        buildFile << """
allprojects {
    apply plugin: 'java'
    group 'net.ddns.flep.gradle-jpms'
    version '1.0-SNAPSHOT'
    sourceCompatibility = 1.9
}

subprojects {
    apply plugin: 'application'
}

        """
    }

    static createGradleProperties(File rootDir) {
        def propertiesFile = new File(rootDir, 'gradle.properties')
        propertiesFile << """
org.gradle.daemon=false
org.gradle.parallel=false
org.gradle.jvmargs=-Xmx4096M
        """
    }
}
