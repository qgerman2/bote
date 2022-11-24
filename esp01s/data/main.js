const socket = new WebSocket('ws://192.168.4.1:8080');
socket.addEventListener('open', function () {
    socket.send('Hello Server!');
    document.addEventListener('keydown', function(e) {
        if (e.repeat) {return}
        if (e.key === "ArrowUp") {
            socket.send("upp");
        } else if (e.key === "ArrowLeft") {
            socket.send("leftp");
        } else if (e.key === "ArrowRight") {
            socket.send("rightp");
        }
    });
    document.addEventListener('keyup', function(e) {
        if (e.repeat) {return}
        if (e.key === "ArrowUp") {
            socket.send("upr");
        } else if (e.key === "ArrowLeft") {
            socket.send("leftr");
        } else if (e.key === "ArrowRight") {
            socket.send("rightr");
        }
    });
    document.getElementById("left").addEventListener('touchstart', ()=>{
        socket.send("leftp");
    });
    document.getElementById("left").addEventListener('touchend', ()=>{
        socket.send("leftr");
    });
    document.getElementById("right").addEventListener('touchstart', ()=>{
        socket.send("rightp");
    });
    document.getElementById("right").addEventListener('touchend', ()=>{
        socket.send("rightr");
    });
    document.getElementById("up").addEventListener('touchstart', ()=>{
        socket.send("upp");
    });
    document.getElementById("up").addEventListener('touchend', ()=>{
        socket.send("upr");
    });
});