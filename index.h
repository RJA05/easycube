const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
.card{
    font-family: Arial, Helvetica, sans-serif;
    max-width: 400px;
    min-height: 250px;
    padding: 30px;
    box-sizing: border-box;
    color: #FFF;
    margin:20px;
    box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
}
</style>
<body>

<div class="card" style="background-color:#02b875;">
  <h4>TEMT6000</h4><br>
  <h1>Brightness:<span id="lightSensorValue">0</span></h1><br>
</div>

 <div class="card" style="background-color:#FE9900;">
   <h4>BMP180 Temperature</h4><br>
   <h1>Temperature:<span id="TemperatureValue">0</span></h1><br>
 </div>

 <div class="card" style="background-color:#2e227b;">
    <h4>BMP180 Pressure</h4><br>
    <h1>Pressure:<span id="PressureValue">0</span></h1><br>
  </div>

<script>

//Light sensor
setInterval(function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("lightSensorValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readLightSensor", true);
  xhttp.send();
}, 1000); //1000mSeconds update rate

//Temperature sensor
setInterval(function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("TemperatureValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readTemperature", true);
  xhttp.send();
}, 1000); //1000mSeconds update rate

//Temperature sensor
setInterval(function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("PressureValue").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "readPressure", true);
  xhttp.send();
}, 1000); //1000mSeconds update rate

</script>
</body>
</html>
)=====";