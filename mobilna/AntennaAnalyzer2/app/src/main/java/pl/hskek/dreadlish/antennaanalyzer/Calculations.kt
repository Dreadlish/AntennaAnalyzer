package pl.hskek.dreadlish.antennaanalyzer

import org.apache.commons.math3.complex.Complex

class ModuleAndAngleComplex (val module: Double, val angle: Double) {
    val complex: Complex = Complex(0.0, angle * Math.PI/180).exp().multiply(Math.pow(10.0, module/10.0))
}

class ArduinoADCValue (value: Int) {
    val translated : Double = value.toDouble() / 1024.0  * 3.3
}

class AD8302Values(magVlt: ArduinoADCValue, phsVlt: ArduinoADCValue) {
    val magnitude = (magVlt.translated - 0.9) / 0.03
    val phase = Math.abs((phsVlt.translated - 1.8) / 0.01)
    val complexVal = ModuleAndAngleComplex(magnitude, phase)

    override fun toString(): String {
        return "MAG %4.3f, PHS %4.3f".format(magnitude, phase)
    }
}

class Sparams(measured: HashMap<String, AD8302Values>) {
    val s11 = measured["GENS1_S1Reverse"]!!.complexVal.complex.divide(measured["GENS1_S1Forward"]!!.complexVal.complex)
    val s21 = measured["GENS1_S2Forward"]!!.complexVal.complex.divide(measured["GENS1_S1Forward"]!!.complexVal.complex)
    val s12 = measured["GENS2_S1Reverse"]!!.complexVal.complex.divide(measured["GENS2_S2Reverse"]!!.complexVal.complex)
    val s22 = measured["GENS2_S2Forward"]!!.complexVal.complex.divide(measured["GENS2_S2Reverse"]!!.complexVal.complex)
}