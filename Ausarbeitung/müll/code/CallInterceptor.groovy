class CallInterceptor implements Interceptor{
    boolean invokeMethods = true

    @Override
    Object beforeInvoke(Object object, String methodName, Object[] arguments) {
        print "    PROX    CAL: $methodName -> "
        arguments.each {
            print "[$it]"
        }
        println ""
    }

    @Override
    Object afterInvoke(Object object, String methodName, Object[] arguments, Object result) {
        println "    PROX    RES: $methodName <- [$result]"
        result
    }

    @Override
    boolean doInvoke() {
        return invokeMethods
    }
}