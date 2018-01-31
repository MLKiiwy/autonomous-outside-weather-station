[
    {
        "id": "d3ba270a.9569d8",
        "type": "tab",
        "label": "Test MQTT "
    },
    {
        "id": "799892cf.d0a3ac",
        "type": "ui_gauge",
        "z": "d3ba270a.9569d8",
        "name": "Temperature",
        "group": "f13fe10d.5f01e",
        "order": 1,
        "width": 0,
        "height": 0,
        "gtype": "gage",
        "title": "Temperature",
        "label": "C",
        "format": "{{value}}",
        "min": "-20",
        "max": "50",
        "colors": [
            "#0000ff",
            "#00ff00",
            "#ffff00"
        ],
        "seg1": "",
        "seg2": "",
        "x": 593,
        "y": 275,
        "wires": []
    },
    {
        "id": "e26f8b19.ea0098",
        "type": "mqtt in",
        "z": "d3ba270a.9569d8",
        "name": "weather/outside/temperature",
        "topic": "weather/outside/temperature",
        "qos": "2",
        "broker": "381a79cd.abbb26",
        "x": 227,
        "y": 302,
        "wires": [
            [
                "799892cf.d0a3ac"
            ]
        ]
    },
    {
        "id": "44ebe242.38aa4c",
        "type": "ui_gauge",
        "z": "d3ba270a.9569d8",
        "name": "Light",
        "group": "f13fe10d.5f01e",
        "order": 2,
        "width": 0,
        "height": 0,
        "gtype": "gage",
        "title": "Light",
        "label": "Lux",
        "format": "{{value}}",
        "min": "0",
        "max": "32000",
        "colors": [
            "#0000ff",
            "#00ff00",
            "#ff0000"
        ],
        "seg1": "1000",
        "seg2": "10000",
        "x": 551,
        "y": 343,
        "wires": []
    },
    {
        "id": "5cb47326.26942c",
        "type": "mqtt in",
        "z": "d3ba270a.9569d8",
        "name": "weather/outside/light",
        "topic": "weather/outside/light",
        "qos": "2",
        "broker": "381a79cd.abbb26",
        "x": 204,
        "y": 359,
        "wires": [
            [
                "44ebe242.38aa4c"
            ]
        ]
    },
    {
        "id": "cba6f8a2.a92868",
        "type": "ui_gauge",
        "z": "d3ba270a.9569d8",
        "name": "Charge",
        "group": "b3832c63.a1e94",
        "order": 3,
        "width": 0,
        "height": 0,
        "gtype": "wave",
        "title": "Charge",
        "label": "%",
        "format": "{{value}}",
        "min": "0",
        "max": "100",
        "colors": [
            "#ff0000",
            "#ffff00",
            "#00ff00"
        ],
        "seg1": "",
        "seg2": "",
        "x": 463,
        "y": 427,
        "wires": []
    },
    {
        "id": "d76e9cce.0c311",
        "type": "mqtt in",
        "z": "d3ba270a.9569d8",
        "name": "weather/outside/charge",
        "topic": "weather/outside/charge",
        "qos": "2",
        "broker": "381a79cd.abbb26",
        "x": 206,
        "y": 426,
        "wires": [
            [
                "cba6f8a2.a92868",
                "63b912ba.5553ec"
            ]
        ]
    },
    {
        "id": "872644c7.3ad048",
        "type": "ui_gauge",
        "z": "d3ba270a.9569d8",
        "name": "Humidity",
        "group": "f13fe10d.5f01e",
        "order": 4,
        "width": 0,
        "height": 0,
        "gtype": "wave",
        "title": "Humidity",
        "label": "%",
        "format": "{{value}}",
        "min": "10",
        "max": "100",
        "colors": [
            "#ffff00",
            "#00ffff",
            "#0000ff"
        ],
        "seg1": "50",
        "seg2": "80",
        "x": 463,
        "y": 513,
        "wires": []
    },
    {
        "id": "5afc58c3.7dcae8",
        "type": "mqtt in",
        "z": "d3ba270a.9569d8",
        "name": "weather/outside/humidity",
        "topic": "weather/outside/humidity",
        "qos": "2",
        "broker": "381a79cd.abbb26",
        "x": 216,
        "y": 512,
        "wires": [
            [
                "872644c7.3ad048"
            ]
        ]
    },
    {
        "id": "e7d639cc.d8cc48",
        "type": "ui_gauge",
        "z": "d3ba270a.9569d8",
        "name": "Pressure",
        "group": "6537e6fb.6fe448",
        "order": 5,
        "width": 0,
        "height": 0,
        "gtype": "gage",
        "title": "Pressure",
        "label": "%",
        "format": "{{value | number:1}}",
        "min": "980",
        "max": "1050",
        "colors": [
            "#0000ff",
            "#00ff00",
            "#ff0000"
        ],
        "seg1": "",
        "seg2": "",
        "x": 700,
        "y": 553,
        "wires": []
    },
    {
        "id": "1a6a541d.16148c",
        "type": "mqtt in",
        "z": "d3ba270a.9569d8",
        "name": "weather/outside/pressure",
        "topic": "weather/outside/pressure",
        "qos": "2",
        "broker": "381a79cd.abbb26",
        "x": 214,
        "y": 573,
        "wires": [
            [
                "adb8eb44.2ae488"
            ]
        ]
    },
    {
        "id": "adb8eb44.2ae488",
        "type": "function",
        "z": "d3ba270a.9569d8",
        "name": "/100",
        "func": "msg.payload = msg.payload/100;\nreturn msg;",
        "outputs": 1,
        "noerr": 0,
        "x": 494,
        "y": 577,
        "wires": [
            [
                "e7d639cc.d8cc48",
                "e679f75c.0316d8"
            ]
        ]
    },
    {
        "id": "63b912ba.5553ec",
        "type": "ui_chart",
        "z": "d3ba270a.9569d8",
        "name": "Charge",
        "group": "b3832c63.a1e94",
        "order": 6,
        "width": 0,
        "height": 0,
        "label": "chart",
        "chartType": "line",
        "legend": "false",
        "xformat": "HH:mm",
        "interpolate": "linear",
        "nodata": "",
        "ymin": "0",
        "ymax": "100",
        "removeOlder": "10",
        "removeOlderPoints": "",
        "removeOlderUnit": "60",
        "cutout": 0,
        "colors": [
            "#1f77b4",
            "#aec7e8",
            "#ff7f0e",
            "#2ca02c",
            "#98df8a",
            "#d62728",
            "#ff9896",
            "#9467bd",
            "#c5b0d5"
        ],
        "x": 463,
        "y": 471,
        "wires": [
            [],
            []
        ]
    },
    {
        "id": "e679f75c.0316d8",
        "type": "ui_chart",
        "z": "d3ba270a.9569d8",
        "name": "Pressure",
        "group": "6537e6fb.6fe448",
        "order": 7,
        "width": 0,
        "height": 0,
        "label": "Pressure",
        "chartType": "line",
        "legend": "false",
        "xformat": "HH:mm",
        "interpolate": "linear",
        "nodata": "",
        "ymin": "980",
        "ymax": "1050",
        "removeOlder": 1,
        "removeOlderPoints": "",
        "removeOlderUnit": "3600",
        "cutout": 0,
        "colors": [
            "#1f77b4",
            "#aec7e8",
            "#ff7f0e",
            "#2ca02c",
            "#98df8a",
            "#d62728",
            "#ff9896",
            "#9467bd",
            "#c5b0d5"
        ],
        "x": 703,
        "y": 617,
        "wires": [
            [],
            []
        ]
    },
    {
        "id": "f13fe10d.5f01e",
        "type": "ui_group",
        "z": "",
        "name": "Default",
        "tab": "25744b91.681044",
        "order": 1,
        "disp": true,
        "width": "6"
    },
    {
        "id": "381a79cd.abbb26",
        "type": "mqtt-broker",
        "z": "",
        "broker": "localhost",
        "port": "1883",
        "clientid": "",
        "usetls": false,
        "compatmode": true,
        "keepalive": "60",
        "cleansession": true,
        "willTopic": "",
        "willQos": "0",
        "willPayload": "",
        "birthTopic": "",
        "birthQos": "0",
        "birthPayload": ""
    },
    {
        "id": "b3832c63.a1e94",
        "type": "ui_group",
        "z": "",
        "name": "Battery",
        "tab": "25744b91.681044",
        "order": 3,
        "disp": true,
        "width": "6"
    },
    {
        "id": "6537e6fb.6fe448",
        "type": "ui_group",
        "z": "",
        "name": "Pressure",
        "tab": "25744b91.681044",
        "order": 2,
        "disp": true,
        "width": "6"
    },
    {
        "id": "25744b91.681044",
        "type": "ui_tab",
        "z": "",
        "name": "Weather",
        "icon": "weather"
    }
]