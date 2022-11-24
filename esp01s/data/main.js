const socket = new WebSocket('ws://192.168.4.1:8080');
var both = false;
var servo1 = 90;
var servo2 = 90;
var motor = 0;

function enviar() {
    socket.send(String.fromCharCode(servo1, servo2, motor))
}

socket.addEventListener('open', function () {
    enviar()
    document.getElementById("both").addEventListener('change', ()=>{
        if (both) {
            both = false;
        } else {
            both = true;
        }
    })
    document.getElementById("servo1").addEventListener('input', (event)=>{
        servo1 = document.getElementById("servo1").value
        if (both) {
            servo2 = document.getElementById("servo1").value
            document.getElementById("servo2").value = servo1
        }
        enviar()
    });
    document.getElementById("servo2").addEventListener('input', (event)=>{
        servo2 = document.getElementById("servo2").value
        if (both) {
            servo1 = document.getElementById("servo2").value
            document.getElementById("servo1").value = servo2
        }
        enviar()
    });
    document.getElementById("on").addEventListener('click', ()=>{
        motor = 1
        enviar()
    });
    document.getElementById("off").addEventListener('click', ()=>{
        motor = 0
        enviar()
    });
});