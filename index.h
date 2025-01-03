const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style >

* {margin: 0; padding: 10px;}
#container {
    display: flex;
    justify-content: center; 
    height: 100%; 
    width:100%; 
    font-size:0;
}
#left, #right, #middle {display: inline-block; *display: inline; zoom: 1; vertical-align: top; font-size: 12px;}
#left {width: 25%;}
#middle {width: 25%;}
#right {width: 25%;}

.card{
    text-wrap: balance;
    word-break:break-all;
    border-radius: 25px;
    font-family:Arial, Helvetica, sans-serif, sans-serif;
    max-width: 400px;
    min-height: 250px;
    padding: 30px;
    box-sizing: border-box;
    color: #FFF;
    margin:20px;
    box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
}


.readings{
    font-size: 10px;
    border-radius: 25px;
    box-sizing: border-box;
    font-family:Arial, Helvetica, sans-serif;
    width: 100%;
    min-height: 120px;
    max-height: 200px;
    color: #000000;
    background-color: #FFF;
    display: flex;
    align-items: center;
}

</style>
<body>

<div id="container" >

    <div id="left" class="card" style="background-color:#02b875;">
        <h1>TEMT6000</h1><br>
        <div class="readings">
            <h2>Brightness: <span id="lightSensorValue">0</span></h2><br>
        </div>
    </div>
      
    <div id="middle" class="card" style="background-color:#FE9900;">
         <h1>BMP180</h1><br>
         <div class="readings">
            <h2>Temperature: <span id="TemperatureValue">0</span> 
            <br/> Pressure: <span id="PressureValue">0</span> </h2><br>
         </div>

    </div>
      
    <div id="right" class="card" style="background-color:#83279a;">
          <h1>Ina219</h1><br>
          <div class="readings">
            <h2>Load Voltage: <span id="LoadVoltage">0</span> 
            <br/> Current (mA): <span id="Current">0</span>
            <br/> Power (mW): <span id="Power">0</span> </h2><br>
          </div>

    </div>

</div>



<script>

//sensor read
setInterval(function() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
        var response = this.responseText; 
        const data = response.split("|"); // Split data using delimiter
        document.getElementById("lightSensorValue").innerHTML = data[0];
        document.getElementById("TemperatureValue").innerHTML = data[1];
        document.getElementById("PressureValue").innerHTML = data[2];
        document.getElementById("LoadVoltage").innerHTML = data[3];
        document.getElementById("Current").innerHTML = data[4];
        document.getElementById("Power").innerHTML = data[5];
    }
  };
  xhttp.open("GET", "readSensors", true);
  xhttp.send();
}, 1200); //1000mSeconds update rate

</script>
</body>
</html>
)=====";