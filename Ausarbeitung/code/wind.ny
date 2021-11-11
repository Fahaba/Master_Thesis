;nyquist plug-in
;type generate
;version 1
;name "Wind"

;control dur "Duration" float "duration" 0.1 0.0 360
;control scal "scale" float "scale" 0.1 0.0 10.0
;control cps "cps" float "cps" 1 0 2000
;control bw "bw" float "bw" 1 0 100
  
define function buildPWL()
	return pwl(0, 0.74, 0.2, 0.96, 0.5, 0.8, 0.75, 1.16, 0.9, 0.97, 1, 0.74, 1)

define function buildENV()
	return env(0.07, 0.08, 0.1, 1, 0.6, 0.8, 1)
	
define function buildReson(scal, cps, bw, vpwl)
	return reson(scale(scal, noise()), cps, bw, 2) + 
		reson(scale(scal * 1.13, noise()), cps * vpwl, bw * 1.042, 2)

define function wind(dur, scal, cps, bw)
  return stretch(dur, buildENV() * buildReson(scal, cps, bw, buildPWL()))
  
return wind(dur, scal, cps, bw)