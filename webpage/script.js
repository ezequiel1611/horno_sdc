let chart;
let data = [];
let labels = [];
let updateInterval;
let isRunning = false;
let speed = 1000;
let power = 50;
let trigger = 90;
const canvas = document.getElementById('myChart');
const context = canvas.getContext('2d');

function resizeCanvas() {
    canvas.width = canvas.clientWidth;
    canvas.height = canvas.clientHeight;
    chart.update();
}

// Inicializa el gráfico
function initChart() {
    const ctx = document.getElementById('myChart').getContext('2d');
    chart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: labels,
            datasets: [{
                label: 'Temperatura en Tiempo Real',
                data: data,
                borderColor: 'rgba(75, 192, 192, 1)',
                borderWidth: 2,
                fill: false
            }]
        },
        options: {
            scales: {
                x: {
                    display: true,
                    title: {
                        display: true,
                        text: 'Tiempo'
                    }
                },
                y: {
                    display: true,
                    title: {
                        display: true,
                        text: 'Temperatura'
                    },
                    suggestedMin: 20,
                    suggestedMax: 40
                }
            }
        }
    });
}

// Función para agregar un nuevo dato al gráfico
function addData() {
    const currentTime = new Date().toLocaleTimeString();
    //const newValue = Math.floor(Math.random() * 100);  // Simula un valor entre 0 y 100
    const dbRef = ref(window.database);
    get(child(dbRef, 'READINGS/temperature/')).then((snapshot) => {
        if (snapshot.exists()) {
            const newValue = snapshot.val();
            console.log(newValue);
            data.push(newValue);
        } else {
            console.log("No data available");
        }
    }).catch((error) => {
        console.log(error);
    });
    
    labels.push(currentTime);

    if (labels.length > 100) {  // Limita el número de puntos mostrados
        labels.shift();
        data.shift();
    }

    chart.update();
}

// Función para iniciar o pausar la actualización de datos
function toggleUpdate() {
    if (isRunning) {
        clearInterval(updateInterval);
    } else {
        updateInterval = setInterval(addData, speed);
    }
    isRunning = !isRunning;
    set(ref(window.database, 'CONTROL/start/'), isRunning)
        .then(() => {
            console.log('Dato actualizado correctamente:', isRunning);
        })
        .catch((error) =>{
            console.error('Error al actualizar el dato:', error);
        });
}

// Actualiza la velocidad del intervalo con el slider
function sliderUpdate() {
    trigger = Math.abs(this.value);
    power = ((180 - trigger) / 180) * 100;
    document.getElementById('powerValue').textContent = Math.round(power) + "%";
    set(ref(window.database, 'CONTROL/power/'), trigger)
        .then(() => {
            console.log('Dato actualizado correctamente:', trigger);
        })
        .catch((error) => {
            console.error('Error al actualizar el dato:', error);
        });
}

function downloadTxtFile() {
    const textContent = data.join('\n');
    const blob = new Blob([textContent], { type: 'text/plain' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = "datos.m";
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
}

document.getElementById('powerSlider').addEventListener('input', sliderUpdate);
document.getElementById('createDataset').addEventListener('click', downloadTxtFile);
document.getElementById('toggleButton').addEventListener('click', toggleUpdate);

// Inicia el gráfico al cargar la página
initChart();
window.addEventListener('resize', resizeCanvas);