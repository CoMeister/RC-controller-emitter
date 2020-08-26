//var Chart = require('chart.js');
var ctx = document.getElementById('light0');
var color = Chart.helpers.color;

var config = {
    type: 'line',
    data: {
        datasets: [{
            label: 'Light0',
            backgroundColor: 'rgba(0, 59, 142, 0.7)',    //couleur des points
            borderColor: 'rgba(0, 59, 142, 0.7)',        //couleur trait et bord point
            lineTension: 0,
            fill: false,
            data: [{
                x: new Date('2020-03-25T00:00:00Z'),
                y: 34
            }, {
                x: new Date('2020-03-25T00:00:02Z'),
                y: 43
            }, {
                x: new Date('2020-03-25T00:00:04Z'),
                y: 56
            }, {
                x: new Date('2020-03-25T00:00:06Z'),
                y: 2
            }],
        }, {
            label: 'Light 1',
            backgroundColor: 'rgba(28, 142, 0, 0.7)',    //couleur des points
            borderColor: 'rgba(28, 142, 0, 0.7)',        //couleur trait et bord point
            lineTension: 0,
            fill: false,
            data: [{
                x: new Date('2020-03-25T00:00:10Z'),
                y: 6
            }, {
                x: new Date('2020-03-25T00:00:20Z'),
                y: 34
            },{
                x: new Date('2020-03-25T00:00:30Z'),
                y: 5
            }, {
                x: new Date('2020-03-25T00:00:40Z'),
                y: 92
            }, {
                x: new Date('2020-03-25T23:00:50Z'),
                y: 28
            }]
        }]
    },
    options: {
        responsive: true,
        title: {
            display: true,
            text: 'Chart.js Time Point Data'
        },
        scales: {
            xAxes: [{
                type: 'time',
                time:{
                    parser: "h:mm:ss",
                    stepSize:1,
                },
                display: true,
                scaleLabel: {
                    display: true,
                    labelString: 'Date'
                },
                ticks: {
                    max: "2020-03-25T23:59:59Z",
                    min: "2020-03-25T00:00:00Z",
                    major: {
                        fontStyle: 'bold',
                        fontColor: '#FF0000'
                    }
                }
            }],
            yAxes: [{
                display: true,
                scaleLabel: {
                    display: true,
                    labelString: 'value'
                },
                ticks: {
                    max: 100,
                    min: 0,
                    stepSize: 1
                }
            }]
        }
    }
};



$(document).ready(function(){
    var myChart = new Chart(ctx, config);

    $("#send").click(function(){
        var light0Percent = $("#led0").val();
        var light1Percent = $("#led1").val();

        console.log(light0Percent + " " + light1Percent);
    });
});