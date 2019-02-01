package pl.hskek.dreadlish.antennaanalyzer

import android.bluetooth.BluetoothAdapter
import android.graphics.Color
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.util.Log
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import com.github.mikephil.charting.charts.LineChart
import com.github.mikephil.charting.components.YAxis
import com.github.mikephil.charting.data.Entry
import com.github.mikephil.charting.data.LineData
import com.github.mikephil.charting.data.LineDataSet
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet

class MainActivity : AppCompatActivity() {
    var analyzer: Analyzer? = null
    var handler: Handler = Handler()
    var start: Double = 100e6;
    var stop: Double = 2500e6;
    var step: Double = 100e6;
    val measurementRunnable: Runnable = Runnable {
        val freqsSequence = generateSequence(start * 1e6) {
            (it + (step*1e6)).takeIf { it <= (stop*1e6) }
        }
        for (freq in freqsSequence) {
            if(analyzer != null) {
                analyzer!!.doSingleMeasurement(freq, freqVals)
            }
        }
    }
    val freqVals : HashMap<Double, HashMap<String, AD8302Values>> = HashMap()
    val refreshRunable : Runnable = object: Runnable {
        override fun run() {
            validateAndShowChart()
            handler.postDelayed(this, 5000)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val adapter = BluetoothAdapter.getDefaultAdapter();

        if(!adapter.isEnabled) {
            Toast.makeText(this, "Bluetooth nie jest włączony. Włącz go i zrestartuj aplikację.", Toast.LENGTH_LONG).show()
        } else {
            analyzer = Analyzer()
        }

        val button: Button = findViewById(R.id.measure_button)
        button.setOnClickListener {
            start = findViewById<EditText>(R.id.start_value).text.toString().toDouble()
            step = findViewById<EditText>(R.id.step_value).text.toString().toDouble()
            stop = findViewById<EditText>(R.id.stop_value).text.toString().toDouble()
            Toast.makeText(this, "Wykonywanie pomiaru od ${start}MHz do ${stop}MHz co ${step}MHz", Toast.LENGTH_SHORT).show()
            Thread(measurementRunnable).start()
            handler.postDelayed(refreshRunable, 5000)
        }
    }

    private fun validateAndShowChart() {
        val mapped = freqVals.toSortedMap().mapValues {
            Sparams(it.value)
        }
        val chart: LineChart = findViewById(R.id.chart1)
        val lineDataSetS11 = LineDataSet(mapped.map {
            Entry((it.key / 1e6).toFloat(), (10.0*Math.log10(it.value.s11.abs())).toFloat())
        }, "S11")
        lineDataSetS11.color = Color.BLACK
        lineDataSetS11.axisDependency = YAxis.AxisDependency.LEFT
        lineDataSetS11.setCircleColor(Color.BLACK)
        lineDataSetS11.setDrawValues(false)
        val lineDataSetS12 = LineDataSet(mapped.map {
            Entry((it.key / 1e6).toFloat(), (10.0*Math.log10(it.value.s12.abs())).toFloat())
        }, "S12")
        lineDataSetS12.color = Color.BLUE
        lineDataSetS12.setCircleColor(Color.BLUE)
        lineDataSetS12.axisDependency = YAxis.AxisDependency.LEFT
        lineDataSetS12.setDrawValues(false)
        val lineDataSetS21 = LineDataSet(mapped.map {
            Entry((it.key / 1e6).toFloat(), (10.0*Math.log10(it.value.s21.abs())).toFloat())
        }, "S21")
        lineDataSetS21.color = Color.MAGENTA
        lineDataSetS21.setCircleColor(Color.MAGENTA)
        lineDataSetS21.axisDependency = YAxis.AxisDependency.LEFT
        lineDataSetS21.setDrawValues(false)
        val lineDataSetS22 = LineDataSet(mapped.map {
            Entry((it.key / 1e6).toFloat(), (10.0*Math.log10(it.value.s22.abs())).toFloat())
        }, "S22")
        lineDataSetS22.color = Color.RED
        lineDataSetS22.setCircleColor(Color.RED)
        lineDataSetS22.axisDependency = YAxis.AxisDependency.LEFT
        lineDataSetS22.setDrawValues(false)
        val lineDataSets: List<ILineDataSet> = arrayListOf(lineDataSetS11, lineDataSetS12, lineDataSetS21, lineDataSetS22)
        val lineData = LineData(lineDataSets)
        val yAxis = chart.axisLeft
        yAxis.axisMinimum = -60f
        yAxis.axisMaximum = 10f
        chart.axisRight.setDrawLabels(false)
        chart.description.text = ""
        chart.data = lineData
        chart.invalidate()
    }
}
