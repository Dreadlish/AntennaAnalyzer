package pl.hskek.dreadlish.antennaanalyzer

import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothSocket
import android.util.Log
import java.io.InputStream
import java.io.OutputStream
import java.util.*
import kotlin.collections.HashMap

data class AnalyzerRawValues(val magnitude: Int = 0, val phase: Int = 0)

val serialUUID: UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")


class Analyzer {
    private val inStream: InputStream
    private val outStream: OutputStream


    init {
        val adapter = BluetoothAdapter.getDefaultAdapter();

        val hcs = adapter.bondedDevices.filter { it.name == "HC-05" }
        val device = adapter.getRemoteDevice(hcs[0].address);
        val socket: BluetoothSocket = device.createInsecureRfcommSocketToServiceRecord(serialUUID);
        socket.connect()
        outStream = socket.outputStream
        inStream = socket.inputStream
    }

    fun writeRegister(registerName: String, value: Number) {
        outStream.write("$registerName$value\n".toByteArray())
    }

    fun readRawValues() : AnalyzerRawValues {
        outStream.write("p\n".toByteArray())
        var magnitude = 0
        var phase = 0
        while (inStream.available() == 0);
        while (true) {
            val line = readSingleLine(inStream).split("\n")[0]
            if(line == "") {
                continue
            }
            val split = line.split(" ")
            if(split[0] == "M" && split[2] == "P")
            {
                magnitude = split[1].toInt(16)
                phase = split[3].toInt(16)
                break
            }
            while (inStream.available() == 0);
        }
        return AnalyzerRawValues(magnitude, phase)
    }

    private fun readSingleLine(inStream: InputStream): String {
        var strOut = ""
        var lastChar: Char = 0.toChar();
        while(lastChar != '\n') {
            lastChar = inStream.read().toChar()
            strOut += lastChar
        }
        return strOut
    }

    fun setFrequency(freq: Double) {
        val freq_regs = FrequencyRegisters(freq)
        writeRegister("d", freq_regs.diva)
        writeRegister("n", freq_regs.n)
        writeRegister("c", freq_regs.cdiv)
        writeRegister("b", freq_regs.bs)
        writeRegister("m", freq_regs.m)
        writeRegister("a", freq_regs.f)
        writeRegister("r", freq_regs.r)
        writeRegister("f", freq_regs.fb)
        writeRegister("w", 0) // actual write
    }

    fun doSingleMeasurement(
        freq: Double,
        freqVals: HashMap<Double, HashMap<String, AD8302Values>>
    ) {
        val values: HashMap<String, AD8302Values> = HashMap()
        Log.i("AntennaAnalyzer", "Setting frequency %.2f".format(freq))
        setFrequency(freq)
        getAnalyzerValues("GENS1_S1Forward", values)
        getAnalyzerValues("GENS1_S1Reverse", values)
        getAnalyzerValues("GENS1_S2Forward", values)
        getAnalyzerValues("GENS1_S2Reverse", values)
        getAnalyzerValues("GENS2_S1Forward", values)
        getAnalyzerValues("GENS2_S1Reverse", values)
        getAnalyzerValues("GENS2_S2Forward", values)
        getAnalyzerValues("GENS2_S2Reverse", values)
        Log.i("AntennaAnalyzer", values.toString())
        freqVals[freq] = values
    }

    private fun getAnalyzerValues(
        switchesVal: String,
        values: HashMap<String, AD8302Values>
    ) {
        writeRegister("s", getSwitchValues(switchesVal))
        val rawVals = readRawValues()
        val magVlt = ArduinoADCValue(rawVals.magnitude)
        val phsVlt = ArduinoADCValue(rawVals.phase)
        values[switchesVal] = AD8302Values(magVlt, phsVlt)
    }
}

fun getSwitchValues(switches: String): Number {
    return when (switches) {
        "GENS2_S1Forward" -> 0b0000
        "GENS2_S1Reverse" -> 0b0010
        "GENS2_S2Reverse" -> 0b0001
        "GENS2_S2Forward" -> 0b0101
        "GENS1_S1Forward" -> 0b1000
        "GENS1_S1Reverse" -> 0b1010
        "GENS1_S2Forward" -> 0b1001
        "GENS1_S2Reverse" -> 0b1101
        else -> 0b1111
    }
}