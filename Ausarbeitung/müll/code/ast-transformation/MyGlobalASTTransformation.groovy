package org.example

import org.codehaus.groovy.ast.ASTNode
import org.codehaus.groovy.ast.ClassNode
import org.codehaus.groovy.ast.MethodNode
import org.codehaus.groovy.ast.ModuleNode
import org.codehaus.groovy.ast.expr.ArgumentListExpression
import org.codehaus.groovy.ast.expr.ConstantExpression
import org.codehaus.groovy.ast.expr.MethodCallExpression
import org.codehaus.groovy.ast.expr.VariableExpression
import org.codehaus.groovy.ast.stmt.BlockStatement
import org.codehaus.groovy.ast.stmt.ExpressionStatement
import org.codehaus.groovy.ast.stmt.Statement
import org.codehaus.groovy.control.CompilePhase
import org.codehaus.groovy.control.SourceUnit
import org.codehaus.groovy.transform.ASTTransformation
import org.codehaus.groovy.transform.GroovyASTTransformation

// compile phase before bytecode generation
@GroovyASTTransformation(phase = CompilePhase.SEMANTIC_ANALYSIS)
class MyGlobalASTTransformation implements ASTTransformation {

    @Override
    void visit(ASTNode[] astNodes, SourceUnit sourceUnit) {
        // traverse ast tree from root recursively
        // astNodes[0] is the root node
        recursiveTraverse(astNodes[0])
    }

    void recursiveTraverse(Object obj) {

        // Module nodes contain class declarations
        if (obj instanceof ModuleNode)
            obj.getClasses().each { clazz -> recursiveTraverse(clazz) }
        // traverse methods for given class
        else if (obj instanceof ClassNode)
            obj.getMethods().each { m -> recursiveTraverse(m) }


        if (obj instanceof  MethodNode) {
            // anchor - build new statement for printing out method information e.g.name
            Statement s1 = new ExpressionStatement(
                    new MethodCallExpression(
                            new VariableExpression("this"),
                            new ConstantExpression("println"),
                            new ArgumentListExpression(
                                    new ConstantExpression(obj.toString())
                            )
                    )
            )

            // add the new statement to the method node
            def existingStatements = ((BlockStatement)obj.code).statements
            existingStatements.add(0, s1)
        }
    }
}
