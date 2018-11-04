from flask import Blueprint, render_template
import pygal
from Database.database import *

bp = Blueprint(__name__, __name__, template_folder='templates')

@bp.route('/')
def show():
    return render_template("index.html")

@bp.route('/graphs')
def showgraphs():
    try:
        sensor1 = getDataByDeviceId(sensorId1[0], sensorId1[1], sensorId1[2])
        sensor2 = getDataByDeviceId(sensorId2[0], sensorId2[1], sensorId2[2])

        temp1   = []
        temp2   = []
        hum1    = []
        hum2    = []
        pId1    = []
        pId2    = []

        for i in sensor1:
            pId1.append(i[4])
            temp1.append(i[6])
            hum1.append(i[8])

        for i in sensor2:
            pId2.append(i[4])
            temp2.append(i[6])
            hum2.append(i[8])


        temperature_line_chart = pygal.Line(legend_at_bottom=True, style = pygal.style.styles['default'](background = 'aliceblue'))
        temperature_line_chart.title    = 'Temperature [°C]'
        temperature_line_chart.x_labels = map(str, range(0, max(max(pId1),max(pId2))))
        temperature_line_chart.add('Sensor1',   temp1)
        temperature_line_chart.add('Sensor2',   temp2)
        temperature_line_chart.range = [0, 60]
        temperature_graph_data = temperature_line_chart.render_data_uri()

        humidity_line_chart = pygal.Line(legend_at_bottom=True, style = pygal.style.styles['default'](background = 'aliceblue'))
        humidity_line_chart.title       = 'Humidity [%]'
        humidity_line_chart.x_labels    = map(str, range(0, max(max(pId1),max(pId2))))
        humidity_line_chart.add('Sensor1', hum1)
        humidity_line_chart.add('Sensor2', hum2)
        humidity_line_chart.range = [0, 100]
        humidity_graph_data = humidity_line_chart.render_data_uri()

        ### Max Temperatures ###
        celsius_formatter                  = lambda x: '{:.10g}°C'.format(x)
        maxtemperature1                    = pygal.SolidGauge(  inner_radius    = 0.70,
                                                                show_legend     = False,
                                                                style           = pygal.style.styles['default'](value_font_size     = 50,
                                                                                                                title_font_size     = 50,
                                                                                                                tooltip_font_size   = 30,
                                                                                                                background          = 'aliceblue'))
        maxtemperature1.title              = "Sensor1 Max Temperature"
        maxtemperature1.value_formatter    = celsius_formatter
        maxtemperature1.add('Sensor1 Max Temperature', [{'value': max(temp1), 'max_value': 60, 'color': 'red'}])
        maxtemperature1_data               = maxtemperature1.render_data_uri()

        maxtemperature2                    = pygal.SolidGauge(  inner_radius    = 0.70,
                                                                show_legend     = False,
                                                                style           = pygal.style.styles['default'](value_font_size     = 50,
                                                                                                                title_font_size     = 50,
                                                                                                                tooltip_font_size   = 30,
                                                                                                                background          = 'aliceblue'))
        maxtemperature2.title              = "Sensor2 Max Temperature"
        maxtemperature2.value_formatter    = celsius_formatter
        maxtemperature2.add('Sensor2 Max Temperature', [{'value': max(temp2), 'max_value': 60, 'color': 'blue'}])
        maxtemperature2_data               = maxtemperature2.render_data_uri()

        ### Max Humidities ###
        percent_formatter                  = lambda x: '{:.10g}%'.format(x)
        maxhumidity1                       = pygal.SolidGauge(  inner_radius    = 0.70,
                                                                show_legend     = False,
                                                                style           = pygal.style.styles['default'](value_font_size     = 50,
                                                                                                                title_font_size     = 50,
                                                                                                                tooltip_font_size   = 30,
                                                                                                                background          = 'aliceblue'))
        maxhumidity1.title                 = "Sensor1 Max Humidity"
        maxhumidity1.value_formatter       = percent_formatter
        maxhumidity1.add('Max Humidity', [{'value': max(hum1), 'max_value': 100, 'color': 'red'}])
        maxhumidity1_data                  = maxhumidity1.render_data_uri()

        maxhumidity2                       = pygal.SolidGauge(  inner_radius    = 0.70,
                                                                show_legend     = False,
                                                                style           = pygal.style.styles['default'](value_font_size     = 50,
                                                                                                                title_font_size     = 50,
                                                                                                                tooltip_font_size   = 30,
                                                                                                                background          = 'aliceblue'))
        maxhumidity2.title                 = "Sensor2 Max Humidity"
        maxhumidity2.value_formatter       = percent_formatter        
        maxhumidity2.add('Max Humidity', [{'value': max(hum2), 'max_value': 100, 'color': 'blue'}])
        maxhumidity2_data                  = maxhumidity2.render_data_uri()

        return render_template("graphs.html",
            temperature_graph_data  = temperature_graph_data,
            humidity_graph_data     = humidity_graph_data,
            maxtemperature1_data    = maxtemperature1_data,
            maxtemperature2_data    = maxtemperature2_data,
            maxhumidity1_data       = maxhumidity1_data,
            maxhumidity2_data       = maxhumidity2_data
            )
    except Exception as e:
        return(str(e))