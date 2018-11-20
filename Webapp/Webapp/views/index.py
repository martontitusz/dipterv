from flask import Blueprint, render_template
import pygal
from Database.database import *

bp = Blueprint(__name__, __name__, template_folder='templates')

tablepagecounter    = 0
graphpagecounter    = 0
rowperpage          = 10
graphdataperpage    = 10

indexID             = 0
indexTimestamp      = 1
indexDeviceId0      = 2
indexDeviceId1      = 3
indexDeviceId2      = 4
indexPacketId       = 5
indexTemperatureId  = 6
indexTemperature    = 7
indexHumidityId     = 8
indexHumidity       = 9


def showtables(page):
    try:
        sensor1 = getDataByDeviceId(sensorId1[0], sensorId1[1], sensorId1[2])
        sensor2 = getDataByDeviceId(sensorId2[0], sensorId2[1], sensorId2[2])

        tablepagecounter = int((max(len(sensor1), len(sensor2))-1)/rowperpage)+1

        temp1	= []
        temp2   = []
        hum1 	= []
        hum2 	= []
        pId1 	= []
        pId2 	= []

        start   = (page - 1) * rowperpage
        stop    = (page * rowperpage)

        for i in sensor1:
            pId1.append(i[indexPacketId])
            temp1.append(i[indexTemperature])
            hum1.append(i[indexHumidity])

        for i in sensor2:
            pId2.append(i[indexPacketId])
            temp2.append(i[indexTemperature])
            hum2.append(i[indexHumidity])

        line_chart1         = pygal.Bar()
        line_chart1.add('PacketID', pId1[start:stop])
        line_chart1.add('Temperature [°C]', temp1[start:stop])
        line_chart1.add('Humidity [%]', hum1[start:stop])
        table_data1         = line_chart1.render_table()

        line_chart2         = pygal.Bar()
        line_chart2.add('PacketID', pId2[start:stop])
        line_chart2.add('Temperature [°C]', temp2[start:stop])
        line_chart2.add('Humidity [%]', hum2[start:stop])
        table_data2         = line_chart2.render_table()
        
        table1_title        = "Data from device: " + sensorId1[0] + " "+sensorId1[1] + " "+sensorId1[2]
        table2_title        = "Data from device: " + sensorId2[0] + " "+sensorId2[1] + " "+sensorId2[2]

        table_foot_start    = """<table><tfoot><tr><td colspan="3">"""
        table_foot_prew     = """<div class="links"><a href="/tables""" + str(max(page-1, 1)) + """">&laquo;</a>"""
        table_foot_first    = """<a class="active" href="/tables1">1</a>"""
        
        table_foot_middle   = ""
        for i in range(1, tablepagecounter):
            table_foot_middle = table_foot_middle + """<a href="/tables""" + str(i+1) + """">""" + str(i+1) + """</a>"""

        table_foot_next     = """<a href="/tables""" + str(min(page+1, tablepagecounter)) + """">&raquo;</a>"""
        table_foot_stop     = """</td></tr></tfoot></table>"""

        table_foot          = table_foot_start + table_foot_prew + table_foot_first + table_foot_middle + table_foot_next + table_foot_stop


        summary_table1      = pygal.Bar()
        if (len(pId1) > 0):
            summary_table1.add('Min Temperature[°C]', min(temp1))
            summary_table1.add('Max Temperature[°C]', max(temp1))
            summary_table1.add('Min Humidity[%]', min(hum1))
            summary_table1.add('Max Humidity[%]', max(hum1))
        else:
            summary_table1.add('Min Temperature[°C]', None)
            summary_table1.add('Max Temperature[°C]', None)
            summary_table1.add('Min Humidity[%]', None)
            summary_table1.add('Max Humidity[%]', None)

        summary_table1.add('Received Packets', len(pId1))
        summary_table_data1 = summary_table1.render_table()

        summary_table2      = pygal.Bar()
        if (len(pId2) > 0):
            summary_table2.add('Min Temperature[°C]', min(temp2))
            summary_table2.add('Max Temperature[°C]', max(temp2))
            summary_table2.add('Min Humidity[%]', min(hum2))
            summary_table2.add('Max Humidity[%]', max(hum2))
        else:
            summary_table2.add('Min Temperature[°C]', None)
            summary_table2.add('Max Temperature[°C]', None)
            summary_table2.add('Min Humidity[%]', None)
            summary_table2.add('Max Humidity[%]', None)

        summary_table2.add('Received Packets', len(pId2))
        summary_table_data2 = summary_table2.render_table()

        return render_template("tables.html",
            table1_title        = table1_title,
            table2_title        = table2_title,
            table_data1         = table_data1,
            table_data2         = table_data2,
            table_foot          = table_foot,
            summary_table_data1 = summary_table_data1,
            summary_table_data2 = summary_table_data2
            )

    except Exception as e:
        return(str(e))

