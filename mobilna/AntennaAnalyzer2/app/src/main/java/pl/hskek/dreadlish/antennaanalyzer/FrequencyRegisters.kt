package pl.hskek.dreadlish.antennaanalyzer

import org.apache.commons.math3.fraction.Fraction

class FrequencyRegisters internal constructor(fout: Double) {
    var diva: Int = 0
    var fb = 1
    var n: Int = 0
    val cdiv: Int
    val bs: Int
    var m = 4095
    var f: Int = 0
    internal val r = 1
    val fpd = 20e6

    init {
        this.diva = when {
            3e9 <= fout -> 0
            1.5e9 <= fout -> 1
            750e6 <= fout -> 2
            375e6 <= fout -> 3
            187.5e6 <= fout -> 4
            93.75e6 <= fout -> 5
            46.875e6 <= fout -> 6
            else -> 7
        }
        val div: Int = 1 shl this.diva
        val fvco = fout * div
        var d: Int = 1
        for (i in 0..1) {
            d = if (this.fb == 1) {
                1
            } else {
                Math.min(div, 16)
            }
            this.n = (fvco / this.fpd / d).toInt()
            if (this.n in 19..4091) {
                break
            } else {
                this.fb = (if(this.fb == 0) 1 else 0)
            }
        }
        this.cdiv = Math.round(fpd / 100e3).toInt()
        this.bs = Math.round(fpd / 50e3).toInt()
        val x = Fraction((fvco - d.toDouble() * fpd * this.n.toDouble()) / (d * fpd))
        this.f = x.numerator
        this.m = x.denominator
        if (f == 1 && m == 1) {
            this.f = 4094
            this.m = 4095
        }

        if ((m < 2 || m > 4095) && f == 0) {
            this.m = 2
        }
    }
}
