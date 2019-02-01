package pl.hskek.dreadlish.antennaanalyzer

class Calibration(val startFreqIn : Double, val stopFreqIn: Double, val stepIn: Double, val short: AD8302Values, val open: AD8302Values, val load: AD8302Values, val through: AD8302Values) {
    val startFreq : Double;
    val stopFreq : Double;
    val step : Double;

    init {
        startFreq = startFreqIn
        stopFreq = stopFreqIn
        step = stepIn
    }

}