@bp.route('/tables1')
def showtables1():
    return showtables(1)

@bp.route('/tables2')
def showtables2():
    return showtables(2)

@bp.route('/tables3')
def showtables3():
    return showtables(3)

@bp.route('/tables4')
def showtables4():
    return showtables(4)


def showgraphs(page):
    try:
        sensor1 = getDataByDeviceId(sensorId1[0], sensorId1[1], sensorId1[2])
        sensor2 = getDataByDeviceId(sensorId2[0], sensorId2[1], sensorId2[2])

        graphpagecounter = int((max(len(sensor1), len(sensor2))-1)/graphdataperpage)+1

        temp1	= []
        temp2	= []
        hum1 	= []
        hum2 	= []
        pId1 	= []
        pId2 	= []

        for i in sensor1:
            pId1.append(i[indexPacketId])
            temp1.append(i[indexTemperature])
            hum1.append(i[indexHumidity])

        for i in sensor2:
            pId2.append(i[indexPacketId])
            temp2.append(i[indexTemperature])
            hum2.append(i[indexHumidity])

        start   = (page - 1) * graphdataperpage
        stop    = (page * graphdataperpage)

        temperature_line_chart          = pygal.Line(legend_at_bottom=True, style = pygal.style.styles['default'](background = 'aliceblue'))
        temperature_line_chart.title    = 'Temperature [°C]'
        temperature_line_chart.x_labels = map(str, range(start, stop))
        temperature_line_chart.add('Sensor1',   temp1[start:stop])
        temperature_line_chart.add('Sensor2',   temp2[start:stop])

        if (len(pId1) > 0) and (len(pId2) > 0):
            temperature_line_chart.range = [min(min(temp1),min(temp2))-5, max(max(temp1),max(temp2))+5]
        else:
            temperature_line_chart.range = [0, 50]
        temperature_graph_data = temperature_line_chart.render_data_uri()

        humidity_line_chart             = pygal.Line(legend_at_bottom=True, style = pygal.style.styles['default'](background = 'aliceblue'))
        humidity_line_chart.title       = 'Humidity [%]'
        humidity_line_chart.x_labels    = map(str, range(start, stop))
        humidity_line_chart.add('Sensor1', hum1[start:stop])
        humidity_line_chart.add('Sensor2', hum2[start:stop])
        humidity_line_chart.range = [0, 100]
        humidity_graph_data = humidity_line_chart.render_data_uri()

        ### Max Temperatures ###
        celsius_formatter               = lambda x: '{:.10g}°C'.format(x)
        maxtemperature1                 = pygal.SolidGauge( inner_radius    = 0.70,
                                                            show_legend     = False,
                                                            style           = pygal.style.styles['default'](value_font_size     = 50,
                                                                                                            title_font_size     = 50,
                                                                                                            tooltip_font_size   = 30,
                                                                                                            background          = 'aliceblue'))
        maxtemperature1.title           = "Sensor1 Max Temperature"
        maxtemperature1.value_formatter = celsius_formatter

        if (len(pId1) > 0):
            maxtemperature1.add('Sensor1 Max Temperature', [{'value': max(temp1), 'max_value': 60, 'color': 'red'}])
        else:
            maxtemperature1.add('Sensor1 Max Temperature', [{'value': 0, 'max_value': 60, 'color': 'red'}])
        
        maxtemperature1_data            = maxtemperature1.render_data_uri()

        maxtemperature2                 = pygal.SolidGauge( inner_radius    = 0.70,
                                                            show_legend     = False,
                                                            style           = pygal.style.styles['default'](value_font_size     = 50,
                                                                                                            title_font_size     = 50,
                                                                                                            tooltip_font_size   = 30,
                                                                                                            background          = 'aliceblue'))
        maxtemperature2.title           = "Sensor2 Max Temperature"
        maxtemperature2.value_formatter = celsius_formatter
        if (len(pId2) > 0):
            maxtemperature2.add('Sensor2 Max Temperature', [{'value': max(temp2), 'max_value': 60, 'color': 'blue'}])
        else:
            maxtemperature2.add('Sensor2 Max Temperature', [{'value': 0, 'max_value': 60, 'color': 'blue'}])
        maxtemperature2_data            = maxtemperature2.render_data_uri()

        ### Max Humidities ###
        percent_formatter               = lambda x: '{:.10g}%'.format(x)
        maxhumidity1                    = pygal.SolidGauge( inner_radius    = 0.70,
                                                            show_legend     = False,
                                                            style           = pygal.style.styles['default'](value_font_size     = 50,
                                                                                                            title_font_size     = 50,
                                                                                                            tooltip_font_size   = 30,
                                                                                                            background          = 'aliceblue'))
        maxhumidity1.title              = "Sensor1 Max Humidity"
        maxhumidity1.value_formatter    = percent_formatter
        if (len(pId1) > 0):
            maxhumidity1.add('Max Humidity', [{'value': max(hum1), 'max_value': 100, 'color': 'red'}])
        else:
            maxhumidity1.add('Max Humidity', [{'value': 0, 'max_value': 100, 'color': 'red'}])
        maxhumidity1_data               = maxhumidity1.render_data_uri()

        maxhumidity2                    = pygal.SolidGauge( inner_radius    = 0.70,
                                                            show_legend     = False,
                                                            style           = pygal.style.styles['default'](value_font_size     = 50,
                                                                                                            title_font_size     = 50,
                                                                                                            tooltip_font_size   = 30,
                                                                                                            background          = 'aliceblue'))
        maxhumidity2.title              = "Sensor2 Max Humidity"
        maxhumidity2.value_formatter    = percent_formatter
        if (len(pId2) > 0):
            maxhumidity2.add('Max Humidity', [{'value': max(hum2), 'max_value': 100, 'color': 'blue'}])
        else:
            maxhumidity2.add('Max Humidity', [{'value': 0, 'max_value': 100, 'color': 'blue'}])
        maxhumidity2_data               = maxhumidity2.render_data_uri()

        graph_foot_start    = """<table><tfoot><tr><td colspan="3">"""
        graph_foot_prew     = """<div class="links"><a href="/graphs""" + str(max(page-1, 1)) + """">&laquo;</a>"""
        graph_foot_first    = """<a class="active" href="/graphs1">1</a>"""
        
        graph_foot_middle   = ""
        for i in range(1, graphpagecounter):
            graph_foot_middle = graph_foot_middle + """<a href="/graphs""" + str(i+1) + """">""" + str(i+1) + """</a>"""

        graph_foot_next     = """<a href="/graphs""" + str(min(page+1, graphpagecounter)) + """">&raquo;</a>"""
        graph_foot_stop     = """</td></tr></tfoot></table>"""

        graph_foot          = graph_foot_start + graph_foot_prew + graph_foot_first + graph_foot_middle + graph_foot_next + graph_foot_stop



        return render_template("graphs.html",
            temperature_graph_data  = temperature_graph_data,
            humidity_graph_data     = humidity_graph_data,
            maxtemperature1_data    = maxtemperature1_data,
            maxtemperature2_data    = maxtemperature2_data,
            maxhumidity1_data       = maxhumidity1_data,
            maxhumidity2_data       = maxhumidity2_data,
            graph_foot              = graph_foot
            )
    except Exception as e:
        return(str(e))

@bp.route('/graphs1')
def showgraphs1():
    return showgraphs(1)

@bp.route('/graphs2')
def showgraphs2():
    return showgraphs(2)

@bp.route('/graphs3')
def showgraphs3():
    return showgraphs(3)

@bp.route('/graphs4')
def showgraphs4():
    return showgraphs(4)